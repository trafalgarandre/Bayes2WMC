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
    double C = 1;


	CNFFormula generateIndicator();
    CNFFormula generateParameter();
    void createKB();
    void inputBayes(string input);
public:

	Bayes(string input);
	void printKB(string cnfFileName, string CFileName);
    void addEvid(string evidFileName);
};
#endif 