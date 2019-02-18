#include <iostream>
#include <cstdlib>
#include "SHA512.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

const unsigned int BLOCK_SIZE = 128;			//1024 bits per message block
const unsigned int WORD_SIZE = 8;			//64 bits per message word
const unsigned int messageWords = 16;		//Number of message words
vector<string> messageParse;		//Hold all message blocks
string wordBlocks[messageWords];	//Hold 16 words of current message block

int main(int argCount, char *argValues[]) {
	ifstream file("testFile.txt", ifstream::binary | ifstream::ate);
	if (!file.is_open())
	{
		cerr << "File cannot be opened." << endl;
		exit(1);
	}

	int fileSize = file.tellg();
	file.seekg(0, ifstream::beg);

	string contents{ istreambuf_iterator<char>(file), istreambuf_iterator<char>() };
	
	int numBlocks = fileSize / 128;
	int start = 0;
	int end = BLOCK_SIZE;
	for (int i = 0; i < numBlocks; i++)
	{
		start = i * end; 
		messageParse.push_back(contents.substr(start, end));
	}
	string shortBlock = contents.substr(numBlocks * end);
	int padding = ((896 % 1024) - ((shortBlock.length() % 128) * 8 + 1) - 7) / 8;
	shortBlock.append(1, 0x80);
	shortBlock.append(padding, 0);
	shortBlock.append(16, 0);
	messageParse.push_back(shortBlock);

	//cout << padding << endl << shortBlock;

	start = 0;
	end = WORD_SIZE;
	for (int j = 0; j < messageParse.size(); j++)
	{
		for (int k = 0; k < messageWords; k++)
		{
			start = k * end;
			wordBlocks[k] = messageParse[j].substr(start, end);
		}


	}
}