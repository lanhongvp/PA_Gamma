#include "lanalloc.h"
#include <vector>
#include <deque>
#include <iostream>

using namespace std;

int main() {
    int a[5] = {0,1,2,3,4};
    int val;
    int cap = 4;
    // test on vector
    cout << "TEST ON VECTOR" << endl;
    vector<int,LAN::allocator<int>> v(a,a+5);
    for(int i=0;i<cap;i++) {
        cin >> val;
        v.push_back(val);
    }

    for(int i=0;i<v.size();i++) {
        cout << v[i] << " ";
    }
    cout << "TEST ON DEQUE" << endl;
    deque<int,LAN::allocator<int>> dq(10);
    for(int i=0;i<cap;i++) {
        cin >> val;
        dq.push_back(val);
    }

    for(int i=0;i<dq.size();i++) {
        cout << dq[i] << " ";
    }

}
