#include <cstdint>
#include <fstream>

using namespace std;

struct dictionaryRow
{
	uint32_t code;
	uint8_t codeSize;
	uint8_t symbol;
};

class Node
{
public:
	Node* left;
	Node* right;
	uint_fast32_t count;

	dictionaryRow letter;

	Node(uint8_t symbol);
	Node(Node* left, Node* right);

	~Node();

	void setCode(uint32_t code, uint8_t codeSize);
};

bool readChar(ifstream& inputF, char& c);

bool compareP(pair<uint8_t, uint_fast32_t> first, pair<uint8_t, uint_fast32_t> second);

bool compareDR(dictionaryRow first, dictionaryRow second);

bool compareDRL(dictionaryRow first, dictionaryRow second);

void getTable(ifstream& inputF, uint16_t& tableSize, dictionaryRow* &table, uint32_t& realFSize);

void encode(ifstream& inputF, ofstream& outputF, uint16_t& tableSize, dictionaryRow* &table, uint32_t& cryptFSize, uint32_t& realFSize);

void readTable(ifstream& inputF, uint16_t& tableSize, dictionaryRow* &table);

void decode(ifstream& inputF, ofstream& outputF, uint16_t& tableSize, dictionaryRow* &table, uint32_t& cryptFSize, uint32_t& realFSize);
