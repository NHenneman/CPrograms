#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "Singleton.h"
#include "Publication.h"
using namespace std;

int main(int argCount, char *argValues[]) {
	vector<shared_ptr<Publication>> pubs;
	ifstream inFile;
	ofstream outFile;

	/*
	* Open the input file and make sure it was successful.  If not, exit
	* with an error message.
	*/
	inFile.open("articles.txt");
	if (!inFile.is_open()) {
		cerr << "Couldn't open file articles.txt for input.  Program Exitting\n";
		return EXIT_FAILURE;
	}
	/*
	* Read the input file and store in the pubs vector.
	*/
	while (!inFile.eof() && inFile.good()) {
		shared_ptr<Publication> newpub = make_shared<Publication>();
		inFile >> *newpub;
		if (inFile.good()) {
			pubs.push_back(newpub);
			cout << *newpub << endl;
		}

		newpub = nullptr;
	}
	
	char command;
	cout << "What would you like to do?" << endl
		<< "'L' = List articles in file" << endl
		<< "'A' = Add an article" << endl
		<< "'D' = Delete an article" << endl
		<< "'S' = Search the articles" << endl
		<< "'Q' = Save and Quit program" << endl;
	cin >> command;

	while (true)
	{
		if (command == 'L' || command == 'l')
		{
			cout << endl;
			for (auto pub : pubs) { cout << *pub << endl; }
		}

		else if (command == 'A' || command == 'a')
		{
			cout << endl
				 << "Enter a publication in form: \n"
				 << "Title\n" 
				 << "Author1\n"
				 << "Author2\n"
				 << endl
				 << "Date\n"
				 << "Pages\n"
				 << "Keyword1\n"
				 << "Keyword2\n"
				 << endl;
			
			//Adds a new publication to the end of a publication list
			shared_ptr<Publication> newpub = make_shared<Publication>();
			cin >> *newpub;
			pubs.push_back(newpub);
			cout << *newpub << endl
				<< "Add another publication(Y or N): ";
			cin >> command;
			newpub = nullptr;

			while(command == 'Y'){
				shared_ptr<Publication> newpub = make_shared<Publication>();
				cin >> *newpub;
				pubs.push_back(newpub);
				cout << *newpub << endl;
				
				newpub = nullptr;
				cout << "Add another publication (Y or N): ";
				cin >> command;
			}
		}

		else if (command == 'Q' || command == 'q')
		{
			outFile.open("Articles.txt");

			//Checks if the file opened successfully
			if (!outFile.is_open()) {
				cerr << "Couldn't open file articles.txt for output.  Program Exitting\n";
				return EXIT_FAILURE;
			}
			
			//Prints publications to file
			for (auto pub : pubs) {
				outFile << *pub << endl;
			}

			outFile.close();
			break;
		}

		else if (command == 'D' || command == 'd')
		{
			string search;
			int i = 0;
			cout << "Enter 'Title', 'Author', or 'Keyword' to delete a publication.\n";
			getline(cin, search);
			getline(cin, search);
			cout << endl;

			//If found, deletes selected publications
			for (auto pub : pubs) {
				if (pub->contains(search)) {
					pubs.erase(pubs.begin()+i);
				}
				i++;
			}
			cout << endl;
		}

		else if (command == 'S' || command == 's')
		{
			string search;
			int i = 0;
			cout << "Enter 'Title', 'Author', or 'Keyword' to list a publication.\n";
			getline(cin, search);
			getline(cin, search);
			cout << endl;
			
			//If found, prints selected publications to screen
			for (auto pub : pubs) {
				if (pub->contains(search)) {
					cout << *(pubs.at(i));
				}
				i++;
			}
		}

		else{ cout << "Not a valid command." << endl; }

		cout << "What would you like to do?" << endl
			<< "'L' = List articles in file" << endl
			<< "'A' = Add an article" << endl
			<< "'D' = Delete an article" << endl
			<< "'S' = Search the articles" << endl
			<< "'Q' = Save and Quit program" << endl;
		cin >> command;
	}

	return EXIT_SUCCESS;
}
