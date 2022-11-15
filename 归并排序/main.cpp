#include <iostream>
#include <vector>

using namespace std;

void merge(vector<int> &nums,int start,int mid,int end){
    vector<int> temp(nums);
    
}
void mergeSort(vector<int> &nums,int start,int end){
    if(start<end){
        int mid = (start+end)/2;
        mergeSort(nums,start,mid);
        mergeSort(nums,mid+1,end);
        merge(nums,start,mid,end);
    }
}
int main(){

    vector<int> nums;
    int a;
    while(cin>>a){
        nums.push_back(a);
        if(cin.get()=='\n'){
            break;
        }
    }
    
}