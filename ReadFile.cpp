/* 
 * File:   ReadFile.cpp
 * Author: Azarias
 * 
 * Created on 2 février 2015, 12:14
 */

#include "ReadFile.hpp"

ReadFile::ReadFile(const char* fileName) {
    myFile.open(fileName);
}

vector<vector<vector<int> > > ReadFile::fileToArray() {
    string line;
    if (myFile.is_open()) {
        while (getline(myFile, line)) {
            vector<vector<int>> untableau;
            while (line[0] != '$') {
                untableau.push_back(traite_string(line));
                getline(myFile, line);
            }
            myArray.push_back(untableau);

        }
        myFile.close();
    } else {
        cout << "Fichier non ouvert" << endl;
        exit(EXIT_FAILURE);
    }
    return myArray;
}

void ReadFile::afficherFile() {
    for (int tableau = 0; tableau < myArray.size(); tableau++) {
        for (int i = 0; i < myArray[tableau].size(); i++) {
            for (int j = 0; j < myArray[tableau][i].size(); j++) {
                cout << myArray[tableau][i][j] << " ";
            }
            cout << endl;
        }
        cout << endl << "=========" << endl;
    }

}

vector<int> ReadFile::traite_string(string& string_line) {
    vector<int> maliste;
    for (int i = 0; i < string_line.length(); i++) {
        string total = "";
        while (string_line[i] != ',' && i < string_line.length() && string_line[i] != '\n' && !isspace(string_line[i])) {
            total += string_line[i];
            i++;
        }
        int monEntier = atoi(total.c_str());
        maliste.push_back(monEntier);


    }
    return maliste;
}

ReadFile::~ReadFile() {

}

