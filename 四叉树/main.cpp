#include <iostream>
#include <vector>
#include <queue>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;


struct point {
    float x;
    float y;
};
typedef struct quadtree{
    point p;
    struct quadtree *leftUp;
    struct quadtree *leftDown;
    struct quadtree *rightUp;
    struct quadtree *rightDown;
}quadtree,*treenode;
vector<point> ll;
void setNodeNULL(treenode &T){
    T->leftDown = NULL;
    T->leftUp = NULL;
    T->rightDown = NULL;
    T->rightUp = NULL;
}
void insertnode(treenode &T,point p){
    treenode newnode=(quadtree *)malloc(sizeof(quadtree));
    if(p.x>=T->p.x&&p.y>=T->p.y){ 
        if(T->rightUp!=NULL)
        {
            insertnode(T->rightUp, p);
        }
        else{
            newnode->p.x = p.x;
            newnode->p.y = p.y;
            T->rightUp = newnode;
            setNodeNULL(newnode);
        }   
    }
    else if(p.x>=T->p.x&&p.y<T->p.y){
        if(T->rightDown!=NULL)
        {
            insertnode(T->rightDown, p);
        }
        else{
            newnode->p.x = p.x;
            newnode->p.y = p.y;
            T->rightDown = newnode;
            setNodeNULL(newnode);
        }       
    }
    else if(p.x<T->p.x&&p.y>=T->p.y)
    {
        if(T->leftUp!=NULL)
        {
            insertnode(T->leftUp, p);
        }
        else{
            newnode->p.x = p.x;
            newnode->p.y = p.y;
            T->leftUp = newnode;
            setNodeNULL(newnode);
        }   
    }
    else if(p.x<T->p.x&&p.y<T->p.y)
    {
        if(T->leftDown!=NULL)
        {
            insertnode(T->leftDown, p);
        }
        else{
            newnode->p.x = p.x;
            newnode->p.y = p.y;
            T->leftDown = newnode;
            setNodeNULL(newnode);
        }  
    }
}
void printNode(treenode T){
    if(T){
        cout << T->p.x << ' ' << T->p.y<<endl;
        printNode(T->rightUp);
        printNode(T->leftUp);
        printNode(T->leftDown);
        printNode(T->rightDown);

    }

}
/*void levelprint(treenode T){
    queue<point> q;
    if(T){
        point p;
        p.x = T->p.x;
        p.y = T->p.y;
        q.push(p);
        while(!q.empty()){
            p=q.front();
            cout << p.x << ' ' << p.y << endl;
            if(T->rightUp)
                //???
            if(T->leftUp)
                //???
            if(T->leftDown)
                //???
            if(T->rightDown)
                //???
        }
    }
}
*/
//�������������޷�������180���������ڵĲ����ٷֽڵ�
//ѡ��ɾ������Ҫ�ĺ�ѡ�ڵ㣨С�ڵ���4��
void opQuad(treenode &t,int i){
    if(i==1){
        if(t->leftDown!=NULL)
            opQuad(t->leftDown,i);
        else
            ll.push_back(t->p);
    }
    else if(i==2){
        if(t->rightDown!=NULL)
            opQuad(t->rightDown,i);
        else
            ll.push_back(t->p);
    }
    else if(i==3){
        if(t->rightUp!=NULL)
            opQuad(t->rightUp,i);
        else
            ll.push_back(t->p);
    }
    else if(i==4){
        if(t->leftUp!=NULL)
            opQuad(t->leftUp,i);
        else
            ll.push_back(t->p);
    }
}
//��ѯ�ĸ����޵ĺ�ѡ��
void queryCandidatePoints(treenode &t){
    if(t->rightUp!=NULL)
        opQuad(t->rightUp,1);
    if(t->leftUp!=NULL)
        opQuad(t->leftUp,2);
    if(t->leftDown!=NULL)
        opQuad(t->leftDown,3);
    if(t->rightDown!=NULL)
        opQuad(t->rightDown,4);
}
bool judgeSize_X(point p1,point p2){
    if(p1.x>=p2.x)
        return true;
    return false;
}
bool judgeSize_Y(point p1,point p2){
    if(p1.y>=p2.y)
        return true;
    return false;
}
//ѡ����Ѻ�ѡ��
bool bestCandidate(point p,treenode &deletenode){
    int flag=0;
    for(int i=0;i<ll.size();i++){
        if(judgeSize_X(p,deletenode->p)&&judgeSize_X(ll[i],deletenode->p)){
            if(!judgeSize_X(p,ll[i]))
                flag++;
        }
        if(!judgeSize_X(p,deletenode->p)&&!judgeSize_X(ll[i],deletenode->p)){
            if(judgeSize_X(p,ll[i]))
                flag++;
        }
        if(judgeSize_Y(p,deletenode->p)&&judgeSize_Y(ll[i],deletenode->p)){
            if(!judgeSize_Y(p,ll[i]))
                flag++;
        }       
        if(!judgeSize_Y(p,deletenode->p)&&!judgeSize_Y(ll[i],deletenode->p)){
            if(judgeSize_Y(p,ll[i]))
                flag++;
        }
        if(flag==2)
            return true;
        
    }
    return false;

}
//��Ѻ�ѡ�����鲻Ψһ��ѡ�õڶ��ֶ����ж���
int judgeByMeasure(vector<point> &list,treenode &deletenode){
    int min=9999,flag=0;
    for(int i=0;i<list.size();i++){
        int measure=(list[i].x-deletenode->p.x)*(list[i].y-deletenode->p.y);
        measure= fabs(measure);
        if(measure<min){
            flag=i;
        }
    }
    return flag;
    
}
//���ǲ����ϴ������ע�� 
int main(){
    treenode t=(quadtree *)malloc(sizeof(quadtree));
    vector<point> list;
    point p;
    cout << "������������ֵx,y,����(-1,-1)����" << endl;
    while(1){       
        cin >> p.x >> p.y;
        if(p.x==-1&&p.y==-1)
            break;
        list.push_back(p);
    }
    t->p.x = list[0].x;
    t->p.y = list[0].y;
    
    setNodeNULL(t);
    for (int i = 1; i < list.size();i++){
        insertnode(t,list[i]);
    }
    //��ѯ��ѡ�ڵ�
    queryCandidatePoints(t);
    //�������Ψһ���滻��(��һ�ַ���)

    //���������������ɸѡΨһ���滻�㣬ʹ�õڶ��ַ���
    vector<point> res;
    for(int i=0;i<ll.size();i++){
        //�˴�tӦ����ɾ���Ľڵ㣬���β���ѡ�õ��Ǹ��ڵ㣬����Ϊt
        cout<<ll[i].x<<' '<<ll[i].y<<endl;
        if(bestCandidate(ll[i],t))
            res.push_back(ll[i]);
    }
    cout<<"*************"<<endl;
    for(int i=0;i<res.size();i++)
        cout<<res[i].x<<' '<<res[i].y<<endl;
    cout<<"**************"<<endl;
    cout<<res[judgeByMeasure(res,t)].x<<' '<< res[judgeByMeasure(res,t)].y<<endl;
       // printNode(t);
    
}