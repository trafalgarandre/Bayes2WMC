#include <iostream>
#include "src/Bayes.h"

using namespace std;

int main(int argc, char** argv)
{
	//cout << "start";
	string input, evid;
	input = argv[1];
	evid = argv[2];
	int type = 0;
	cout << argc;
	if (argc == 4) {
		type = stoi(argv[3]);
	}
	Bayes* bayes = new Bayes(input, type);
	bayes->addEvid(evid);
	bayes->printKB("cnf.txt","weight.txt");
    return 0;
}
