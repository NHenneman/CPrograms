#ifndef __PUBLICATION_H__
#define __PUBLICATION_H__
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "Title.h"
#include "Author.h"
#include "Keyword.h"
#include "Journal.h"
using namespace std;

class Publication {
private:
protected:
	shared_ptr<Title> title;
	shared_ptr<Journal> journal;
	vector<shared_ptr<Keyword>> keywordList;
	vector<shared_ptr<Author>> authorList;
	string pages;
	string date;
public:

	void setPages(string pagelist) { pages = pagelist; }
	void setDate(string d) { date = d; }
	void setTitle(string title) {
		this->title = Singleton::getInstance()->addTitle(title);
	}
	void setJournal(string journalName) {
		this->journal = Singleton::getInstance()->addJournal(journalName);
	}
	void addKeyword(string keyword) {
		keywordList.push_back(Singleton::getInstance()->addKeyword(keyword));
	}
	void addAuthor(string author) {
		authorList.push_back(Singleton::getInstance()->addAuthor(author));
	}

	//Checks if a passed string is in the publication
	bool contains(string reference) {
		if (reference == title->getName()){ return true; }
		if (reference == date) { return true; }
		if (reference == pages) { return true; }
		for (auto author : authorList) {
			if (reference == author->getName()) { return true; }
		}
		for (auto keyword : keywordList) {
			if (reference == keyword->getName()) { return true; }
		}
		return false;
	}
	friend istream& operator>> (istream& instr, Publication& pub);
	friend ostream& operator<< (ostream& outstr, Publication& pub);
};

istream& operator>> (istream& instr, Publication& pub) {
	string title, author, pubName, pagelist, date, keyword;
	getline(instr, title);
	if (instr.eof() || instr.bad()) {
		return instr;
	}
	pub.setTitle(title);
	do {
		getline(instr, author);
		if (author != "") {
			// add author to author list
			pub.addAuthor(author);
		}
	} while (author != "");
	getline(instr, pubName);
	pub.setJournal(pubName);
	getline(instr, date);
	pub.setDate(date);
	getline(instr, pagelist);
	pub.setPages(pagelist);
	do {
		getline(instr, keyword);
		if (keyword != "") {
			// add keyword to list
			pub.addKeyword(keyword);
		}
	} while (keyword != "");

	return instr;
}

ostream& operator<< (ostream& outstr, Publication& pub) {
	outstr << *(pub.title) << endl;
	for (auto author : pub.authorList) {
		outstr << *author << endl;
	}
	outstr << endl << *(pub.journal) << endl;
	outstr << pub.pages << endl;
	outstr << pub.date << endl;
	for (auto keyword : pub.keywordList) {
		outstr << *keyword << endl;
	}
	return outstr;
}

#endif
