#include <iostream>
#include <algorithm>
#include "Bayes.h"

using namespace std;

void printHelp() {
	cout << "Usage: ./bayes2wmc [-h] <input> <evid> [<type>] \n";
	cout << '\n';
	cout << "-h        Display usage\n";
	cout << "<type>    Type of encoding, choose either 0 or 1(default = 0)\n";
	cout << "Example: ./bayes2wmc input.uai input.uai.evid 1\n";
}

class InputParser{
    public:
        InputParser (int &argc, char **argv){
            for (int i=1; i < argc; ++i)
                this->tokens.push_back(std::string(argv[i]));
        }

        const std::string& getCmdOption(const std::string &option) const{
            std::vector<std::string>::const_iterator itr;
            itr =  std::find(this->tokens.begin(), this->tokens.end(), option);
            if (itr != this->tokens.end() && ++itr != this->tokens.end()){
                return *itr;
            }
            static const std::string empty_string("");
            return empty_string;
        }

        bool cmdOptionExists(const std::string &option) const{
            return std::find(this->tokens.begin(), this->tokens.end(), option)
                   != this->tokens.end();
        }
    private:
        std::vector <std::string> tokens;
};

int main(int argc, char** argv)
{
	InputParser inputParser(argc, argv);
	if (inputParser.cmdOptionExists("-h") || argc < 3) {
		printHelp();
		return 0;
	}
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
