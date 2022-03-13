# Global-Routing
NTUST Algorithm Design and Application

#### Problem Description
This programming assignment asks you to write a global router that can route 2-pin nets (connection between two points). The problem description below is a simplified routing problem. Given the problem size (the number of horizontal and vertical tiles), capacity, and a netlist, the global router routes all nets in the routing region. The main objective is to minimize the total overflows. Here the overflow on a tile boundary is calculated as the amount of demand that excesses the capacity, i.e., overflow = max(0, demand-capacity).

#### Input
The file format for the global routing is illustrated, with comments in italics (these will not be in actual input files). The 1st line gives the problem size in terms of the number of horizontal and vertical tiles. Each global routing tile (tile in short) has a capacity on its four boundaries to measure the available space, which is the maximum number of routing paths passing through boundaries. The capacity value is given by the 2nd line. The 3rd line gives the number of nets and following indicate each net, including starting position and terminal position. The input file format is as follows:  

> grid # # //number of horizontal tiles, number of vertical tiles  
> capacity # //capacity of tile  
> num net # //number of nets  
> net_id xs ys xt yt  
> ...  
> //repeat for the appropriate number of nets  

#### Output
All the routes in the output could only be horizontal lines and vertical lines. For example (18, 61)-(19, 62) is not acceptable, because it is diagonal. Remember that each route could be different either in the x or y location only, and the difference must be 1. The output file format is as follows:

> [net_id] [# of routes, k]  
> [x11] [y11] [x12] [y12]  
> [x21] [y21] [x22] [y22]  
> ...  
> [x(k-1)1] [y(k-1)1] [xk2] [yk2]  
> //repeat for the appropriate number of nets  

Note that for a certain net, x11, y11, xk2 and yk2 must be the same as xs, ys, xt and yt in the input file respectively. Also, for any i, xi2 and yi2 must be the same as x(i+1)1 and y(i+1)1 respectively.

#### Sample case
![](https://github.com/naiyu0609/Global-Routing/blob/main/1.PNG)  

| Sample Input |  
grid 4 4  
capacity 2  
num net 3  
0 2 3 0 0  
1 0 2 3 0  
2 1 0 1 3  

| Sample Output |  
0 5  
2 3 1 3  
1 3 1 2  
1 2 1 1  
1 1 1 0  
1 0 0 0  
2 3  
1 0 1 1  
1 1 1 2  
1 2 1 3  
1 5  
0 2 1 2  
1 2 1 1  
1 1 2 1  
2 1 3 1  
3 1 3 0  

The total overflow is 1, which is caused by the boundary between tiles (1,1) and (1,2).
(The total wirelength is 13.)
