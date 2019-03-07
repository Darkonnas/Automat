#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <string>
#include <tuple>

using namespace std;

class Machine {
	friend class RegularGrammar;
	set<string> Q, F;
	set<char> S;
	map<pair<string, char>, set<string>> d;
	string q0;
	bool isDeterminated;
	bool hasLamdaTransitions;

public:
	Machine();
	Machine(ifstream &buffer);
	Machine(RegularGrammar &grammar);
	set<string> lamdaClosure(string state);
	bool Evaluate(string word);
	void printConfiguration();
	void convertToDFA();
};

class RegularGrammar {
	friend class Machine;
	set<char> N;
	set<char> T;
	char S;
	set<tuple<char, char, char>> P;

public:
	RegularGrammar();
	RegularGrammar(ifstream &buffer);
	RegularGrammar(Machine &machine);
	bool Evaluate(string word);
	void printConfiguration();
};
