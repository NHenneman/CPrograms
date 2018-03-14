#ifndef __AUTHOR_H__
#define __AUTHOR_H__
#include <string>
using namespace std;

class Author {
private:
protected:
	string name;
public:
	Author(string n = "") { name = n; }
	string getName() { return name; }
};

ostream& operator<< (ostream& ostr, Author author) {
	ostr << author.getName();
	return ostr;
}
#endif