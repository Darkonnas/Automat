#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <string>
#include <tuple>
#include <stack>

class RegularAutomata {
	friend class RegularGrammar;
	std::set<std::string> Q, F;
	std::set<char> S;
	std::map<std::pair<std::string, char>, std::set<std::string>> d;
	std::string q0;
	bool hasNondeterminism;
	bool hasLambdaTransitions;

public:
	RegularAutomata();
	RegularAutomata(std::ifstream &buffer);
	RegularAutomata(RegularGrammar &grammar);
	std::set<std::string> lambdaClosure(std::string state);
	bool Evaluate(std::string word);
	void printConfiguration();
	void convertToDFA();
};

class RegularGrammar {
	friend class RegularAutomata;
	std::set<char> N;
	std::set<char> T;
	char S;
	std::set<std::tuple<char, char, char>> P;

public:
	RegularGrammar();
	RegularGrammar(std::ifstream &buffer);
	RegularGrammar(RegularAutomata &machine);
	bool Evaluate(std::string word);
	void printConfiguration();
};

class PushDownAutomata {
	std::set<std::string> Q, F;
	std::set<char> S, G;
	std::string q0;
	char Z0;
	std::map<std::tuple<std::string, char, char>, std::map<std::string, std::set<std::string>>> d;
	bool hasNondeterminism;
	bool hasLambdaTransitions;
public:
	PushDownAutomata();
	PushDownAutomata(std::ifstream& buffer);
	//PushDownAutomata(ContextFreeGrammar& grammar);
	std::map<std::string, std::set<std::stack<char>>> lambdaClosure(std::pair<std::string, std::set<std::stack<char>>> initConfiguation);
	bool Evaluate(std::string word);
	void printConfiguration();
};
