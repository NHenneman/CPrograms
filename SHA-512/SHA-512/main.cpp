#include <iostream>
#include <cstdlib>
#include "SHA512.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>

using namespace std;

//Function Prototypes
void SetMessageSchedule();

const unsigned int BLOCK_SIZE = 128;			//1024 bits per message block
const unsigned int WORD_SIZE = 8;				//64 bits per message word
const unsigned int messageWords = 16;			//Number of message words
vector<string> messageParse;					//Hold all message blocks
unsigned long long wordBlocks[messageWords];	//Hold 16 words of current message block
unsigned long long messageSchedule[80];			//Hold message schedule

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
	unsigned long long finalHash[8];

	SetMessageSchedule();

	for (int j = 0; j < messageParse.size(); j++)
	{
		//Set initial Hashes
		for (int i = 0; i < 8; i++)
		{
			finalHash[i] = initialHash[i];
		}

		//Create bitstring of message words
		for (int k = 0; k < messageWords; k++)
		{
			bitset<64> b;
			for (int i = 0; i < WORD_SIZE; i++) {
				char c = messageParse[j][i + k * 8];
				for (int j = 7; j >= 0 && c; --j) {
					if (c & 0x1) {
						b.set(8 * i + j);
					}
					c >>= 1;
				}
			}
			wordBlocks[k] = b.to_ullong();
		}

		unsigned long long a, b, c, d, e, f, g, h, t1, t2;

		a = finalHash[0];
		b = finalHash[1];
		c = finalHash[2];
		d = finalHash[3];
		e = finalHash[4];
		f = finalHash[5];
		g = finalHash[6];
		h = finalHash[7];
		for (int t = 1; t <= 79; t++)
		{
			t1 = h + Summation1(e) + Ch(e, f, g) + constants[t] + messageSchedule[t];
			t2 = Summation0(a) + Maj(a, b, c);
			h = g;
			g = f;
			f = e;
			e = d + t1;
			d = c;
			c = b;
			b = a;
			a = t1 + t2;
		}

		finalHash[0] += a;
		finalHash[1] += b;
		finalHash[2] += c;
		finalHash[3] += d;
		finalHash[4] += e;
		finalHash[5] += f;
		finalHash[6] += g;
		finalHash[7] += h;
	}

	cout << hex << finalHash[0] << finalHash[1] << finalHash[2] << finalHash[3]
		 << finalHash[4] << finalHash[5] << finalHash[6] << finalHash[7] << endl;
}

void SetMessageSchedule()
{
	for (int i = 0; i < 80; i++)
	{
		if (i <= 15)
		{
			messageSchedule[i] = wordBlocks[i];
		}
		else
		{
			messageSchedule[i] = Sigma1(messageSchedule[i - 2]) + messageSchedule[i - 7] + Sigma0(messageSchedule[i - 15]) + messageSchedule[i - 16];

		}
	}
}

/*
string TextToBinaryString(string words) {
	string binaryString = "";
	for (char& _char : words) {
		binaryString += bitset<8>(_char).to_string();
	}
	return binaryString;
}
*/