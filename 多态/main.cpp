#include <iostream>

using namespace std;

class Shape{
    protected:
        int width,heigh;
    public:
        Shape(int a=0,int b=0){
            width=a;
            heigh=b;
        }
        virtual int area(){
            cout<<"Parent class area:"<<endl;
            return 0;
        }
};
class Rectangle:public Shape{
    public:
    Rectangle(int a=0,int b=0):Shape(a,b){}
    int area(){
        cout << "Rectangle class area :" <<endl;
         return (width * heigh); 
    }
};
int main(){
    Shape *shape;
    Rectangle rec(10,7);
    shape=&rec;
    shape->area();
    return 0;
}