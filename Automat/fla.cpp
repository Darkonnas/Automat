#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <map>
#include <stack>
#include <string>
#include "fla.h"

RegularAutomata::RegularAutomata() {
	hasNondeterminism = false;
	hasLambdaTransitions = false;
	int nrStates;
	std::cout << "Number of states: ";
	std::cin >> nrStates;
	std::cout << "States: ";
	for (int i = 0; i < nrStates; ++i) {
		std::string state;
		std::cin >> state;
		Q.insert(state);
	}
	int alphaDim;
	std::cout << "Alphabet dimension: ";
	std::cin >> alphaDim;
	std::cout << "Letters: ";
	for (int i = 0; i < alphaDim; ++i) {
		char letter;
		std::cin >> letter;
		S.insert(letter);
	}
	int transCount;
	std::cout << "Transition count: ";
	std::cin >> transCount;
	std::cout << "Transitions:  (format: initialState letter resultState !For lambda transitions use '~'!)\n";
	for (int i = 0; i < transCount; ++i) {
		std::string initState, finState;
		char rule = '~';
		bool validTransition = false;
		do {
			validTransition = true;
			std::cin >> initState >> rule >> finState;
			if (Q.find(initState) == Q.end() || Q.find(finState) == Q.end() || (S.find(rule) == S.end() && rule != '~'))
				validTransition = false;
			if (!validTransition)
				std::cout << "Invalid transition!\n";
		} while (!validTransition);
		if (rule == '~') {
			hasLambdaTransitions = true;
		}
		if (d.find(std::make_pair(initState, rule)) == d.end()) {
			std::set<std::string> finStates;
			finStates.insert(finState);
			d.emplace(std::make_pair(initState, rule), finStates);
		}
		else {
			hasNondeterminism = true;
			d.at(std::make_pair(initState, rule)).insert(finState);
		}
	}
	int finStateCount;
	bool validInitState;
	do {
		std::cout << "Initial state: ";
		validInitState = true;
		std::cin >> q0;
		if (Q.find(q0) == Q.end())
			validInitState = false;
		if (!validInitState)
			std::cout << "Invalid initial state!\n";
	} while (!validInitState);
	std::cout << "Number of final states: ";
	std::cin >> finStateCount;
	std::cout << "Final states: ";
	for (int i = 0; i < finStateCount; ++i) {
		std::string finState;
		bool validFinState;
		do {
			validFinState = true;
			std::cin >> finState;
			if (Q.find(finState) == Q.end())
				validFinState = false;
			if (!validFinState)
				std::cout << "Invalid final state! <" << finState << ">\n";
		} while (!validFinState);
		F.insert(finState);
	}
	std::cout << "Generated RegularAutomata!\n";
}

RegularAutomata::RegularAutomata(std::ifstream & buffer) {
	hasNondeterminism = false;
	hasLambdaTransitions = false;
	int nrStates;
	buffer >> nrStates;
	for (int i = 0; i < nrStates; ++i) {
		std::string state;
		buffer >> state;
		Q.insert(state);
	}
	int alphaDim;
	buffer >> alphaDim;
	for (int i = 0; i < alphaDim; ++i) {
		char letter;
		buffer >> letter;
		S.insert(letter);
	}
	int transCount;
	buffer >> transCount;
	for (int i = 0; i < transCount; ++i) {
		std::string initState, finState;
		char rule;
		buffer >> initState >> rule >> finState;
		if (rule == '~') {
			hasLambdaTransitions = true;
		}
		if (d.find(std::make_pair(initState, rule)) == d.end()) {
			std::set<std::string> finStates;
			finStates.insert(finState);
			d.emplace(std::make_pair(initState, rule), finStates);
		}
		else {
			hasNondeterminism = true;
			d.at(std::make_pair(initState, rule)).insert(finState);
		}
	}
	int finStateCount;
	buffer >> q0;
	buffer >> finStateCount;
	for (int i = 0; i < finStateCount; ++i) {
		std::string finState;
		buffer >> finState;
		F.insert(finState);
	}
	std::cout << "Generated Regular Automata!\n";
	printConfiguration();
}

RegularAutomata::RegularAutomata(const RegularGrammar& grammar) {
	hasNondeterminism = false;
	hasLambdaTransitions = false;
	std::map<char, std::string> NonTermToState;
	NonTermToState.emplace(grammar.S, "q0");
	Q.insert("q0");
	q0 = "q0";
	std::string stateCode = "q";
	int currentState = 1;
	for (const auto currentNonTerm : grammar.N) {
		if (NonTermToState.find(currentNonTerm) == NonTermToState.end()) {
			Q.insert(stateCode + std::to_string(currentState));
			NonTermToState.emplace(currentNonTerm, stateCode + std::to_string(currentState));
			++currentState;
		}
	}
	S = grammar.T;
	for (const auto currentProduction : grammar.P) {
		std::string initState = NonTermToState.at(currentProduction.first);
		for (const auto rightHandSide : currentProduction.second) {
			char rule = rightHandSide.first;
			if (rule != '~' && rightHandSide.second != 0) {
				std::string destState = NonTermToState.at(rightHandSide.second);
				if (d.find(std::make_pair(initState, rule)) == d.end()) {
					std::set<std::string> destStates;
					destStates.insert(destState);
					d.emplace(std::make_pair(initState, rule), destStates);
				}
				else {
					hasNondeterminism = true;
					d.at(std::make_pair(initState, rule)).insert(destState);
				}
			}
			else {
				if (rule != '~') {
					std::string destState;
					std::cout << "Input destination state (non-member of Q -> new state) manually for tranzition: d(" << initState << "," << rule << ")=";
					std::cin >> destState;
					if (d.find(std::make_pair(initState, rule)) == d.end()) {
						std::set<std::string> destStates;
						destStates.insert(destState);
						d.emplace(std::make_pair(initState, rule), destStates);
					}
					else {
						hasNondeterminism = true;
						d.at(std::make_pair(initState, rule)).insert(destState);
					}
					Q.insert(destState);
					F.insert(destState);
				}
				else {
					F.insert(initState);
				}
			}
		}
	}
	std::cout << "Generated Regular Automata!\n";
	printConfiguration();
}

std::set<std::string> RegularAutomata::lambdaClosure(const std::string state) const {
	std::set<std::string> closure;
	closure.insert(state);
	unsigned int cardinal = 0;
	while (cardinal != closure.size()) {
		cardinal = closure.size();
		std::set<std::string> nextClosure = closure;
		for (const auto currentState : nextClosure) {
			if (d.find(std::make_pair(currentState, '~')) != d.end()) {
				std::set<std::string> nextStates = d.at(std::make_pair(currentState, '~'));
				nextClosure.insert(nextStates.begin(), nextStates.end());
			}
		}
		closure = nextClosure;
	}
	return closure;
}

bool RegularAutomata::Evaluate(const std::string word) const {
	if (word.size() == 1 && word.at(0) == '~' && F.find(q0) != F.end())
		return true;
	std::set<std::string> currentStates;
	if (hasLambdaTransitions) {
		currentStates = lambdaClosure(q0);
	}
	else {
		currentStates.insert(q0);
	}
	for (const auto letter : word) {
		std::set<std::string> nextStates;
		for (const auto state : currentStates) {
			std::map<std::pair<std::string, char>, std::set<std::string>>::const_iterator transition = d.find(std::make_pair(state, letter));
			if (transition != d.end()) {
				std::set<std::string> transitions = d.at(std::make_pair(state, letter));
				nextStates.insert(transitions.begin(), transitions.end());
			}
		}
		currentStates = nextStates;
		if (hasLambdaTransitions) {
			std::set<std::string> currentClosure = currentStates;
			for (const auto state : currentClosure) {
				std::set<std::string> closure = lambdaClosure(state);
				currentStates.insert(closure.begin(), closure.end());
			}
		}
	}
	for (const auto state : currentStates) {
		if (F.find(state) != F.end())
			return true;
	}
	return false;
}

void RegularAutomata::printConfiguration() const {
	std::cout << "-----\nMachine configuration:\n\n";
	if (hasNondeterminism && hasLambdaTransitions)
		std::cout << "The machine contains nondeterminism and lambda-transitions!\n";
	else {
		if (hasNondeterminism)
			std::cout << "The machine contains nondeterminism!\n";
		else {
			if(hasLambdaTransitions)
				std::cout << "The machine contains lambda-transitions!\n";
		}
	}
	std::cout << "Number of states: " << Q.size() << "\n";
	std::cout << "States: ";
	for (const auto state : Q)
		std::cout << state << " ";
	std::cout << "\nInitial state: " << q0 << "\n";
	std::cout << "Number of final states: " << F.size() << "\n";
	std::cout << "Final states: ";
	for (const auto state : F)
		std::cout << state << " ";
	std::cout << "\nAlphabet dimension: " << S.size() << "\n";
	std::cout << "Letters: ";
	for (const auto letter : S)
		std::cout << letter << " ";
	std::cout << "\nTransition count: " << d.size() << "\n";
	std::cout << "Transitions:\n";
	for (const auto transition : d) {
		if (!hasNondeterminism)
			for (const auto state : transition.second)
				std::cout << "d(" << transition.first.first << "," << transition.first.second << ")=" << state << "\n";
		else {
			std::cout << "d(" << transition.first.first << "," << transition.first.second << ")={";
			for (const auto state : transition.second) {
				std::cout << state;
				if(++transition.second.find(state) != transition.second.end())
					std::cout << ",";
			}
			std::cout << "}\n";
		}
	}
	std::cout << "\n-----\n";
}

void RegularAutomata::convertToDFA() {

}

RegularGrammar::RegularGrammar() {
	int cardinal;
	std::cout << "Number of nonterminals: ";
	std::cin >> cardinal;
	std::cout << "Nonterminals: ";
	for (int i = 0; i < cardinal; ++i) {
		char nonterminal;
		std::cin >> nonterminal;
		N.insert(nonterminal);
	}
	std::cout << "Number of terminals: ";
	std::cin >> cardinal;
	std::cout << "Terminals: ";
	for (int i = 0; i < cardinal; ++i) {
		char terminal;
		std::cin >> terminal;
		T.insert(terminal);
	}
	std::cout << "Axiom: ";
	std::cin >> S;
	std::cout << "Number of productions: ";
	std::cin >> cardinal;
	std::cout << "Productions: (Format: Nonterminal->~ / Nonterminal->terminal / Nonterminal->terminalNonterminal)\n";

	for (int i = 0; i < cardinal; ++i) {
		std::string production;
		std::cin >> production;
		std::istringstream prodStream(production);
		char initNonterm, Term, destNonterm = 0;
		prodStream >> initNonterm;
		prodStream.ignore(2, '>');
		prodStream >> Term >> destNonterm;
		if (P.find(initNonterm) == P.end()) {
			std::set<std::pair<char, char>> auxSet;
			auxSet.emplace(Term, destNonterm);
			P.at(initNonterm) = auxSet;
		}
		else {
			P.at(initNonterm).emplace(Term, destNonterm);
		}
	}
	std::cout << "Generated Regular Grammar!\n";
}

RegularGrammar::RegularGrammar(std::ifstream & buffer) {
	int cardinal;
	buffer >> cardinal;
	for (int i = 0; i < cardinal; ++i) {
		char nonterminal;
		buffer >> nonterminal;
		N.insert(nonterminal);
	}
	buffer >> cardinal;
	for (int i = 0; i < cardinal; ++i) {
		char terminal;
		buffer >> terminal;
		T.insert(terminal);
	}
	buffer >> S;
	buffer >> cardinal;

	for (int i = 0; i < cardinal; ++i) {
		std::string production;
		buffer >> production;
		std::istringstream prodStream(production);
		char initNonterm, Term, destNonterm = 0;
		prodStream >> initNonterm;
		prodStream.ignore(2, '>');
		prodStream >> Term >> destNonterm;
		if (P.find(initNonterm) == P.end()) {
			std::set<std::pair<char, char>> auxSet;
			auxSet.emplace(Term, destNonterm);
			P.emplace(initNonterm, auxSet);
		}
		else {
			P.at(initNonterm).emplace(Term, destNonterm);
		}
	}
	std::cout << "Generated Regular Grammar!\n";
	printConfiguration();
}

RegularGrammar::RegularGrammar(const RegularAutomata& automata) {
	std::map<std::string, char> StateToNonTerm;
	StateToNonTerm.emplace(automata.q0, 'S');
	N.insert('S');
	S = 'S';
	char NonTermCode = 'A';
	int currentNonTerm = 0;
	for (const auto state : automata.Q) {
		if (StateToNonTerm.find(state) == StateToNonTerm.end()) {
			N.insert(NonTermCode + currentNonTerm);
			StateToNonTerm.emplace(state, NonTermCode + currentNonTerm);
			++currentNonTerm;
		}
	}
	T = automata.S;
	for (const auto currentTransition : automata.d) {
		char initNonTerm = StateToNonTerm.at(currentTransition.first.first);
		if (P.find(initNonTerm) == P.end()) {
			std::set<std::pair<char, char>> auxSet;
			P.emplace(initNonTerm, auxSet);
		}
		char Term = currentTransition.first.second;
		for (const auto rightHandSide : currentTransition.second) {
			char destNonTerm = StateToNonTerm.at(rightHandSide);
			P.at(initNonTerm).emplace(Term, destNonTerm);
			if (automata.F.find(rightHandSide) != automata.F.end()) {
				if (P.find(destNonTerm) == P.end()) {
					std::set<std::pair<char, char>> auxSet;
					P.emplace(destNonTerm, auxSet);
				}
				P.at(destNonTerm).emplace('~', 0);
			}
		}
	}
	std::cout << "Generated Regular Automata!\n";
	printConfiguration();
}

bool RegularGrammar::Evaluate(const std::string word) const {
	std::set<char> currentNonterms;
	currentNonterms.insert(S);
	for (const auto letter : word) {
		std::set<char> nextNonterms;
		for (const auto production : P) {
			char initNonterm = production.first;
			for (const auto rightHandSide : production.second) {
				char terminal = rightHandSide.first;
				char destNonterm = rightHandSide.second;
				if (currentNonterms.find(initNonterm) != currentNonterms.end() && terminal == letter) {
					nextNonterms.insert(destNonterm);
				}
			}
		}
		currentNonterms = nextNonterms;
	}
	for (const auto nonterm : currentNonterms) {
		if (nonterm == 0 || P.at(nonterm).find(std::make_pair('~', 0)) != P.at(nonterm).end())
			return true;
	}
	return false;
}

void RegularGrammar::printConfiguration() const {
	std::cout << "-----\nGrammar configuration:\n\n";
	std::cout << "Number of nonterminals: " << N.size() << '\n';
	std::cout << "Nonterminals: ";
	for (const auto nonterminal : N) {
		std::cout << nonterminal << ' ';
	}
	std::cout << "\nNumber of terminals: " << T.size() << '\n';
	std::cout << "Terminals: ";
	for (const auto terminal : T) {
		std::cout << terminal << ' ';
	}
	std::cout << "\nAxiom: " << S << '\n';
	std::cout << "Productions count: " << P.size() << '\n';
	std::cout << "Productions:\n";
	for (const auto production : P) {
		std::cout << production.first << "->";
		for (const auto rightHandSide : production.second) {
			std::cout << rightHandSide.first;
			if(rightHandSide.second != 0)
				std::cout<<rightHandSide.second;
			if (++production.second.find(rightHandSide) != production.second.end())
				std::cout << '|';
		}
		std::cout << '\n';
	}
	std::cout << "-----\n";
}

PushDownAutomata::PushDownAutomata() {
	hasNondeterminism = false;
	hasLambdaTransitions = false;
	int nrStates;
	std::cout << "Number of states: ";
	std::cin >> nrStates;
	std::cout << "States: ";
	for (int i = 0; i < nrStates; ++i) {
		std::string state;
		std::cin >> state;
		Q.insert(state);
	}
	bool validInitState;
	do {
		std::cout << "Initial state: ";
		validInitState = true;
		std::cin >> q0;
		if (Q.find(q0) == Q.end())
			validInitState = false;
		if (!validInitState)
			std::cout << "Invalid initial state!\n";
	} while (!validInitState);
	int finStateCount;
	std::cout << "Number of final states: ";
	std::cin >> finStateCount;
	std::cout << "Final states: ";
	for (int i = 0; i < finStateCount; ++i) {
		std::string finState;
		bool validFinState;
		do {
			validFinState = true;
			std::cin >> finState;
			if (Q.find(finState) == Q.end())
				validFinState = false;
			if (!validFinState)
				std::cout << "Invalid final state! <" << finState << ">\n";
		} while (!validFinState);
		F.insert(finState);
	}
	int alphaDim;
	std::cout << "Alphabet dimension: ";
	std::cin >> alphaDim;
	std::cout << "Letters: ";
	for (int i = 0; i < alphaDim; ++i) {
		char letter;
		std::cin >> letter;
		S.insert(letter);
	}
	int stackAlphaDim;
	std::cout << "Stack Alphabet dimension: ";
	std::cin >> stackAlphaDim;
	std::cout << "Characters: ";
	for (int i = 0; i < stackAlphaDim; ++i) {
		char character;
		std::cin >> character;
		G.insert(character);
	}
	bool validInitStackCharacter;
	do {
		std::cout << "Initial stack character: ";
		validInitStackCharacter = true;
		std::cin >> Z0;
		if (G.find(Z0) == G.end())
			validInitStackCharacter = false;
		if (!validInitStackCharacter)
			std::cout << "Invalid initial stack character!\n";
	} while (!validInitStackCharacter);
	int transCount;
	std::cout << "Transition count: ";
	std::cin >> transCount;
	std::cout << "Transitions:  (Format: initialState letter stack-top resultState stack-push; For lambda transitions use '~'!)\n";
	for (int i = 0; i < transCount; ++i) {
		std::string initState, finState, stackPush;
		char letter, stackTop;
		bool validTransition;
		do {
			validTransition = true;
			std::cin >> initState >> letter >> stackTop >> finState >> stackPush;
			if (Q.find(initState) == Q.end() || (S.find(letter) == S.end() && letter != '~') || G.find(stackTop) == G.end() || Q.find(finState) == Q.end())
				validTransition = false;
			for (const auto character : stackPush)
				if (G.find(character) == G.end()) {
					validTransition = false;
					break;
				}
			if (!validTransition)
				std::cout << "Invalid transition!\n";
		} while (!validTransition);
		if (letter == '~') {
			hasLambdaTransitions = true;
		}
		if (d.find(std::make_tuple(initState, letter, stackTop)) == d.end()) {
			std::map<std::string, std::set<std::string>> finConfigurations;
			std::set<std::string> auxSet;
			auxSet.insert(stackPush);
			finConfigurations.emplace(finState, auxSet);
			d.emplace(std::make_tuple(initState, letter, stackTop), finConfigurations);
		}
		else {
			hasNondeterminism = true;
			if (d.at(std::make_tuple(initState, letter, stackTop)).find(finState) == d.at(std::make_tuple(initState, letter, stackTop)).end()) {
				std::set<std::string> auxSet;
				auxSet.insert(stackPush);
				d.at(std::make_tuple(initState, letter, stackTop)).emplace(finState, auxSet);
			}
			else {
				d.at(std::make_tuple(initState, letter, stackTop)).at(finState).insert(stackPush);
			}
		}
	}
	std::cout << "Generated PushDown Automata!\n";
}

PushDownAutomata::PushDownAutomata(std::ifstream & buffer) {
	hasNondeterminism = false;
	hasLambdaTransitions = false;
	int nrStates;
	buffer >> nrStates;
	for (int i = 0; i < nrStates; ++i) {
		std::string state;
		buffer >> state;
		Q.insert(state);
	}
	buffer >> q0;
	int finStateCount;
	buffer >> finStateCount;
	for (int i = 0; i < finStateCount; ++i) {
		std::string finState;
		buffer >> finState;
		F.insert(finState);
	}
	int alphaDim;
	buffer >> alphaDim;
	for (int i = 0; i < alphaDim; ++i) {
		char letter;
		buffer >> letter;
		S.insert(letter);
	}
	int stackAlphaDim;
	buffer >> stackAlphaDim;
	for (int i = 0; i < stackAlphaDim; ++i) {
		char character;
		buffer >> character;
		G.insert(character);
	}
	buffer >> Z0;
	int transCount;
	buffer >> transCount;
	for (int i = 0; i < transCount; ++i) {
		std::string initState, finState, stackPush;
		char letter, stackTop;
		buffer >> initState >> letter >> stackTop >> finState >> stackPush;
		if (letter == '~') {
			hasLambdaTransitions = true;
		}
		if (d.find(std::make_tuple(initState, letter, stackTop)) == d.end()) {
			std::map<std::string, std::set<std::string>> finConfigurations;
			std::set<std::string> auxSet;
			auxSet.insert(stackPush);
			finConfigurations.emplace(finState, auxSet);
			d.emplace(std::make_tuple(initState, letter, stackTop), finConfigurations);
		}
		else {
			hasNondeterminism = true;
			if (d.at(std::make_tuple(initState, letter, stackTop)).find(finState) == d.at(std::make_tuple(initState, letter, stackTop)).end()) {
				std::set<std::string> auxSet;
				auxSet.insert(stackPush);
				d.at(std::make_tuple(initState, letter, stackTop)).emplace(finState, auxSet);
			}
			else {
				d.at(std::make_tuple(initState, letter, stackTop)).at(finState).insert(stackPush);
			}
		}
	}
	std::cout << "Generated PushDown Automata!\n";
	printConfiguration();
}

std::map<std::string, std::set<std::stack<char>>> PushDownAutomata::lambdaClosure(const std::pair<std::string, std::set<std::stack<char>>> initConfiguration) const {
	std::map<std::string, std::set<std::stack<char>>> closureConfigurations;
	closureConfigurations.emplace(initConfiguration.first, initConfiguration.second);
	unsigned int cardinal = 0;
	while (cardinal != closureConfigurations.size()) {
		cardinal = closureConfigurations.size();
		auto nextClosureConfigurations = closureConfigurations;
		for (const auto currentConfiguration : closureConfigurations) {
			for (const auto currentStack : currentConfiguration.second) {
				if (currentStack.empty())
					continue;
				auto nextConfigurations = d.find(std::make_tuple(currentConfiguration.first, '~', currentStack.top()));
				if (nextConfigurations != d.end()) {
					for (const auto nextConfiguration : nextConfigurations->second) {
						if (nextClosureConfigurations.find(nextConfiguration.first) == nextClosureConfigurations.end()) {
							std::set<std::stack<char>> auxSet;
							nextClosureConfigurations.emplace(nextConfiguration.first, auxSet);
						}
						for (const auto stackPush : nextConfiguration.second) {
							std::stack<char> nextStack = currentStack;
							nextStack.pop();
							if (stackPush.at(0) != '~')
								for (auto character = stackPush.crbegin(); character != stackPush.crend(); ++character)
									nextStack.push(*character); 
							nextClosureConfigurations.at(nextConfiguration.first).emplace(nextStack);
						}
					}
				}
			}
		}
		closureConfigurations = nextClosureConfigurations;
	}
	return closureConfigurations;
}

bool PushDownAutomata::Evaluate(const std::string word) const {
	std::map<std::string, std::set<std::stack<char>>> currentConfigurations;
	std::stack<char> auxStack;
	auxStack.push(Z0);
	std::set<std::stack<char>> auxSet;
	auxSet.emplace(auxStack);
	if (hasLambdaTransitions) {
		currentConfigurations = lambdaClosure(std::make_pair(q0, auxSet));
	}
	else {
		currentConfigurations.emplace(q0, auxSet);
	}
	for (const auto letter : word) {
		std::map<std::string, std::set<std::stack<char>>> nextConfigurations;
		for (const auto configuration : currentConfigurations) {
			for (const auto currentStack : configuration.second) {
				if (currentStack.empty())
					continue;
				auto transition = d.find(std::make_tuple(configuration.first, letter, currentStack.top()));
				if (transition != d.end()) {
					for (const auto nextConfiguration : transition->second) {
						if (nextConfigurations.find(nextConfiguration.first) == nextConfigurations.end()) {
							std::set<std::stack<char>> auxSet;
							nextConfigurations.emplace(nextConfiguration.first, auxSet);
						}
						for (const auto stackPush : nextConfiguration.second) {
							std::stack<char> auxStack = currentStack;
							auxStack.pop();
							if (stackPush.at(0) != '~')
								for (std::string::const_reverse_iterator character = stackPush.crbegin(); character != stackPush.crend(); ++character)
									auxStack.push(*character);
							nextConfigurations.at(nextConfiguration.first).emplace(auxStack);
						}
					}
				}
			}
		}
		currentConfigurations = nextConfigurations;
		if (hasLambdaTransitions) {
			auto currentClosureConfigurations = currentConfigurations;
			for (const auto closureConfiguration : currentClosureConfigurations) {
				auto nextClosureConfigurations = lambdaClosure(closureConfiguration);
				for (const auto nextClosureConfiguration : nextClosureConfigurations) {
					if (currentConfigurations.find(nextClosureConfiguration.first) == currentConfigurations.end()) {
						std::set<std::stack<char>> auxSet;
						currentConfigurations.emplace(nextClosureConfiguration.first, auxSet);
					}
					currentConfigurations.at(nextClosureConfiguration.first).insert(nextClosureConfiguration.second.begin(), nextClosureConfiguration.second.end());
				}
			}
		}
	}
	for (const auto configuration : currentConfigurations) {
		if (F.find(configuration.first) != F.end())
			return true;
	}
	return false;
}

void PushDownAutomata::printConfiguration() const {
	std::cout << "-----\nMachine configuration:\n\n";
	if (hasNondeterminism && hasLambdaTransitions)
		std::cout << "The machine contains nondeterminism and lambda-transitions!\n";
	else {
		if (hasNondeterminism)
			std::cout << "The machine contains nondeterminism!\n";
		else {
			if (hasLambdaTransitions)
				std::cout << "The machine contains lambda-transitions!\n";
		}
	}
	std::cout << "Number of states: " << Q.size() << "\n";
	std::cout << "States: ";
	for (const auto state : Q)
		std::cout << state << " ";
	std::cout << "Initial state: " << q0 << "\n";
	std::cout << "Number of final states: " << F.size() << "\n";
	std::cout << "Final states: ";
	for (const auto state : F)
		std::cout << state << " ";
	std::cout << "\nAlphabet dimension: " << S.size() << "\n";
	std::cout << "Letters: ";
	for (const auto letter : S)
		std::cout << letter << " ";
	std::cout << "\nStack alphabet dimension: " << G.size() << "\n";
	std::cout << "Characters: ";
	for (const auto character : G)
		std::cout << character << " ";
	std::cout << "\nInitial std::stack character: " << Z0 << "\n";
	std::cout << "Transition count: " << d.size() << "\n";
	std::cout << "Transitions:\n";
	for (const auto transition : d) {
		if (!hasNondeterminism)
			for (const auto configuration : transition.second)
				for (const auto stackPush : configuration.second)
					std::cout << "d(" << std::get<0>(transition.first) << "," << std::get<1>(transition.first) << "," << std::get<2>(transition.first) << ")=(" << configuration.first << "," << stackPush << ")\n";
		else {
			std::cout << "d(" << std::get<0>(transition.first) << "," << std::get<1>(transition.first) << "," << std::get<2>(transition.first) << ")={";
			for (const auto configuration : transition.second) {
				for (const auto stackPush : configuration.second) {
					std::cout << "(" << configuration.first << "," << stackPush << ")";
					if (++configuration.second.find(stackPush) != configuration.second.end())
						std::cout << ",";
				}
				if (++transition.second.find(configuration.first) != transition.second.end())
					std::cout << ",";
			}
			std::cout << "}\n";
		}
	}
	std::cout << "\n-----\n";
}