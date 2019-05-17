#include "lanalloc.h"
#include <vector>
#include <deque>
#include <list>
#include <iostream>

using namespace std;

int main() {
    int a[5] = {0,1,2,3,4};
    int val;
    int cap = 4;
    // test on vector
    // cout << "TEST ON VECTOR" << endl;
    // vector<int,LAN::allocator<int>> v(a,a+5);
    // for(int i=0;i<cap;i++) {
    //     cin >> val;
    //     v.push_back(val);
    // }

    // for(int i=0;i<v.size();i++) {
    //     cout << v[i] << " ";
    // }
    // cout << "TEST ON DEQUE" << endl;
    // deque<int,LAN::allocator<int>> dq;
    // for(int i=0;i<cap;i++) {
    //     cin >> val;
    //     dq.push_back(val);
    // }

    // for(int i=0;i<dq.size();i++) {
    //     cout << dq[i] << " ";
    // }
    cout << "TEST ON LIST" << endl;
    list<int,LAN::allocator<int> > l;
    for(int i=0;i<cap;i++) {
        cin >> val;
        l.push_back(val);
    }
    list<int> ::iterator j;
    for(j=l.begin();j!=l.end();j++) {
        cout << (*j) << " ";
    }
}
