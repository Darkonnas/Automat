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

    RegularAutomata(std::ifstream &);

    RegularAutomata(const RegularGrammar &);

    std::set<std::string> lambdaClosure(const std::string &) const;

    bool Evaluate(const std::string &word) const;

    void printConfiguration() const;

    void convertToDFA();
};

class RegularGrammar {
    friend class RegularAutomata;

    std::set<char> N;
    std::set<char> T;
    char S;
    std::map<char, std::set<std::pair<char, char>>> P;

public:
    RegularGrammar();

    RegularGrammar(std::ifstream &);

    RegularGrammar(const RegularAutomata &);

    bool Evaluate(const std::string &) const;

    void printConfiguration() const;
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

    PushDownAutomata(std::ifstream &);

    //PushDownAutomata(const ContextFreeGrammar& grammar);
    std::map<std::string, std::set<std::stack<char>>> lambdaClosure(const std::pair<std::string, std::set<std::stack<char>>> &) const;

    bool Evaluate(const std::string &) const;

    void printConfiguration() const;
};
