#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
// 製造動態陣列,初始值為0
int **make2Darray(int row, int col, int value){//for graph
    int **temp = malloc(sizeof(int*) * row);
    for(int count=0; count<row; count++){
        temp[count] = malloc(sizeof(int) * col);
    }
    for(int count1=0; count1<row; count1++){
        for(int count2=0; count2<col; count2++){
            temp[count1][count2] = value;
        }
    }
    return temp;
}
// function用於dijkstra,用來找下一個點做延伸
int minDistance(int dist[], bool sptSet[], int V){
    //min用來表示最近的距離,初始是無限大,下面迴圈會迭代找到
    int min = INT_MAX;
    //min_index表示該點的index
    int min_index;
    // 用for找尚未進入sptSet以及距離符合的點(數字大的點開始)
    for (int v = 0; v < V; v++){
        if (sptSet[v] == false && dist[v] <= min){
            min = dist[v];
            min_index = v;
        }
    }
    // 回傳找到的下一個點的index
    return min_index;
}
// function用於dijkstra,利用parent顯示路徑
void printPath(int *parent, int tail){
    // 當找到source回傳stack
    if (parent[tail] == -1){
        printf("%d ", tail);
        return;
    }
    // 遞迴
    printPath(parent, parent[tail]);
    printf("%d ", tail);
}
// 跑dijkstra演算法,存取 shortest path tree(spt) 並且回傳當前成功的路徑數量
//###########################################################################################
//parent[RID = 0~RN][0~VN-1], src = RL[RID][1], dst = RL[RID][2], flag cancel
//###########################################################################################
int dijkstra(int **graph, int *node, int **Fix_graph, int VN, int RN, int PN, int *RL, int *parent, int *RID){
    //###########################################################################################
    int src = RL[1];
    int dst = RL[2];
    int ID = RL[0];
    // 表示各點到src距離
    int dist[VN];
    // true表示該點已經用過且在spt當中,false則表示還沒用過
    bool sptSet[VN];
    // 存每個點的parent
//    int parent[VN];
    // 初始化值sptSet = false, parent = -1, dist = INT_MAX
    for (int count = 0; count < VN; count++){
        dist[count] = INT_MAX;
        sptSet[count] = false;
//###########################################################################################
//        parent[count] = -2;
//###########################################################################################
    }
    // 設定起點dist, parent用於製作spt
//###########################################################################################
    dist[src] = 0;
    parent[src] = -1;
//###########################################################################################
    // 找spt, 完成各點dist值和建立parent
    for (int count = 0; count < VN-1; count++){
        // 找可行的下個點延伸
        int min_v = minDistance(dist, sptSet, VN);
        // 點進入spt,用srtSet=true表示
        sptSet[min_v] = true;
        // 用點製造與他接觸的點的dist, parent
        for (int v = 0; v < VN; v++){
            // Update dist[v] only if is not in sptSet, there is an edge from u to v, and total weight of path from src to  v through u is smaller than current value of dist[v]
            if(!sptSet[v] && dist[min_v]+1 < dist[v] && Fix_graph[min_v][v] > 0 && dist[min_v] != INT_MAX){
                    dist[v] = dist[min_v] + 1;
                    parent[v] = min_v;
            }
        }
    }
    // 檢查vertex capacity & link
    // availability of link/node 初始化值為1,假設路徑可行
    int abt_link = 1, abt_node = 1;
    // iterate從dst往src方向跑,檢查各vertex是否符合條件
    int itr = dst;
    while(itr != -1){
        // 檢查 vertex capacity
        if(abt_node == 1){
            // 判斷起點終點 因為只要消耗1個capacity
            if(itr == dst || itr == src){
                if(node[itr] < 1){
                    abt_node = 0;
                }
            }
            // 非起點終點要消耗2個capacity
            else{
                if(node[itr] < 2){
                    abt_node = 0;
                }
            }
        }
        // 檢查 link
        if(abt_link == 1){
            // 沒parent表示失敗
            if(parent[itr] == -2){
                abt_link = 0;
            }
            // 沒檢查node-node link 數量因為前面建spt有檢查
        }
        // capacity || link任一失敗都不算成功path
        if(abt_node == 0 || abt_link == 0){
//            if(flag == 1)
//                printf("this path not available\n");
            // 只要遇到有失敗後面也不用跑了
            break;
        }
        itr=parent[itr];
    }
    // 路徑成功就path_num+1
    if(abt_node == 1 && abt_link == 1){
        PN++;
        RID[ID] = 1;
        // 當flag = 1才印出path
//        else if(flag == 1){
//        printf("%d ", RL[0]);
//        printPath(parent, dst);
//        printf("\n");
//        }
        int count = dst;
        // 扣除path消耗的capacit, link
        while(count != src){
            node[count]--;
            node[parent[count]]--;
            graph[count][parent[count]]--;
            graph[parent[count]][count]--;
            count = parent[count];
        }
    }
    // 回傳成功的路徑數
    return PN;
}
// 主程式
int main(){
    // 讀取vertex, link, request數量
    int node_num, link_num, req_num;
    scanf("%d %d %d", &node_num, &link_num, &req_num);
    // 建立每個vertex的capacit
    int node[node_num], nodeID, node_cpy[node_num];
    for(int count=0; count<node_num; count++){
        scanf("%d %d", &nodeID, &node[count]);
        node_cpy[count] = node[count];
    }
    // 建立零矩陣
	int **graph, **graph_cpy;
	graph = make2Darray(node_num, node_num, 0);
	graph_cpy = make2Darray(node_num, node_num, 0);
    // 轉圖為矩陣
    int linkID, linkend1, linkend2, channel_num;
    for(int count=0; count<link_num; count++){
        scanf("%d %d %d %d", &linkID, &linkend1, &linkend2, &channel_num);
        graph[linkend1][linkend2] = channel_num, graph[linkend2][linkend1] = channel_num;
        graph_cpy[linkend1][linkend2] = channel_num, graph_cpy[linkend2][linkend1] = channel_num;
    }
//    // 紀錄request
//    int reqID, req_src[req_num], req_dst[req_num];
//    for(int count=0; count<req_num; count++){
//        scanf("%d %d %d", &reqID, &req_src[count], &req_dst[count]);
//    }
//#########################################################################
    int **req_list;
    req_list = make2Darray(req_num, 3, 0);
    for(int count = 0; count < req_num; count++){
        scanf("%d %d %d", &req_list[count][0], &req_list[count][1], &req_list[count][2]);
    }
//#############################################################################
	// 計算成功的路徑數
	int path_num = 0, req_ID[req_num], **parent;
	for(int count = 0; count < req_num; count++){
        req_ID[count] = 0;
	}

	parent = make2Darray(req_num, node_num , -2);

	for(int count = 0; count < req_num; count++){
        path_num = dijkstra(graph_cpy, node_cpy, graph, node_num, req_num, path_num, req_list[count], parent[count], req_ID);
	}

	for(int i = 0; i < req_num; i++){
        printf("%d ", req_ID[i]);
	}

    printf("\n");
////###########################################################################################
    // 列印答案
    printf("%d\n", path_num);
    for(int count = 0; count < req_num; count++){
        if(req_ID[count] == 1){
            printf("%d ", count);
            printPath(parent[count], req_list[count][2]);
            printf("\n");
        }
    }
////###########################################################################################
    // 清除動態宣告的空間
    for(int count=0; count<node_num; count++){
        free(graph[count]), free(graph_cpy[count]);
    }
    free(graph), free(graph_cpy);
    // 中止程式
	return 0;
}
