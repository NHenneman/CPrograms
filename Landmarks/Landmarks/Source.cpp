//Nathan Henneman
//Project 2: Landmarks
//October 4 2017

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

//creates a class to make landmarks
class Landmark {
protected:
	string name, longitude, latitude;

public:
	Landmark(string name, string latitude, string longitude);
	Landmark();
	string getName();
	string getLatitude();
	string getLongitude();
	void print();
};

//landmark constructor
Landmark::Landmark(string name, string latitude, string longitude)
{
	this->name = name;
	this->latitude = latitude;
	this->longitude = longitude;
}
//general constructor
Landmark::Landmark()
{

}
//returns the infomation about the landmark
string Landmark::getName()
{
	return name;
}

string Landmark::getLongitude()
{
	return longitude;
}

string Landmark::getLatitude()
{
	return latitude;
}
//prints landmark infomation
void Landmark::print()
{
	cout << name << endl << latitude << endl << longitude << endl << endl;
}

int main()
{
	//creates array of landmarks
	Landmark *Landmarks;
	Landmarks = new Landmark[15];

	//fills array with landmark information
	string name, latitude, longitude;
	ifstream landmarkIn;
	landmarkIn.open("landmarks.txt");
	int j;
	for (j = 0; j < 15; j++)
	{
		if (landmarkIn.eof()) { break; }
		getline(landmarkIn, name);
		getline(landmarkIn, latitude);
		getline(landmarkIn, longitude);
		Landmarks[j] = Landmark(name, latitude, longitude);
	}

	//asks user what is to be done
	char command;
	cout << "What would you like to do?" << endl
		<< "'L' = List Landmarks in file" << endl
		<< "'A' = Add a new landmark to list" << endl
		<< "'S' = Save landmark file" << endl
		<< "'Q' = Quit program" << endl;
	cin >> command;

	while (true)
	{
		if (command == 'L' || command == 'l')
		{
			for (int i = 0; i < j; i++)
			{
				Landmarks[i].print();
			}
			cout << "What would you like to do?" << endl;
			cin >> command;
		}
		if (command == 'A' || command == 'a')
		{
			cout << "Please enter new landmark name: ";
			getline(cin, name);
			cout << "Please enter new landmark latitude coordinates: ";
			getline(cin, latitude);
			cout << "Please enter new landmark longitude coordinates: ";
			getline(cin, longitude);
			Landmarks[j] = Landmark(name, latitude, longitude);
			j++;
			cout << "What would you like to do?" << endl;
			cin >> command;
		}
		if (command == 'S' || command == 's')
		{
			ofstream landmarkOut;
			landmarkOut.open("landmarks.txt");
			for (int i = 0; i < j; i++)
			{
				name = Landmarks[i].getName();
				latitude = Landmarks[i].getLatitude();
				longitude = Landmarks[i].getLongitude();
				landmarkOut << name << endl
					<< latitude << endl
					<< longitude << endl;
			}
			landmarkOut.close();
			cout << "What would you like to do?" << endl;
			cin >> command;
		}
		if (command == 'Q' || command == 'q')
		{
			break;
		}
		else
		{
			cout << "Not a valid command." << endl;
			cout << "What would you like to do?" << endl
				<< "'L' = List Landmarks in file" << endl
				<< "'A' = Add a new landmark to list" << endl
				<< "'S' = Save landmark file" << endl
				<< "'Q' = Quit program" << endl;
			cin >> command;
		}
	}

	//closes open files and exits program
	landmarkIn.close();
	return(0);
}