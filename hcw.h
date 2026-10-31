#ifndef COURSEWORK_HCW_H
#define COURSEWORK_HCW_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cctype>

using namespace std;

class cpp_parser;

class File {
    vector<string> t;
    bool flag;
    string encoding = "UTF-8";

    string check_encoding(const string& path) {
        ifstream f(path, ios::binary);
        if (!f.is_open())
            return "Unknown";
        unsigned char bom[3] = {0};
        f.read(reinterpret_cast<char*>(bom), 3);
        if (f.gcount() == 3 && bom[0] == 0xEF && bom[1] == 0xBB && bom[2] == 0xBF) {
            return "UTF-8 BOM";
            }
        return "UTF-8";
    }
public:
    string pathin;
    string pathout;
    File(): pathin(""), pathout(""), flag(false) {}

    File(const string& in, const string& out) : pathin(in), pathout(out), flag(false) {
        open(in);
        pathout = out;
    }
    const string& operator[](size_t index) const { return t[index]; }
    string& operator[](size_t index) {
        flag = true;
        return t[index];
    }
    bool ismodified() const{return flag;}
    void Changeout(string path) {
        pathout = path;
    }

    void Save() {
        ofstream f(pathout);
        if (!f.is_open()) return;
        f.clear();
        for (size_t i = 0; i < t.size(); i++) {
            f << t[i];
            if (i+1 != t.size()) f << '\n';
        }
        flag = false;
    }

    void open(const string& path) {
        t.clear();
        flag = false;
        ifstream f(path);
        if (!f.is_open()) return;
        encoding = check_encoding(path);
        if (encoding == "UTF-8 BOM") {
            f.seekg(3);
        }
        pathin = pathout = path;
        string line;
        while (getline(f, line)) {
            t.emplace_back(line);
        }
    }

    void create(const string& path) {
        pathin = path;
        pathout = path;
        t.clear();
        t.emplace_back("");
        flag = false;
        ofstream f(path);
        if (!f.is_open()) {
            cerr << "Cant create file: " << path << " try another directory";
        }
    }
    string getencoding()const {return encoding;}

    size_t linecount() const {return t.size();}

    ~File() {
    }

};

struct keywords {
    vector<int> line;
    vector<string> word;
    vector<int> index;
};
class parser {
protected:
    keywords k;

    virtual bool isKeyword(const string& word) const = 0;

    void checkandadd(const string& word, int linen, int index) {
        if (word != "") {
            if (isKeyword(word)) {
                k.line.emplace_back(linen);
                k.word.emplace_back(word);
                k.index.emplace_back(index);
            }
        }
    }
public:
    virtual ~parser() {}

    const keywords& getkwords()const {return k;};

    void colour(const File& f) {
        k.line.clear();
        k.index.clear();
        k.word.clear();
        int linen = 0;
        size_t lk = f.linecount();

        for (size_t i = 0; i < lk; i++) {
            string str = f[i];
            int lenght = 0, index = 0;
            linen++;
            string word;
            for (int g = 0; g < str.length(); g++) {
                const char& c = str[g];
                if (isdigit((unsigned char)c) || c == '_' || isalpha((unsigned char)c)) {
                    if (lenght == 0) index = g;
                    word += c;
                    ++lenght;
                }
                else {
                    checkandadd(word, linen, index);
                    word = "";
                    lenght = 0;
                }
            }
            checkandadd(word, linen, index);
            word = "";
            lenght = 0;
        }
    }

};
class cpp_parser : public  parser{
    bool isKeyword(const string& word) const override {
        return (word == "if" || word == "else" || word == "for" || word == "and" ||
                word == "or" || word == "int" || word == "class" || word == "string" ||
                word == "bool" || word == "void" || word == "char" || word == "return" ||
                word == "const");
    }
};

class interface {
    vector<File> files;
    int usageindex = -1;
    cpp_parser pars;
public:
    bool hasActive() const { return usageindex >= 0 && usageindex < (int)files.size(); }

    void newFile(const string& path) {
        files.emplace_back();
        files.back().create(path);
        usageindex = static_cast<int>(files.size() - 1);
    }
    void openFile(const string& path) {
        files.emplace_back();
        files.back().open(path);
        usageindex = static_cast<int>(files.size()-1);

    }
    void saveFile() {
        if (!hasActive()) return;
        files[usageindex].Save();
    }
    void saveAsFile(const string& path) {
        if (!hasActive()) return;
        files[usageindex].pathout = path;
        files[usageindex].Save();
    }

    void closeAllTab() {
        for (size_t i = 0; i < files.size(); i++) {
            if (files[i].ismodified()) {
                cerr << "Warning: File " << files[i].pathin << " has unsaved changes!\n";
            }
        }
        files.clear();
        usageindex = -1;
    }

    void closeTab(int index) {
        if (index < 0 || index >= files.size()) return;
        if (files[index].ismodified()) {
            cerr << "Warning, changes not saved. Do you want to save it?";
        }
        if (usageindex >= index && usageindex != 0) usageindex-=1;
        if (usageindex == 0) usageindex--;
        files.erase(files.begin()+index);
    }

    void update_colour() {
        if (hasActive()) {
            pars.colour(files[usageindex]);
        }
    }
};

#endif //COURSEWORK_HCW_H