#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <istream>
#include <ostream>
#include <string>
#include <stack>
#include <queue>
#include <cstring>
#include <map>
#include <iterator>
#include <list>
#include <set>
#include <functional>
#include <memory.h>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string.h>
#include <strings.h>
#include <utility>
#include <vector>

using namespace std;

#define  DOUINT  pair<int,int>

inline int equals(int a, int b){
	return a == b;
}
inline int Combine(int a, int b,int d){
	if (a > b)
    {
        int c = a;
        a = b;
        b = c;
    }
    return a*100+b*10+d;
}
inline void VertorOut(vector<int> a){
    for (const auto& i : a)
        cout << i << " ";
}


class Node
{
public:
    bool bIsAmplifier;
    void PutAmplifier(void);
private:
    
};

class Edge
{
public:
    unsigned short int iTo;//线终点
    int iDis;//线距离
    vector<int> Edge_num;//线编号
    int* loss;
private:
    
};



class Task
{
public:
    unsigned short int iStart;
    unsigned short int iEnd;
    unsigned int cost;
    vector<int> Route;//路径
    vector<int> EdgeNum;//路径所经历的边的编号
    int pass_num;//所用通道号
    vector<int> AmplifierNode;//所经历的有放大器的点
    
private:
};

int ReadFile();//读文件
bool Sort(const Edge &t1, const Edge &t2);//对输入数据进行排序，已让相同边的距离按大小排序
int ConfirmRoute(Task *ptTask);
int dijkstra(int Start,int End,vector<int> *Route,int j);//求最短路径
void GetRoute(int i,vector<int> *Route);//从父子序列中得到route

bool ConfirmPassNum(vector<int> Route,vector<int> *EdgeNums,int j);//确定任务所用通道号
void UpdateMap(int j,vector<int> Route);
DOUINT GetEdgeNum(int Start,int End,vector<Edge> edge);//通过起止点，确认所用边的编号，根据这个编号确认其在gEdge中的位置
// int GetLineNum(int Start,int End,vector<Line> line);
void SetNewEdge(int Start,int End);//在起止点上放一条新边，它的长度最小，位置在原同边位置上
void GetAmplifierNode(vector<int> *AmplifierNode,vector<int> Route);//返回有放大器的点
void OutPut(Task *pcTask);//输出函数


const int inf = 0x3f3f3f3f; //代表无穷大。


unsigned int iNode_num;    
unsigned int iLine_num;    
unsigned int iTask_num;    
unsigned int iPass_num;    
unsigned int iMax_DisToStance;

//距离原点距离
bool *Deter;
int *DisToS;
Node *node;
vector<Edge> *g_tEdge;//数组，序号是点，内容是边的集合核心
// vector<Line> *g_tline;
int *father;//父子数组，序号是子，内容是父，创建输出路径

int NewEdgeNum = 0;//新增边的数量
vector<int> NewEdgeNode;  //新增边起止集合
vector<vector<int>> input_data;//输入数据
set<int> *passEdge;



/*初始化点状态和每一点原点距离*/
void init()
{
    // for (int i = 0;i < iNode_num;i++)
	memset(DisToS,inf,sizeof(int[iNode_num]));
	memset(Deter,false,sizeof(bool[iNode_num]));
}



int main()
{
    int i;
    int ret;   
    ret = ReadFile();
    if (ret != 0)
    {
        cout << "read err" << endl;
    }

    iNode_num     = input_data[0][0];
    iLine_num     = input_data[0][1];
    iTask_num     = input_data[0][2];
    iPass_num     = input_data[0][3];
    iMax_DisToStance = input_data[0][4];
    /**/
    Deter = new bool[iNode_num];//是否已确定
    DisToS  = new int[iNode_num]; //距离原点距离
    node    = new Node[iNode_num];
    g_tEdge = new vector<Edge>[iNode_num];//数组，序号是点，内容是边的集合核心
    // g_tline = new vector<Line>[iNode_num];
    father  = new int[iNode_num];//父子数组，序号是子，内容是父，创建输出路径
    passEdge = new set<int>[iPass_num];
    
    Edge temp;
    
    for (i = 1; i < iLine_num + 1; i++)
    {
        temp.Edge_num.clear();
        temp.iTo = input_data[i][1];
        temp.iDis = input_data[i][2];
        temp.loss = new int[iPass_num]();
        temp.Edge_num.push_back(i-1);
        // edge_nums[i-1] = Combine(input_data[i][0], input_data[i][1]);
        // memset(temp.PassStatus,0,sizeof(temp.PassStatus));
        g_tEdge[input_data[i][0]].push_back(temp);
        temp.iTo = input_data[i][0];
        g_tEdge[input_data[i][1]].push_back(temp);     
    }
    for (i = 0; i < iNode_num; i++)
    {
        sort(g_tEdge[i].begin(),g_tEdge[i].end(),Sort);
    }
    /**/


    int count;
    Task *tTask = new Task[iTask_num];
    for (i = 0; i < iTask_num; i++)
    {
        tTask[i].pass_num = -1;
        tTask[i].iStart = input_data[1 + iLine_num + i][0];
        tTask[i].iEnd   = input_data[1 + iLine_num + i][1];
        tTask[i].pass_num = ConfirmRoute(&tTask[i]);
        count = 0;
        while(ConfirmPassNum(tTask[i].Route,&tTask[i].EdgeNum,tTask[i].pass_num))
        {
            count ++;
            if ( count > iNode_num)
                break;
        }
        // UpdateMap(tTask[i].pass_num,tTask[i].Route);
        GetAmplifierNode(&tTask[i].AmplifierNode,tTask[i].Route);
    }
    // vector<int> a[iPass_num];
    // for (i = 0; i < iPass_num; i++)
    // {
    //     for (int j = 0; j < iTask_num; j++)
    //     {
    //         if(tTask[j].pass_num == i)
    //         {
    //             for (const auto& n: tTask[j].EdgeNum)
    //                 a[i].push_back(n);
    //         }
    //     }
    //     if (set<int>(a[i].begin(),a[i].end()).size()!=a[i].size())
    //         cout << "err" << endl;
    // }
    OutPut(tTask);
    return 0;
}

int ConfirmRoute(Task *ptTask)
{
    int i;
    int loss[iPass_num];
    vector<int> Route[iPass_num];
    for ( i = 0 ;i < iPass_num;i++)
    {
        init();
        loss[i] = dijkstra(ptTask->iStart,ptTask->iEnd, &Route[i],i);
    }
    // cout <<endl;
    // for(const auto& a : loss)
    //     cout << a << " ";
    int a = min_element(loss,loss + iPass_num) - loss;
    ptTask->Route = Route[a];
    return a;
}

int dijkstra(int Start,int End,vector<int> *Route,int j)
{
    int i, MinDis,pos;//MinDis记录最小值，pos记录最小值的下标
    int m =0;
    father[Start] = -1;
    DisToS[Start] = 0;
    while(1){
        m ++;
		MinDis = inf;
        pos = -1;
		for(i = 0;i < iNode_num ;i++){
			//遍历找出当前的最短的最短路径。
            // cout << i <<" " <<Deter[0] <<" "<< DisToS[i] <<endl;
			if(!Deter[i] && DisToS[i] < MinDis){
				MinDis = DisToS[i];
				pos=i;
			}           
		}
		if(pos == End || m > iNode_num){
			//终点的最短距离已确定
			break;
		}
        // for (const auto& j : ptTask->Route[3])
            // cout << ptTask->Route[3][j] << endl;
		Deter[pos] = true;
		int to,len;//存储以pos为起点的边的终点和权值。
		for(i = 0; i < g_tEdge[pos].size(); i++)//遍历pos顶点所有的边，更新最短路径。
        {
			to = g_tEdge[pos][i].iTo;
            cout << pos << " " << g_tEdge[pos][i].Edge_num[0]<< " "<< g_tEdge[pos][i].loss[j] <<endl;
            len = g_tEdge[pos][i].iDis +  g_tEdge[pos][i].loss[j];
			if(!Deter[to] && DisToS[to] > DisToS[pos] + len){//若这点没确定，而且原D大于已确定点加到此点距离，更新
				DisToS[to] = DisToS[pos] + len;
                father[to] = pos;//下标是子点，内容是父点
            }
		}
	}
    GetRoute(End, Route);
    Route->push_back(End);
    return MinDis;
}

bool ConfirmPassNum(vector<int> Route,vector<int> *EdgeNums,int j)
{
    bool PassIsTrue = false;
    int num = -1;
    int edge_num[Route.size()-1];
    DOUINT tmp;
    int NewNode;
    int dis,b,m,n,x;
    for(m = 0; m < Route.size() - 1; m++)//所经点的循环,对于此点的路径而言，通道j通？不通
    {
        tmp = GetEdgeNum(Route[m],Route[m+1],g_tEdge[Route[m]]);     
        if (g_tEdge[Route[m]][tmp.first].Edge_num.size() > 1)
        {
            for (const auto& a : g_tEdge[Route[m]][tmp.first].Edge_num)
            {   
                if(!passEdge[j].count(a))//此点此边的此通道未占用
                {
                    PassIsTrue = false;
                    edge_num[m] = a;//将此点此边的位置号传回
                    break;
                }
                else
                {
                    PassIsTrue = true;
                }                                       
            }
        }
        else
        {
            for(n = 0; n < tmp.second; n++)//此点所有边的循环
            {
                b = g_tEdge[Route[m]][tmp.first + n].Edge_num[0];
                // passEdge[j].push_back(b); 
                
                if(!passEdge[j].count(b))//此点此边的此通道未占用
                {
                    PassIsTrue = false;
                    edge_num[m] = b;//将此点此边的位置号传回
                    // passEdge[j].pop_back();
                    break;
                }
                else
                {
                    PassIsTrue = true;
                    // passEdge[j].pop_back();
                }                                       
            }
        }          
        if(PassIsTrue == true)
        {
            
            NewNode = m;//将阻碍的边的位置号传回，用来设置新边。
            break;//结束此通道号的循环，进行下一个通道号
        }        
    }
    if(PassIsTrue == false)//此通道号在路径所用边上均有空位，可以将此通道号确定。
    {
        dis = 0;
        n = 0;
        for(m = 0; m < Route.size() -1 ; m++)//所经点的循环
        {
            tmp = GetEdgeNum(Route[m],Route[m+1],g_tEdge[Route[m]]);
            EdgeNums->push_back(edge_num[m]);//将所用到的边的ID放入任务的容器中
            passEdge[j].insert(edge_num[m]);
            if (node[Route[m]].bIsAmplifier == true)
            {
                dis = 0;
            }
            if (edge_num[m] >= iLine_num)
            {
                x = 0;
                for(n = 1; n < tmp.second; n++)//此点所有边的循环
                {   
                    g_tEdge[Route[m]][tmp.first + n].loss[j] = inf;
                } 
                tmp = GetEdgeNum(Route[m + 1],Route[m],g_tEdge[Route[m + 1]]);
                for(n = 1; n < tmp.second; n++)//此点所有边的循环
                {   
                    g_tEdge[Route[m + 1]][tmp.first + n].loss[j] = inf;
                }
            }
            else
            {
                for(n = 0; n < tmp.second; n++)//此点所有边的循环
                {   
                    if (g_tEdge[Route[m]][tmp.first + n].Edge_num[0] == edge_num[m])
                        x = n;
                }
            }
            tmp = GetEdgeNum(Route[m + 1],Route[m],g_tEdge[Route[m + 1]]);
            g_tEdge[Route[m + 1]][tmp.first + x].loss[j] = g_tEdge[Route[m + 1]][tmp.first + x].loss[j] + 1000;
            tmp = GetEdgeNum(Route[m],Route[m+1],g_tEdge[Route[m]]);
            g_tEdge[Route[m]][tmp.first + x].loss[j] = g_tEdge[Route[m]][tmp.first + x].loss[j] + 1000;

            dis = dis + g_tEdge[Route[m]][tmp.first + x].iDis;
            if (dis > iMax_DisToStance)
            {
                node[Route[m]].PutAmplifier();
                dis = g_tEdge[Route[m]][tmp.first + x].iDis;
            }   
        }
        // cout << endl;
        return 0;
    }
    else
    {   
        SetNewEdge(Route[NewNode], Route[NewNode + 1]);
        NewEdgeNum ++;
        return 1;
    }
        
}

//  void UpdateMap(int j,vector<int> Route)
//  {
//     int m,n,b,c;
//     for(m = 0; m < Route.size() - 1; m++)//所经点的循环,对于此点的路径而言，通道j通？不通
//     {
//         DOUINT tmp = GetEdgeNum(Route[m],Route[m+1],g_tEdge[Route[m]]);     
//         if (g_tEdge[Route[m]][tmp.first].Edge_num.size() > 1)
//         {
//             g_tEdge[Route[m]][tmp.first].loss[j] += 1000;

//         }
//         else
//         {
//             for(n = 0; n < tmp.second; n++)//此点所有边的循环
//             {
//                 b = g_tEdge[Route[m]][tmp.first + n].Edge_num[0];
//                 c = g_tEdge[Route[m]][tmp.first + n + 1].Edge_num[0];
//                 if(passEdge[j].count(b))//此点此边的此通道未占用
//                 {
//                     g_tEdge[Route[m]][tmp.first + n].loss[j] += 1000;
//                     break;
//                 }                                       
//             }
//         }                
//     }
//  }

int ReadFile()
{
    int i;
    vector<int> data;
    string line;
    int num;
    getline(cin,line);
    stringstream ss(line);
    while(ss >> num)
    {
        data.push_back(num);
    }
    input_data.push_back(data);
    for (i = 0;i < input_data[0][1] + input_data[0][2]; i++)
    {
        data.clear();
        getline(cin,line);
        stringstream ss(line);
        while(ss >> num)
        {
            data.push_back(num);
        }
        input_data.push_back(data);
    }
    return 0;
}


bool Sort(const Edge &t1, const Edge &t2)
{
    if (t1.iTo * 1000 +  t1.iDis< t2.iTo* 1000 +  t2.iDis)
    {
        return true;
    }
    return false;
}

void GetRoute(int i,vector<int> *Route)
{
    if (father[i] != -1)
    {   
        GetRoute(father[i], Route);
        Route->push_back(father[i]);
    }
}




DOUINT GetEdgeNum(int Start,int End,vector<Edge> edge)
{
    int num = -1;
    int num1 = 0;
    DOUINT tmp;
    for(int i = 0; i < edge.size(); i++)
    {
        if(End == edge[i].iTo)
        {
            if(num < 0)
            {
                num = i;
            }   
            num1 ++;
            // cout << i << " ";
        }
    }
    // cout << endl;
    tmp = make_pair(num, num1);
    return tmp;
}

void SetNewEdge(int Start,int End)
{
    int a;
    if (Start > End)
    {
        a = Start;
        Start = End;
        End = a;
    }
    DOUINT tmp = GetEdgeNum(Start,End,g_tEdge[Start]);
    g_tEdge[Start][tmp.first].Edge_num.push_back(iLine_num + NewEdgeNum);
    tmp = GetEdgeNum(End,Start,g_tEdge[End]);
    g_tEdge[End][tmp.first].Edge_num.push_back(iLine_num + NewEdgeNum);
    NewEdgeNode.push_back(Start);
    NewEdgeNode.push_back(End);
}

void Node::PutAmplifier(void)
{
    bIsAmplifier = true;
}

void GetAmplifierNode(vector<int> *AmplifierNode,vector<int> Route)
{
    // for(const auto& i : Route)
    // {
    //     if(node[i].bIsAmplifier)
    //         AmplifierNode->push_back(i);
    // }
    for(int m = 1; m < Route.size() -1 ; m++)//所经点的循环
    {
        if(node[Route[m]].bIsAmplifier)
             AmplifierNode->push_back(Route[m]);
    }
}

void OutPut(Task *pcTask)
{
    int i;
    vector<int> a;
    cout << NewEdgeNum << endl;
    for (i = 0; i < NewEdgeNum; i++)
    {
        cout << NewEdgeNode[2 * i] << " " << NewEdgeNode[2 * i + 1] << endl;
    }
    for (i = 0; i < iTask_num; i++)
    {
        cout << pcTask[i].pass_num << " " << pcTask[i].EdgeNum.size() << " " << pcTask[i].AmplifierNode.size() << " "; 
        VertorOut(pcTask[i].EdgeNum);
        VertorOut(pcTask[i].AmplifierNode);
        if (i != iTask_num -1)
            cout << endl;
    }
}
