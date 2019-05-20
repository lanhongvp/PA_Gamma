#include "vector.h"
#include <iostream>

using namespace std;

int main() {
    // test construct function
    lanstl::vector<int> v(4,0);
    // test overload of operator[]
    cout << "Test overload of operator [] ";
    cout << v[0] <<endl;
    // test push_back
    v.push_back(5);
    // test size function
    cout << "The cur size of vector is ";
    cout << v.size() <<endl;
    for(int i=0;i<v.size();i++) {
        cout << v[i] <<endl;
    }
    // test pop_back
    v.pop_back();
    cout << "The size of vector after pop back is ";
    cout << v.size() <<endl;
    for(int i=0;i<v.size();i++) {
        cout << v[i] <<endl;
    }
}