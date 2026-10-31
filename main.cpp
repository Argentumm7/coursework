#include <iostream>
#include <string>
using namespace std;

class date {
public:
    string day;
    string month;
    string year;
    date(): date("00", "00", "0000") {}
    date(const string& day, const string& month, const string& year) {
        this->day = day;
        this->month = month;
        this->year = year;
        cout << day << ':' << month << ':' << year << endl;
    }
    date(const string& datestr): date(datestr.substr(0,2),datestr.substr(3,2), datestr.substr(6, 4)){}
};

int main() {
    date a;
    date b("26:10:2026");
    date c("26", "10", "2026");
}