#include <fstream>
#include <iostream>
#include <cstdint>
#include <cstdio>
#include <algorithm>
#include "huffman.hpp"

using namespace std;

uint16_t tableSize;
dictionaryRow* table;

int main(int argc, char* argv[])
{
	char* inputFName;
	char* outputFName;
	bool _key_c, _key_v = false;
	uint8_t it = 1;

	_key_v = true;
	_key_c = false;
	inputFName = "7.in";
	outputFName = "7.out";

	char* _outputFName = tmpnam(nullptr);

	ifstream inputF = ifstream(inputFName, ios_base::binary);
	ofstream outputF = ofstream(_outputFName, ios_base::binary);

	uint32_t realFSize = 0, cryptFSize = 0;

	if (_key_c)
	{
		getTable(inputF, tableSize, table, realFSize);
		inputF.clear();
		inputF.seekg(0, ios::beg);
		encode(inputF, outputF, tableSize, table, cryptFSize, realFSize);
	}
	else
	{
		readTable(inputF, tableSize, table);
		decode(inputF, outputF, tableSize, table, cryptFSize, realFSize);
	}

	inputF.close();
	outputF.close();

	remove(outputFName);
	rename(_outputFName, outputFName);

	uint32_t serviceDataSize = sizeof(tableSize) + sizeof(realFSize) + tableSize * (sizeof(dictionaryRow::code) + sizeof(dictionaryRow::codeSize) + sizeof(dictionaryRow::symbol));
	if (_key_c)
	{
		cout << realFSize << endl;
		cout << cryptFSize << endl;
	}
	else
	{
		cout << cryptFSize << endl;
		cout << realFSize << endl;
	}
	cout << serviceDataSize << endl;

	if (_key_v)
	{
		sort(table, table + tableSize, compareDRL);
		for (int i = 0; i < tableSize; ++i)
		{
			string s;
			uint_fast64_t code = table[i].code;
			for (int j = 0; j < table[i].codeSize; ++j)
			{
				s = (code % 2 ? '1' : '0') + s;
				code /= 2;
			}
			cout << s << ' ' << (int) table[i].symbol << endl;
		}
	}

	return 0;
}
