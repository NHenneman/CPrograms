// Main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <map>
#include <array>
#include <vector>
#include "FileSystem.h"
using namespace std;

int main()
{
	InitializeFileSystem();

	map<string, string> poetry;
	poetry = initializePoetry();
	for (auto i : poetry) {
		WriteFile(i.first, i.second);
	}

	char option = ' ';
	string filename = "";
	string appendFile = "";

	while (1) {
		cout << "What do you want to do?\n"
			<< "'W' to write a file to system\n"
			<< "'R' to read a file from system\n"
			<< "'M' to map the drive\n"
			<< "'A' to modify a document in the system\n"
			<< "'Q' to exit the system\n\n"
			<< "Your Choice:  ";
		cin >> option;

		switch (option) {
		case 'w':
		case 'W':
			filename = "";
			cout << "\n\nWhat is the name of the file you want to save?\n"
				<<"Please enter your filename in form 'C:\\Directory\\Subdirectory\\...\\YourFile.txt'\n"
				<< "Filename:  ";
			cin >> filename;
			if (OpenFile(filename)) {
				cout << "File successfully moved into filesystem!!\n\n";
			}
			else {
				cout << "Error in uploading file. Please try again.\n\n";
			}
			break;
		case 'R':
		case 'r':
			cout << "What is the file that you want to read in?\nFilename:  ";
			cin >> filename;
			cout << endl << endl << ReadFile(filename) << endl << endl;
			break;
		case 'M':
		case 'm':
			MapDrive();
			break;
		case 'A':
		case 'a':
			getline(std::cin, filename);
			cout << "What is the file that holds the appendable text?\nFilename:  ";
			getline(std::cin, appendFile);
			cout << "What is the file that is to be appended to?\nFilename:  ";
			getline(std::cin, filename);
			if (GetApendableFile(filename, appendFile)) {
				cout << "File successfully appended into filesystem!!\n\n";
			}
			else {
				cout << "Error in appending file. Please try again.\n\n";
			}
			break;
		case 'q':
		case 'Q':
			exit(EXIT_SUCCESS);
			break;
		}
		
		//cout << name;
		/*
		for (auto i : poetry) {
			cout << i.second << endl;
		}
		*/
	}
}
