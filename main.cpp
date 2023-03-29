#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <istream>
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
    int Edge_num;//线编号
    bool *PassStatus;//线状态数组
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
int Sort();//对输入数据进行排序，已让相同边的距离按大小排序

void dijkstra(Task *ptTask);//求最短路径
void GetRoute(int i,vector<int> *Route);//从父子序列中得到route

int ConfirmPassNum(vector<int> Route,int *NewNode,vector<int> *EdgeNums);//确定任务所用通道号
DOUINT GetEdgeNum(int Start,int End,vector<Edge> edge);//通过起止点，确认所用边的编号，根据这个编号确认其在gEdge中的位置
void SetNewEdge(int Start,int End);//在起止点上放一条新边，它的长度最小，位置在原同边位置上
void SetAm(int Start,int End);
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
int *father;//父子数组，序号是子，内容是父，创建输出路径

int NewEdgeNum = 0;//新增边的数量
vector<int> NewEdgeNode;  //新增边起止集合
vector<vector<int>> input_data;//输入数据




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
    father  = new int[iNode_num];//父子数组，序号是子，内容是父，创建输出路径
    Sort();

    Edge temp;
    for (i = 1; i < iLine_num + 1; i++)
    {
        temp.iTo = input_data[i][1];
        temp.iDis = input_data[i][2];
        temp.Edge_num = input_data[i][4];
        temp.PassStatus = new bool[iPass_num];
        memset(temp.PassStatus, false, sizeof(bool[iPass_num]));
        // edge_nums[i-1] = Combine(input_data[i][0], input_data[i][1]);
        // memset(temp.PassStatus,0,sizeof(temp.PassStatus));
        g_tEdge[input_data[i][0]].push_back(temp);
        temp.iTo = input_data[i][0];
        g_tEdge[input_data[i][1]].push_back(temp);      
    }
    /**/
    Task *tTask = new Task[iTask_num];
    for (i = 0; i < iTask_num; i++)
    {
        tTask[i].pass_num = -1;
        int NewNode;
        init();
        tTask[i].iStart = input_data[1 + iLine_num + i][0];
        tTask[i].iEnd   = input_data[1 + iLine_num + i][1];
        DisToS[tTask[i].iStart] = 0;
        dijkstra(&tTask[i]);
        tTask[i].cost   = DisToS[tTask[i].iEnd];
        // cout << tTask[i].cost << endl;
        // for (int j = 0; j < tTask[i].Route.size(); j++)
        // {
        //     cout <<tTask[i].Route[j] << "  ";        
        // }
        // cout << endl;

        while(tTask[i].pass_num < 0)
        {
            tTask[i].pass_num = ConfirmPassNum(tTask[i].Route,&NewNode,&tTask[i].EdgeNum);
            if(tTask[i].pass_num < 0)
            {
                SetNewEdge(tTask[i].Route[NewNode],tTask[i].Route[NewNode + 1]); 
                NewEdgeNum ++;
            }
        }
        GetAmplifierNode(&tTask[i].AmplifierNode,tTask[i].Route);

    }
    OutPut(tTask);
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
    //     sort(a[i].begin(),a[i].end());
    //     // for (const auto& n: a[i])
    //     //      cout << n << " ";
    //     // cout << endl;
    //     if (set<int>(a[i].begin(),a[i].end()).size()!=a[i].size())
    //         cout << i << endl;
    // }
    
    return 0;
}

void dijkstra(Task *ptTask)
{
    int i, MinDis,pos;//MinDis记录最小值，pos记录最小值的下标
    father[ptTask->iStart] = -1;
    while(1){
		MinDis = inf;
        pos = -1;
		for(i = 0;i < iNode_num ;i++){
			//遍历找出当前的最短的最短路径。
			if(!Deter[i] && DisToS[i] < MinDis){
				MinDis = DisToS[i];
				pos=i;
			}           
		}
		if(pos == ptTask->iEnd){
			//终点的最短距离已确定
			break;
		}
        // for (const auto& j : ptTask->Route[3])
            // cout << ptTask->Route[3][j] << endl;
		Deter[pos] = true;

		int to,len;//存储以pos为起点的边的终点和权值。
		for(i = 0; i < g_tEdge[pos].size(); i++){
			//遍历pos顶点所有的边，更新最短路径。
			to = g_tEdge[pos][i].iTo;
            len = g_tEdge[pos][i].iDis;
			if(!Deter[to] && DisToS[to] > DisToS[pos] + len){//若这点没确定，而且原D大于已确定点加到此点距离，更新
				DisToS[to] = DisToS[pos] + len;
                father[to] = pos;//下标是子点，内容是父点
            }
		}
	}
    GetRoute(ptTask->iEnd, &ptTask->Route);
    ptTask->Route.push_back(ptTask->iEnd);
}

int ConfirmPassNum(vector<int> Route,int *NewNode,vector<int> *EdgeNums)
{
    bool PassIsTrue = false;
    int num = -1;
    int edge_num[Route.size()-1];
    DOUINT tmp;
    int dis;
    for (int j = 0; j < iPass_num; j++)//通道循环
    {
        for(int m = 0; m < Route.size() - 1; m++)//所经点的循环
        {
            tmp = GetEdgeNum(Route[m],Route[m+1],g_tEdge[Route[m]]);
            
            for(int n = 0; n <  tmp.second; n++)//此点所有边的循环
            {
                if(g_tEdge[Route[m]][ tmp.first+n].PassStatus[j] == false)//此点此边的此通道未占用
                {
                    PassIsTrue = false;
                    edge_num[m] = n;//将此点此边的位置号传回
                    break;
                }                    
                PassIsTrue = true;
            }
            if(PassIsTrue == true)
            {
                *NewNode = m;//将阻碍的边的位置号传回，用来设置新边。
                break;//结束此通道号的循环，进行下一个通道号
            }        
        }
        if(PassIsTrue == false)//此通道号在路径所用边上均有空位，可以将此通道号确定。
        {
            dis = 0;
            for(int m = 0; m < Route.size() -1 ; m++)//所经点的循环
            {
                tmp = GetEdgeNum(Route[m],Route[m+1],g_tEdge[Route[m]]);
                g_tEdge[Route[m]][ tmp.first + edge_num[m]].PassStatus[j] = true;//此边此通号占用
                EdgeNums->push_back(g_tEdge[Route[m]][tmp.first + edge_num[m]].Edge_num);//将所用到的边的ID放入任务的容器中
                // if(g_tEdge[Route[m]][tmp.first + edge_num[m]].iTo != Route[m + 1])
                // {   cout << Route[m] <<  " " << Route[m + 1];}
                tmp = GetEdgeNum(Route[m+1],Route[m],g_tEdge[Route[m + 1]]);
                g_tEdge[Route[m + 1]][ tmp.first + edge_num[m]].PassStatus[j] = true;
                tmp = GetEdgeNum(Route[m],Route[m+1],g_tEdge[Route[m]]);
                if (node[Route[m]].bIsAmplifier == true)
                {
                    dis = 0;
                }   
                dis = dis + g_tEdge[Route[m]][tmp.first + edge_num[m]].iDis;
                if (dis > iMax_DisToStance)
                {
                    node[Route[m]].PutAmplifier();
                    dis = g_tEdge[Route[m]][tmp.first + edge_num[m]].iDis;
                }   
            }
            num = j;
            // cout << endl;
            break;
        }
    }
    return num;
}

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


int Sort()
{
    int i;
    int Edge_num[iLine_num];
    for (i = 1; i < iLine_num + 1; i++)
    {
        input_data[i].push_back(Combine(input_data[i][0], input_data[i][1],input_data[i][2]));
        input_data[i].push_back(i-1);
    }
    sort(input_data.begin()+1,input_data.begin()+1+iLine_num,[](vector<int>a, vector<int>b){return a[3]<b[3];});
    return 0;
}

void GetRoute(int i,vector<int> *Route)
{
    if (father[i] != -1)
    {   
        GetRoute(father[i], Route);
        Route->push_back(father[i]);
    }
}



// int *CountPassNum(vector<int> Route,int num,vector<int>* Task_Lines)
// {
//     int i;
//     for(i = 0; i < Route.size() - 1; i++)
//     {
//         int a = Combine(Route[i],Route[i + 1]);
//         Task_Lines->push_back(a);
//     }
// }


void check(vector<int> &src, map<int,int> &dst)
{
	for (size_t i = 0; i < src.size(); ++i) {
		int checkStr = src[i];
		int flag = 0;
		for (size_t j = 0; j < i; ++j) {
			if (equals(checkStr, src[j])) {
				flag = 1;
				break;
			}
		}
		if (!flag) {
			int count = 1;
			for (size_t j = i + 1; j < src.size(); ++j) {
				if (equals(checkStr, src[j])){
					++count;
				}
			}
			dst.insert(pair<int, int>(checkStr, count));
		}
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
                num = i;
            num1 ++;
        }
    }
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
    g_tEdge[Start].insert(g_tEdge[Start].begin() + tmp.first,g_tEdge[Start][tmp.first]);
    memset(g_tEdge[Start][tmp.first].PassStatus, false, sizeof(bool[iPass_num]));
    g_tEdge[Start][tmp.first].Edge_num = iLine_num + NewEdgeNum;
    DOUINT tmp1 = GetEdgeNum(End,Start,g_tEdge[End]);
    g_tEdge[End].insert(g_tEdge[End].begin() + tmp1.first,g_tEdge[Start][tmp.first]);
	g_tEdge[End][tmp1.first].iTo = Start;
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
        cout << endl;
    }
}
