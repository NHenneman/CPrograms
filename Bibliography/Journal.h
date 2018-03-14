#ifndef __JOURNAL_H__
#define __JOURNAL_H__
#include <string>
using namespace std;

class Journal {
private:
protected:
	string name;
public:
	Journal(string n = "") { name = n; }
	string getName() { return name; }
};

ostream& operator<< (ostream& outstr, Journal journal) {
	outstr << journal.getName();
	return outstr;
}
#endif