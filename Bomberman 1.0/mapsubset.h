#include <vector>

#ifndef MAPSUBSET_H
#define MAPSUBSET_H


template<class T>
struct MapSubsetStruct {
    T object;
    int x, y;
};



template<class T>
class MapSubset {

private:
    T** objectArray;
    int rows, cols;
    std::vector<MapSubsetStruct<T>*> objectVector;

public:
    MapSubset(int width, int height);
    void insertAt(T, int, int);
    bool isObjectAt(int, int);
    T removeAt(int, int);
    T getObjectAt(int, int);
    std::vector<MapSubsetStruct<T>*>* const getObjectVector();
    void clear();
};



template<class T>
MapSubset<T>::MapSubset(int col, int row) {

    objectArray = new T*[row];

    if (objectArray != NULL) {
        for (int i = 0; i < row; i++) {
            objectArray[i] = new T[col];
            for (int j = 0; j < col; j++) {
                objectArray[i][j]=NULL;
            }
        }
    }

    this->rows = col;
    this->cols = row;
}


template<class T>
void MapSubset<T>::insertAt(T object, int x, int y) {
    //assertassert(objectArray[x][y] == NULL);
    objectArray[x][y] = object;
    MapSubsetStruct<T>* info = new MapSubsetStruct<T>;
    info->object = object;
    info->x = x;
    info->y = y;
    objectVector.push_back(info);
}


template<class T>
bool MapSubset<T>::isObjectAt(int x, int y) {
    if (x > rows || y > cols || x < 0 || y < 0) {
        return false;
    }

    return objectArray[x][y] != NULL;
}


template<class T>
T MapSubset<T>::removeAt(int x, int y) {
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

    return NULL;
}


template<class T>
T MapSubset<T>::getObjectAt(int x, int y) {
    return objectArray[x][y];
}


template<class T>
std::vector<MapSubsetStruct<T>*>* const MapSubset<T>::getObjectVector() {
    return &objectVector;
}


template<class T>
void MapSubset<T>::clear() {
    typename std::vector<MapSubsetStruct<T>*>::iterator
    begin = objectVector.begin(),
            end = objectVector.end();
    while (begin != end) {
        objectArray[(*begin)->x][(*begin)->y] = NULL;
        begin++;
    }
    objectVector.clear();
}

#endif

