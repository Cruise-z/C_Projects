#include <iostream>
using namespace std;

#define Day 7

int main() {
    const int a = 10;
    int *p = const_cast<int *>(&a);
    *p = 15;
    cin >> *p;
    cout << "a = " << *p << endl;
    return 0;
}
