// Определить класс «Вектор». Объект класса представляет собой одномерный массив целых чисел. Обязательные методы класса:
// - обращение к отдельному элементу вектора с контролем выхода за пределы индексов;
// - задание произвольных границ индексов при создании объекта;
// - получение границ индексов;
// - поэлементное сложение двух векторов с одинаковыми границами индексов;
// - поэлементное вычитание двух векторов с одинаковыми границами индексов;
// - сравнение двух векторов.

#include <iostream>

using namespace std;

class vec {
    int* mas;
    int start, end;
public:
    ~vec(){ delete[] mas; }

    vec(int start, int end) : start(start), end(end) {
        if (end < start){ return;}
        mas = new int[end-start+1];
    }

    vec(const vec& v) : mas(v.mas), start(v.start), end(v.end) {}

    const int &operator[](const int index) const {
        if (index < start || index > end) { throw invalid_argument("index out of range");}
        return mas[index-start];
    }

    int& operator[](int index) {
        if (index < start || index > end) { throw invalid_argument("index out of range"); }
        return mas[index];
    }

    int getsize() const { return end-start+1; }
    int getstart() const { return  start; }
    int getend() const { return end; }
};

bool operator<(const vec& a, const vec& b) {
    if (a.getsize() < b.getsize()){ return true;}
    return false;
}

bool operator>(const vec& a, const vec& b) {
    if (a.getsize() > b.getsize()){ return true;}
    return false;
}

bool operator==(const vec& a, const vec& b) {
    if (a.getsize() == b.getsize()){ return true;}
    return false;
}



vec operator+(const vec& a, const vec& b) {
    if (a.getsize() != b.getsize()) { return a; }
    vec c(a.getstart(), a.getend());
    for (size_t i = 0; i < a.getsize(); ++i) {
        c[i] = a[i+a.getstart()] + b[i+a.getstart()];
    }
    return c;
}

vec operator-(const vec& a, const vec& b) {
    if (a.getsize() != b.getsize()) { return a; }
    vec c(a.getstart(), a.getend());
    for (size_t i = 0; i < a.getsize(); ++i) {
        c[i] = a[i+a.getstart()] - b[i+b.getstart()];
    }
    return c;
}

vec operator*(const vec& a, const vec& b) {
    if (a.getsize() != b.getsize()) { return a; }
    vec c(a.getstart(), a.getend());
    for (size_t i = 0; i < a.getsize(); ++i) {
        c[i] = a[i+a.getstart()] * b[i+b.getstart()];
    }
    return c;
}

vec operator/(const vec& a, const vec& b) {
    if (a.getsize() != b.getsize()) { return a; }
    vec c(a.getstart(), a.getend());
    for (size_t i = 0; i < a.getsize(); ++i) {
        c[i] = a[i+a.getstart()] / b[i+b.getstart()];
    }
    return c;
}

vec operator%(const vec& a, const vec& b) {
    if (a.getsize() != b.getsize()) { return a; }
    vec c(a.getstart(), a.getend());
    for (size_t i = 0; i < a.getsize(); ++i) {
        c[i] = a[i+a.getstart()] % b[i+b.getstart()];
    }
    return c;
}

vec operator^(const vec& a, const vec& b) {
    if (a.getsize() != b.getsize()) { return a; }
    vec c(a.getstart(), a.getend());
    for (size_t i = 0; i < a.getsize(); ++i) {
        c[i] = a[i+a.getstart()] ^ b[i+b.getstart()];
    }
    return c;
}

vec operator-(const vec& a) {
    vec c(a.getstart(), a.getend());
    for (size_t i = 0; i < a.getsize(); ++i) {
        c[i] = -a[i+a.getstart()];
    }
    return c;
}