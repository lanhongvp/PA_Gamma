#include "vector.h"
#include "list.h"
#include <iostream>

using namespace std;

int main() {
    // test construct function
    cout << "Vector Test" << endl;
    lanstl::vector<int> v(4,2);
    // test overload of operator[]
    for(int i=0;i<v.size();i++) {
        cout << v[i] <<" ";
    }
    cout << endl;
    // test push_back
    v.push_back(5);
    // test size function
    cout << "The cur size of vector is ";
    cout << v.size() <<endl;
    for(int i=0;i<v.size();i++) {
        cout << v[i] <<" ";
    }
    cout << endl;
    // test pop_back
    cout << "Th;e size of vector after pop back is ";
    cout << v.size() <<endl;
    for(int i=0;i<v.size();i++) {
        cout << v[i] << " ";
    }
    cout << endl;

    cout << "List Test" << endl;
    lanstl::list<int> l_t(4,3);
    cout << "The cur size of list is ";
    cout << l_t.size() << endl;
    // test operator []
    for(auto i=0;i<l_t.size();i++) {
        cout << l_t[i] << " ";
    }
    cout << endl;
    // test push_back
    l_t.push_back(5);
    cout << "The size of list after push back ";
    cout << l_t.size() << endl;
    // test push_front
    l_t.push_front(4);
    cout << "The size of list after push front ";
    cout << l_t.size() << endl;
    for(auto i=0;i<l_t.size();i++) {
        cout << l_t[i] << " ";
    }
    cout << endl;
    // test pop_back
    l_t.pop_back();
    cout << "The size of list after pop back ";
    cout << l_t.size() << endl;
    // test operator []
    for(auto i=0;i<l_t.size();i++) {
        cout << l_t[i] << " ";
    }
    cout << endl;

    cout << "The size of list after pop front ";
    l_t.pop_front();
    cout << l_t.size() << endl;
    for(auto i=0;i<l_t.size();i++) {
        cout << l_t[i] << " ";
    }
    cout << endl;
    cout << "Test done" << endl;
}