#include <fstream>

using namespace std;

int main()
{
	ofstream of = ofstream("4.in", ios_base::binary);
	of << "jhu8byhjoij98h";
	of << (char) 0;
	of << "jhfg8hapaa";
	of << (char) 255;
	of.close();

	of = ofstream("5.in", ios_base::binary);
	for (int i = 0; i < 256; ++i)
		of << (char) i;
	of.close();

	of = ofstream("6.in", ios_base::binary);
	of << "KK";
	of.close();

	of = ofstream("7.in", ios_base::binary);
	of << "jfqouhrguhguoh4982hg9herkgnfovneibh9q0y348gh432i4i9ghw943gh98w43hg8hw34gbekgbkjgnoshgis7y4oghiugshgiuheiugbknl;afjqjpfjqiopjwp4hq8oth47gbinnv,xvn,sbviuwh74343htigwguwhguw4ehg7wh4g7iwh4gi4hglsnjkghsjk";
	of.close();

	return 0;
}
