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
//返回与所查象限方向的相差180‘的象限内的不可再分节点
//选出删除所需要的候选节点（小于等于4）
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
//查询四个象限的候选点
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
//选出最佳候选点
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
//最佳候选点数组不唯一，选用第二种度量判定法
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
//这是测试上传代码的注释 
int main(){
    treenode t=(quadtree *)malloc(sizeof(quadtree));
    vector<point> list;
    point p;
    cout << "请输入点的坐标值x,y,并以(-1,-1)结束" << endl;
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
    //查询候选节点
    queryCandidatePoints(t);
    //锁定最后唯一的替换点(第一种方法)

    //如果上述方法不能筛选唯一的替换点，使用第二种方法
    vector<point> res;
    for(int i=0;i<ll.size();i++){
        //此处t应该是删除的节点，本次测试选用的是根节点，所以为t
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