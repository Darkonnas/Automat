#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "machine.h"

using namespace std;

int main() {
	int decision;
	do {
		cout << "Please choose one of the options below:\n\t1. Initialize Machine from file\n\t2. Initialize Machine manually\n\t3. Exit the program\nYour choice: ";
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
			Machine M(buffer);
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
			for (int i = 0; i < evaluatesNo; ++i) {
				bool result = M.Evaluate(words[i]);
				if (result)
					cout << words[i] << " was accepted!\n";
				else
					cout << words[i] << " was rejected!\n";
			}
			break;
		}
		case 2: {
			Machine M;
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
			for (int i = 0; i < evaluatesNo; ++i) {
				bool result = M.Evaluate(words[i]);
				if (result)
					cout << words[i] << " was accepted!\n";
				else
					cout << words[i] << " was rejected!\n";
			}
			break;
		}
		case 3: {
			cout << "Have a good day!";
			break;
		}
		default: {
			cout << "Wrong input! ";
			break;
		}
		}
	} while (decision > 3);
	return 0;
}
