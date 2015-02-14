/* 
 * File:   ReadFile.h
 * Author: Azarias
 *
 * Created on 2 février 2015, 12:14
 */

#ifndef READFILE_H
#define	READFILE_H

#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <ctype.h>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

class ReadFile {
public:
    ReadFile(const char* fileName);
    vector<vector<vector<int> > > fileToArray();
    void afficherFile();
    virtual ~ReadFile();
private:
    ifstream myFile;
    vector<vector<vector<int> > > myArray;
    vector<int> traite_string(string& string_line);
};

#endif	/* READFILE_H */

