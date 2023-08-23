#include "roadplan.h"
//#define num 20    
int points[N][2];
double alpha=2,beta=5,rou=0.1,alpha1=0.1,qzero=0.1;
double dis[N+1][N+1],Lnn;
struct AntColonySystem
{
	double info[N+1][N+1],visible[N+1][N+1];
};
struct ACSAnt
{
	struct AntColonySystem *antColony;
	int startCity, cururentCity;
	bool allowed[N];	
	int Tour[N][2];
	int currentTourIndex;
};
struct Tour_
{
	int Tour[N][2];
};
struct Tour_ Search(struct ACSAnt* ants,int num);	
int Choose(struct ACSAnt* ants,int num);
void MoveToNextCity(int nextCity,struct ACSAnt* ants);
void UpdateLocalPathRule(int i, int j,struct ACSAnt* ants);	//局部更新规则	
double Transition(int i, int j,struct ACSAnt* ants);	//计算当前节点到下一节点转移的概率
void InitParameter(double value,struct AntColonySystem* acs,int num);	//初始化	
double CalAdjacentDistance(int node,int num);	
double calculateSumOfDistance(struct Tour_ Tourx,int nums);	
int ChooseNextNode(int currentNode, int visitedNode[],int num) ;
void MoveToNextCity(int nextCity,struct ACSAnt* ants);	//移动到下一节点
void UpdateGlobalPathRule(int* bestTour, int globalBestLength,struct AntColonySystem* acs,int num);

struct data calc_path(struct data datax,int num) 
{
	for(int i=0;i<num;i++)
	{
		points[i][0]=(int)((datax.map_read[i][0]+10)/20),points[i][1]=(int)((datax.map_read[i][1]+10)/20);
	}
	
	for(int i=0;i<num;i++)
		dis[i][i]=1000;
	
	struct AntColonySystem* acs = (struct AntColonySystem*)malloc(sizeof(struct AntColonySystem));	//锟斤拷群系统锟斤拷锟斤拷
	struct ACSAnt* ants=(struct ACSAnt*)malloc(sizeof(struct ACSAnt));
	for(int i=0;i<num;i++)
	for(int j=i+1;j<num;j++)
	{
		 dis[i][j]=dis[j][i]=(double)sqrt((points[i][0]-points[j][0])*(points[i][0]-points[j][0])+(points[i][1]-points[j][1])*(points[i][1]-points[j][1]));
	}
	 
	int node = rand() % num;	
	Lnn = CalAdjacentDistance(node,num);
	double initInfo = 1 / (num * Lnn);
	InitParameter(initInfo,acs, num);
	
	int globalTour[N][2];      			
	double globalBestLength = 10000;	 
	
	
	for (int i = 0; i < NcMax; i++)    
	{
		int localTour[num][2];	
		double localBestLength = 10000;	
		double tourLength=0;
		ants->antColony=acs;
		for (int j = 0; j < num; j++)
		{
			ants->startCity=(int)(j%num);
			struct Tour_ Tourx = Search(ants, num);
			tourLength = calculateSumOfDistance(Tourx, num);
			if (tourLength < localBestLength )
			{
				for (int m = 0; m< num; m++)
				{
					int row = Tourx.Tour[m][0];
					int col = Tourx.Tour[m][1];
					localTour[m][0] = row;
					localTour[m][1] = col;
				}
				
				localBestLength = tourLength;
			}
		}
		if (localBestLength < globalBestLength || abs(globalBestLength - 0.0) < 0.000001)
		{
			for (int m = 0; m< num; m++)
			{
				globalTour[m][0] = localTour[m][0];
				globalTour[m][1] = localTour[m][1];
			}
			globalBestLength = localBestLength;
		}
		UpdateGlobalPathRule(*globalTour, globalBestLength,acs,num);
	}
	double dis=sqrt(points[0][0]*points[0][0]+points[0][1]*points[0][1]),disx;
	int startx=0;
	for(int i=1;i<num;i++)
	{
		disx=sqrt(points[i][0]*points[i][0]+points[i][1]*points[i][1]);
		if(disx<dis)
		{
			startx=i;
			dis=disx;
		}
	}
	struct data datay;

	for(int i=0;i<num;i++)
	{
		if(globalTour[i][0]==startx)//            
		{
			int k=0,m=(i+1==num?0:i+1),n=(i-1==-1?num-1:i-1);
			//zean已修改
			if(datax.map_read[globalTour[m][0]][0]>datax.map_read[globalTour[i][0]][0]||(datax.map_read[globalTour[m][0]][0]>datax.map_read[globalTour[n][0]][0]))
			{
				for(int j=i;j>=0;j--)
				{
					datay.map_read[k][0]=datax.map_read[globalTour[j][0]][0];
					datay.map_read[k][1]=datax.map_read[globalTour[j][0]][1];
					k++;
				}
				for(int j=num-1;j>=i+1;j--)
				{
					datay.map_read[k][0]=datax.map_read[globalTour[j][0]][0];
					datay.map_read[k][1]=datax.map_read[globalTour[j][0]][1];
					k++;
				}
			}
			else
			{
				
				for(int j=i;j<num;j++)
				{
					datay.map_read[k][0]=datax.map_read[globalTour[j][0]][0];
					datay.map_read[k][1]=datax.map_read[globalTour[j][0]][1];
					k++;
				}
				for(int j=0;j<i;j++)
				{
					datay.map_read[k][0]=datax.map_read[globalTour[j][0]][0];
					datay.map_read[k][1]=datax.map_read[globalTour[j][0]][1];
					k++;
				}
			}

			break;
		}
	}
	return datay;
} 

void InitParameter(double value,struct AntColonySystem* acs,int num)
{
	
	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < num; j++)
		{
			acs->info[i][j] = value;   
			acs->info[j][i] = value;
			if (i != j)
			{
				acs->visible[i][j] = 1.0 / dis[i][j];   
				acs->visible[j][i] = acs->visible[i][j];
			}
		}
	}
}
//全局信息素更新
void UpdateGlobalPathRule(int* bestTour, int globalBestLength,struct AntColonySystem* acs,int num)
{
	for (int i = 0; i < num; i++)
	{
		int row = *(bestTour + 2 * i);          
		int col = *(bestTour + 2 * i + 1);
		acs->info[row][col] = (1.0 - rou) * acs->info[row][col] + rou * (1.0 / globalBestLength);   
		acs->info[col][row] =acs-> info[row][col];
	}
}
int Choose(struct ACSAnt* ants,int num)
{
	int nextCity = -1;
	double q = rand() / (double)RAND_MAX;   
	if (q <= qzero)	
	{
		double probability = -1.0;
		for (int i = 0; i < num; i++)
		{
			
			if (1 == ants->allowed[i])
			{
				double prob = Transition(ants->cururentCity, i,ants);  
				if (prob  > probability)
				{
					nextCity = i;
					probability = prob;
				}
			}
		}
	}
	else
	{
				
		double p = rand() / (double)RAND_MAX;	
		unsigned long long sum = 0;
		double probability = 0.0;		
		for (int i = 0; i < num; i++)	
		{
			if (1 == ants->allowed[i])
			{
				double x=Transition(ants->cururentCity, i,ants);
				sum = sum+(unsigned long long)(x*10000000000000);
			}
		}
		for (int j = 0; j < num; j++)
		{
			if (1 == ants->allowed[j] && sum > 0)
			{
				probability += ((double)((unsigned long long)(Transition(ants->cururentCity, j,ants)*10000000000000))) / (double)sum; 
				if (probability >= p || (p > 0.9 && probability > 0.9))
				{
					nextCity = j;
					break;
				}
			}
		}

	}
	if(nextCity==-1)
	for(int i=0;i<num;i++)
	if(ants->allowed[i]==1)
	{
		nextCity=i;break;
	}
	return nextCity;
}
double Transition(int i, int j,struct ACSAnt* ants)
{
	if (i != j)
	{
		return (double)((pow(ants->antColony->info[i][j], alpha) * pow(ants->antColony->visible[i][j], beta))); 
	}
	else
	{
		return 0.0;
	}
}
void MoveToNextCity(int nextCity,struct ACSAnt* ants)
{
	ants->allowed[nextCity] = 0;    //禁忌表
	ants->Tour[ants->currentTourIndex][0] = ants->cururentCity;	//当前路径
	ants->Tour[ants->currentTourIndex][1] = nextCity;
	ants->currentTourIndex++;
	ants->cururentCity = nextCity;
}
struct Tour_ Search(struct ACSAnt* ants,int num)
{
	ants->cururentCity = ants->startCity;
	int toCity;
	ants->currentTourIndex = 0;        
	
	for (int i = 0; i < num; i++)
	{
		ants->allowed[i] = 1;         
	}
	ants->allowed[ants->cururentCity] = 0;	
	int endCity;
	int count = 0;
	do
	{
		count++;
		endCity = ants->cururentCity;
		toCity = Choose(ants,num);	
		if (toCity >= 0)
		{
			MoveToNextCity(toCity,ants);    
			UpdateLocalPathRule(endCity, toCity,ants);  
			ants->cururentCity = toCity;
		}
	} while (toCity >= 0);
	
	MoveToNextCity(ants->startCity,ants);
	UpdateLocalPathRule(endCity, ants->startCity,ants);
	struct Tour_ Tourx;
	for(int i=0;i<num;i++)
	Tourx.Tour[i][0]=ants->Tour[i][0],Tourx.Tour[i][1]=ants->Tour[i][1];
	return Tourx;  
}
void UpdateLocalPathRule(int i, int j , struct ACSAnt* ants)
{
	ants->antColony->info[i][j] = (1.0 - alpha1) * ants->antColony->info[i][j] + alpha1 * (1.0 / (N * Lnn));
	ants->antColony->info[j][i] = ants->antColony->info[i][j];
}
int ChooseNextNode(int currentNode, int visitedNode[],int num)
{
	int nextNode = -1;
	double shortDistance = 0.0;
	for (int i = 0; i < num; i++)
	{
		if (1 == visitedNode[i])
		{
			if (shortDistance == 0.0)
			{
				shortDistance = dis[currentNode][i];
				nextNode = i;
			}
			if (shortDistance < dis[currentNode][i])
			{
				shortDistance = dis[currentNode][i];////!!!!!!!!!!!!!!!!!!!!!!!
				nextNode = i;
			}
		}
	}
	return nextNode;
}

double CalAdjacentDistance(int node,int num)
{
	double sum = 0.0;
	int visitedNode[num];
	for (int j = 0; j < num; j++)
	{
		visitedNode[j] = 1;
	}
	visitedNode[node] = 0;
	int currentNode = node;
	int nextNode;
	do
	{
		nextNode = ChooseNextNode(currentNode, visitedNode, num);
		if (nextNode >= 0)
		{
			sum += dis[currentNode][nextNode];  
			currentNode = nextNode;
			visitedNode[currentNode] = 0;
		}
	} while (nextNode >= 0);           
	sum += dis[currentNode][node];
	return sum;
}
double calculateSumOfDistance(struct Tour_ Tourx,int num)
{

	double sum = 0;
	for(int i = 0; i< num ;i++)
	{
		int row = Tourx.Tour[i][0];
		int col = Tourx.Tour[i][1];
		sum += dis[row][col];
	}	
	return sum;
}