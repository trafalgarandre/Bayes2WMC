#include <iostream>
#include "src/Bayes.h"

using namespace std;

int main(int argc, char** argv)
{
	//cout << "start";
	string input, evid;
	input = argv[1];
	evid = argv[2];
	Bayes* bayes = new Bayes(input);
	bayes->addEvid(evid);
	bayes->printKB("cnf.txt","weight.txt");
    return 0;
}
