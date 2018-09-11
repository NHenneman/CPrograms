#ifndef __KEYWORD_H__
#define __KEYWORD_H__
#include <string>
using namespace std;

class Keyword {
private:
protected:
	string name;
public:
	Keyword(string n = "") { name = n; }
	string getName() { return name; }
};

ostream & operator<< (ostream& outstr, Keyword keyword) {
	outstr << keyword.getName();
	return outstr;
}
#endif