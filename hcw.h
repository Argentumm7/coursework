#ifndef COURSEWORK_HCW_H
#define COURSEWORK_HCW_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cctype>
#include <algorithm>

using namespace std;

class cpp_parser;

class file_open_error : public runtime_error {
    string path_;
public:
    explicit file_open_error(const string& path) :
        runtime_error("Не удалось открыть файл для чтения: " + path),
        path_(path)
    {}
    const string& get_path()const { return path_; }
};

class file_save_error : public runtime_error {
    string path_;
public:
    explicit file_save_error(const string& path) :
        runtime_error("Не удалось сохранить в файл: " + path),
        path_(path)
    {}
    const string& get_path()const { return path_; }
};


class file_create_error : public runtime_error {
    string path_;
public:
    explicit file_create_error(const string& path) :
        runtime_error("Не удалось создать файл: " + path),
        path_(path)
    {}
    const string& get_path()const { return path_; }
};

class index_out_of_range_error : public runtime_error {
    size_t index_, size_;
public:
    index_out_of_range_error(size_t index, size_t size)
        : runtime_error("Индекс " + to_string(index) +
                        " вне диапазона [0, " + to_string(size) + ")"),
          index_(index), size_(size)
    {}
    size_t get_index() const noexcept { return index_; }
    size_t get_size()  const noexcept { return size_; }
};

class no_active_file_error : public runtime_error {
public:
    no_active_file_error() : runtime_error("Нет активного файла") {}
};

class unsaved_changes_error : public runtime_error {
    string path_;
public:
    explicit unsaved_changes_error(const string& path)
        : runtime_error("Есть несохранённые изменения: " + path),
          path_(path)
    {}
    const string& get_path() const noexcept { return path_; }
};

class File {
    vector<string> t;
    bool flag;
    string encoding = "UTF-8";

    string check_encoding(const string& path) {
        ifstream f(path, ios::binary);
        if (!f.is_open())
            throw file_open_error(path);
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
    const string& operator[](size_t index) const {
        if (index >= t.size())
            throw index_out_of_range_error(index, t.size());
        return t[index];
    }
    string& operator[](size_t index) {
        if (index >= t.size())
            throw index_out_of_range_error(index, t.size());
        flag = true;
        return t[index];
    }

    bool ismodified() const{return flag;}
    void Changeout(string path) {
        pathout = path;
    }

    void Save() {
        if (pathout.empty())
            throw file_save_error("пустой путь");
        ofstream f(pathout);
        if (!f.is_open()) throw file_save_error(pathout);
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
        if (!f.is_open()) throw file_open_error(path);
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
        ofstream f(path);
        if (!f.is_open()) throw file_create_error(path);
        pathin = path;
        pathout = path;
        t.clear();
        t.emplace_back("");
        flag = false;
    }
    string getencoding()const {return encoding;}

    size_t linecount() const {return t.size();}

    ~File() {
    }

};

struct TrieNode {
    TrieNode* children[26] = {nullptr};
    bool isend = false;
    std::string word;
};

class Trie {
    TrieNode* root;
    void collect(TrieNode* node, std::vector<std::string>& answ) {
        if (node == nullptr) return;
        if (node->isend) answ.emplace_back(node-> word);
        for (int i = 0; i < 26; i++) {
            collect(node->children[i], answ);
        }
    }
    void destroy(TrieNode* node) {
        if (!node) return;
        for (int i = 0; i < 26; ++i)
            destroy(node->children[i]);
        delete node;
    }
    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;

public:
    Trie(): root(new TrieNode()){}

    ~Trie() {
        destroy(root);
    }
    void insert(const std::string& word) {
        TrieNode* node = root;
        for (char c : word) {
            if (c < 'a' || c > 'z') {
                continue;
            }
            int idx = c - 'a';
            if (!node->children[idx]) {
                node->children[idx] = new TrieNode();
            }
            node = node->children[idx];
        }
        node->isend = true;
        node->word = word;
    }

    std::vector<std::string> gettokens(const std::string& prefix) {
        TrieNode* node = root;
        for (char c : prefix) {
            if (c < 'a' || c > 'z') {
                return {};
            }
            int idx = c-'a';
            if (!node->children[idx]) return {};
            node = node->children[idx];
        }
        std::vector<std::string> answ;
        collect(node, answ);
        return answ;
    }

    bool contains(const std::string& word) const { //разобрать где и зачем это применимо
        TrieNode* node = root;
        for (char c : word) {
            if (c < 'a' || c > 'z') return false;
            int idx = c - 'a';
            if (!node->children[idx]) return false;
            node = node->children[idx];
        }
        return node->isend;
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
    Trie trie;
public:
    cpp_parser() {
        for(const std::string& kw:{
            "if", "else", "for", "and", "or",
               "int", "class", "string", "bool", "void",
               "char", "return", "const"}) {
            trie.insert(kw);
               }
    }
    std::vector<std::stirng> viewtokens(const std::string& prefix) {
        return trie.gettokens(prefix);
    }
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
        try {
            files.back().create(path);
        } catch (...) {
            files.pop_back();
            throw;
        }
        usageindex = static_cast<int>(files.size() - 1);
    }

    void openFile(const string& path) {
        files.emplace_back();
        try {
            files.back().open(path);
        } catch (...) {
            files.pop_back();
            throw;
        }
        usageindex = static_cast<int>(files.size() - 1);
    }

    void saveFile() {
        if (!hasActive()) throw no_active_file_error();
        files[usageindex].Save();
    }

    void saveAsFile(const string& path) {
        if (!hasActive()) throw no_active_file_error();
        if (path.empty()) throw file_save_error("<пустой путь>");
        files[usageindex].pathout = path;
        files[usageindex].Save();
    }

    void closeAllTab() {
        for (size_t i = 0; i < files.size(); i++) {
            if (files[i].ismodified())
                throw unsaved_changes_error(files[i].pathin);
        }
        files.clear();
        usageindex = -1;
    }
    void closeTab(int index) {
        if (index < 0 || index >= (int)files.size())
            throw index_out_of_range_error(index, files.size());
        if (files[index].ismodified())
            throw unsaved_changes_error(files[index].pathin);
        files.erase(files.begin() + index);
        if (files.empty()) {
            usageindex = -1;
        } else if (index < usageindex) {
            usageindex--;
        } else if (index == usageindex) {
            usageindex = min(usageindex, (int)files.size() - 1);
        }
    }

    void update_colour() {
        if (!hasActive()) throw no_active_file_error();
        pars.colour(files[usageindex]);
    }
};

#endif //COURSEWORK_HCW_H