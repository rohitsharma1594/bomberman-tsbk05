#include <vector>
#include <assert.h>

#ifndef MAPSUBSET_H
#define MAPSUBSET_H


template<class T>
struct MapSubsetStruct {
	T object;
	int x, y;
};



template<class T, int width, int height>
class MapSubset {
	T objectArray[width][height];
	std::vector<MapSubsetStruct<T>*> objectVector;
public:
	MapSubset();
	void insertAt(T, int, int);
	bool isObjectAt(int, int);
	T removeAt(int, int);
	T getObjectAt(int, int);
	std::vector<MapSubsetStruct<T>*>* const getObjectVector();
	void clear();
};







template<class T, int width, int height>
MapSubset<T, width, height>::MapSubset() {
}


template<class T, int width, int height>
void MapSubset<T, width, height>::insertAt(T object, int x, int y) {
	assert(objectArray[x][y] == NULL);
	objectArray[x][y] = object;
	MapSubsetStruct<T>* info = new MapSubsetStruct<T>;
	info->object = object;
	info->x = x;
	info->y = y;
	objectVector.push_back(info);
}


template<class T, int width, int height>
bool MapSubset<T, width, height>::isObjectAt(int x, int y) {
	return objectArray[x][y] != NULL;
}


template<class T, int width, int height>
T MapSubset<T, width, height>::removeAt(int x, int y) {
	objectArray[x][y] = NULL;

	typename std::vector<MapSubsetStruct<T>*>::iterator begin = objectVector.begin(), end = objectVector.end();
	while (begin != end) {
		if ((*begin)->x == x && (*begin)->y == y) {
			T temp = (*begin)->object;
			objectVector.erase(begin);
			return temp;
		}
		begin++;
	}

	assert(false);
	return NULL; // this should never happen, we should actually crash here, I think assert does this?
}
	

template<class T, int width, int height>
T MapSubset<T, width, height>::getObjectAt(int x, int y) {
	return objectArray[x][y];
}


template<class T, int width, int height>
std::vector<MapSubsetStruct<T>*>* const MapSubset<T, width, height>::getObjectVector(){
	return &objectVector;
}


template<class T, int width, int height>
void MapSubset<T, width, height>::clear() {
	typename std::vector<MapSubsetStruct<T>*>::iterator begin = objectVector.begin(), end = objectVector.end();
	while (begin != end) {
		objectArray[(*begin)->x][(*begin)->y] = NULL;
		begin++;
	}
	objectVector.clear();
}



#endif

