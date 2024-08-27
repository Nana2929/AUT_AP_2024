
#include<vector>
#include<iostream>

using namespace std;
int main(){
    vector<int> v{1,2,3,4,5};
    // cout the address
    cout << &v << endl;
    v = {1,2,5,4,5,6}; // you can change the size of the vector/the content and the address will remain the same
    cout << &v << endl;
    for (int i = 0; i < v.size(); i++){
        cout << v[i] << " ";
    }

}