#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

int **make2Darray(int row, int col){//for graph
    int **temp;
    temp = malloc(sizeof(int*) * row);

    for(int count=0; count<row; count++)
        temp[count] = malloc(sizeof(int) * col);

    for(int count1=0; count1<row; count1++)
        for(int count2=0; count2<col; count2++)
            temp[count1][count2] = 0;

    return temp;
}

////print graph
//void print2Darray(int **matrix, int row, int col){//for graph
//    for(int count1=0; count1<row; count1++){
//        for(int count2=0; count2<col; count2++)
//            printf("%d ", matrix[count1][count2]);
//        printf("\n");
//    }
//}

// A utility function to find the vertex with minimum distance value, from the set of vertices not yet included in shortest path tree
int minDistance(int dist[], bool sptSet[], int V){//for func dijkstra
    // Initialize min value
    int min = INT_MAX, min_index = -1;
    for (int v = 0; v < V; v++)
        if (sptSet[v] == false && dist[v] < min){
            min = dist[v];
            min_index = v;
        }
    return min_index;
}
// 顯示路徑(正向找 反向印)(dijkstra裡面用到)
void printPath(int *parent, int j){
    // 當找到source回傳
    if (parent[j] == -1){
        printf("%d ", j);
        return;
    }
    // 遞迴，找到source才列印
    printPath(parent, parent[j]);
    printf("%d ", j);
}

// 跑 dijkstra 存取 shortest path tree 並且回傳當前成功的路徑數量
int dijkstra(int **graph, int src, int dst, int V, int *node, int path_num, int flag, int req_ID){
//    if(flag == 1){
//        printf("node_num : ");
//        for(int count=0; count<V; count++)
//            printf("%d ", node[count]);
//        printf("\n");
//    }
    // The output array.  dist[i] will hold the shortest distance from src to i
    int dist[V];
    // sptSet[i] will be true if vertex i is included in shortest path tree or shortest distance from src to i is finalized
    bool sptSet[V];
    // declare array to store parents for each node and set the parent of source as -1
    int parent[V];
    // Initialize all distances as INFINITE and stpSet[] as false and parentas -2
    for (int count = 0; count < V; count++){
        dist[count] = INT_MAX;
        sptSet[count] = false;
        parent[count] = -2;
    }
    // default for source
    dist[src] = 0;
    parent[src] = -1;
    // Find shortest path for all vertices
    for (int count = 0; count < V; count++){
        // Pick the minimum distance vertex from the set of vertices not yet processed. u is always equal to src in the first iteration.
        int u = minDistance(dist, sptSet, V);
//        if(flag == 1)
//            printf("%d\n", u);
        // If no node in rest sptSet can choose to be next point, stop making  spt
        if(u == -1)
            break;
        // Mark the picked vertex as processed
        sptSet[u] = true;
        // Update dist value of the adjacent vertices of the picked vertex.
        for (int v = 0; v < V; v++){
            // Update dist[v] only if is not in sptSet, there is an edge from u to v, and total weight of path from src to  v through u is smaller than current value of dist[v]
            if(!sptSet[v] && dist[u]+1 < dist[v] && graph[u][v] > 0){
                if(u == src && node[u] > 0 && node[v] > 1){
                    dist[v] = dist[u] + 1, parent[v] = u;
//                    if(flag ==1)
//                        printf("src : %d %d\n", v, dist[v]);
                }
                else if(v == dst && node[u] > 1 && node[v] > 0){
                    dist[v] = dist[u] + 1, parent[v] = u;
//                    if(flag ==1)
//                        printf("dst : %d %d\n", v, dist[v]);
                }
                else if(node[u] > 1 && node[v] > 1){
                    dist[v] = dist[u] + 1, parent[v] = u;
//                    if(flag ==1)
//                        printf("path : %d %d\n", v, dist[v]);
                }
            }
        }
    }
    //check the availability of memories for nodes and links of the path
    int abt_link=1, abt_node=1;
    int itr=dst;
    while(itr != -1){
        //check memories capacity
        if(abt_node == 1){
            if(itr == dst || itr == src)
                if(node[itr] < 1){
//                    if(flag == 1)
//                        printf("src/dst : %d no capacity(%d)\n", itr, node[itr]);
                    abt_node = 0;
                }
            else
                if(node[itr] < 2){
//                    if(flag == 1)
//                        printf("path : %d no capacity(%d)\n", itr, node[itr]);
                    abt_node = 0;
                }
        }
        //check if linkage exist
        if(abt_link == 1){
            if(parent[itr] == -2){
//                if(flag == 1)
//                    printf("parent[%d] : %d no linkage(%d)\n", itr, parent[itr], parent[itr]);
                abt_link = 0;
            }
        }
        if(abt_node == 0 || abt_link == 0){
//            if(flag == 1)
//                printf("this path not available\n");
            break;
        }
        itr=parent[itr];
    }
    //if path is legal, then execute changes to matrix/graph
    if(abt_node == 1 && abt_link == 1){
        path_num++;
        if(flag == 1){
            printf("%d ", req_ID);
            printPath(parent, dst);
            printf("\n");
        }
        int count = dst;
        while(count != src){
            node[count]--;
            node[parent[count]]--;
            graph[count][parent[count]]--;
            graph[parent[count]][count]--;
            count = parent[count];
        }
    }
//    if(flag == 1){
//        printf("node_num : ");
//        for(int count=0; count<V; count++)
//            printf("%d ", node[count]);
//        printf("\n");
//    }
    //let main know the ID value&situation
    return path_num;
}

int main(){
    //read input data
    int node_num, link_num, req_num;
    scanf("%d %d %d", &node_num, &link_num, &req_num);
    //construct array of quantum memories for each node
    int node[node_num], nodeID, node_cpy[node_num];
    for(int count=0; count<node_num; count++){
        scanf("%d %d", &nodeID, &node[count]);
        node_cpy[count] = node[count];
    }
    //construct zero matrix by graph
	int **graph, **cpy;
	graph = make2Darray(node_num, node_num);
	cpy = make2Darray(node_num, node_num);
    //record graph into matrix
    int linkID, linkend1, linkend2, channel_num;
    for(int count=0; count<link_num; count++){
        scanf("%d %d %d %d", &linkID, &linkend1, &linkend2, &channel_num);
        graph[linkend1][linkend2] = channel_num, graph[linkend2][linkend1] = channel_num;
        cpy[linkend1][linkend2] = channel_num, cpy[linkend2][linkend1] = channel_num;
    }
    //record request
    int reqID, req_src[req_num], req_dst[req_num];
    for(int count=0; count<req_num; count++){
        scanf("%d %d %d", &reqID, &req_src[count], &req_dst[count]);
    }
	//store shortest path and count how man path is accepted
	int path_num = 0;
	for(int count=0; count<req_num; count++){
        path_num = dijkstra(cpy, req_src[count], req_dst[count], node_num, node_cpy, path_num, 0, count);
	}
    //print answer
    printf("%d\n", path_num);
    for(int count=0; count<req_num; count++){
        path_num = dijkstra(graph, req_src[count], req_dst[count], node_num, node, path_num, 1, count);
    }
    //clear pointer
    for(int count=0; count<node_num; count++){
        free(graph[count]), free(cpy[count]);
    }

    free(graph), free(cpy);
	return 0;
}

