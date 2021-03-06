#ifndef BAYES_H
#define BAYES_H

#include <unordered_map>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <string.h>

#include "CNFFormula.h"

using namespace std;

class Bayes {
private:
	int num_nodes;
    vector<int> cardinality;
    vector<vector<int>> parent;
    vector<vector<double>> cpt;
    unordered_map<string, int> hmap;
    unordered_map<int, double> probs;
    int seed = 1;
    CNFFormula formula;


	CNFFormula generateIndicator();
    CNFFormula generateParameter1();
    CNFFormula generateParameter2();
    void createKB(int type);
    void inputBayes(string input);
public:

	Bayes(string input, int type);
	void printKB(string cnfFileName, string CFileName);
    void addEvid(string evidFileName);
};
#endif 