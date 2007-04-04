#ifndef MAPUTIL_H
#define MAPUTIL_H

#include <fstream>
#include <string>
#include "mapsubset.h"
#include "model.h"
#include <iostream>
#include <freeglut.h>

using namespace std;

template<class T>
class MapUtilities
{
public:
    static void getMapSize(string, int&, int&);
    static int getMapSizeCols(string);
    static int getMapSizeRows(string);
    static int readMap(string, MapSubset<T*>*, Model*, GLfloat, char);
};

/**
	Reads the file and set the map size. 
**/
template<class T>
void MapUtilities<T>::getMapSize(string filename, int& rows, int& cols)
{

    string line;
    ifstream is;
    is.open(filename.data());

    cols = 0; //columns init
    rows = 0; //rows init

    cout<<"Loading maze data from "<<filename<<endl;

    if (is.is_open() && !is.eof())
    { //Check file and get matrix size
        getline (is, line);
        rows++;

        // Setting length to first row length.
        cols = line.length();

        while(!is.eof())
        {
            getline(is, line);
            rows++;

            if(line.length()!=(uint)cols)
            {
                cout<<"Row length does not match default."<<endl;
                cout<<"Plz check "<<filename<<", row "<<rows<<endl;
                cout<<"Aborting."<<endl;
                exit(EXIT_FAILURE);
            }
        }
    }
    else
    {
        cout<<"Failed to open file!"<<endl;
        exit(EXIT_FAILURE);
    }

    is.close();

    cout<<"Read maze size."<<endl;
    cout<<"Max columns = "<<cols<<endl;
    cout<<"Max rows = "<<rows<<endl;

}

/**
	Returns the nr of columns the map has. 
**/
template<class T>
int MapUtilities<T>::getMapSizeCols(string filename)
{

    string line;
    ifstream is;
    is.open(filename.data());

    int cols = 0; //columns init
    int rows = 0;

    cout<<"Loading maze data from "<<filename<<endl;

    if (is.is_open() && !is.eof())
    { //Check file and get matrix size
        getline (is, line);
        rows++;

        // Setting length to first row length.
        cols = line.length();

        while(!is.eof())
        {
            getline(is, line);
            rows++;

            if(line.length()!=cols)
            {
                cout<<"Row length does not match default."<<endl;
                cout<<"Plz check "<<filename<<", row "<<rows<<endl;
                cout<<"Aborting."<<endl;
                exit(EXIT_FAILURE);
            }
        }
    }
    else
    {
        cout<<"Failed to open file!"<<endl;
        exit(EXIT_FAILURE);
    }

    is.close();

    cout<<"Read maze size."<<endl;
    cout<<"Max columns = "<<cols<<endl;

    return cols;

}

/**
	Returns the nr of rows the map has. 
**/
template<class T>
int MapUtilities<T>::getMapSizeRows(string filename)
{

    string line;
    ifstream is;
    is.open(filename.data());

    int cols = 0; //columns init
    int rows = 0; //rows init

    cout<<"Loading maze data from "<<filename<<endl;

    if (is.is_open() && !is.eof())
    { //Check file and get matrix size
        getline (is, line);
        rows++;

        // Setting length to first row length.
        cols = line.length();

        while(!is.eof())
        {
            getline(is, line);
            rows++;

            if(line.length()!=cols)
            {
                cout<<"Row length does not match default."<<endl;
                cout<<"Plz check "<<filename<<", row "<<rows<<endl;
                cout<<"Aborting."<<endl;
                exit(EXIT_FAILURE);
            }
        }
    }
    else
    {
        cout<<"Failed to open file!"<<endl;
        exit(EXIT_FAILURE);
    }

    is.close();

    cout<<"Read maze size."<<endl;
    cout<<"Max rows = "<<rows<<endl;

    return rows;

}

/**
	Reads the map and inserts new elements of the class T 
	where position match the blockChar, into the subset.
	Also the class needs to be a object sublclass.
**/
template<class T>
int MapUtilities<T>::readMap(string filename,
                             MapSubset<T*>* subset,
                             Model* model,
														 GLfloat modelScale, 
														 char blockChar)
{
    string line;
    ifstream is;
    is.open(filename.data());
    int cnt_rows=0;

    cout<<"Loading maze data from "<< filename <<endl;

    if (is.is_open() && !is.eof())
    { //Read the file
        while(!is.eof())
        {
            getline(is, line);

            for( int i=0;(uint)i<line.length();i++)
            {
                cout<<line.at(i);
                if(line.at(i)==blockChar)
                { //Add object
									subset->insertAt(new T(model, i, cnt_rows, 0, 0, modelScale), i, cnt_rows);
                }
            }
            cout<<" - Blocks("<<line.length()<<")"<<endl;
            cnt_rows++;
        }
    }
    else
    {
        cout<<"Failed to open file!"<<endl;
        exit(EXIT_FAILURE);
    }

    is.close();

    cout<<endl<<"Done loading maze data."<<endl;
    return 1;

}

#endif
