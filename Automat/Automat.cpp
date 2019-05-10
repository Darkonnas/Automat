#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "fla.h"

using namespace std;

int main() {
	ifstream initRegularAutomata("regular_automata_init.txt");
	ifstream initRegularGrammar("regular_grammar_init.txt");
	ifstream initPushDownAutomata("push_down_automata_init.txt");
	RegularAutomata RA(initRegularAutomata);
	RegularGrammar RG(initRegularGrammar);
	PushDownAutomata PDA(initPushDownAutomata);
	initRegularAutomata.close(); initRegularGrammar.close(); initPushDownAutomata.close();
	char choice;
	do {
		system("cls");
		int decision;
		do {																													 
			cout << "Please choose one of the options below:\n\t1. Initialize Regular Automata from file\n\t2. Initialize Regular Automata manually\n\t3. Initialize Regular Automata from Regular Grammar\n\t4. Print Regular Automata configuration\n\t5. Evaluate words using Regular Automata\n\t6. Initialize Regular Grammar from file\n\t7. Initialize Regular Grammar manually\n\t8. Print Regular Grammar configuration\n\t9. Evaluate words using Regular Grammar\n\t10. Initialize PushDown Automata from file\n\t11. Initialize PushDown Automata manually\n\t12. Print PushDown Automata configuration\n\t13. Evaluate words using PushDown Automata\n\t14. Exit the program\nYour choice: ";
			cin >> decision;
			switch (decision) {
			case 1: {
				string inputFile;
				ifstream buffer;
				do {
					cout << "Input file used for Regular Automata initialization: ";
					cin >> inputFile;
					buffer.open(inputFile);
					if (!buffer.is_open())
						cout << "Cannot open specified file!\n";
				} while (!buffer.is_open());
				RegularAutomata newRA(buffer);
				RA = newRA;
				break;
			}
			case 2: {
				RegularAutomata newRA;
				RA = newRA;
				break;
			}
			case 3: {
				RegularAutomata newRA(RG);
				RA = newRA;
				break;
			}
			case 4: {
				RA.printConfiguration();
				break;
			}
			case 5: {
				int evaluatesNo;
				vector<string> words;
				cout << "How many words do you wish to evaluate? ";
				cin >> evaluatesNo;
				cout << "Words to evaluate: \n";
				for (int i = 0; i < evaluatesNo; ++i) {
					string word;
					cin >> word;
					words.push_back(word);
				}
				cout << "Results:\n";
				for (int i = 0; i < evaluatesNo; ++i) {
					bool result = RA.Evaluate(words[i]);
					if (result)
						cout << words[i] << " was accepted!\n";
					else
						cout << words[i] << " was rejected!\n";
				}
				break;
			}
			case 6: {
				string inputFile;
				ifstream buffer;
				do {
					cout << "Input file used for Regular Grammar initialization: ";
					cin >> inputFile;
					buffer.open(inputFile);
					if (!buffer.is_open())
						cout << "Cannot open specified file!\n";
				} while (!buffer.is_open());
				RegularGrammar newRG(buffer);
				RG = newRG;
				break;
			}
			case 7: {
				RegularGrammar newRG;
				RG = newRG;
				break;
			}
			case 8: {
				RG.printConfiguration();
				break;
			}
			case 9: {
				int evaluatesNo;
				vector<string> words;
				cout << "How many words do you wish to evaluate? ";
				cin >> evaluatesNo;
				cout << "Words to evaluate: \n";
				for (int i = 0; i < evaluatesNo; ++i) {
					string word;
					cin >> word;
					words.push_back(word);
				}
				cout << "Results:\n";
				for (int i = 0; i < evaluatesNo; ++i) {
					bool result = RG.Evaluate(words[i]);
					if (result)
						cout << words[i] << " was accepted!\n";
					else
						cout << words[i] << " was rejected!\n";
				}
				break;
			}
			case 10: {
				string inputFile;
				ifstream buffer;
				do {
					cout << "Input file used for PushDown Automata initialization: ";
					cin >> inputFile;
					buffer.open(inputFile);
					if (!buffer.is_open())
						cout << "Cannot open specified file!\n";
				} while (!buffer.is_open());
				PushDownAutomata newPDA(buffer);
				PDA = newPDA;
				break;
			}
			case 11: {
				PushDownAutomata newPDA;
				PDA = newPDA;
				break;
			}
			case 12: {
				PDA.printConfiguration();
				break;
			}
			case 13: {
				int evaluatesNo;
				vector<string> words;
				cout << "How many words do you wish to evaluate? ";
				cin >> evaluatesNo;
				cout << "Words to evaluate: \n";
				for (int i = 0; i < evaluatesNo; ++i) {
					string word;
					cin >> word;
					words.push_back(word);
				}
				cout << "Results:\n";
				for (int i = 0; i < evaluatesNo; ++i) {
					bool result = PDA.Evaluate(words[i]);
					if (result)
						cout << words[i] << " was accepted!\n";
					else
						cout << words[i] << " was rejected!\n";
				}
				break;
			}
			case 14: {
				cout << "Have a good day!";
				break;
			}
			default: {
				cout << "Wrong input! ";
				break;
			}
			}
		} while (decision > 14);
		if (decision != 14) {
			cout << "Do you wish to continue? Y/N\t";
			cin >> choice;
		}
		else {
			choice = 'E';
		}
	}while (choice == 'Y');
	if (choice == 'N') {
		cout << "Have a good day!";
	}
	return 0;
}