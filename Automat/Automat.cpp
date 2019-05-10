#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "fla.h"

using namespace std;

int main() {
	/*ifstream initMachine("machine_init.txt");
	ifstream initGrammar("grammar_init.txt");
	RegularAutomata M(initMachine);
	RegularGrammar RG(initGrammar);
	initMachine.close(); initGrammar.close();
	char choice;
	do {
		system("cls");
		int decision;
		do {
			cout << "Please choose one of the options below:\n\t1. Initialize RegularAutomata from file\n\t2. Initialize RegularAutomata manually\n\t3. Initialize RegularAutomata from Grammar\n\t4. Print RegularAutomata configuration\n\t5. Evaluate words using RegularAutomata\n\t6. Initialize Grammar from file\n\t7. Initialize Grammar Manually\n\t8. Print Grammar configuration\n\t9. Evaluate words using Grammar\n\t10. Exit the program\nYour choice: ";
			cin >> decision;
			switch (decision) {
			case 1: {
				string inputFile;
				ifstream buffer;
				do {
					cout << "Input file used for machine initialization: ";
					cin >> inputFile;
					buffer.open(inputFile);
					if (!buffer.is_open())
						cout << "Cannot open specified file!\n";
				} while (!buffer.is_open());
				RegularAutomata newMachine(buffer);
				M = newMachine;
				break;
			}
			case 2: {
				RegularAutomata newMachine;
				M = newMachine;
				break;
			}
			case 3: {
				RegularAutomata newMachine(RG);
				M = newMachine;
				break;
			}
			case 4: {
				M.printConfiguration();
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
					bool result = M.Evaluate(words[i]);
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
					cout << "Input file used for grammar initialization: ";
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
				cout << "Have a good day!";
				break;
			}
			default: {
				cout << "Wrong input! ";
				break;
			}
			}
		} while (decision > 10);
		if (decision != 10) {
			cout << "Do you wish to continue? Y/N\t";
			cin >> choice;
		}
		else {
			choice = 'E';
		}
	}while (choice == 'Y');
	if (choice == 'N') {
		cout << "Have a good day!";
	} */
	ifstream pdaInput("push_down_automata.txt");
	PushDownAutomata A(pdaInput);
	return 0;
}
