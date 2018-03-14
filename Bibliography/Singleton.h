#ifndef __SINGLETON_H__
#define __SINGLETON_H__
#include <vector>
#include <iostream>
#include "Author.h"
#include "Keyword.h"
#include "Title.h"
#include "Journal.h"
#include "Singleton.h"
#include <memory>
#include <set>
using namespace std;
/*
* Singleton Class for Project 3
*
* This is a sample Singleton class that will hold lists of Authors, Titles,
* Journals, and Keywords.
*
* It has also had < operators added for the four items so that they are entered
* only once per set.
*
* Author:  Jerry Heuring
* Date:    October 12, 2017
*
* Revisions:
*
* Bugs:
*
*/
bool operator< (shared_ptr<Journal> lhs, shared_ptr<Journal> rhs) {
	return lhs->getName() < rhs->getName();
}
bool operator< (shared_ptr<Keyword> lhs, shared_ptr<Keyword> rhs) {
	return lhs->getName() < rhs->getName();
}
bool operator< (shared_ptr<Title> lhs, shared_ptr<Title> rhs) {
	return lhs->getName() < rhs->getName();
}
bool operator< (shared_ptr<Author> lhs, shared_ptr<Author> rhs) {
	return lhs->getName() < rhs->getName();
}

class Singleton {
private:
	static bool instanceFlag;
	static Singleton* single;
	set <shared_ptr<Journal>> journalList;
	set <shared_ptr<Keyword>> keywordList;
	set <shared_ptr<Title>> titleList;
	set<shared_ptr<Author>> authorList;

	Singleton() {  // The constructor MUST be private so that we can 
				   // be sure there is only one instance.  

	}
public:
	static Singleton *getInstance() {  // This is the main way a user gets access.
		if (instanceFlag == false) {
			single = new Singleton();
			instanceFlag = true;
			return single;
		}
		else {
			return single;
		}
	}
	~Singleton() { instanceFlag = false; single = nullptr; }
	/*
	* Routine to potentially add an author to the set.  This has been
	* modified so it brings back a pointer to the object IN the set.
	*/
	shared_ptr<Author> addAuthor(string authorName) {
		shared_ptr<Author> authorPtr = make_shared<Author>(authorName);
		if (authorList.find(authorPtr) == authorList.end()) {
			authorList.insert(authorPtr);
		}
		return *(authorList.find(authorPtr));
	}

	shared_ptr<Keyword> addKeyword(string keyword) {
		shared_ptr<Keyword> keywordPtr = make_shared<Keyword>(keyword);
		if (keywordList.find(keywordPtr) == keywordList.end()) {
			keywordList.insert(keywordPtr);
		}
		return *(keywordList.find(keywordPtr));
	}

	shared_ptr<Title> addTitle(string title) {
		shared_ptr<Title> titlePtr = make_shared<Title>(title);
		if (titleList.find(titlePtr) == titleList.end()) {
			titleList.insert(titlePtr);
		}
		return *(titleList.find(titlePtr));
	}

	shared_ptr<Journal> addJournal(string journal) {
		shared_ptr<Journal> journalPtr = make_shared<Journal>(journal);
		if (journalList.find(journalPtr) == journalList.end()) {
			journalList.insert(journalPtr);
		}
		return *(journalList.find(journalPtr));
	}

};

Singleton*Singleton::single = nullptr;
bool Singleton::instanceFlag = false;


#endif
