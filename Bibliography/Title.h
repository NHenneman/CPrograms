#ifndef __TITLE_H__
#define __TITLE_H__
#include <string>
using namespace std;

class Title{
private:
protected:
	string name;
public:
	Title(string n = "") { name = n; }
	string getName() { return name; }
};

ostream& operator<< (ostream& outstr, Title title) {
	outstr << title.getName();
	return outstr;
}
#endif
