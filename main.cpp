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

class Points{//�s�y�� 
    public:
        short int x;
        short int y;  
};

class Vertex{//�sgraph�����ƾ� 
    public:
        float d;
        Points pi;  
        Points self;
};

bool operator< (const Vertex& vertex1, const Vertex &vertex2)//��X�̵u�Z�����I��� 
{
    return vertex1.d < vertex2.d;
}

bool operator> (const Vertex& vertex1, const Vertex &vertex2)//��X�̵u�Z�����I��� 
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
	short int grid_size[2];//Ū��l�j�p 
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
    short int capacity = atoi(buffer);//Ū��ɮe�q 
    
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
    short int num_net = atoi(buffer);//Ū���X���ݭn�s�����I�� 
    
    cout << "#num net: " << num_net << endl;//print capacity
    
    fstream fout;
    fout.open(argv[2],fstream::out);
    if(!fout.is_open())
    {
        cout << "Error: the output file is not opened!!" << endl;
	exit(1);
    }
    
    //point pair
    Points point_pair[num_net][2]; //�O���I�� 
    vector <pair<short int,short int> >dist_table;//�����I���idx

	for(int n = 0; n < num_net; n++){//�̦���J�O���I��
		fin >> buffer;
		fin >> buffer;
		point_pair[n][0].x = atoi(buffer);
		fin >> buffer;
		point_pair[n][0].y = atoi(buffer);
		fin >> buffer;
		point_pair[n][1].x = atoi(buffer);
		fin >> buffer;
		point_pair[n][1].y = atoi(buffer);
		
		dist_table.push_back(make_pair (abs(point_pair[n][0].x - point_pair[n][1].x) + abs(point_pair[n][0].y - point_pair[n][1].y),n));//�s�W��vector��
	}
	
	sort (dist_table.begin(),dist_table.end());//�Ƨ��I��Z��
	
	/*for(int i = 0; i < num_net; i++){
		cout << dist_table[i].second << " " << point_pair[dist_table[i].second][0].x << " " << point_pair[dist_table[i].second][0].y << " " << point_pair[dist_table[i].second][1].x << " " << point_pair[dist_table[i].second][1].y <<endl;
	}*/
	

	int idx;
    Vertex G[grid_size[0]][grid_size[1]];//�гygraph
    short int check[grid_size[0]][grid_size[1]];//�����O�_�j�M�L 
    short int demand_num[grid_size[0]*2-1][grid_size[1]];//������ɸ�V�ƶq 
    float demand[grid_size[0]*2-1][grid_size[1]];//��������v�� 
    priority_queue<Vertex, vector<Vertex>,greater<vector<Vertex>::value_type> > pqVertex;//�sVertex��priority_queue �z�Lpriority_queue���X�̤p�Ȫ�Vertex
    
    for(int i = 0; i < grid_size[0]*2; ++i){//��l����ɸ�V�ƶq
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
	
	
	for(int k = 0; k < num_net; k++){//�}�lDijkstra
		idx = dist_table[k].second;//���I��idx�����X�� 
		
		/*�Ĥ@�������v���W�Lcapacity���u�ޱ�*/ 
		
		for(int i = 0; i < grid_size[0]; ++i){//��l��graph
			for(int j = 0; j < grid_size[1]; ++j){
				G[i][j].d = FLT_MAX;
				G[i][j].pi.x = -1;
				G[i][j].pi.y = -1;
				G[i][j].self.x = i;
				G[i][j].self.y = j;
				check[i][j] = 0;
				if (i == point_pair[idx][1].x & j == point_pair[idx][1].y){
					G[i][j].d = 0;//�_�l�I��0 
				}
				pqVertex.push(G[i][j]);//�[�Jpriority_queue
			}
		}
		
		for(int i = 0; i < grid_size[0]*2-1; ++i){//�v����� 
			for(int j = 0; j < grid_size[1]-1; ++j){
				demand[i][j] = pow(10.0, float(demand_num[i][j])/capacity)+2.0*float(demand_num[i][j])/capacity;
			}
		}
		
		while(!pqVertex.empty()){//�T�{priority_queue�O�_�٦��I 
			if(!check[pqVertex.top().self.x][pqVertex.top().self.y]){//�T�{���I�O�_���� 
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
			    	
		    	if (pqVertex.top().d + demand[pqVertex.top().self.x * 2 - 1][pqVertex.top().self.y] < G[pqVertex.top().self.x - 1][pqVertex.top().self.y].d && pqVertex.top().self.x - 1 >= 0 && demand_num[pqVertex.top().self.x * 2 - 1][pqVertex.top().self.y] < capacity){ //��
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
				if (pqVertex.top().d + demand[pqVertex.top().self.x * 2 + 1][pqVertex.top().self.y] < G[pqVertex.top().self.x + 1][pqVertex.top().self.y].d && pqVertex.top().self.x + 1 < grid_size[0] && demand_num[pqVertex.top().self.x * 2 + 1][pqVertex.top().self.y] < capacity){ //�k 
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
				if (pqVertex.top().d + demand[pqVertex.top().self.x * 2][pqVertex.top().self.y + 1] < G[pqVertex.top().self.x][pqVertex.top().self.y + 1].d && pqVertex.top().self.y + 1 < grid_size[1] && demand_num[pqVertex.top().self.x * 2][pqVertex.top().self.y + 1] < capacity){ //�W
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
				if (pqVertex.top().d + demand[pqVertex.top().self.x * 2][pqVertex.top().self.y] < G[pqVertex.top().self.x][pqVertex.top().self.y - 1].d && pqVertex.top().self.y - 1 >= 0 && demand_num[pqVertex.top().self.x * 2][pqVertex.top().self.y] < capacity){ //�U 
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
			check[pqVertex.top().self.x][pqVertex.top().self.y]=1;//���� 
	      	pqVertex.pop();//�qpriority_queue�R�� 
	      	//cout << "-------------------------------------------" << endl;
		}	
		
		if(G[point_pair[idx][0].x][point_pair[idx][0].y].pi.x == -1 || G[point_pair[idx][0].x][point_pair[idx][0].y].pi.y == -1){//�p�G�]���u�Q�ޱ��S�����|�N��u�[�^�ӦҼ{��overflow���ɭ� 
			for(int i = 0; i < grid_size[0]; ++i){//��l��graph
				for(int j = 0; j < grid_size[1]; ++j){
					G[i][j].d = FLT_MAX;
					G[i][j].pi.x = -1;
					G[i][j].pi.y = -1;
					G[i][j].self.x = i;
					G[i][j].self.y = j;
					check[i][j] = 0;
					if (i == point_pair[idx][1].x & j == point_pair[idx][1].y){
						G[i][j].d = 0;//�_�l�I��0 
					}
					pqVertex.push(G[i][j]);//�[�Jpriority_queue
				}
			}
			
			for(int i = 0; i < grid_size[0]*2-1; ++i){//�v����� 
				for(int j = 0; j < grid_size[1]-1; ++j){
					demand[i][j] = pow(10.0, float(demand_num[i][j])/capacity)+2.0*float(demand_num[i][j])/capacity;
				}
			}
			
			while(!pqVertex.empty()){//�T�{priority_queue�O�_�٦��I 
				if(!check[pqVertex.top().self.x][pqVertex.top().self.y]){//�T�{���I�O�_���� 
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
				    	
			    	if (pqVertex.top().d + demand[pqVertex.top().self.x * 2 - 1][pqVertex.top().self.y] < G[pqVertex.top().self.x - 1][pqVertex.top().self.y].d && pqVertex.top().self.x - 1 >= 0){ //��
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
					if (pqVertex.top().d + demand[pqVertex.top().self.x * 2 + 1][pqVertex.top().self.y] < G[pqVertex.top().self.x + 1][pqVertex.top().self.y].d && pqVertex.top().self.x + 1 < grid_size[0]){ //�k 
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
					if (pqVertex.top().d + demand[pqVertex.top().self.x * 2][pqVertex.top().self.y + 1] < G[pqVertex.top().self.x][pqVertex.top().self.y + 1].d && pqVertex.top().self.y + 1 < grid_size[1]){ //�W
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
					if (pqVertex.top().d + demand[pqVertex.top().self.x * 2][pqVertex.top().self.y] < G[pqVertex.top().self.x][pqVertex.top().self.y - 1].d && pqVertex.top().self.y - 1 >= 0){ //�U 
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
				check[pqVertex.top().self.x][pqVertex.top().self.y]=1;//���� 
		      	pqVertex.pop();//�qpriority_queue�R�� 
		      	//cout << "-------------------------------------------" << endl;
			}	
		} 
		
		
		int xx_last = point_pair[idx][0].x;//�_�I 
		int yy_last = point_pair[idx][0].y;//�_�I 
		int xx;//���I
		int yy;//���I
		int count = 0;//�p��u��
		
		while(xx_last!=point_pair[idx][1].x | yy_last!=point_pair[idx][1].y){//�p��u��
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
		
		while(xx_last!=point_pair[idx][1].x | yy_last!=point_pair[idx][1].y){//��X���|
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
