#include "Bayes.h"
#include "iostream"

CNFFormula Bayes::generateIndicator() {
    CNFFormula result;
    for (int i = 0; i < num_nodes; i++) {
        vector<int> clause;
        for (int j = 0; j < cardinality[i]; j++) {
            string key = to_string(i) + " " + to_string(j);
            hmap.emplace(key, seed);
            probs.emplace(seed, 1);
            probs.emplace(-seed, 1);
            clause.push_back(seed);
            seed++;
        }
        result.addClause(clause);
        vector<int> only1Clause;
        for (int j = 1; j < cardinality[i]; j++) {
            for (int k = 0; k < j; k++) {
                string key1 = to_string(i) + " " + to_string(j);
                string key2 = to_string(i) + " " + to_string(k);
                only1Clause.push_back(-(hmap.at(key1)));
                only1Clause.push_back(-(hmap.at(key2)));
                result.addClause(only1Clause);
                only1Clause.clear();
            }
        }
    }
    return result;
}

CNFFormula Bayes::generateParameter2() {
   // cout << "param 2";
    CNFFormula result;
    for (int i = 0; i < num_nodes; i++) {
        vector<int> parents = parent[i];
        parents.push_back(i);
        int pos = parents.size() - 1;
        int arr[parents.size()];
        memset(arr, 0, sizeof(arr));
        int index = 0;
        while (pos >= 0) {
            //cout << "INDEX " << index << '\n';
            //cout << "new\n";
            CNFFormula param_clause;
            for (int j = 0; j < parents.size(); j++) {
                if (j != parents.size() - 1) {
                    string key = to_string(parent[i][j]) + " " + to_string(arr[j]);
                    param_clause.or1Var(-hmap.at(key));
                    //cout << key << " ";
                }
            }
            //cout << (to_string(i) + " " + to_string(arr[parents.size() - 1])) << '\n';
            param_clause.or1Var(hmap.at(to_string(i) + " " + to_string(arr[parents.size() - 1])));
            double temp_prob = cpt[i][index];
            double temp_prob2 = 1;
            for (int j = 0; j < arr[parents.size() - 1]; j++) {
                temp_prob2 -= cpt[i][index - j - 1];
                param_clause.or1Var(seed - j - 1);
            }
            if (arr[parents.size() - 1] != cardinality[parents[parents.size() - 1]] - 1) {
                temp_prob = temp_prob / temp_prob2;
                //cout << fixed << " " << temp_prob << " " << 1 - temp_prob << '\n';
               // cout << "the fuck" << arr[parents.size() - 1] << "\n";
                param_clause.or1Var(-seed);
                probs.emplace(seed, temp_prob);
                probs.emplace(-seed, 1 - temp_prob);
                seed++;
            }
            result.andFormula(param_clause);
            bool step = false;
            while (arr[pos] == cardinality[parents[pos]] - 1) {
                pos--;
                step = true;
            }
            index++;
            if (pos >= 0) {
                arr[pos]++;
                if (step) {
                    for (int j = pos + 1; j < parents.size(); j++) {
                        arr[j] = 0;
                    }
                    pos = parents.size() - 1;
                }
            }
        }
    }
    
    return result;

}

CNFFormula Bayes::generateParameter1() {
    CNFFormula result;
    //cout << "Haha\n";
    for (int i = 0; i < num_nodes; i++) {
        vector<int> parents = parent[i];
        parents.push_back(i);
        int pos = parents.size() - 1;
        int arr[parents.size()];
        memset(arr, 0, sizeof(arr));
        int index = 0;
        CNFFormula param_clause_2;
        while (pos >= 0) {
            CNFFormula param_clause_1;
            for (int j = 0; j < parents.size(); j++) {
                if (j != parents.size() - 1) {
                    string key = to_string(parent[i][j]) + " " + to_string(arr[j]);
                    param_clause_1.and1Var(hmap.at(key));
                } else {
                    string key = to_string(i) + " " + to_string(arr[j]);
                    param_clause_1.and1Var(hmap.at(key));
                }
            }
            
            probs.emplace(seed, cpt[i][index]);
            probs.emplace(-seed, 1);
            param_clause_1.or1Var(-seed);
            param_clause_2.or1Var(seed);
            seed++;
            result.andFormula(param_clause_1);
            bool step = false;
            
            while (arr[pos] == cardinality[parents[pos]] - 1) {
                pos--;
                step = true;
            }
            index++;
            if (pos >= 0) {
                arr[pos]++;
                if (step) {
                    for (int j = pos + 1; j < parents.size(); j++) {
                        arr[j] = 0;
                    }
                    pos = parents.size() - 1;
                }
            }
        }
        result.andFormula(param_clause_2);
    }
    
    return result;
}

void Bayes::inputBayes(string input) {
    ifstream bayesFile;
    bayesFile.open(input);
    
    string type;
    bayesFile >> type;
    int ignore;
    if (type != "BAYES") {
        throw "Not Bayes";
    }

    bayesFile >> num_nodes;

    for (int i = 0; i < num_nodes; i++) {
        int temp;
        bayesFile >> temp;
        cardinality.push_back(temp);
        parent.push_back({});
        cpt.push_back({});
    }

    bayesFile >> ignore;

    for (int i = 0; i < num_nodes; i++) {
        int cpt_nums;
        bayesFile >> cpt_nums;
        for (int j = 0; j < cpt_nums - 1; j++) {
            int temp;
            bayesFile >> temp;
            parent[i].push_back(temp);
        }
        bayesFile >> ignore;
    }

    for (int i = 0; i < num_nodes; i++) {
        int num_network_params;
        bayesFile >> num_network_params;
        int den_num = 0;
        for (int j = 0; j < num_network_params; j++) {
            string temp_str;
            double temp;
            bayesFile >> temp_str;
            if (!temp_str.empty()) {
                temp = stod(temp_str);
                cpt[i].push_back(temp);
            }
        }
    }
    

    bayesFile.close();
}

void Bayes::createKB(int type) {
    formula = generateIndicator();
    CNFFormula par;
    if (type == 0) {
        par = generateParameter1();
    } else {
        par = generateParameter2();
    }
    formula.andFormula(par);
}

void Bayes::addEvid(string evidFileName) {
    
    ifstream evidFile;
    evidFile.open(evidFileName);
    
    int n;
    CNFFormula evid;
    evidFile >> n;
    while (n--) {
        int index, val;
        evidFile >> index >> val;
        string key = to_string(index) + " " + to_string(val);
        evid.and1Var(hmap.at(key));
    }
    formula.andFormula(evid);
    
    evidFile.close();
}

Bayes::Bayes(string input, int type) {
    inputBayes(input);
    createKB(type);
}

void Bayes::printKB(string cnfFileName, string weightFileName) {
    
    ofstream cnfFile, weightFile;
    cnfFile.open(cnfFileName);
    weightFile.open(weightFileName);
    cnfFile << "p cnf " << seed - 1<< " " << formula.getNumClause() << "\n";

    for (vector<int> clause: formula.getFormula()) {
        for (int i: clause) {
            cnfFile << i << " ";
        }
        cnfFile << "0\n";
    }

    weightFile << "p " << seed - 1 << '\n';
    for (pair<int, double> element : probs) {
        weightFile << fixed << "w " << element.first << " " << element.second << " 0\n";
    }

    weightFile.close();
    cnfFile.close();
    
}