#include <iostream>
#include <array>
using namespace std;

constexpr int foo(int i)
{
    return i + 5;
}

int main()
{
    int i = 10;
    std::array<int, foo(5)> arr; // OK
    
    foo(i); // Call is Ok
    cout << foo(i) << endl;
    cout << "Arr size is " << arr.size() <<endl;

    // output of array
    for(int i=0;i<arr.size();i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    // // But...
    // std::array<int, foo(i)> arr1; // Error
   
}
