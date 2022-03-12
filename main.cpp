#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <vector>
#include <climits> 
#include <cfloat> 
#include <math.h>
#include <queue>
#include <algorithm>

using namespace std;

class Points{//存座標 
    public:
        short int x;
        short int y;  
};

class Vertex{//存graph中的數據 
    public:
        float d;
        Points pi;  
        Points self;
};

bool operator< (const Vertex& vertex1, const Vertex &vertex2)//找出最短距離的點對用 
{
    return vertex1.d < vertex2.d;
}

bool operator> (const Vertex& vertex1, const Vertex &vertex2)//找出最短距離的點對用 
{
    return vertex1.d > vertex2.d;
}


int main(int argc, char **argv){
	if(argc != 3)
    {
        cout << "Usage: ./[exe] [input file] [output file]" << endl;
        exit(1);
    }

    // open the input file
    fstream fin;
    fin.open(argv[1], fstream::in);
    if(!fin.is_open())
    {
        cout << "Error: the input file is not opened!!" << endl;
        exit(1);
    }
	
	//initial
    char buffer[100];
    fin >> buffer;
    
    if(strcmp(buffer, "grid")){
    	cout << "Error: the text in the input file is incorrect!!" << endl;
        exit(1);
	}
	short int grid_size[2];//讀格子大小 
	fin >> buffer;
    grid_size[0] = atoi(buffer);
    fin >> buffer;
    grid_size[1] = atoi(buffer);
    
    cout << "#grid: " << grid_size[0] << " x " << grid_size[1] << endl;//print grid size
    
    fin >> buffer;
    
    if(strcmp(buffer, "capacity")){
    	cout << "Error: the text in the input file is incorrect!!" << endl;
        exit(1);
	}
	
	fin >> buffer;
    short int capacity = atoi(buffer);//讀邊界容量 
    
    cout << "#capacity: " << capacity << endl;//print capacity
    
    fin >> buffer;
    
    if(strcmp(buffer, "num")){
    	cout << "Error: the text in the input file is incorrect!!" << endl;
        exit(1);
	}
	
	fin >> buffer;
    
    if(strcmp(buffer, "net")){
    	cout << "Error: the text in the input file is incorrect!!" << endl;
        exit(1);
	}
	
	fin >> buffer;
    short int num_net = atoi(buffer);//讀有幾條需要連接的點對 
    
    cout << "#num net: " << num_net << endl;//print capacity
    
    fstream fout;
    fout.open(argv[2],fstream::out);
    if(!fout.is_open())
    {
        cout << "Error: the output file is not opened!!" << endl;
	exit(1);
    }
    
    //point pair
    Points point_pair[num_net][2]; //記錄點對 
    vector <pair<short int,short int> >dist_table;//紀錄點對及idx

	for(int n = 0; n < num_net; n++){//依次輸入記錄點對
		fin >> buffer;
		fin >> buffer;
		point_pair[n][0].x = atoi(buffer);
		fin >> buffer;
		point_pair[n][0].y = atoi(buffer);
		fin >> buffer;
		point_pair[n][1].x = atoi(buffer);
		fin >> buffer;
		point_pair[n][1].y = atoi(buffer);
		
		dist_table.push_back(make_pair (abs(point_pair[n][0].x - point_pair[n][1].x) + abs(point_pair[n][0].y - point_pair[n][1].y),n));//新增到vector裡
	}
	
	sort (dist_table.begin(),dist_table.end());//排序點對距離
	
	/*for(int i = 0; i < num_net; i++){
		cout << dist_table[i].second << " " << point_pair[dist_table[i].second][0].x << " " << point_pair[dist_table[i].second][0].y << " " << point_pair[dist_table[i].second][1].x << " " << point_pair[dist_table[i].second][1].y <<endl;
	}*/
	

	int idx;
    Vertex G[grid_size[0]][grid_size[1]];//創造graph
    short int check[grid_size[0]][grid_size[1]];//紀錄是否搜尋過 
    short int demand_num[grid_size[0]*2-1][grid_size[1]];//紀錄邊界跨越數量 
    float demand[grid_size[0]*2-1][grid_size[1]];//紀錄邊界權重 
    priority_queue<Vertex, vector<Vertex>,greater<vector<Vertex>::value_type> > pqVertex;//存Vertex到priority_queue 透過priority_queue取出最小值的Vertex
    
    for(int i = 0; i < grid_size[0]*2; ++i){//初始化邊界跨越數量
		for(int j = 0; j < grid_size[1]; ++j){
			demand_num[i][j] = 0;
		}
	}
	
	/*for(int j = grid_size[1]-1; j >= 0; --j){
		for(int i = 0; i < grid_size[0]*2-1; ++i){
			cout<< demand_num[i][j] << " ";
		}
		cout << endl;
	}*/
	
	
	for(int k = 0; k < num_net; k++){//開始Dijkstra
		idx = dist_table[k].second;//找點的idx之後輸出用 
		
		/*第一次先把權重超過capacity的線拔掉*/ 
		
		for(int i = 0; i < grid_size[0]; ++i){//初始化graph
			for(int j = 0; j < grid_size[1]; ++j){
				G[i][j].d = FLT_MAX;
				G[i][j].pi.x = -1;
				G[i][j].pi.y = -1;
				G[i][j].self.x = i;
				G[i][j].self.y = j;
				check[i][j] = 0;
				if (i == point_pair[idx][1].x & j == point_pair[idx][1].y){
					G[i][j].d = 0;//起始點為0 
				}
				pqVertex.push(G[i][j]);//加入priority_queue
			}
		}
		
		for(int i = 0; i < grid_size[0]*2-1; ++i){//權重更改 
			for(int j = 0; j < grid_size[1]-1; ++j){
				demand[i][j] = pow(10.0, float(demand_num[i][j])/capacity)+2.0*float(demand_num[i][j])/capacity;
			}
		}
		
		while(!pqVertex.empty()){//確認priority_queue是否還有點 
			if(!check[pqVertex.top().self.x][pqVertex.top().self.y]){//確認此點是否完成 
				/*cout << pqVertex.top().d << endl;
		    	cout << pqVertex.top().self.x << " " << pqVertex.top().self.y << endl;
		    	
		    	cout << "xxxxxxxxxxxxxxxxxxxxxxxx" << endl;
		    	for(int j = grid_size[1]-1; j >= 0; --j){
					for(int i = 0; i < grid_size[0]; ++i){
						cout<< check[i][j] << " ";
					}
					cout << endl;
				}
				cout << "xxxxxxxxxxxxxxxxxxxxxxxx" << endl;*/
			    	
		    	if (pqVertex.top().d + demand[pqVertex.top().self.x * 2 - 1][pqVertex.top().self.y] < G[pqVertex.top().self.x - 1][pqVertex.top().self.y].d && pqVertex.top().self.x - 1 >= 0 && demand_num[pqVertex.top().self.x * 2 - 1][pqVertex.top().self.y] < capacity){ //左
					G[pqVertex.top().self.x - 1][pqVertex.top().self.y].d = pqVertex.top().d + demand[pqVertex.top().self.x * 2 - 1][pqVertex.top().self.y];
					G[pqVertex.top().self.x - 1][pqVertex.top().self.y].pi = pqVertex.top().self;
					pqVertex.push(G[pqVertex.top().self.x - 1][pqVertex.top().self.y]);
					/*cout << pqVertex.top().self.x - 1<< " " << pqVertex.top().self.y << endl;
					cout << pqVertex.top().self.x * 2 - 1 << " " << pqVertex.top().self.y * 2 << endl;
					cout << "demand:" << demand[pqVertex.top().self.x * 2 - 1][pqVertex.top().self.y]<<endl;
					cout << "d:" << G[pqVertex.top().self.x - 1][pqVertex.top().self.y].d<<endl;
					cout << "pi:" << G[pqVertex.top().self.x - 1][pqVertex.top().self.y].pi.x << " " << G[pqVertex.top().self.x - 1][pqVertex.top().self.y].pi.y <<endl;
					cout << "************" << endl;*/
				}
				if (pqVertex.top().d + demand[pqVertex.top().self.x * 2 + 1][pqVertex.top().self.y] < G[pqVertex.top().self.x + 1][pqVertex.top().self.y].d && pqVertex.top().self.x + 1 < grid_size[0] && demand_num[pqVertex.top().self.x * 2 + 1][pqVertex.top().self.y] < capacity){ //右 
					G[pqVertex.top().self.x + 1][pqVertex.top().self.y].d = pqVertex.top().d + demand[pqVertex.top().self.x * 2 + 1][pqVertex.top().self.y];
					G[pqVertex.top().self.x + 1][pqVertex.top().self.y].pi = pqVertex.top().self;
					pqVertex.push(G[pqVertex.top().self.x + 1][pqVertex.top().self.y]);
					/*cout << pqVertex.top().self.x + 1<< " " << pqVertex.top().self.y << endl;
					cout << pqVertex.top().self.x * 2 + 1 << " " << pqVertex.top().self.y * 2 << endl;
					cout << "demand:" << demand[pqVertex.top().self.x * 2 + 1][pqVertex.top().self.y]<<endl;
					cout << "d:" << G[pqVertex.top().self.x + 1][pqVertex.top().self.y].d<<endl;
					cout << "pi:" << G[pqVertex.top().self.x + 1][pqVertex.top().self.y].pi.x << " " << G[pqVertex.top().self.x + 1][pqVertex.top().self.y].pi.y <<endl;
					cout << "************" << endl;*/
				}
				if (pqVertex.top().d + demand[pqVertex.top().self.x * 2][pqVertex.top().self.y + 1] < G[pqVertex.top().self.x][pqVertex.top().self.y + 1].d && pqVertex.top().self.y + 1 < grid_size[1] && demand_num[pqVertex.top().self.x * 2][pqVertex.top().self.y + 1] < capacity){ //上
					G[pqVertex.top().self.x][pqVertex.top().self.y + 1].d = pqVertex.top().d + demand[pqVertex.top().self.x * 2][pqVertex.top().self.y + 1];
					G[pqVertex.top().self.x][pqVertex.top().self.y + 1].pi = pqVertex.top().self;
					pqVertex.push(G[pqVertex.top().self.x][pqVertex.top().self.y + 1]);
					/*cout << pqVertex.top().self.x << " " << pqVertex.top().self.y + 1 << endl;
					cout << pqVertex.top().self.x * 2 << " " << pqVertex.top().self.y * 2 + 1 << endl;
					cout << "demand:" << demand[pqVertex.top().self.x * 2][pqVertex.top().self.y + 1]<<endl;
					cout << "d:" << G[pqVertex.top().self.x][pqVertex.top().self.y + 1].d<<endl;
					cout << "pi:" << G[pqVertex.top().self.x][pqVertex.top().self.y + 1].pi.x << " " << G[pqVertex.top().self.x][pqVertex.top().self.y + 1].pi.y <<endl;
					cout << "************" << endl;*/
				}
				if (pqVertex.top().d + demand[pqVertex.top().self.x * 2][pqVertex.top().self.y] < G[pqVertex.top().self.x][pqVertex.top().self.y - 1].d && pqVertex.top().self.y - 1 >= 0 && demand_num[pqVertex.top().self.x * 2][pqVertex.top().self.y] < capacity){ //下 
					G[pqVertex.top().self.x][pqVertex.top().self.y - 1].d = pqVertex.top().d + demand[pqVertex.top().self.x * 2][pqVertex.top().self.y];
					G[pqVertex.top().self.x][pqVertex.top().self.y - 1].pi = pqVertex.top().self;
					pqVertex.push(G[pqVertex.top().self.x][pqVertex.top().self.y - 1]);
					/*cout << pqVertex.top().self.x << " " << pqVertex.top().self.y - 1 << endl;
					cout << pqVertex.top().self.x * 2 << " " << pqVertex.top().self.y * 2 - 1 << endl;
					cout << "demand:" << demand[pqVertex.top().self.x * 2][pqVertex.top().self.y]<<endl;
					cout << "d:" << G[pqVertex.top().self.x][pqVertex.top().self.y - 1].d<<endl;
					cout << "pi:" << G[pqVertex.top().self.x][pqVertex.top().self.y - 1].pi.x << " " << G[pqVertex.top().self.x][pqVertex.top().self.y - 1].pi.y <<endl;
					cout << "************" << endl;*/
				}
			}
			check[pqVertex.top().self.x][pqVertex.top().self.y]=1;//完成 
	      	pqVertex.pop();//從priority_queue刪除 
	      	//cout << "-------------------------------------------" << endl;
		}	
		
		if(G[point_pair[idx][0].x][point_pair[idx][0].y].pi.x == -1 || G[point_pair[idx][0].x][point_pair[idx][0].y].pi.y == -1){//如果因為線被拔掉沒有路徑就把線加回來考慮有overflow的時候 
			for(int i = 0; i < grid_size[0]; ++i){//初始化graph
				for(int j = 0; j < grid_size[1]; ++j){
					G[i][j].d = FLT_MAX;
					G[i][j].pi.x = -1;
					G[i][j].pi.y = -1;
					G[i][j].self.x = i;
					G[i][j].self.y = j;
					check[i][j] = 0;
					if (i == point_pair[idx][1].x & j == point_pair[idx][1].y){
						G[i][j].d = 0;//起始點為0 
					}
					pqVertex.push(G[i][j]);//加入priority_queue
				}
			}
			
			for(int i = 0; i < grid_size[0]*2-1; ++i){//權重更改 
				for(int j = 0; j < grid_size[1]-1; ++j){
					demand[i][j] = pow(10.0, float(demand_num[i][j])/capacity)+2.0*float(demand_num[i][j])/capacity;
				}
			}
			
			while(!pqVertex.empty()){//確認priority_queue是否還有點 
				if(!check[pqVertex.top().self.x][pqVertex.top().self.y]){//確認此點是否完成 
					/*cout << pqVertex.top().d << endl;
			    	cout << pqVertex.top().self.x << " " << pqVertex.top().self.y << endl;
			    	
			    	cout << "xxxxxxxxxxxxxxxxxxxxxxxx" << endl;
			    	for(int j = grid_size[1]-1; j >= 0; --j){
						for(int i = 0; i < grid_size[0]; ++i){
							cout<< check[i][j] << " ";
						}
						cout << endl;
					}
					cout << "xxxxxxxxxxxxxxxxxxxxxxxx" << endl;*/
				    	
			    	if (pqVertex.top().d + demand[pqVertex.top().self.x * 2 - 1][pqVertex.top().self.y] < G[pqVertex.top().self.x - 1][pqVertex.top().self.y].d && pqVertex.top().self.x - 1 >= 0){ //左
						G[pqVertex.top().self.x - 1][pqVertex.top().self.y].d = pqVertex.top().d + demand[pqVertex.top().self.x * 2 - 1][pqVertex.top().self.y];
						G[pqVertex.top().self.x - 1][pqVertex.top().self.y].pi = pqVertex.top().self;
						pqVertex.push(G[pqVertex.top().self.x - 1][pqVertex.top().self.y]);
						/*cout << pqVertex.top().self.x - 1<< " " << pqVertex.top().self.y << endl;
						cout << pqVertex.top().self.x * 2 - 1 << " " << pqVertex.top().self.y * 2 << endl;
						cout << "demand:" << demand[pqVertex.top().self.x * 2 - 1][pqVertex.top().self.y]<<endl;
						cout << "d:" << G[pqVertex.top().self.x - 1][pqVertex.top().self.y].d<<endl;
						cout << "pi:" << G[pqVertex.top().self.x - 1][pqVertex.top().self.y].pi.x << " " << G[pqVertex.top().self.x - 1][pqVertex.top().self.y].pi.y <<endl;
						cout << "************" << endl;*/
					}
					if (pqVertex.top().d + demand[pqVertex.top().self.x * 2 + 1][pqVertex.top().self.y] < G[pqVertex.top().self.x + 1][pqVertex.top().self.y].d && pqVertex.top().self.x + 1 < grid_size[0]){ //右 
						G[pqVertex.top().self.x + 1][pqVertex.top().self.y].d = pqVertex.top().d + demand[pqVertex.top().self.x * 2 + 1][pqVertex.top().self.y];
						G[pqVertex.top().self.x + 1][pqVertex.top().self.y].pi = pqVertex.top().self;
						pqVertex.push(G[pqVertex.top().self.x + 1][pqVertex.top().self.y]);
						/*cout << pqVertex.top().self.x + 1<< " " << pqVertex.top().self.y << endl;
						cout << pqVertex.top().self.x * 2 + 1 << " " << pqVertex.top().self.y * 2 << endl;
						cout << "demand:" << demand[pqVertex.top().self.x * 2 + 1][pqVertex.top().self.y]<<endl;
						cout << "d:" << G[pqVertex.top().self.x + 1][pqVertex.top().self.y].d<<endl;
						cout << "pi:" << G[pqVertex.top().self.x + 1][pqVertex.top().self.y].pi.x << " " << G[pqVertex.top().self.x + 1][pqVertex.top().self.y].pi.y <<endl;
						cout << "************" << endl;*/
					}
					if (pqVertex.top().d + demand[pqVertex.top().self.x * 2][pqVertex.top().self.y + 1] < G[pqVertex.top().self.x][pqVertex.top().self.y + 1].d && pqVertex.top().self.y + 1 < grid_size[1]){ //上
						G[pqVertex.top().self.x][pqVertex.top().self.y + 1].d = pqVertex.top().d + demand[pqVertex.top().self.x * 2][pqVertex.top().self.y + 1];
						G[pqVertex.top().self.x][pqVertex.top().self.y + 1].pi = pqVertex.top().self;
						pqVertex.push(G[pqVertex.top().self.x][pqVertex.top().self.y + 1]);
						/*cout << pqVertex.top().self.x << " " << pqVertex.top().self.y + 1 << endl;
						cout << pqVertex.top().self.x * 2 << " " << pqVertex.top().self.y * 2 + 1 << endl;
						cout << "demand:" << demand[pqVertex.top().self.x * 2][pqVertex.top().self.y + 1]<<endl;
						cout << "d:" << G[pqVertex.top().self.x][pqVertex.top().self.y + 1].d<<endl;
						cout << "pi:" << G[pqVertex.top().self.x][pqVertex.top().self.y + 1].pi.x << " " << G[pqVertex.top().self.x][pqVertex.top().self.y + 1].pi.y <<endl;
						cout << "************" << endl;*/
					}
					if (pqVertex.top().d + demand[pqVertex.top().self.x * 2][pqVertex.top().self.y] < G[pqVertex.top().self.x][pqVertex.top().self.y - 1].d && pqVertex.top().self.y - 1 >= 0){ //下 
						G[pqVertex.top().self.x][pqVertex.top().self.y - 1].d = pqVertex.top().d + demand[pqVertex.top().self.x * 2][pqVertex.top().self.y];
						G[pqVertex.top().self.x][pqVertex.top().self.y - 1].pi = pqVertex.top().self;
						pqVertex.push(G[pqVertex.top().self.x][pqVertex.top().self.y - 1]);
						/*cout << pqVertex.top().self.x << " " << pqVertex.top().self.y - 1 << endl;
						cout << pqVertex.top().self.x * 2 << " " << pqVertex.top().self.y * 2 - 1 << endl;
						cout << "demand:" << demand[pqVertex.top().self.x * 2][pqVertex.top().self.y]<<endl;
						cout << "d:" << G[pqVertex.top().self.x][pqVertex.top().self.y - 1].d<<endl;
						cout << "pi:" << G[pqVertex.top().self.x][pqVertex.top().self.y - 1].pi.x << " " << G[pqVertex.top().self.x][pqVertex.top().self.y - 1].pi.y <<endl;
						cout << "************" << endl;*/
					}
				}
				check[pqVertex.top().self.x][pqVertex.top().self.y]=1;//完成 
		      	pqVertex.pop();//從priority_queue刪除 
		      	//cout << "-------------------------------------------" << endl;
			}	
		} 
		
		
		int xx_last = point_pair[idx][0].x;//起點 
		int yy_last = point_pair[idx][0].y;//起點 
		int xx;//終點
		int yy;//終點
		int count = 0;//計算線長
		
		while(xx_last!=point_pair[idx][1].x | yy_last!=point_pair[idx][1].y){//計算線長
			count++;
			xx = G[xx_last][yy_last].pi.x;
			yy = G[xx_last][yy_last].pi.y;
			
			if(xx == xx_last){
				if (yy > yy_last){
					demand_num[xx*2][yy]++;
				}
				else{
					demand_num[xx*2][yy+1]++;
				}
			}
			else{
				if (xx > xx_last){
					demand_num[xx*2 - 1][yy]++;
				}
				else{
					demand_num[xx*2 + 1][yy]++;
				}
			}
			
			xx_last = xx;
			yy_last = yy;
		}
		
		xx_last = point_pair[idx][0].x;
		yy_last = point_pair[idx][0].y;
		xx=0;
		yy=0;
		
		fout << idx << " " <<count<<endl;
		
		while(xx_last!=point_pair[idx][1].x | yy_last!=point_pair[idx][1].y){//輸出路徑
			fout << xx_last << " " << yy_last <<" ";
			xx = G[xx_last][yy_last].pi.x;
			yy = G[xx_last][yy_last].pi.y;
			fout << xx << " " << yy <<endl;
			
			xx_last = xx;
			yy_last = yy;
		}
	}
	

	cout << "Finish"<<endl;
    // output something to the output file
    return 0;
}
