# Global-Routing
NTUST Algorithm Design and Application

#### Problem Description
This programming assignment asks you to write a global router that can route 2-pin nets (connection between two points). The problem description below is a simplified routing problem. Given the problem size (the number of horizontal and vertical tiles), capacity, and a netlist, the global router routes all nets in the routing region. The main objective is to minimize the total overflows. Here the overflow on a tile boundary is calculated as the amount of demand that excesses the capacity, i.e., overflow = max(0, demand-capacity).

#### Input
The file format for the global routing is illustrated, with comments in italics (these will not be in actual input files). The 1st line gives the problem size in terms of the number of horizontal and vertical tiles. Each global routing tile (tile in short) has a capacity on its four boundaries to measure the available space, which is the maximum number of routing paths passing through boundaries. The capacity value is given by the 2nd line. The 3rd line gives the number of nets and following indicate each net, including starting position and terminal position. The input file format is as follows:  

>> grid # # //number of horizontal tiles, number of vertical tiles  
>> capacity # //capacity of tile  
>> num net # //number of nets  
>> net_id xs ys xt yt  
>> ...  
>> //repeat for the appropriate number of nets  
