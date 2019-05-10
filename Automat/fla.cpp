#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <map>
#include <string>
#include "fla.h"

using namespace std;

RegularAutomata::RegularAutomata() {
	hasNondeterminism = false;
	hasLambdaTransitions = false;
	int nrStates;
	cout << "Number of states: ";
	cin >> nrStates;
	cout << "States: ";
	for (int i = 0; i < nrStates; ++i) {
		string state;
		cin >> state;
		Q.insert(state);
	}
	int alphaDim;
	cout << "Alphabet dimension: ";
	cin >> alphaDim;
	cout << "Letters: ";
	for (int i = 0; i < alphaDim; ++i) {
		char letter;
		cin >> letter;
		S.insert(letter);
	}
	int transCount;
	cout << "Transition count: ";
	cin >> transCount;
	cout << "Transitions:  (format: initialState letter resultState !For lambda transitions use '~'!)\n";
	for (int i = 0; i < transCount; ++i) {
		string initState, finState;
		char rule;
		bool validTransition;
		do {
			validTransition = true;
			cin >> initState >> rule >> finState;
			if (Q.find(initState) == Q.end() || Q.find(finState) == Q.end() || (S.find(rule) == S.end() && rule != '~'))
				validTransition = false;
			if (!validTransition)
				cout << "Invalid transition!\n";
		} while (!validTransition);
		if (rule == '~') {
			hasLambdaTransitions = true;
		}
		if (d.find(make_pair(initState, rule)) == d.end()) {
			set<string> finStates;
			finStates.insert(finState);
			d.emplace(make_pair(initState, rule), finStates);
		}
		else {
			hasNondeterminism = true;
			d.at(make_pair(initState, rule)).insert(finState);
		}
	}
	int finStateCount;
	bool validInitState;
	do {
		cout << "Initial state: ";
		validInitState = true;
		cin >> q0;
		if (Q.find(q0) == Q.end())
			validInitState = false;
		if (!validInitState)
			cout << "Invalid initial state!\n";
	} while (!validInitState);
	cout << "Number of final states: ";
	cin >> finStateCount;
	cout << "Final states: ";
	for (int i = 0; i < finStateCount; ++i) {
		string finState;
		bool validFinState;
		do {
			validFinState = true;
			cin >> finState;
			if (Q.find(finState) == Q.end())
				validFinState = false;
			if (!validFinState)
				cout << "Invalid final state! <" << finState << ">\n";
		} while (!validFinState);
		F.insert(finState);
	}
	cout << "Generated RegularAutomata!\n";
}

RegularAutomata::RegularAutomata(ifstream &buffer) {
	hasNondeterminism = false;
	hasLambdaTransitions = false;
	int nrStates;
	buffer >> nrStates;
	for (int i = 0; i < nrStates; ++i) {
		string state;
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
		string initState, finState;
		char rule;
		buffer >> initState >> rule >> finState;
		if (rule == '~') {
			hasLambdaTransitions = true;
		}
		if (d.find(make_pair(initState, rule)) == d.end()) {
			set<string> finStates;
			finStates.insert(finState);
			d.emplace(make_pair(initState, rule), finStates);
		}
		else {
			hasNondeterminism = true;
			d.at(make_pair(initState, rule)).insert(finState);
		}
	}
	int finStateCount;
	buffer >> q0;
	buffer >> finStateCount;
	for (int i = 0; i < finStateCount; ++i) {
		string finState;
		buffer >> finState;
		F.insert(finState);
	}
	cout << "Generated RegularAutomata!\n";
	printConfiguration();
}

RegularAutomata::RegularAutomata(RegularGrammar &grammar) {
	hasNondeterminism = false;
	hasLambdaTransitions = false;
	map<char, string> NonTermToState;
	NonTermToState.insert(make_pair(grammar.S, "q0"));
	Q.insert("q0");
	string stateCode = "q";
	int currentState = 1;
	for (set<char>::iterator currentNonTerm = grammar.N.begin(); currentNonTerm != grammar.N.end(); ++currentNonTerm) {
		if (NonTermToState.find(*currentNonTerm) == NonTermToState.end()) {
			Q.insert(stateCode + to_string(currentState));
			NonTermToState.insert(make_pair(*currentNonTerm, stateCode + to_string(currentState)));
			++currentState;
		}
	}
	S = grammar.T;
	q0 = grammar.S;
	for (set<tuple<char, char, char>>::iterator currentProduction = grammar.P.begin(); currentProduction != grammar.P.end(); ++currentProduction) {
		string initState = NonTermToState.at(get<0>(*currentProduction));
		char rule = get<1>(*currentProduction);
		if (rule != '~' && get<2>(*currentProduction) != 0) {
			string destState = NonTermToState.at(get<2>(*currentProduction));
			if (d.find(make_pair(initState, rule)) == d.end()) {
				set<string> destStates;
				destStates.insert(destState);
				d.emplace(make_pair(initState, rule), destStates);
			}
			else {
				hasNondeterminism = true;
				d.at(make_pair(initState, rule)).insert(destState);
			}
		}
		else {
			if(rule != '~'){
				string destState;
				cout << "Input destination state (member of Q) manually for tranzition: d(" << initState << "," << rule << ")=";
				cin >> destState;
				if (d.find(make_pair(initState, rule)) == d.end()) {
					set<string> destStates;
					destStates.insert(destState);
					d.emplace(make_pair(initState, rule), destStates);
				}
				else {
					hasNondeterminism = true;
					d.at(make_pair(initState, rule)).insert(destState);
				}
				F.insert(destState);
			}
			else {
				F.insert(initState);
			}
		}
	}
	cout << "Generated RegularAutomata!\n";
	printConfiguration();
}

set<string> RegularAutomata::lambdaClosure(string state) {
	set<string> closure;
	closure.insert(state);
	int cardinal = 0;
	while (cardinal != closure.size()) {
		cardinal = closure.size();
		set<string> nextClosure = closure;
		for (set<string>::iterator currentState = nextClosure.begin(); currentState != nextClosure.end(); ++currentState) {
			if (d.find(make_pair(*currentState, '~')) != d.end()) {
				set<string> nextStates = d.at(make_pair(*currentState, '~'));
				nextClosure.insert(nextStates.begin(), nextStates.end());
			}
		}
		closure = nextClosure;
	}
	return closure;
}

bool RegularAutomata::Evaluate(string word) {
	set<string> currentStates;
	if (hasLambdaTransitions) {
		currentStates = lambdaClosure(q0);
	}
	else {
		currentStates.insert(q0);
	}
	for (string::iterator letter = word.begin(); letter != word.end(); ++letter) {
		set<string> nextStates;
		for (set<string>::iterator state = currentStates.begin(); state != currentStates.end(); ++state) {
			map<pair<string, char>, set<string>>::iterator transition = d.find(make_pair(*state, *letter));
			if (transition != d.end()) {
				set<string> transitions = d.at(make_pair(*state, *letter));
				nextStates.insert(transitions.begin(), transitions.end());
			}
		}
		currentStates = nextStates;
		if (hasLambdaTransitions) {
			set<string> currentClosure = currentStates;
			for (set<string>::iterator state = currentClosure.begin(); state != currentClosure.end(); ++state) {
				set<string> closure = lambdaClosure(*state);
				currentStates.insert(closure.begin(), closure.end());
			}
		}
		if (currentStates.empty())
			return false;
	}
	for (set<string>::iterator state = currentStates.begin(); state != currentStates.end(); ++state) {
		if (F.find(*state) != F.end())
			return true;
	}
	return false;
}

void RegularAutomata::printConfiguration() {
	cout << "-----\nMachine configuration:\n\n";
	if (hasNondeterminism && hasLambdaTransitions)
		cout << "The machine contains nondeterminism and lambda-transitions!\n";
	else {
		if (hasNondeterminism)
			cout << "The machine contains nondeterminism!\n";
		else
			cout << "The machine contains lambda-transitions!\n";
	}
	cout << "Number of states: " << Q.size() << "\n";
	cout << "States: ";
	for (set<string>::iterator state = Q.begin(); state != Q.end(); ++state)
		cout << *state << " ";
	cout << "\nAlphabet dimension: " << S.size() << "\n";
	cout << "Letters: ";
	for (set<char>::iterator letter = S.begin(); letter != S.end(); ++letter)
		cout << *letter << " ";
	cout << "\nTransition count: " << d.size() << "\n";
	cout << "Transitions:\n";
	for (map<pair<string, char>, set<string>>::iterator transition = d.begin(); transition != d.end(); ++transition) {
		if(!hasNondeterminism)
		    for (set<string>::iterator state = (*transition).second.begin(); state != (*transition).second.end(); ++state)
			    cout << "d(" << (*transition).first.first << "," << (*transition).first.second << ")=" << *state << "\n";
		else {
			cout << "d(" << (*transition).first.first << "," << (*transition).first.second << ")={";
			for (set<string>::iterator state = (*transition).second.begin(); state != (*transition).second.end();) {
				cout << *state;
				if (++state != (*transition).second.end())
					cout << ",";
			}
			cout << "}\n";
		}
	}
	cout << "Initial state: " << q0 << "\n";
	cout << "Number of final states: " << F.size() << "\n";
	cout << "Final states: ";
	for (set<string>::iterator state = F.begin(); state != F.end(); ++state)
		cout << *state << " ";
	cout << "\n-----\n";
}

void RegularAutomata::convertToDFA() {

}

RegularGrammar::RegularGrammar() {
	int cardinal;
	cout << "Number of nonterminals: ";
	cin >> cardinal;
	cout << "Nonterminals: ";
	for (int i = 0; i < cardinal; ++i) {
		char nonterminal;
		cin >> nonterminal;
		N.insert(nonterminal);
	}
	cout << "Number of terminals: ";
	cin >> cardinal;
	cout << "Terminals: ";
	for (int i = 0; i < cardinal; ++i) {
		char terminal;
		cin >> terminal;
		T.insert(terminal);
	}
	cout << "Axiom: ";
	cin >> S;
	cout << "Number of productions: ";
	cin >> cardinal;
	cout << "Productions: (Format: Nonterminal->~ / Nonterminal->terminal / Nonterminal->terminalNonterminal)\n";

	for (int i = 0; i < cardinal; ++i) {
		string production;
		cin >> production;
		istringstream prodStream(production);
		char initNonterm, Term, destNonterm = 0;
		prodStream >> initNonterm;
		prodStream.ignore(2, '>');
		prodStream >> Term >> destNonterm;
		P.insert(make_tuple(initNonterm, Term, destNonterm));
	}
	cout << "Generated grammar!\n";
}

RegularGrammar::RegularGrammar(ifstream &buffer) {
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
		string production;
		buffer >> production;
		istringstream prodStream(production);
		char initNonterm, Term, destNonterm = 0;
		prodStream >> initNonterm;
		prodStream.ignore(2, '>');
		prodStream >> Term >> destNonterm;
		P.insert(make_tuple(initNonterm, Term, destNonterm));
	}
	cout << "Generated Grammar!\n";
	printConfiguration();
}

bool RegularGrammar::Evaluate(string word) {
	set<char> currentNonterms;
	currentNonterms.insert(S);
	for (string::iterator letter = word.begin(); letter != word.end(); ++letter) {
		set<char> nextNonterms;
		for (set<tuple<char, char, char>>::iterator production = P.begin(); production != P.end(); ++production) {
			char initNonterm = get<0>(*production);
			char terminal = get<1>(*production);
			char destNonterm = get<2>(*production);
			if (currentNonterms.find(initNonterm) != currentNonterms.end() && terminal == *letter) {
				nextNonterms.insert(destNonterm);
			}
		}
		currentNonterms = nextNonterms;
	}
	for (set<char>::iterator nonterm = currentNonterms.begin(); nonterm != currentNonterms.end(); ++nonterm) {
		if (*nonterm == 0 || P.find(make_tuple(*nonterm, '~', 0)) != P.end())
			return true;
	}
	return false;
}

void RegularGrammar::printConfiguration() {
	cout << "-----\nGrammar configuration:\n\n";
	cout << "Number of nonterminals: " << N.size() << '\n';
	cout << "Nonterminals: ";
	for (set<char>::iterator nonterminal = N.begin(); nonterminal != N.end(); ++nonterminal) {
		cout << *nonterminal << ' ';
	}
	cout << "\nNumber of terminals: " << T.size() << '\n';
	cout << "Terminals: ";
	for (set<char>::iterator terminal = T.begin(); terminal != T.end(); ++terminal) {
		cout << *terminal << ' ';
	}
	cout << "\nAxiom: " << S << '\n';
	cout << "Productions count: " << P.size() << '\n';
	cout << "Productions:\n";
	for (set<tuple<char,char,char>>::iterator production = P.begin(); production != P.end(); ++production) {
		cout << get<0>(*production) << "->" << get<1>(*production) << get<2>(*production) << '\n';
	}
	cout << "-----\n";
}

PushDownAutomata::PushDownAutomata() {
	hasNondeterminism = false;
	hasLambdaTransitions = false;
	int nrStates;
	cout << "Number of states: ";
	cin >> nrStates;
	cout << "States: ";
	for (int i = 0; i < nrStates; ++i) {
		string state;
		cin >> state;
		Q.insert(state);
	}
	bool validInitState;
	do {
		cout << "Initial state: ";
		validInitState = true;
		cin >> q0;
		if (Q.find(q0) == Q.end())
			validInitState = false;
		if (!validInitState)
			cout << "Invalid initial state!\n";
	} while (!validInitState);
	int finStateCount;
	cout << "Number of final states: ";
	cin >> finStateCount;
	cout << "Final states: ";
	for (int i = 0; i < finStateCount; ++i) {
		string finState;
		bool validFinState;
		do {
			validFinState = true;
			cin >> finState;
			if (Q.find(finState) == Q.end())
				validFinState = false;
			if (!validFinState)
				cout << "Invalid final state! <" << finState << ">\n";
		} while (!validFinState);
		F.insert(finState);
	}
	int alphaDim;
	cout << "Alphabet dimension: ";
	cin >> alphaDim;
	cout << "Letters: ";
	for (int i = 0; i < alphaDim; ++i) {
		char letter;
		cin >> letter;
		S.insert(letter);
	}
	int stackAlphaDim;
	cout << "Stack Alphabet dimension: ";
	cin >> stackAlphaDim;
	cout << "Characters: ";
	for (int i = 0; i < alphaDim; ++i) {
		char character;
		cin >> character;
		G.insert(character);
	}
	bool validInitStackCharacter;
	do {
		cout << "Initial stack character: ";
		validInitStackCharacter = true;
		cin >> Z0;
		if (G.find(Z0) == G.end())
			validInitStackCharacter = false;
		if (!validInitStackCharacter)
			cout << "Invalid initial stack character!\n";
	} while (!validInitStackCharacter);
	int transCount;
	cout << "Transition count: ";
	cin >> transCount;
	cout << "Transitions:  (Format: initialState letter stack-top resultState stack-push; For lambda transitions use '~'!)\n";
	for (int i = 0; i < transCount; ++i) {
		string initState, finState, stackPush;
		char letter, stackTop;
		bool validTransition;
		do {
			validTransition = true;
			cin >> initState >> letter >> stackTop >> finState >> stackPush;
			if (Q.find(initState) == Q.end() || (S.find(letter) == S.end() && letter != '~') || G.find(stackTop) == G.end() || Q.find(finState) == Q.end())
				validTransition = false;
			for(string::iterator character = stackPush.begin(); character != stackPush.end(); ++character)
				if (G.find(*character) == G.end()) {
					validTransition = false;
					break;
				}
			if (!validTransition)
				cout << "Invalid transition!\n";
		} while (!validTransition);
		if (letter == '~') {
			hasLambdaTransitions = true;
		}
		if (d.find(make_tuple(initState, letter, stackTop)) == d.end()) {
			map<string, set<string>> finConfigurations;
			set<string> auxSet; 
			auxSet.insert(stackPush);
			finConfigurations.insert(make_pair(finState, auxSet));
			d.emplace(make_tuple(initState, letter, stackTop), finConfigurations);
		}
		else {
			hasNondeterminism = true;
			if (d.at(make_tuple(initState, letter, stackTop)).find(finState) == d.at(make_tuple(initState, letter, stackTop)).end()) {
				set<string> auxSet;
				auxSet.insert(stackPush);
				d.at(make_tuple(initState, letter, stackTop)).emplace(finState, auxSet);
			}
			else {
				d.at(make_tuple(initState, letter, stackTop)).at(finState).insert(stackPush);
			}
		}
	}
	cout << "Generated PushDownAutomata!\n";
}

PushDownAutomata::PushDownAutomata(ifstream & buffer) {
	hasNondeterminism = false;
	hasLambdaTransitions = false;
	int nrStates;
	buffer >> nrStates;
	for (int i = 0; i < nrStates; ++i) {
		string state;
		buffer >> state;
		Q.insert(state);
	}
	buffer >> q0;
	int finStateCount;
	buffer >> finStateCount;
	for (int i = 0; i < finStateCount; ++i) {
		string finState;
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
		string initState, finState, stackPush;
		char letter, stackTop;
		buffer >> initState >> letter >> stackTop >> finState >> stackPush;
		if (letter == '~') {
			hasLambdaTransitions = true;
		}
		if (d.find(make_tuple(initState, letter, stackTop)) == d.end()) {
			map<string, set<string>> finConfigurations;
			set<string> auxSet;
			auxSet.insert(stackPush);
			finConfigurations.insert(make_pair(finState, auxSet));
			d.emplace(make_tuple(initState, letter, stackTop), finConfigurations);
		}
		else {
			hasNondeterminism = true;
			if (d.at(make_tuple(initState, letter, stackTop)).find(finState) == d.at(make_tuple(initState, letter, stackTop)).end()) {
				set<string> auxSet;
				auxSet.insert(stackPush);
				d.at(make_tuple(initState, letter, stackTop)).emplace(finState, auxSet);
			}
			else {
				d.at(make_tuple(initState, letter, stackTop)).at(finState).insert(stackPush);
			}
		}
	}
	cout << "Generated RegularAutomata!\n";
	printConfiguration();
}

 /*map<string, set<stack<char>>> PushDownAutomata::lambdaClosure(pair<string, set<stack<char>>> initConfiguration) {
	map<string, set<stack<char>>> closureConfigurations;
	closureConfigurations.emplace(initConfiguration.first, initConfiguration.second);
	int cardinal = 0;
	while (cardinal != closureConfigurations.size()) {
		cardinal = closureConfigurations.size();
		map<string, set<stack<char>>> nextClosureConfigurations = closureConfigurations;
		for (map<string, set<stack<char>>>::iterator currentConfiguration = nextClosureConfigurations.begin(); currentConfiguration != nextClosureConfigurations.end(); ++currentConfiguration) {
			for (set<stack<char>>::iterator currentStack = currentConfiguration->second.begin(); currentStack != currentConfiguration->second.end(); ++currentStack) {
				if (d.find(make_tuple(currentConfiguration->first, '~', currentStack->top())) != d.end()) {
					for(stack<string>::iterator stackPush = )
					stack<char> nextStack = *currentStack;
					nextStack.pop();
					nextStack.insert(d.at(make_tuple(currentConfiguration->first, '~', currentStack->top())))
					nextClosure.insert(nextStates.begin(), nextStates.end());
				}
			}
		}
		closure = nextClosure;
	}
	return closure;
}

bool RegularAutomata::Evaluate(string word) {
	set<string> currentStates;
	if (hasLambdaTransitions) {
		currentStates = lambdaClosure(q0);
	}
	else {
		currentStates.insert(q0);
	}
	for (string::iterator letter = word.begin(); letter != word.end(); ++letter) {
		set<string> nextStates;
		for (set<string>::iterator state = currentStates.begin(); state != currentStates.end(); ++state) {
			map<pair<string, char>, set<string>>::iterator transition = d.find(make_pair(*state, *letter));
			if (transition != d.end()) {
				set<string> transitions = d.at(make_pair(*state, *letter));
				nextStates.insert(transitions.begin(), transitions.end());
			}
		}
		currentStates = nextStates;
		if (hasLambdaTransitions) {
			set<string> currentClosure = currentStates;
			for (set<string>::iterator state = currentClosure.begin(); state != currentClosure.end(); ++state) {
				set<string> closure = lambdaClosure(*state);
				currentStates.insert(closure.begin(), closure.end());
			}
		}
		if (currentStates.empty())
			return false;
	}
	for (set<string>::iterator state = currentStates.begin(); state != currentStates.end(); ++state) {
		if (F.find(*state) != F.end())
			return true;
	}
	return false;
} */

void PushDownAutomata::printConfiguration() {
	cout << "-----\nMachine configuration:\n\n";
	if (hasNondeterminism && hasLambdaTransitions)
		cout << "The machine contains nondeterminism and lambda-transitions!\n";
	else {
		if (hasNondeterminism)
			cout << "The machine contains nondeterminism!\n";
		else
			cout << "The machine contains lambda-transitions!\n";
	}
	cout << "Number of states: " << Q.size() << "\n";
	cout << "States: ";
	for (set<string>::iterator state = Q.begin(); state != Q.end(); ++state)
		cout << *state << " ";
	cout << "Initial state: " << q0 << "\n";
	cout << "Number of final states: " << F.size() << "\n";
	cout << "Final states: ";
	for (set<string>::iterator state = F.begin(); state != F.end(); ++state)
		cout << *state << " ";
	cout << "\nAlphabet dimension: " << S.size() << "\n";
	cout << "Letters: ";
	for (set<char>::iterator letter = S.begin(); letter != S.end(); ++letter)
		cout << *letter << " ";
	cout << "\nStack alphabet dimension: " << G.size() << "\n";
	cout << "Characters: ";
	for (set<char>::iterator character = G.begin(); character != G.end(); ++character)
		cout << *character << " ";
	cout << "\nInitial stack character: " << Z0 << "\n";
	cout << "Transition count: " << d.size() << "\n";
	cout << "Transitions:\n";
	for (map<tuple<string, char, char>, map<string, set<string>>>::iterator transition = d.begin(); transition != d.end(); ++transition) {
		if (!hasNondeterminism)
			for (map<string, set<string>>::iterator configuration = transition->second.begin(); configuration != transition->second.end(); ++configuration)
				for(set<string>::iterator stackPush = configuration->second.begin(); stackPush != configuration->second.end(); ++stackPush)
					cout << "d(" << get<0>(transition->first) << "," << get<1>(transition->first) << "," << get<2>(transition->first) << ")=(" << configuration->first << "," << *stackPush <<")\n";
		else {
			cout << "d(" << get<0>(transition->first) << "," << get<1>(transition->first) << "," << get<2>(transition->first) << ")={";
			for (map<string, set<string>>::iterator configuration = transition->second.begin(); configuration != transition->second.end(); ++configuration)
				for (set<string>::iterator stackPush = configuration->second.begin(); stackPush != configuration->second.end(); ++stackPush) {
					cout << "(" << configuration->first << "," << *stackPush << ")";
					map<string, set<string>>::iterator nextConfiguration = configuration;
					set<string>::iterator nextStackPush = stackPush;
					if (++nextConfiguration != transition->second.end() && ++nextStackPush != configuration->second.end())
						cout << ",";
				}
			cout << "}\n";
		}
	}
	cout << "\n-----\n";
}