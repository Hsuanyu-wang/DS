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

int findpath(int *dist, int src, int dst, int shell, int V, int *parent, int **graph, int *node){
    if(dst == src){
        printf("%d ", dst);
        return 1;
    }
    for(int count = 0; count < V; count++){
        if(dist[count] == shell && graph[count][dst] > 0 && node[count] >= 2){
            parent[dst] = count;
            dst = count;
        }
        else
            return 0;
    }
    return 0;
}

// 顯示路徑(正向找 反向印)(dijkstra裡面用到)
void printPath(int *parent, int Rdst, int Rsrc){
    // 當找到source回傳
    if (parent[Rdst] == parent[Rsrc]){
        printf("%d ", Rdst);
        return;
    }
    // 遞迴，找到source才列印
    printPath(parent, parent[Rdst], Rsrc);
    printf("%d ", Rdst);
}

// 跑 dijkstra 存取 shortest path tree 並且回傳當前成功的路徑數量
int dijkstra(int V, int RN, int **RL, int **graph, int *node){

//初始化
    printf("######## 1 #########\n");
    int dist[V], PN = 0, PID[V], parent[V];//dist存點的距離, PN存成功數量, PID存成功的ID
    bool sptSet[V];
    for (int count = 0; count < V; count++){
        dist[count] = INT_MAX;
        sptSet[count] = false;
        PID[count] = 0;
        parent[V] = -2;
    }
    //用0當src做spt,距離為0,parent設為-1
    dist[0] = 0;
    parent[0] = -1;

//做spt
    printf("######## 2 #########\n");
    for (int count = 0; count < V; count++){
        int u = minDistance(dist, sptSet, V);
        printf("%d\n", u);
        if(u == -1)
            break;
        sptSet[u] = true;
        for (int v = 0; v < V; v++){
            if(!sptSet[v] && dist[u]+1<dist[v] && graph[u][v]>0 && dist[u]!=INT_MAX){
                    dist[v] = dist[u] + 1;
                    parent[v] = u;
                    printf("%d %d\n", v, dist[v]);
            }
        }
    }

//跑每個request
    printf("######## 3 #########\n");
    int shell = 0, src = 0, dst = 0, dir = 0;

    for(int count = 0; count < RN; count++){
        //起點或終點沒有容量就跳到下一個request
        if(node[RL[count][1]] < 1 || node[RL[count][2]] < 1){
            continue;
        }
        //檢查各request起點終點有沒有讀近來
        printf("dist[src] dist[dst] %d %d\n", dist[RL[count][1]], dist[RL[count][2]]);
        //
        if(dist[RL[count][2]] >= dist[RL[count][1]]){
            dst = RL[count][2];
            src = RL[count][1];
        }


        shell = dist[dst]-1;
        printf("shell %d\n", shell);

        //check each shell to find path
        if(findpath(dist, src, dst, shell, V, parent, graph, node) == 1){
            printf("%d ", dst);
            PN++;
            PID[count] = 1;
//            graph[count][parent[count]]--, graph[parent[count]][count]--;
//            node[count]--, node[parent[count]]--;
        }
    }

//檢查成功的request並且列印出成功數和路徑
    printf("######## 4 #########\n");
    printf("%d\n", PN);
    for(int count = 0; count < V; count++){
        if(PID[count] == 1){
            printf("%d ", RL[count][0]);
            printPath(parent, RL[count][2], RL[count][1]);
            printf("\n");
        }
        else
            printf("PN = 0\n");
    }

    printf("######## 5 #########\nend\n");
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

