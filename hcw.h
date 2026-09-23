#ifndef COURSEWORK_HCW_H
#define COURSEWORK_HCW_H

#endif //COURSEWORK_HCW_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

class File {
    string pathin;
    string pathout;
    vector<string> t;
    bool flag;
public:
    File(): pathin(""), pathout(""), flag(false) {}

    File(const string& in, const string& out);

    void Changeout(string path) {
        pathout = path;
    }

    void Save(string path) {
        pathin = path;
        ofstream f(pathin);
        if (!f.is_open()) return;
        f.clear();
        for (size_t i = 0; i < t.size(); i++) {
            f << t[i];
        }
        t.clear();
        flag = false;
    }

    void open(const string& path) {
        t.clear();
        flag = false;
        ifstream f(path);
        if (!f.is_open()) return;
        string line;
        while (getline(f, line)) {
            string word;
            for (const char& c : line) {
                if (c != ' ' || c != '\t') {
                    if (!word.empty()) {
                        t.push_back(word);
                    }
                    else {
                        word += c;
                    }
                }
            }
            if (!word.empty()) t.push_back(word);
        }
    }

};

class interface {

};