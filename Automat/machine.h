#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <string>

using namespace std;

class Machine {
	set<string> Q, F;
	set<char> S;
	map<pair<string, char>, set<string>> d;
	string q0;

public:
	Machine();
	Machine(ifstream &buffer);
	bool Evaluate(string word);
	void printConfiguration();
};

Machine::Machine() {
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
	cout << "Transitions:  (format: initialState letter resultState)\n";
	for (int i = 0; i < transCount; ++i) {
		string initState, finState;
		char rule;
		bool validTransition;
		do {
			validTransition = true;
			cin >> initState >> rule >> finState;
			if (Q.find(initState) == Q.end() || Q.find(finState) == Q.end() || S.find(rule) == S.end())
				validTransition = false;
			if (!validTransition)
				cout << "Invalid transition!\n";
		} while (!validTransition);
		if (d.find(make_pair(initState, rule)) == d.end()) {
			set<string> finStates;
			finStates.insert(finState);
			d.emplace(make_pair(initState, rule), finStates);
		}
		else {
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
	cout << "Generated machine!\n";
}

Machine::Machine(ifstream &buffer) {
	int nrStates;
	cout << "Generating machine...\n";
	cout << "Number of states: ";
	buffer >> nrStates;
	cout << nrStates << "\n";
	cout << "States: ";
	for (int i = 0; i < nrStates; ++i) {
		string state;
		buffer >> state;
		cout << state << " ";
		Q.insert(state);
	}
	int alphaDim;
	cout << "\nAlphabet dimension: ";
	buffer >> alphaDim;
	cout << alphaDim << "\nLetters: ";
	for (int i = 0; i < alphaDim; ++i) {
		char letter;
		buffer >> letter;
		cout << letter << " ";
		S.insert(letter);
	}
	int transCount;
	cout << "\nTransition count: ";
	buffer >> transCount;
	cout << transCount << "\nTransitions:\n";
	for (int i = 0; i < transCount; ++i) {
		string initState, finState;
		char rule;
		buffer >> initState >> rule >> finState;
		cout << "\td(" << initState << "," << rule << ")=" << finState << "\n";
		if (d.find(make_pair(initState, rule)) == d.end()) {
			set<string> finStates;
			finStates.insert(finState);
			d.emplace(make_pair(initState, rule), finStates);
		}
		else {
			d.at(make_pair(initState, rule)).insert(finState);
		}
	}
	int finStateCount;
	cout << "\nInitial state: ";
	buffer >> q0;
	cout << q0 << "\nNumber of final states: ";
	buffer >> finStateCount;
	cout << finStateCount << "\nFinal states: ";
	for (int i = 0; i < finStateCount; ++i) {
		string finState;
		buffer >> finState;
		cout << finState << " ";
		F.insert(finState);
	}
	cout << "\n";
}

bool Machine::Evaluate(string word) {
	set<string> currentStates;
	currentStates.insert(q0);
	for (string::iterator letter = word.begin(); letter != word.end(); ++letter) {
		set<string> nextStates;
		for (set<string>::iterator state = currentStates.begin(); state != currentStates.end(); ++state) {
			map<pair<string, char>, set<string>>::iterator transition = d.find(make_pair(*state, *letter));
			if (transition != d.end()) {
				set<string> transitions = d.at(make_pair(*state, *letter));
				for (set<string>::iterator nextState = transitions.begin(); nextState != transitions.end(); ++nextState)
					nextStates.insert(*nextState);
			}
		}
		currentStates = nextStates;
		if (currentStates.empty())
			return false;
	}
	for (set<string>::iterator state = currentStates.begin(); state != currentStates.end(); ++state) {
		if (F.find(*state) != F.end())
			return true;
	}
	return false;
}

void Machine::printConfiguration() {
	cout << "-----\nMachine configuration:\n\n";
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
	for (map<pair<string, char>, set<string>>::iterator transition = d.begin(); transition != d.end(); ++transition)
		for (set<string>::iterator state = (*transition).second.begin(); state != (*transition).second.end(); ++state)
			cout << "d(" << (*transition).first.first << "," << (*transition).first.second << ")=" << *state << "\n";
	cout << "Initial state: " << q0 << "\n";
	cout << "Number of final states: " << F.size() << "\n";
	cout << "Final states: ";
	for (set<string>::iterator state = F.begin(); state != F.end(); ++state)
		cout << *state << " ";
	cout << "\n";
}
