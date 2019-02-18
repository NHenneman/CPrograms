#include <vector>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

/*
*  Readfile -- a "basic" routine to read in the obj file and put the
*  values in vectors.  The vectors are passed in by reference so that
*  the modified values are returned to the program. *
*
*  Author:  Jerry Heuring
*  Date:    September 28, 2018
*
*  Bugs:  File open is NOT checked!! a bad file will result in
*         bogus data!!
*
*  Revisions:
*		Nathan Henneman - 10 November 2018
		Modify functions to check for error and read values differently
*/
void readfile(string filename, vector<float>& vertices,	vector<unsigned int>& indices) {
	string inputLine;
	float x, y, z;
	unsigned int i1, i2, i3;

	ifstream inFile(filename);
	if (!inFile.is_open())
	{
		cerr << "File " << filename << "cannot be opened." << endl;
		exit(1);
	}

	while (getline(inFile, inputLine)) {
		if (inputLine.substr(0, 2) == "v ") { // process a vertex
			istringstream s(inputLine.substr(2));
			s >> x >> y >> z;
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
		}
		else if (inputLine.substr(0, 2) == "f") { // process a face...
			istringstream s(inputLine.substr(2));
			s >> i1 >> i2 >> i3;
			i1--; i2--; i3--;
			indices.push_back(i1);
			indices.push_back(i2);
			indices.push_back(i3);
		}
	}
}

/*
* readfile2 -- a basic routine that will read the file and set up the data in a
* pair of vectors.  One vector contains the vertices and the second contains the
* normals.  The two vectors are passed in by reference and returned to the calling
* routine.
*/
void readfile2(string filename,	vector<float>& vertices, vector<float>& normals) {

	vector <float> localVertices;
	vector <float> localNormals;
	vector <unsigned int> localIndices;
	string inputLine;
	float x, y, z;
	float dx, dy, dz;
	unsigned int i1, i2, i, j;
	char ch;

	ifstream inFile(filename);
	if (!inFile.is_open())
	{
		cerr << "File " << filename << "cannot be opened." << endl;
		exit(1);
	}

	localVertices.push_back(0.0);
	localVertices.push_back(0.0);
	localVertices.push_back(0.0);
	localNormals.push_back(0.0f);
	localNormals.push_back(0.0f);
	localNormals.push_back(0.0f);
	while (getline(inFile, inputLine)) {
		if (inputLine.substr(0, 2) == "v ") { // process a vertex
			istringstream s(inputLine.substr(2));
			s >> x >> y >> z;
			localVertices.push_back(x);
			localVertices.push_back(y);
			localVertices.push_back(z);
		}
		else if (inputLine.substr(0, 2) == "vn") {
			istringstream s(inputLine.substr(2));
			s >> dx >> dy >> dz;
			localNormals.push_back(dx);
			localNormals.push_back(dy);
			localNormals.push_back(dz);
		}
		else if (inputLine.substr(0, 2) == "f") { // process a face...
			for (i = 0; i < 3; i++) {
				istringstream s(inputLine.substr(2));
				s >> i1 >> ch >> ch >> i2;
				for (j = 0; j < 3; j++) {
					vertices.push_back(localVertices[3 * i1 + j]);
					normals.push_back(localNormals[3 * i2 + j]);
				}
			}
		}
	}
}