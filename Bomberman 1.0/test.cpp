#include "mapsubset.h"
#include <iostream>

using namespace std;

MapSubset<int*, 10, 10> ints;
int moo = 1;
char a;

int main(int argc, char *argv[]) {
	cout << ints.getObjectVector().size() << endl;
	ints.insertAt(&moo, 1, 1);
	ints.insertAt(&moo, 1, 2);
	cout << ints.getObjectVector().size() << endl;
	cout << "Testing iterators.." << endl;
	vector<MapSubsetStruct<int*>*> objectVector = ints.getObjectVector();
	vector<MapSubsetStruct<int*>*>::iterator begin = objectVector.begin(), end = objectVector.end();
	while (begin != end) {
		cout << (*begin)->x << ", " << (*begin)->y << endl;
		ints.removeAt((*begin)->x, (*begin)->y);
		begin++;
		cout << ints.getObjectVector().size() << endl;
	}
	cout << "Done!" << endl;
	ints.removeAt(1, 1);
	cout << ints.getObjectVector().size() << endl;
	ints.insertAt(&moo, 1, 1);
	cout << ints.getObjectVector().size() << endl;

	cout << "Press a+enter to continue...";
	cin >> a;
}