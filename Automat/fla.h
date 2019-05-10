#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <string>
#include <tuple>
#include <stack>

using namespace std;

class RegularAutomata {
	friend class RegularGrammar;
	set<string> Q, F;
	set<char> S;
	map<pair<string, char>, set<string>> d;
	string q0;
	bool hasNondeterminism;
	bool hasLambdaTransitions;

public:
	RegularAutomata();
	RegularAutomata(ifstream &buffer);
	RegularAutomata(RegularGrammar &grammar);
	set<string> lambdaClosure(string state);
	bool Evaluate(string word);
	void printConfiguration();
	void convertToDFA();
};

class RegularGrammar {
	friend class RegularAutomata;
	set<char> N;
	set<char> T;
	char S;
	set<tuple<char, char, char>> P;

public:
	RegularGrammar();
	RegularGrammar(ifstream &buffer);
	RegularGrammar(RegularAutomata &machine);
	bool Evaluate(string word);
	void printConfiguration();
};

class PushDownAutomata {
	set<string> Q, F;
	set<char> S, G;
	string q0;
	char Z0;
	map<tuple<string, char, char>, map<string, set<string>>> d;
	bool hasNondeterminism;
	bool hasLambdaTransitions;
public:
	PushDownAutomata();
	PushDownAutomata(ifstream& buffer);
	//PushDownAutomata(ContextFreeGrammar& grammar);
	map<string, set<stack<char>>> lambdaClosure(pair < string, set<stack<char>>> initConfiguation);
	bool Evaluate(string word);
	void printConfiguration();
};
