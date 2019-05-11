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

RegularAutomata::RegularAutomata(RegularGrammar& grammar) {
	hasNondeterminism = false;
	hasLambdaTransitions = false;
	std::map<char, std::string> NonTermToState;
	NonTermToState.insert(std::make_pair(grammar.S, "q0"));
	Q.insert("q0");
	std::string stateCode = "q";
	int currentState = 1;
	for (std::set<char>::iterator currentNonTerm = grammar.N.begin(); currentNonTerm != grammar.N.end(); ++currentNonTerm) {
		if (NonTermToState.find(*currentNonTerm) == NonTermToState.end()) {
			Q.insert(stateCode + std::to_string(currentState));
			NonTermToState.insert(std::make_pair(*currentNonTerm, stateCode + std::to_string(currentState)));
			++currentState;
		}
	}
	S = grammar.T;
	q0 = grammar.S;
	for (std::set<std::tuple<char, char, char>>::iterator currentProduction = grammar.P.begin(); currentProduction != grammar.P.end(); ++currentProduction) {
		std::string initState = NonTermToState.at(std::get<0>(*currentProduction));
		char rule = std::get<1>(*currentProduction);
		if (rule != '~' && std::get<2>(*currentProduction) != 0) {
			std::string destState = NonTermToState.at(std::get<2>(*currentProduction));
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
				std::cout << "Input destination state (member of Q) manually for tranzition: d(" << initState << "," << rule << ")=";
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
				F.insert(destState);
			}
			else {
				F.insert(initState);
			}
		}
	}
	std::cout << "Generated Regular Automata!\n";
	printConfiguration();
}

std::set<std::string> RegularAutomata::lambdaClosure(std::string state) {
	std::set<std::string> closure;
	closure.insert(state);
	unsigned int cardinal = 0;
	while (cardinal != closure.size()) {
		cardinal = closure.size();
		std::set<std::string> nextClosure = closure;
		for (std::set<std::string>::iterator currentState = nextClosure.begin(); currentState != nextClosure.end(); ++currentState) {
			if (d.find(std::make_pair(*currentState, '~')) != d.end()) {
				std::set<std::string> nextStates = d.at(std::make_pair(*currentState, '~'));
				nextClosure.insert(nextStates.begin(), nextStates.end());
			}
		}
		closure = nextClosure;
	}
	return closure;
}

bool RegularAutomata::Evaluate(std::string word) {
	std::set<std::string> currentStates;
	if (hasLambdaTransitions) {
		currentStates = lambdaClosure(q0);
	}
	else {
		currentStates.insert(q0);
	}
	for (std::string::iterator letter = word.begin(); letter != word.end(); ++letter) {
		std::set<std::string> nextStates;
		for (std::set<std::string>::iterator state = currentStates.begin(); state != currentStates.end(); ++state) {
			std::map<std::pair<std::string, char>, std::set<std::string>>::iterator transition = d.find(std::make_pair(*state, *letter));
			if (transition != d.end()) {
				std::set<std::string> transitions = d.at(std::make_pair(*state, *letter));
				nextStates.insert(transitions.begin(), transitions.end());
			}
		}
		currentStates = nextStates;
		if (hasLambdaTransitions) {
			std::set<std::string> currentClosure = currentStates;
			for (std::set<std::string>::iterator state = currentClosure.begin(); state != currentClosure.end(); ++state) {
				std::set<std::string> closure = lambdaClosure(*state);
				currentStates.insert(closure.begin(), closure.end());
			}
		}
		if (currentStates.empty())
			return false;
	}
	for (std::set<std::string>::iterator state = currentStates.begin(); state != currentStates.end(); ++state) {
		if (F.find(*state) != F.end())
			return true;
	}
	return false;
}

void RegularAutomata::printConfiguration() {
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
	for (std::set<std::string>::iterator state = Q.begin(); state != Q.end(); ++state)
		std::cout << *state << " ";
	std::cout << "\nAlphabet dimension: " << S.size() << "\n";
	std::cout << "Letters: ";
	for (std::set<char>::iterator letter = S.begin(); letter != S.end(); ++letter)
		std::cout << *letter << " ";
	std::cout << "\nTransition count: " << d.size() << "\n";
	std::cout << "Transitions:\n";
	for (std::map<std::pair<std::string, char>, std::set<std::string>>::iterator transition = d.begin(); transition != d.end(); ++transition) {
		if (!hasNondeterminism)
			for (std::set<std::string>::iterator state = transition->second.begin(); state != transition->second.end(); ++state)
				std::cout << "d(" << transition->first.first << "," << transition->first.second << ")=" << *state << "\n";
		else {
			std::cout << "d(" << transition->first.first << "," << transition->first.second << ")={";
			for (std::set<std::string>::iterator state = transition->second.begin(); state != transition->second.end();) {
				std::cout << *state;
				if (++state != (*transition).second.end())
					std::cout << ",";
			}
			std::cout << "}\n";
		}
	}
	std::cout << "Initial state: " << q0 << "\n";
	std::cout << "Number of final states: " << F.size() << "\n";
	std::cout << "Final states: ";
	for (std::set<std::string>::iterator state = F.begin(); state != F.end(); ++state)
		std::cout << *state << " ";
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
		P.insert(std::make_tuple(initNonterm, Term, destNonterm));
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
		P.insert(std::make_tuple(initNonterm, Term, destNonterm));
	}
	std::cout << "Generated Regular Grammar!\n";
	printConfiguration();
}

bool RegularGrammar::Evaluate(std::string word) {
	std::set<char> currentNonterms;
	currentNonterms.insert(S);
	for (std::string::iterator letter = word.begin(); letter != word.end(); ++letter) {
		std::set<char> nextNonterms;
		for (std::set<std::tuple<char, char, char>>::iterator production = P.begin(); production != P.end(); ++production) {
			char initNonterm = std::get<0>(*production);
			char terminal = std::get<1>(*production);
			char destNonterm = std::get<2>(*production);
			if (currentNonterms.find(initNonterm) != currentNonterms.end() && terminal == *letter) {
				nextNonterms.insert(destNonterm);
			}
		}
		currentNonterms = nextNonterms;
	}
	for (std::set<char>::iterator nonterm = currentNonterms.begin(); nonterm != currentNonterms.end(); ++nonterm) {
		if (*nonterm == 0 || P.find(std::make_tuple(*nonterm, '~', 0)) != P.end())
			return true;
	}
	return false;
}

void RegularGrammar::printConfiguration() {
	std::cout << "-----\nGrammar configuration:\n\n";
	std::cout << "Number of nonterminals: " << N.size() << '\n';
	std::cout << "Nonterminals: ";
	for (std::set<char>::iterator nonterminal = N.begin(); nonterminal != N.end(); ++nonterminal) {
		std::cout << *nonterminal << ' ';
	}
	std::cout << "\nNumber of terminals: " << T.size() << '\n';
	std::cout << "Terminals: ";
	for (std::set<char>::iterator terminal = T.begin(); terminal != T.end(); ++terminal) {
		std::cout << *terminal << ' ';
	}
	std::cout << "\nAxiom: " << S << '\n';
	std::cout << "Productions count: " << P.size() << '\n';
	std::cout << "Productions:\n";
	for (std::set<std::tuple<char, char, char>>::iterator production = P.begin(); production != P.end(); ++production) {
		std::cout << std::get<0>(*production) << "->" << std::get<1>(*production) << std::get<2>(*production) << '\n';
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
			for (std::string::iterator character = stackPush.begin(); character != stackPush.end() && *character != '~'; ++character)
				if (G.find(*character) == G.end()) {
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
			finConfigurations.insert(std::make_pair(finState, auxSet));
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
			finConfigurations.insert(std::make_pair(finState, auxSet));
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

std::map<std::string, std::set<std::stack<char>>> PushDownAutomata::lambdaClosure(std::pair<std::string, std::set<std::stack<char>>> initConfiguration) {
	std::map<std::string, std::set<std::stack<char>>> closureConfigurations;
	closureConfigurations.emplace(initConfiguration.first, initConfiguration.second);
	unsigned int cardinal = 0;
	while (cardinal != closureConfigurations.size()) {
		cardinal = closureConfigurations.size();
		std::map<std::string, std::set<std::stack<char>>> nextClosureConfigurations = closureConfigurations;
		for (std::map<std::string, std::set<std::stack<char>>>::iterator currentConfiguration = closureConfigurations.begin(); currentConfiguration != closureConfigurations.end(); ++currentConfiguration) {
			for (std::set<std::stack<char>>::iterator currentStack = currentConfiguration->second.begin(); currentStack != currentConfiguration->second.end(); ++currentStack) {
				if (currentStack->empty())
					continue;
				std::map<std::tuple<std::string, char, char>, std::map<std::string, std::set<std::string>>>::iterator nextConfigurations = d.find(std::make_tuple(currentConfiguration->first, '~', currentStack->top()));
				if (nextConfigurations != d.end()) {
					for (std::map<std::string, std::set<std::string>>::iterator nextConfiguration = nextConfigurations->second.begin(); nextConfiguration != nextConfigurations->second.end(); ++nextConfiguration) {
						if (nextClosureConfigurations.find(nextConfiguration->first) == nextClosureConfigurations.end()) {
							std::set<std::stack<char>> auxSet;
							nextClosureConfigurations.emplace(nextConfiguration->first, auxSet);
						}
						for (std::set<std::string>::iterator stackPush = nextConfiguration->second.begin(); stackPush != nextConfiguration->second.end(); ++stackPush) {
							std::stack<char> nextStack = *currentStack;
							nextStack.pop();
							if (stackPush->at(0) != '~')
								for (std::string::const_reverse_iterator character = stackPush->rbegin(); character != stackPush->rend(); ++character)
									nextStack.push(*character); 
							nextClosureConfigurations.at(nextConfiguration->first).insert(nextStack);
						}
					}
				}
			}
		}
		closureConfigurations = nextClosureConfigurations;
	}
	return closureConfigurations;
}

bool PushDownAutomata::Evaluate(std::string word) {
	std::map<std::string, std::set<std::stack<char>>> currentConfigurations;
	std::stack<char> auxStack;
	auxStack.push(Z0);
	std::set<std::stack<char>> auxSet;
	auxSet.insert(auxStack);
	if (hasLambdaTransitions) {
		currentConfigurations = lambdaClosure(std::make_pair(q0, auxSet));
	}
	else {
		currentConfigurations.emplace(q0, auxSet);
	}
	for (std::string::iterator letter = word.begin(); letter != word.end() && !currentConfigurations.empty(); ++letter) {
		std::map<std::string, std::set<std::stack<char>>> nextConfigurations;
		for (std::map<std::string, std::set<std::stack<char>>>::iterator configuration = currentConfigurations.begin(); configuration != currentConfigurations.end(); ++configuration) {
			for (std::set<std::stack<char>>::iterator currentStack = configuration->second.begin(); currentStack != configuration->second.end(); ++currentStack) {
				if (currentStack->empty())
					continue;
				std::map<std::tuple<std::string, char, char>, std::map<std::string, std::set<std::string>>>::iterator transition = d.find(std::make_tuple(configuration->first, *letter, currentStack->top()));
				if (transition != d.end()) {
					for (std::map<std::string, std::set<std::string>>::iterator nextConfiguration = transition->second.begin(); nextConfiguration != transition->second.end(); ++nextConfiguration) {
						if (nextConfigurations.find(nextConfiguration->first) == nextConfigurations.end()) {
							std::set<std::stack<char>> auxSet;
							nextConfigurations.emplace(nextConfiguration->first, auxSet);
						}
						for (std::set<std::string>::iterator stackPush = nextConfiguration->second.begin(); stackPush != nextConfiguration->second.end(); ++stackPush) {
							std::stack<char> auxStack = *currentStack;
							auxStack.pop();
							if (stackPush->at(0) != '~')
								for (std::string::const_reverse_iterator character = stackPush->rbegin(); character != stackPush->rend(); ++character)
									auxStack.push(*character);
							nextConfigurations.at(nextConfiguration->first).insert(auxStack);
						}
					}
				}
			}
		}
		currentConfigurations = nextConfigurations;
		if (hasLambdaTransitions) {
			std::map<std::string, std::set<std::stack<char>>> currentClosureConfigurations = currentConfigurations;
			for (std::map<std::string, std::set<std::stack<char>>>::iterator closureConfiguration = currentClosureConfigurations.begin(); closureConfiguration != currentClosureConfigurations.end(); ++closureConfiguration) {
				std::map<std::string, std::set<std::stack<char>>> nextClosureConfigurations = lambdaClosure(*closureConfiguration);
				for (std::map<std::string, std::set<std::stack<char>>>::iterator nextClosureConfiguration = nextClosureConfigurations.begin(); nextClosureConfiguration != nextClosureConfigurations.end(); ++nextClosureConfiguration) {
					if (currentConfigurations.find(nextClosureConfiguration->first) == currentConfigurations.end()) {
						std::set<std::stack<char>> auxSet;
						currentConfigurations.emplace(nextClosureConfiguration->first, auxSet);
					}
					currentConfigurations.at(nextClosureConfiguration->first).insert(nextClosureConfiguration->second.begin(), nextClosureConfiguration->second.end());
				}
			}
		}
	}
	for (std::map<std::string, std::set<std::stack<char>>>::iterator configuration = currentConfigurations.begin(); configuration != currentConfigurations.end(); ++configuration) {
		if (F.find(configuration->first) != F.end())
			return true;
	}
	return false;
}

void PushDownAutomata::printConfiguration() {
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
	for (std::set<std::string>::iterator state = Q.begin(); state != Q.end(); ++state)
		std::cout << *state << " ";
	std::cout << "Initial state: " << q0 << "\n";
	std::cout << "Number of final states: " << F.size() << "\n";
	std::cout << "Final states: ";
	for (std::set<std::string>::iterator state = F.begin(); state != F.end(); ++state)
		std::cout << *state << " ";
	std::cout << "\nAlphabet dimension: " << S.size() << "\n";
	std::cout << "Letters: ";
	for (std::set<char>::iterator letter = S.begin(); letter != S.end(); ++letter)
		std::cout << *letter << " ";
	std::cout << "\nStack alphabet dimension: " << G.size() << "\n";
	std::cout << "Characters: ";
	for (std::set<char>::iterator character = G.begin(); character != G.end(); ++character)
		std::cout << *character << " ";
	std::cout << "\nInitial std::stack character: " << Z0 << "\n";
	std::cout << "Transition count: " << d.size() << "\n";
	std::cout << "Transitions:\n";
	for (std::map<std::tuple<std::string, char, char>, std::map<std::string, std::set<std::string>>>::iterator transition = d.begin(); transition != d.end(); ++transition) {
		if (!hasNondeterminism)
			for (std::map<std::string, std::set<std::string>>::iterator configuration = transition->second.begin(); configuration != transition->second.end(); ++configuration)
				for (std::set<std::string>::iterator stackPush = configuration->second.begin(); stackPush != configuration->second.end(); ++stackPush)
					std::cout << "d(" << std::get<0>(transition->first) << "," << std::get<1>(transition->first) << "," << std::get<2>(transition->first) << ")=(" << configuration->first << "," << *stackPush << ")\n";
		else {
			std::cout << "d(" << std::get<0>(transition->first) << "," << std::get<1>(transition->first) << "," << std::get<2>(transition->first) << ")={";
			for (std::map<std::string, std::set<std::string>>::iterator configuration = transition->second.begin(); configuration != transition->second.end(); ++configuration) {
				for (std::set<std::string>::iterator stackPush = configuration->second.begin(); stackPush != configuration->second.end(); ++stackPush) {
					std::cout << "(" << configuration->first << "," << *stackPush << ")";
					std::set<std::string>::iterator nextStackPush = stackPush;
					if (++nextStackPush != configuration->second.end())
						std::cout << ",";
				}
				std::map<std::string, std::set<std::string>>::iterator nextConfiguration = configuration;
				if (++nextConfiguration != transition->second.end())
					std::cout << ",";
			}
			std::cout << "}\n";
		}
	}
	std::cout << "\n-----\n";
}