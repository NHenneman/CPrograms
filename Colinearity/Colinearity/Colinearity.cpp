// Name: Nathan Henneman
//Project: Determining a Perpendicularity to a Plane
//Date: 14 September 2017

#include <iostream>
#include <cmath>
using namespace std;

void slope(double p1[], double p2[], double p12[]);
void normalize(double vector[]);
bool colinear(double vector1[], double vector2[]);
void perpendicular(double vector1[], double vector2[]);

int main()
{
	char again = 'Y';
	//Get location of three 3D points from user
	while (again == 'Y' || again == 'y') {
		double point1[3], point2[3], point3[3];
		cout << "Please enter point 1 in form 'x.x y.y z.z':";
		cin >> point1[0] >> point1[1] >> point1[2];
		cout << "Please enter point 2 in form 'x.x y.y z.z':";
		cin >> point2[0] >> point2[1] >> point2[2];
		cout << "Please enter point 3 in form 'x.x y.y z.z':";
		cin >> point3[0] >> point3[1] >> point3[2];

		//Get the normalized slope of between each of the three points
		double point12[3], point13[3], point23[3];
		slope(point1, point2, point12);
		slope(point1, point3, point13);
		slope(point2, point3, point23);

		//Tests to see if points colinear
		bool colin = colinear(point12, point23);

		//Output to screen if points colinear and a perpendicular vector
		if (colin)
			cout << "\nThe three points are colinear.\n";
		else {
			cout << "\nThe three points are not colinear\n";
			perpendicular(point12, point23);
		}

		cout << "Do you want to do another comparision ( N for NO, Y for YES):  ";
		cin >> again;
	}
	return 0;
}

//Calculates normalized vector between two points
//Assumes vectors passed in are of length three
void slope(double p1[], double p2[], double p12[]) {
	for (int i = 0; i < 3; i++) {
		p12[i] = p2[i] - p1[i];
	}
	normalize(p12);
	return;
}

//Calculate normalization of 3D vector
void normalize(double vector[]) {
	double vec_mag = sqrt(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
	for (int i = 0; i < 3; i++) {
		vector[i] = vector[i] / vec_mag;
	}
	return;
}

//Tests to see of two 3D vectors are colinear
bool colinear(double vector1[], double vector2[]) {
	if (vector1[0] == vector2[0])
		if (vector1[1] == vector2[1])
			if (vector1[2] == vector2[2])
				return true;
			else
				return false;
		else
			return false;
	else
		return false;
}

//Calculates a perpendicular vector given two 3D vectors
void perpendicular(double vector1[], double vector2[]) {
	double perpendicular[3];
	perpendicular[0] = vector1[1] * vector2[2] - vector1[2] * vector2[1];
	perpendicular[1] = vector1[0] * vector2[2] - vector1[2] * vector2[0];
	perpendicular[2] = vector1[0] * vector2[1] - vector1[1] * vector2[0];
	normalize(perpendicular);
	cout << "A vector perpendicular to the plane is ";
	for (int i = 0; i < 3; i++) {
		cout << perpendicular[i] << " ";
	}
	cout << endl;
	return;
}