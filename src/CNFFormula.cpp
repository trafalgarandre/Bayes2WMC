#include "CNFFormula.h"
#include <iostream>

CNFFormula::CNFFormula() {
	formula.clear();
}

CNFFormula::CNFFormula(vector<vector<int>> _formula) {
	formula = _formula;
}

vector<vector<int>> CNFFormula::getFormula() {
	return formula;
}

int CNFFormula::getNumClause() {
	return formula.size();
}

void CNFFormula::addClause(vector<int> clause) {
	formula.push_back(clause);
}

void CNFFormula::andFormula(CNFFormula f) {
	for (vector<int> clause: f.getFormula()) {
		addClause(clause);
	}
}

void CNFFormula::or1Var(int v) {
	int size = getNumClause();
	//cout << "or " << v << '\n';
	if (formula.size() == 0) {
		//cout << "push2 \n";
		formula.push_back({v});
	} else {
		for (int i = 0; i < size; i++) {
			formula[i].push_back(v);
			//cout << "push \n"; 
		}
	}
}

void CNFFormula::and1Var(int v) {
	addClause({v});
}

CNFFormula CNFFormula::and1Var(CNFFormula f, int v) {
	CNFFormula* result = new CNFFormula();
	result->andFormula(f);
	result->and1Var(v);
	return *result;
}

CNFFormula CNFFormula::or1Var(CNFFormula f, int v) {
	CNFFormula* result = new CNFFormula();
	result->andFormula(f);
	result->or1Var(v);
	return *result;
}

void CNFFormula::print(CNFFormula f) {
	for (vector<int> clause: f.getFormula()) {
        for (int i: clause) {
            cout << i << " ";
        }
        cout << "0\n";
    }
}