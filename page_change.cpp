#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<iomanip>
using namespace std;
/*
合工大自考实践报告
页面置换的 FIFO 和 LRU算法
2023.10.10
测试数据 #为结束输入
7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1 #
*/
int GetDistance(int currentPageID,int page);//按照页面编号获取在物理块内的页面最久未使用的时间（哪个距离当前pageID最远）
class BLOCK{
public:
	int blockNum;	//物理块总数
	int pageNum;	//物理块中的页面数量 
	int *pageID;	//页面号（大小为blockNum）
	int *stayTime;	//页面在物理块中的停留时间（与物理块ID对应）
	BLOCK(int num)
	{
		int i;
		pageNum=0;
		blockNum=num;
		pageID=new int[num];
		stayTime=new int[num];
		for(i=0;i<num;i++)
		{
			pageID[i]=-1;	//初始化每个物理块中没有放置，页面号表示为-1
			stayTime[i]=0;	//初始化停留时间为0
		}
	}
	void Init()
	{
		int i;
		int num=blockNum;
		pageNum=0;
		pageID=new int[num];
		stayTime=new int[num];
		for(i=0;i<num;i++)
		{
			pageID[i]=-1;	//初始化每个物理块中没有放置，页面号表示为-1
			stayTime[i]=0;	//初始化停留时间为0
		}
	}
	void ShowPage()
	{
		int i;
		for(i=0;i<blockNum;i++)
		cout<<"Page["<<i<<"]: "<<pageID[i]<<endl;
	}
	void ShowStayTime()
	{
		int i;
		for(i=0;i<blockNum;i++)
		cout<<"Stay["<<i<<"]: "<<stayTime[i]<<endl;
	}
	int GetLongestStay()	//获取在物理块中停留时间最长的页面所在物理块号 
	{
		int i;
		int max_pos=0;
		for(i=0;i<pageNum;i++)
		if(stayTime[max_pos]<stayTime[i])
		max_pos=i;
		return max_pos;
	}
	int GetRencentNotUse(int currentPageID)	//获取在物理块中最近最久未使用的页面编号 
	{
		//默认currentPageID一定大于等于BLOCKNUM
		int i;
		int DestID=0;
		for(i=0;i<blockNum;i++)
		{
			if(GetDistance(currentPageID,pageID[i])>GetDistance(currentPageID,pageID[DestID]))
			DestID=i;
		}
		return DestID;
	}
};	//物理块数据结构定义
 
//-----------------------全局变量-------------------------
int BLOCKNUM;		//物理块数
int *PVS;			//PageVisitSequence页面访问序列
int PVS_NUM;		//页面访问序列长度
int **replaceTable;	//页面置换表格（维度：BLOCKNUM*PVS_NUM）
int *replaceArray;	//页面置换标志数组（大小为访问页面的次数，存储每次访问是否进行页面置换） 
int *lackArray;		//缺页中断标志数组（大小为访问页面的次数，存储每次访问是否存在缺页中断） 
//-----------------------函数声明-------------------------
void showMenu();							//菜单显示
void InputAndInit();						//数据输入和变量初始化
void ReplaceFIFO(BLOCK block);				//FIFO页面置换算法
int FindPage(int pageID,BLOCK block);		//页面查找（按照页面编号在物理块中查找页面是否存在） 
void ShowReplaceTable();					//置换表格输出
void ReplaceLRU(BLOCK block);				//LRU页面置换算法
void InfoDisplay();							//初始化信息显示 
int GetReplaceTimes();						//获取页面置换总次数
int GetLackTimes();							//获取缺页中断总次数 
//-----------------------函数定义-------------------------
int main()
{
	int select;
	int i;
	cout<<"请按提示输入算法模拟需要的数据..."<<endl;
	InputAndInit();
	BLOCK block(BLOCKNUM);	//定义物理块（注意该语句必须在变量和数据初始化后，否则BLOCKNUM未知） 
	cout<<"信息初始化完成！"<<endl<<endl;
	showMenu();
	cout<<"请输入要进行的操作..."<<endl;
	cin>>select;
	while(1)
	{
		switch(select)
		{
			case 1:
				InfoDisplay();
				cout<<endl;
			break;
			case 2:
				ReplaceFIFO(block);
				cout<<"|=================> FIFO页面调度算法正在执行......"<<endl;
				ShowReplaceTable();
				cout<<"页面置换次数为："<<GetReplaceTimes()<<endl;
				cout<<"缺页中断次数为："<<GetLackTimes()<<endl;
				cout<<"缺页率为："<<(double) GetLackTimes()/20<<endl;
				cout<<endl;
				cout<<endl;
			break;
			case 3:
				ReplaceLRU(block);
				cout<<"|=================> LRU页面调度算法正在执行......"<<endl;
				ShowReplaceTable();
				cout<<"页面置换次数为："<<GetReplaceTimes()<<endl;
				cout<<"缺页中断次数为："<<GetLackTimes()<<endl;
				cout<<"缺页率为："<<(double) GetLackTimes()/20<<endl;
				cout<<endl;
				cout<<endl;
			break;
			case 0:
				cout<<"欢迎下次使用..."<<endl;
			return 0;
			default:
				cout<<"输入有误，请重新输入..."<<endl;
				cout<<endl;
			break; 
		}
		//------防止页面置换和缺页次数计算错误------------- 
		for(i=0;i<PVS_NUM;i++)
		{
			replaceArray[i]=0; 					//页面置换标志数组初始化为0 
			lackArray[i]=0; 					//缺页中断标志数组初始化为0 
		} 
		showMenu();
		cout<<"请输入要进行的操作（退出请输入0）..."<<endl;
		cin>>select;
	}
	
	delete[] PVS;
	delete[] replaceArray;
	delete[] lackArray;
	for(i=0;i<BLOCKNUM;i++)
	delete[] replaceTable[i];
	delete[] replaceTable;
	return 0;
}
//----------------------FIFO页面置换算法--------------------------
void ReplaceFIFO(BLOCK block)			//FIFO页面置换算法
{
	int i,j;
	for(i=0;i<BLOCKNUM;i++)
		for(j=0;j<PVS_NUM;j++)
			replaceTable[i][j]=0;
	block.Init();
	int replacePosition;				//待置换位置 
	for(i=0;i<PVS_NUM;i++)				//依次对页面访问序列的每一个页面PVS[i]进行操作
	{
		for(j=0;j<block.pageNum;j++)
		block.stayTime[j]++;			//每循环一次，物理块（0~pageNum）停留时间自增 
		if(block.pageNum<block.blockNum)
		{
			if(!FindPage(PVS[i],block))	//若页面PVS[i]不存在
			{
				lackArray[i]=1;			//由于访问页面不存在造成页面中断 
				block.pageID[block.pageNum]=PVS[i];
				block.pageNum++;
			}
		}
		else							//FIFO算法（置换停留时间最长的页面所在物理块位置） 
		{
//			TODO:若页面已存在的情况（上述三条语句应该是页面不存在的情况，应加上if(页面在物理块中不存在)的判断） By_2018.06.15_16:11
			if(!FindPage(PVS[i],block))	//若页面PVS[i]不存在
			{
				replaceArray[i]=1;		//由于访问页面不存在且无空闲物理块造成页面置换
				lackArray[i]=1;			//由于访问页面不存在造成页面中断 
				replacePosition=block.GetLongestStay();
				block.pageID[replacePosition]=PVS[i];	//选择停留时间最长的页面置换
				block.stayTime[replacePosition]=0;		//置换后，该页面所在物理位置停留时间清零 
			}
		}
		for(j=0;j<BLOCKNUM;j++)
		replaceTable[j][i]=block.pageID[j];	//将访问一次页面后的结果存入数组中（replaceTable）
	}
}
int FindPage(int pageID,BLOCK block)		//页面查找（按照页面编号在以存放页面的物理块（长度为pageNum）中查找页面是否存在） 
{
	int i=0;
	for(i=0;i<block.pageNum;i++)
	if(block.pageID[i]==pageID)
	break;
	return !(i==block.pageNum);				//若页面存在，则返回1，否则返回0 
}
//----------------------LRU页面置换算法--------------------------
void ReplaceLRU(BLOCK block)			//LRU页面置换算法
{
	int i,j;
	for(i=0;i<BLOCKNUM;i++)
		for(j=0;j<PVS_NUM;j++)
			replaceTable[i][j]=0;
	block.Init();
	int replacePosition;				//待置换位置 
	for(i=0;i<PVS_NUM;i++)				//依次对页面访问序列的每一个页面PVS[i]进行操作
	{
		for(j=0;j<block.pageNum;j++)
		block.stayTime[j]++;			//每循环一次，物理块（0~pageNum）停留时间自增 
		if(block.pageNum<block.blockNum)
		{
			if(!FindPage(PVS[i],block))	//若页面PVS[i]不存在
			{
				lackArray[i]=1;			//由于访问页面不存在造成页面中断 
				block.pageID[block.pageNum]=PVS[i];
				block.pageNum++;
			}
		}
		else							//FIFO算法（置换停留时间最长的页面所在物理块位置） 
		{
//			TODO:若页面已存在的情况（上述三条语句应该是页面不存在的情况，应加上if(页面在物理块中不存在)的判断） By_2018.06.15_16:11
			if(!FindPage(PVS[i],block))	//若页面PVS[i]不存在
			{
				replaceArray[i]=1;		//由于访问页面不存在且无空闲物理块造成页面置换
				lackArray[i]=1;			//由于访问页面不存在造成页面中断 
				replacePosition=block.GetRencentNotUse(i);
				block.pageID[replacePosition]=PVS[i];	//选择停留时间最长的页面置换
				block.stayTime[replacePosition]=0;		//置换后，该页面所在物理位置停留时间清零 
			}
		}
		for(j=0;j<BLOCKNUM;j++)
		replaceTable[j][i]=block.pageID[j];	//将访问一次页面后的结果存入数组中（replaceTable）
	}
}
//----------------------OTHRES--------------------------
void showMenu()						//菜单显示 
{
	cout<<"\t\t|---------------------------------------------------------------|"<<endl;
	cout<<"\t\t|----------------------------MENU-------------------------------|"<<endl;
	cout<<"\t\t|\t1. 初始化信息显示                                       |"<<endl;
	cout<<"\t\t|\t2. FIFO页面置换算法                                     |"<<endl;
	cout<<"\t\t|\t3. LRU页面置换算法                                      |"<<endl;
	cout<<"\t\t|\t0. 退出程序                                             |"<<endl;
	cout<<"\t\t|---------------------------------------------------------------|"<<endl;
}
void InputAndInit()					//数据输入和变量初始化 
{
	int i=0;
	int j=0;
	char PVS_char[100];
	cout<<"请输入物理块数..."<<endl;
	cin>>BLOCKNUM;
	cout<<"请依次输入页面访问序列（0~9，以#结束）..."<<endl;
	cin>>PVS_char[i];
	getchar();
	while(PVS_char[i]!='#')
	{
		i++;
		cin>>PVS_char[i];
		getchar();
	}
	PVS_char[i]='\0';					//设置字符串终止符
	PVS_NUM=i;
	PVS=new int[PVS_NUM];
	for(i=0;i<PVS_NUM;i++)
	PVS[i]=PVS_char[i]-'0';				//字符转数字（页面号）
	
	replaceArray=new int[PVS_NUM];
	lackArray=new int[PVS_NUM];
	for(i=0;i<PVS_NUM;i++)
	{
		replaceArray[i]=0; 					//页面置换标志数组初始化为0 
		lackArray[i]=0; 					//缺页中断标志数组初始化为0 
	} 
	replaceTable=new int*[BLOCKNUM];	//页面置换表初始化
	for(i=0;i<BLOCKNUM;i++)
		replaceTable[i]=new int[PVS_NUM];
}
void ShowReplaceTable()						//置换表格输出 
{
	int i,j;
	cout<<"页面置换过程如下图所示"<<endl<<endl;
	cout<<setw(PVS_NUM*3+14)<<setfill('-')<<"-"<<endl;
	cout.fill(' ');
	cout<<"页面访问序列 ";
	for(i=0;i<PVS_NUM;i++)
	cout<<" "<<setw(2)<<PVS[i];
	cout<<endl;
	cout<<setw(PVS_NUM*3+14)<<setfill('-')<<"-"<<endl;
	cout.fill(' ');
	cout<<"页面置换过程 ";
	for(i=0;i<BLOCKNUM;i++)
	{
		if(i>0)
		cout<<"\t     ";
		for(j=0;j<PVS_NUM;j++)
		{
			if(replaceTable[i][j]!=-1)
			cout<<"|"<<setw(2)<<replaceTable[i][j];
			else cout<<"|"<<setw(2)<<" ";	//-1时代表该物理块无页面，不输出
		}
		cout<<"|"<<endl;
	}
	cout<<setw(PVS_NUM*3+14)<<setfill('-')<<"-"<<endl;
	cout<<"页面置换标志 ";
	cout.fill(' ');
	for(i=0;i<PVS_NUM;i++)
		cout<<" "<<setw(2)<<replaceArray[i];
	cout<<endl;
	cout<<setw(PVS_NUM*3+14)<<setfill('-')<<"-"<<endl;
	cout<<"页面中断标志 ";
	cout.fill(' ');
	for(i=0;i<PVS_NUM;i++)
		cout<<" "<<setw(2)<<lackArray[i];
	cout<<endl;
	cout<<setw(PVS_NUM*3+14)<<setfill('-')<<"-"<<endl<<endl;
}
int GetDistance(int currentPageID,int page)	//按照页面编号获取在物理块内的页面最久未使用的时间（哪个距离当前pageID最远）
{
	int distance=0;
	int i;
	for(i=currentPageID-1;i>=0;i--)
	if(PVS[i]!=page)
	distance++;
	else break;
	return distance;
}
void InfoDisplay()							//初始化信息显示 
{
	int i;
	cout<<"本页面置换模拟算法中: "<<endl;
	cout<<"物理块数为： "<<BLOCKNUM<<endl;
	cout<<"页面访问序列为：";
	for(i=0;i<PVS_NUM;i++)
	cout<<PVS[i]<<" ";
	cout<<endl;
}
int GetReplaceTimes()						//获取页面置换总次数 
{
	int sum=0;
	int i;
	for(i=0;i<PVS_NUM;i++)
	sum+=replaceArray[i];
	return sum;
}
int GetLackTimes()							//获取页面中断总次数 
{
	int sum=0;
	int i;
	for(i=0;i<PVS_NUM;i++)
	sum+=lackArray[i];
	return sum;
}