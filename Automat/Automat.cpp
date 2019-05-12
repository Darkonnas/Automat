//#include <vld.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "fla.h"

int main() {
	std::ifstream initRegularAutomata("ra_init.txt");
	std::ifstream initRegularGrammar("rg_init.txt");
	std::ifstream initPushDownAutomata("pda_init.txt");
	RegularAutomata RA(initRegularAutomata);
	RegularGrammar RG(initRegularGrammar);
	PushDownAutomata PDA(initPushDownAutomata);
	initRegularAutomata.close(); initRegularGrammar.close(); initPushDownAutomata.close();
	char choice;
	do {
		system("cls");
		int decision;
		do {																													 
			std::cout << "Please choose one of the options below:\n\t1. Initialize Regular Automata from file\n\t2. Initialize Regular Automata manually\n\t3. Initialize Regular Automata from Regular Grammar\n\t4. Print Regular Automata configuration\n\t5. Evaluate words using Regular Automata\n\t6. Initialize Regular Grammar from file\n\t7. Initialize Regular Grammar manually\n\t8. Initialize Regular Grammar from Regular Automata\n\t9. Print Regular Grammar configuration\n\t10. Evaluate words using Regular Grammar\n\t11. Initialize PushDown Automata from file\n\t12. Initialize PushDown Automata manually\n\t13. Print PushDown Automata configuration\n\t14. Evaluate words using PushDown Automata\n\t15. Exit the program\nYour choice: ";
			std::cin >> decision;
			switch (decision) {
			case 1: {
				std::string inputFile;
				std::ifstream buffer;
				do {
					std::cout << "Input file used for Regular Automata initialization: ";
					std::cin >> inputFile;
					buffer.open(inputFile);
					if (!buffer.is_open())
						std::cout << "Cannot open specified file!\n";
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
				std::vector<std::string> words;
				std::cout << "How many words do you wish to evaluate? ";
				std::cin >> evaluatesNo;
				std::cout << "Words to evaluate: \n";
				for (int i = 0; i < evaluatesNo; ++i) {
					std::string word;
					std::cin >> word;
					words.push_back(word);
				}
				if (evaluatesNo > 0) {
					std::cout << "Results:\n";
					for (int i = 0; i < evaluatesNo; ++i) {
						bool result = RA.Evaluate(words[i]);
						if (result)
							std::cout << words[i] << " was accepted!\n";
						else
							std::cout << words[i] << " was rejected!\n";
					}
				}
				break;
			}
			case 6: {
				std::string inputFile;
				std::ifstream buffer;
				do {
					std::cout << "Input file used for Regular Grammar initialization: ";
					std::cin >> inputFile;
					buffer.open(inputFile);
					if (!buffer.is_open())
						std::cout << "Cannot open specified file!\n";
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
				RegularGrammar newRG(RA);
				RG = newRG;
				break;
			}
			case 9: {
				RG.printConfiguration();
				break;
			}
			case 10: {
				int evaluatesNo;
				std::vector<std::string> words;
				std::cout << "How many words do you wish to evaluate? ";
				std::cin >> evaluatesNo;
				std::cout << "Words to evaluate: \n";
				for (int i = 0; i < evaluatesNo; ++i) {
					std::string word;
					std::cin >> word;
					words.push_back(word);
				}
				if (evaluatesNo > 0) {
					std::cout << "Results:\n";
					for (int i = 0; i < evaluatesNo; ++i) {
						bool result = RG.Evaluate(words[i]);
						if (result)
							std::cout << words[i] << " was accepted!\n";
						else
							std::cout << words[i] << " was rejected!\n";
					}
				}
				break;
			}
			case 11: {
				std::string inputFile;
				std::ifstream buffer;
				do {
					std::cout << "Input file used for PushDown Automata initialization: ";
					std::cin >> inputFile;
					buffer.open(inputFile);
					if (!buffer.is_open())
						std::cout << "Cannot open specified file!\n";
				} while (!buffer.is_open());
				PushDownAutomata newPDA(buffer);
				PDA = newPDA;
				break;
			}
			case 12: {
				PushDownAutomata newPDA;
				PDA = newPDA;
				break;
			}
			case 13: {
				PDA.printConfiguration();
				break;
			}
			case 14: {
				int evaluatesNo;
				std::vector<std::string> words;
				std::cout << "How many words do you wish to evaluate? ";
				std::cin >> evaluatesNo;
				std::cout << "Words to evaluate: \n";
				for (int i = 0; i < evaluatesNo; ++i) {
					std::string word;
					std::cin >> word;
					words.push_back(word);
				}
				if (evaluatesNo > 0) {
					std::cout << "Results:\n";
					for (int i = 0; i < evaluatesNo; ++i) {
						bool result = PDA.Evaluate(words[i]);
						if (result)
							std::cout << words[i] << " was accepted!\n";
						else
							std::cout << words[i] << " was rejected!\n";
					}
				}
				break;
			}
			case 15: {
				std::cout << "Have a good day!\n";
				break;
			}
			default: {
				std::cout << "Wrong input! ";
				break;
			}
			}
		} while (decision > 15);
		if (decision != 15) {
			std::cout << "Do you wish to continue? Y/N\t";
			std::cin >> choice;
		}
		else {
			choice = 'E';
		}
	}while (choice == 'Y');
	if (choice == 'N') {
		std::cout << "Have a good day!\n";
	}
	return 0;
}