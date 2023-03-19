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

//print graph
void print2Darray(int **matrix, int row, int col){//for graph
    for(int count1=0; count1<row; count1++){
        for(int count2=0; count2<col; count2++)
            printf("%d ", matrix[count1][count2]);
        printf("\n");
    }
}

void printarray(int *array, int N){
    for(int count = 0; count < N; count++)
    {
        printf("%d ", array[count]);
    }
    printf("\n");
}
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

//int findpath(int *dist, int src, int dst, int shell, int V, int *parent){
//    if(dst == src){
//        printf("%d ", dst);
//        return 1;
//    }
//    for(int count = 0; count < V; count++){
//        if(dist[count] == shell && graph[count][dst] > 0 && node[count] >= 2){
//            parent[dst] = count;
//            dst = count;
//        }
//        else
//            return 0;
//    }
//    return 0;
//}

// 顯示路徑(正向找 反向印)(dijkstra裡面用到)
void printPath(int *parent, int Rdst, int Rsrc){
    // 當找到source回傳
    if (parent[Rdst] == Rsrc){
        printf("%d ", Rdst);
        return;
    }
    // 遞迴，找到source才列印
    printPath(parent, parent[Rdst], Rsrc);
    printf("%d ", Rdst);
}

// 跑 dijkstra 存取 shortest path tree 並且回傳當前成功的路徑數量
void dijkstra(int V, int RN, int **RL, int **graph, int *node){
    //    if(flag == 1){
        printf("node_num : ");
        for(int count=0; count<V; count++)
            printf("%d ", node[count]);
        printf("\n");
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
    dist[0] = 0;
    parent[0] = -1;
    // Find shortest path for all vertices
    for (int count = 0; count < V-1; count++){
        // Pick the minimum distance vertex from the set of vertices not yet processed. u is always equal to src in the first iteration.
        int u = minDistance(dist, sptSet, V);
//        if(flag == 1)
            printf("%d\n", u);
        // If no node in rest sptSet can choose to be next point, stop making  spt
        if(u == -1)
            break;
        // Mark the picked vertex as processed
        sptSet[u] = true;
        // Update dist value of the adjacent vertices of the picked vertex.
        for (int v = 0; v < V; v++){
            // Update dist[v] only if is not in sptSet, there is an edge from u to v, and total weight of path from src to  v through u is smaller than current value of dist[v]
            if(!sptSet[v] && dist[u]+1<dist[v] && graph[u][v]>0 && dist[u]!=INT_MAX){
                    dist[v] = dist[u] + 1;
                    parent[v] = u;
//                    if(flag ==1)
                        printf("%d %d\n", v, dist[v]);
            }
        }
    }

    int path_num = 0, RID[RN];
    for(int count = 0; count < V; count++){
        RID[count] = 0;
        printf("%d ", parent[count]);
    }
    printf("\n");


    for(int count = 0; count < RN; count++){
        //check the availability of memories for nodes and links of the path
        int abt_link = 1, abt_node = 1;
        int itr=RL[count][2];

        if(itr == RL[count][2] || itr == RL[count][1]){
            if(node[itr] < 1){
//                    if(flag == 1)
                printf("src/dst : %d no capacity(%d)\n", itr, node[itr]);
                abt_node = 0;
            }
        }
        while(itr != parent[RL[count][1]]){
            //check memories capacity
            if(abt_node == 1){
                if(node[itr] < 2){
//                    if(flag == 1)
                        printf("path : %d no capacity(%d)\n", itr, node[itr]);
                    abt_node = 0;
                }
            }
            //check if linkage exist
            if(abt_link == 1){
                if(parent[itr] == -2){
    //                if(flag == 1)
                        printf("parent[%d] : %d no linkage(%d)\n", itr, parent[itr], parent[itr]);
                    abt_link = 0;
                }
            }
            if(abt_node == 0 || abt_link == 0){
    //            if(flag == 1)
                    printf("this path not available\n");
                break;
            }

            itr=parent[itr];
        }

        if(abt_node == 1 && abt_link == 1){
            path_num++;
            printf("RID[%d], %d\n", count, RID[count]);
            RID[count] = 1;
            printf("RID[%d], %d\n", count, RID[count]);
    //            if(flag == 1){
    //                printf("%d ", RL[count][0]);
    //                printPath(parent, RL[count][2], RL[count][1]);
    //                printf("\n");
    //            }
            int count1 = RL[count][2];
            printf("RL[%d][2] = %d\n", count, RL[count][2]);

    //            while(count1 != RL[count][1]){
    //                printf("test1\n");
    //                node[count1]--;
    //                node[parent[count1]]--;
    //                graph[count1][parent[count1]]--;
    //                graph[parent[count1]][count1]--;
    //                count1 = parent[count1];
    //            }

            printf("test2\n");
            break;
        }


        //if path is legal, then execute changes to matrix/graph

    //    if(flag == 1){
    //        printf("node_num : ");
    //        for(int count=0; count<V; count++)
    //            printf("%d ", node[count]);
    //        printf("\n");
    //    }
        //let main know the ID value&situation

    }

    printf("%d\n", path_num);
    for(int count = 0; count < RN; count++ ){
        if(RID[count] == 1){
            printf("%d -> %d\n", RL[count][2], RL[count][1]);
            printf("%d ", RL[count][0]);
            printf("%d ", RL[count][1]);
            printPath(parent, RL[count][2], RL[count][1]);
        }
    }
}

int main(){
    //read input data
    int node_num, link_num, req_num;
    scanf("%d %d %d", &node_num, &link_num, &req_num);
    //construct array of quantum memories for each node
    int node[node_num], nodeID;
    for(int count=0; count<node_num; count++){
        scanf("%d %d", &nodeID, &node[count]);
    }
    //construct zero matrix by graph
	int **graph;
	graph = make2Darray(node_num, node_num);
    //record graph into matrix
    int linkID, linkend1, linkend2, channel_num;
    for(int count=0; count<link_num; count++){
        scanf("%d %d %d %d", &linkID, &linkend1, &linkend2, &channel_num);
        graph[linkend1][linkend2] = channel_num, graph[linkend2][linkend1] = channel_num;
    }
    int **req_list;
    req_list = make2Darray(req_num, 3);
    for(int count = 0; count < req_num; count++){
        scanf("%d %d %d", &req_list[count][0], &req_list[count][1], &req_list[count][2]);
    }

    printf("node\n");printarray(node, node_num);
    printf("graph\n"); print2Darray(graph, node_num, node_num);
    printf("request\n");print2Darray(req_list, req_num, 3);

    //dijkstra
    dijkstra(node_num, req_num, req_list, graph, node);

    //clear pointer
    for(int count=0; count<node_num; count++){
        free(graph[count]);
    }
    free(graph);
	return 0;
}

