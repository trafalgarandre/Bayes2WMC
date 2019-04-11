#include "Bayes.h"
#include "iostream"

CNFFormula Bayes::generateIndicator() {
    CNFFormula result;
    for (int i = 0; i < num_nodes; i++) {
        vector<int> clause;
        for (int j = 0; j < cardinality[i]; j++) {
            string key = to_string(i) + " " + to_string(j);
            //cout << key << " :" << seed << '\n';
            hmap.emplace(key, seed);
            clause.push_back(seed);
            probs.emplace(seed, 1);
            probs.emplace(-seed, 1);
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
            }
        }
    }
    return result;
}
CNFFormula Bayes::generateParameter() {
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
            //cout << "looping";
            CNFFormula param_clause_1;
            for (int j = 0; j < parents.size(); j++) {
                if (j != parents.size() - 1) {
                    string key = to_string(parent[i][j]) + " " + to_string(arr[j]);
                    //cout << key << ' ' << -hmap.at(key) << '\n';
                    param_clause_1.and1Var(hmap.at(key));
                } else {
                    string key = to_string(i) + " " + to_string(arr[j]);
                    //cout << key << ' ' << -hmap.at(key) << '\n';
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
                //cout <<"againagain";
                pos--;
                step = true;
            }
            index++;
            if (pos >= 0) {
                if (!step) {
                    arr[pos]++;
                } else {
                    for (int j = pos + 1; j < parents.size(); j++) {
                        arr[j] = 0;
                    }
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

void Bayes::createKB() {
    formula = generateIndicator();
    CNFFormula par = generateParameter();
    formula.andFormula(par);
}

void Bayes::addEvid(string evidFileName) {
    
    //cout << "waiting";
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
    //cout << "finish";
}

Bayes::Bayes(string input) {
    inputBayes(input);
    createKB();
}

void Bayes::printKB(string cnfFileName, string weightFileName) {
    
    ofstream cnfFile, weightFile;
    cnfFile.open(cnfFileName);
    weightFile.open(weightFileName);
    cnfFile << "p cnf " << seed << " " << formula.getNumClause() << "\n";

    for (vector<int> clause: formula.getFormula()) {
        for (int i: clause) {
            cnfFile << i << " ";
        }
        cnfFile << "0\n";
    }

    weightFile << "p " << seed - 1 << '\n';
    for (pair<int, double> element : probs) {
        weightFile << "w " << element.first << " " << element.second << " 0\n";
    }

    weightFile.close();
    cnfFile.close();
    
}