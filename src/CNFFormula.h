#ifndef CNFFORMULA_H
#define CNFFORMULA_H

#include <vector>

using namespace std;

class CNFFormula {
private:
	vector<vector<int>> formula;

public:
	CNFFormula();

	CNFFormula(vector<vector<int>> _formula);

	vector<vector<int>> getFormula();

	int getNumClause();

	void addClause(vector<int> clause);

	void andFormula(CNFFormula f);

	void or1Var(int v);

	void and1Var(int v);

	static CNFFormula and1Var(CNFFormula f, int v);

	static CNFFormula or1Var(CNFFormula f, int v);

	static void print(CNFFormula f);
};
#endif 