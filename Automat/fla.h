#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <string>
#include <tuple>

using namespace std;

class Machine {
	friend class FormalGrammar;
	set<string> Q, F;
	set<char> S;
	map<pair<string, char>, set<string>> d;
	string q0;
	bool isDeterminated;
	bool hasLamdaTransitions;

public:
	Machine();
	Machine(ifstream &buffer);
	Machine(FormalGrammar &grammar);
	set<string> lamdaClosure(string state);
	bool Evaluate(string word);
	void printConfiguration();
};

class FormalGrammar {
	friend class Machine;
	set<string> N;
	set<char> T;
	string S;
	set<tuple<char, char, char>> P;

public:
	FormalGrammar();
	FormalGrammar(ifstream &buffer);
	FormalGrammar(Machine &machine);
	bool Evaluate(string word);
	void printConfiguration();
};
