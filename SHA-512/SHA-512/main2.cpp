#include <iostream>
#include <cstdlib>
#include "SHA512.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

const int BUFFER_SIZE = 128;	//1024 bits per block

int main(int argCount, char *argValues[]) {
	ifstream file("testFile.txt", ifstream::ate);
	if (!file.is_open())
	{
		cerr << "File cannot be opened." << endl;
		exit(1);
	}

	int fileSize = file.tellg();
	file.seekg(0, ifstream::beg);
	int arraySize;
	if (fileSize % 128 == 0)
	{
		arraySize = fileSize;
	}
	else
	{
		arraySize = (fileSize + (128 - (fileSize % 128)));
	}
	
	char blockBuffer[50000];
	file.read(blockBuffer, fileSize);

	if (fileSize != arraySize)
	{
		int padding = (896 % 1024) - ((fileSize % 128) * 8 + 1);
		int numZeros = (padding - 7) / 8;
		int messageLength = fileSize * 8;
		blockBuffer[fileSize] = 0b10000000;
		for (int i = 0; i < numZeros; i++)
		{
			blockBuffer[++fileSize] = '0';
		}
		blockBuffer[++fileSize] = messageLength;
	}
	
	
	
	
	/*for (int i = 0; i <= fileSize; i++)
	{
		if (i % BUFFER_SIZE == 0)
		{
			messageParse.push_back(&blockBuffer[i]);
		}
	}

	int messageSize = messageParse.size();

	for (int i = 0; i < messageSize; i++)
	{
		for(int j = 0; j <= 
	}
*/
	
	/*
		if (size != 128)
		{
			int padding = (896 % 1024) - (size * 8 + 1);
			int numZeros = (padding - 7) / 8;
			int messageLength = size * 8;
			blockBuffer[size] = 0b10000000;
			for (int i = 0; i < numZeros; i++)
			{
				blockBuffer[++size] = 0;
			}
		}
		messageParse.push_back(blockBuffer);
		cout << size << endl;
		

		cout << size;
		int l = 0;
		for (int i = 0; i < size + 1; i++)
		{
			cout << blockBuffer[i];
			l++;
			if (l >= 10) {
				cout << endl;
				l = 0;
			}
		}
		*/
}