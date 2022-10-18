#include <iostream>
#include <unordered_map>
#include<iterator>
using namespace std;
int main()
{
    unordered_map<int, int> Hash;
    Hash.insert({ {1,3},{2,4} });
    unordered_map<int, int>::iterator it;
    cout << it->first << endl;
  
}