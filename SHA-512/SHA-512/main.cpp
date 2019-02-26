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
const unsigned int WORD_SIZE = 64;				//64 bits per message word
const unsigned int messageWords = 16;			//Number of message words
vector<bitset<1024>> messageParse;				//Hold all message blocks
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

	for (int i = 0; i < numBlocks; i++)
	{
		bitset<1024> w;
		for (int j = 0; j < BLOCK_SIZE; j++) {
			char c = contents[i * BLOCK_SIZE + j];
			for (int k = 0; k <= 7 && c; k++) {
				if (c & 0x1) {
					w.set(8 * j + k);
				}
				c >>= 1;
			}
			//cout << w << endl << endl;
		}
		messageParse.push_back(w);
	}

	string shortBlock = contents.substr(numBlocks * BLOCK_SIZE);
	bitset<1024> s;
	for (int i = 0; i < shortBlock.length(); i++)
	{
		char c = shortBlock[i];
		for (int k = 0; k <= 7 && c; k++) {
			if (c & 0x1) {
				s.set(8 * i + k);
			}
			c >>= 1;
		}
	}

	unsigned int padding = ((896 % 1024) - (shortBlock.length() * 8 + 1));
	s = s << padding;	//Pad message block
	s.set(padding);		//Set first digit of padding
	
	bitset<1024> length(fileSize * 8);
	s = (s << 128) | length;

	messageParse.push_back(s);

	unsigned long long finalHash[8];

	//Set the message schedule
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
				b[i] = messageParse[j][k * WORD_SIZE + i];
			}
			
			wordBlocks[k] = b.to_ullong();
		}

		//Perform the Hash calculations for the message
		/*TODO: check each function for correct implementation*/
		unsigned long long a, b, c, d, e, f, g, h, t1, t2;

		a = finalHash[0];
		b = finalHash[1];
		c = finalHash[2];
		d = finalHash[3];
		e = finalHash[4];
		f = finalHash[5];
		g = finalHash[6];
		h = finalHash[7];

		for (int t = 0; t <= 79; t++)
		{
			t1 = h ^ Summation1(e) ^ Ch(e, f, g) ^ constants[t] ^ messageSchedule[t];
			t2 = Summation0(a) ^ Maj(a, b, c);
			h = g;
			g = f;
			f = e;
			e = d ^ t1;
			d = c;
			c = b;
			b = a;
			a = t1 ^ t2;
		}

		finalHash[0] ^= a;
		finalHash[1] ^= b;
		finalHash[2] ^= c;
		finalHash[3] ^= d;
		finalHash[4] ^= e;
		finalHash[5] ^= f;
		finalHash[6] ^= g;
		finalHash[7] ^= h;
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
			messageSchedule[i] = Sigma1(messageSchedule[i - 2]) ^ messageSchedule[i - 7] ^ Sigma0(messageSchedule[i - 15]) ^ messageSchedule[i - 16];

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