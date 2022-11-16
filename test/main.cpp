#include <iostream>

#include<iterator>
using namespace std;
int main()
{
    char buffer[]={"hello"};
    char *end=buffer+4;
    char a=(*end);
    cout<<int(a);

}
    