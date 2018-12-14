#include <fstream>
#include <cstdint>
#include <cstdio>
#include <algorithm>
#include <queue>
#include <stack>
#include "huffman.hpp"

using namespace std;

Node::Node(uint8_t symbol)
{
	this->letter.symbol = symbol;
	this->left = nullptr;
	this->right = nullptr;
}

Node::Node(Node* left, Node* right)
{
	this->left = left;
	this->right = right;
}

Node::~Node()
{
	if (left)
	{
		delete left;
		delete right;
	}
}

void Node::setCode(uint32_t code, uint8_t codeSize)
{
	this->letter.code = code;
	this->letter.codeSize = codeSize;
}

bool readChar(ifstream& inputF, uint8_t& c)
{
	char _c = '\0';
	bool result = (inputF.peek() != EOF);
	if (result)
		inputF.get(_c);
	c = _c;

	return result;
}

bool compareP(pair<uint8_t, uint_fast32_t> first, pair<uint8_t, uint_fast32_t> second)
{
	return first.second < second.second;
}

bool compareDR(dictionaryRow first, dictionaryRow second)
{
	return (first.codeSize == second.codeSize ? first.code < first.code : first.codeSize < second.codeSize);
}

bool compareDRL(dictionaryRow first, dictionaryRow second)
{
	int firstEqSize = first.code >> (first.codeSize - min(first.codeSize, second.codeSize));
	int secondEqSize = second.code >> (second.codeSize - min(first.codeSize, second.codeSize));
	return (firstEqSize == secondEqSize ? first.codeSize < second.codeSize : firstEqSize < secondEqSize);
}

void getTable(ifstream& inputF, uint16_t& tableSize, dictionaryRow* &table, uint32_t& realFSize)
{
	pair<uint8_t, uint_fast32_t> count[256];
	for (uint_fast16_t i = 0; i < 256; ++i)
		count[i] = {i, 0};

	uint8_t symbol;

	while (readChar(inputF, symbol))
	{
		++count[symbol].second;
		++realFSize;
	}

	sort(count, count + 256, compareP);

	tableSize = 0;
	queue<Node*> q_elements, q_nodes;
	for (uint_fast16_t i = 0; i < 256; ++i)
		if (count[i].second)
		{
			q_elements.push(new Node(count[i].first));
			q_elements.back()->count = count[i].second;

			++tableSize;
		}

	table = (dictionaryRow*) malloc(tableSize * sizeof(dictionaryRow));
	if (!tableSize)
		return;

	if (tableSize == 1)
	{
		table[0].code = 0;
		table[0].codeSize = 1;
		table[0].symbol = q_elements.front()->letter.symbol;
		return;
	}

	while ((q_elements.size() != 0) || (q_nodes.size() != 1))
	{
		Node* work[2];
		for (int i = 0; i < 2; ++i)
		{
			if (!q_nodes.size())
			{
				work[i] = q_elements.front();
				q_elements.pop();
				continue;
			}

			if (!q_elements.size())
			{
				work[i] = q_nodes.front();
				q_nodes.pop();
				continue;
			}

			if (q_elements.front()->count < q_nodes.front()->count)
			{
				work[i] = q_elements.front();
				q_elements.pop();
			}
			else
			{
				work[i] = q_nodes.front();
				q_nodes.pop();
			}
		}
		q_nodes.push(new Node(work[0], work[1]));
		q_nodes.back()->count = work[0]->count + work[1]->count;
	}

	stack<Node*> s_nodes;
	s_nodes.push(q_nodes.front());

	s_nodes.top()->letter.code = 0;
	s_nodes.top()->letter.codeSize = 0;

	uint16_t it = 0;
	while (s_nodes.size())
	{
		Node* currentNode = s_nodes.top();
		s_nodes.pop();
		if (currentNode->left)
		{
			currentNode->right->letter.code = 2 * currentNode->letter.code + 1;
			currentNode->right->letter.codeSize = currentNode->letter.codeSize + 1;
			s_nodes.push(currentNode->right);

			currentNode->left->letter.code = 2 * currentNode->letter.code;
			currentNode->left->letter.codeSize = currentNode->letter.codeSize + 1;
			s_nodes.push(currentNode->left);
		}
		else
			table[it++] = currentNode->letter;
	}

	delete q_nodes.front();
}

void encode(ifstream& inputF, ofstream& outputF, uint16_t& tableSize, dictionaryRow* &table, uint32_t& cryptFSize, uint32_t& realFSize)
{
	sort(table, table + tableSize, compareDRL);

	outputF.write((char*) &tableSize, sizeof(tableSize));
	for (uint16_t i = 0; i < tableSize; ++i)
	{
		outputF.write((char*) &(table[i].code), sizeof(table[i].code));
		outputF << table[i].codeSize << table[i].symbol;
	}
	outputF.write((char*) &realFSize, sizeof(realFSize));

	uint_fast64_t buffer = 0;
	uint_fast8_t bufferSize = 0;

	sort(table, table + tableSize, compareDR);

	uint8_t symbol;
	while (readChar(inputF, symbol))
	{
		for (uint16_t i = 0; i < tableSize; ++i)
			if (table[i].symbol == symbol)
			{
				buffer = (buffer << table[i].codeSize) + table[i].code;
				bufferSize += table[i].codeSize;
				break;
			};
		while (bufferSize >= 8)
		{
			bufferSize -= 8;
			outputF << (uint8_t) (buffer >> bufferSize);
			buffer = buffer % ((uint64_t) 1 << bufferSize);
			++cryptFSize;
		}
	}
	if (bufferSize)
	{
		outputF << (uint8_t) (buffer << (8 - bufferSize));
		++cryptFSize;
	}
}

void readTable(ifstream& inputF, uint16_t& tableSize, dictionaryRow* &table)
{
	inputF.read((char*) &tableSize, sizeof(tableSize));

	table = (dictionaryRow*) malloc(tableSize * sizeof(dictionaryRow));
	for (uint16_t i = 0; i < tableSize; ++i)
	{
		inputF.read((char*) &(table[i].code), sizeof(table[i].code));
		readChar(inputF, table[i].codeSize);
		readChar(inputF, table[i].symbol);
	}
}

void decode(ifstream& inputF, ofstream& outputF, uint16_t& tableSize, dictionaryRow* &table, uint32_t& cryptFSize, uint32_t& realFSize)
{
	inputF.read((char*) &realFSize, sizeof(realFSize));
	queue<Node*> q;
	for (uint16_t i = 0; i < tableSize; ++i)
	{
		q.push(new Node(table[i].symbol));
		q.back()->setCode(table[i].code, table[i].codeSize);
	}

	Node* work[2];
	while (q.size() > 1)
	{
		work[0] = q.front();
		q.pop();
		work[1] = q.front();
		if ((work[0]->letter.codeSize == work[1]->letter.codeSize) && ((work[1]->letter.code ^ work[0]->letter.code) == 1))
		{
			q.pop();
			if (work[0]->letter.code < work[1]->letter.code)
				q.push(new Node(work[0], work[1]));
			else
				q.push(new Node(work[1], work[0]));
			q.back()->setCode(work[0]->letter.code / 2, work[0]->letter.codeSize - 1);
		}
		else
			q.push(work[0]);
	}

	uint_fast32_t symbolCount = 0;
	uint_fast64_t buffer = 0;
	uint_fast8_t bufferSize = 0;
	Node* root = q.front();

	uint8_t symbol;
	bool newSymbol = readChar(inputF, symbol);
	while (newSymbol || bufferSize > 0)
	{
		if (bufferSize + 8 <= 64)
		{
			if (newSymbol)
			{
				++cryptFSize;
				buffer = buffer << 8;
				buffer += symbol;
				bufferSize += 8;
				if (bufferSize + 8 <= 64)
				{
					newSymbol = readChar(inputF, symbol);
					continue;
				}
			}
			newSymbol = readChar(inputF, symbol);
		}

		Node* currentNode = root;
		uint8_t currentCodeSize = 0;
		while (currentNode->left)
		{
			++currentCodeSize;
			if (currentCodeSize > bufferSize)
				return;
			if ((buffer >> (bufferSize - currentCodeSize)) % 2)
				currentNode = currentNode->right;
			else
				currentNode = currentNode->left;
		}

		if (symbolCount >= realFSize)
			return;

		++symbolCount;
		outputF << currentNode->letter.symbol;
		bufferSize -= currentCodeSize;
		buffer = buffer % ((uint64_t) 1 << bufferSize);
	}

	delete root;
}
