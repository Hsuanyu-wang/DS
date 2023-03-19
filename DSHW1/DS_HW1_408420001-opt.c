#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
// �s�y�ʺA�}�C,��l�Ȭ�0
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
// function�Ω�dijkstra,�Ψӧ�U�@���I������
int minDistance(int dist[], bool sptSet[], int V){
    //min�ΨӪ�̪ܳ񪺶Z��,��l�O�L���j,�U���j��|���N���
    int min = INT_MAX;
    //min_index��ܸ��I��index
    int min_index;
    // ��for��|���i�JsptSet�H�ζZ���ŦX���I(�Ʀr�j���I�}�l)
    for (int v = 0; v < V; v++){
        if (sptSet[v] == false && dist[v] <= min){
            min = dist[v];
            min_index = v;
        }
    }
    // �^�ǧ�쪺�U�@���I��index
    return min_index;
}
// function�Ω�dijkstra,�Q��parent��ܸ��|
void printPath(int *parent, int tail){
    // ����source�^��stack
    if (parent[tail] == -1){
        printf("%d ", tail);
        return;
    }
    // ���j
    printPath(parent, parent[tail]);
    printf("%d ", tail);
}
// �]dijkstra�t��k,�s�� shortest path tree(spt) �åB�^�Ƿ�e���\�����|�ƶq
//###########################################################################################
//parent[RID = 0~RN][0~VN-1], src = RL[RID][1], dst = RL[RID][2], flag cancel
//###########################################################################################
int dijkstra(int **graph, int *node, int **Fix_graph, int VN, int RN, int PN, int *RL, int *parent, int *RID){
    //###########################################################################################
    int src = RL[1];
    int dst = RL[2];
    int ID = RL[0];
    // ��ܦU�I��src�Z��
    int dist[VN];
    // true��ܸ��I�w�g�ιL�B�bspt��,false�h����٨S�ιL
    bool sptSet[VN];
    // �s�C���I��parent
//    int parent[VN];
    // ��l�ƭ�sptSet = false, parent = -1, dist = INT_MAX
    for (int count = 0; count < VN; count++){
        dist[count] = INT_MAX;
        sptSet[count] = false;
//###########################################################################################
//        parent[count] = -2;
//###########################################################################################
    }
    // �]�w�_�Idist, parent�Ω�s�@spt
//###########################################################################################
    dist[src] = 0;
    parent[src] = -1;
//###########################################################################################
    // ��spt, �����U�Idist�ȩM�إ�parent
    for (int count = 0; count < VN-1; count++){
        // ��i�檺�U���I����
        int min_v = minDistance(dist, sptSet, VN);
        // �I�i�Jspt,��srtSet=true���
        sptSet[min_v] = true;
        // ���I�s�y�P�L��Ĳ���I��dist, parent
        for (int v = 0; v < VN; v++){
            // Update dist[v] only if is not in sptSet, there is an edge from u to v, and total weight of path from src to  v through u is smaller than current value of dist[v]
            if(!sptSet[v] && dist[min_v]+1 < dist[v] && Fix_graph[min_v][v] > 0 && dist[min_v] != INT_MAX){
                    dist[v] = dist[min_v] + 1;
                    parent[v] = min_v;
            }
        }
    }
    // �ˬdvertex capacity & link
    // availability of link/node ��l�ƭȬ�1,���]���|�i��
    int abt_link = 1, abt_node = 1;
    // iterate�qdst��src��V�],�ˬd�Uvertex�O�_�ŦX����
    int itr = dst;
    while(itr != -1){
        // �ˬd vertex capacity
        if(abt_node == 1){
            // �P�_�_�I���I �]���u�n����1��capacity
            if(itr == dst || itr == src){
                if(node[itr] < 1){
                    abt_node = 0;
                }
            }
            // �D�_�I���I�n����2��capacity
            else{
                if(node[itr] < 2){
                    abt_node = 0;
                }
            }
        }
        // �ˬd link
        if(abt_link == 1){
            // �Sparent��ܥ���
            if(parent[itr] == -2){
                abt_link = 0;
            }
            // �S�ˬdnode-node link �ƶq�]���e����spt���ˬd
        }
        // capacity || link���@���ѳ����⦨�\path
        if(abt_node == 0 || abt_link == 0){
//            if(flag == 1)
//                printf("this path not available\n");
            // �u�n�J�즳���ѫ᭱�]���ζ]�F
            break;
        }
        itr=parent[itr];
    }
    // ���|���\�Npath_num+1
    if(abt_node == 1 && abt_link == 1){
        PN++;
        RID[ID] = 1;
        // ��flag = 1�~�L�Xpath
//        else if(flag == 1){
//        printf("%d ", RL[0]);
//        printPath(parent, dst);
//        printf("\n");
//        }
        int count = dst;
        // ����path���Ӫ�capacit, link
        while(count != src){
            node[count]--;
            node[parent[count]]--;
            graph[count][parent[count]]--;
            graph[parent[count]][count]--;
            count = parent[count];
        }
    }
    // �^�Ǧ��\�����|��
    return PN;
}
// �D�{��
int main(){
    // Ū��vertex, link, request�ƶq
    int node_num, link_num, req_num;
    scanf("%d %d %d", &node_num, &link_num, &req_num);
    // �إߨC��vertex��capacit
    int node[node_num], nodeID, node_cpy[node_num];
    for(int count=0; count<node_num; count++){
        scanf("%d %d", &nodeID, &node[count]);
        node_cpy[count] = node[count];
    }
    // �إ߹s�x�}
	int **graph, **graph_cpy;
	graph = make2Darray(node_num, node_num, 0);
	graph_cpy = make2Darray(node_num, node_num, 0);
    // ��Ϭ��x�}
    int linkID, linkend1, linkend2, channel_num;
    for(int count=0; count<link_num; count++){
        scanf("%d %d %d %d", &linkID, &linkend1, &linkend2, &channel_num);
        graph[linkend1][linkend2] = channel_num, graph[linkend2][linkend1] = channel_num;
        graph_cpy[linkend1][linkend2] = channel_num, graph_cpy[linkend2][linkend1] = channel_num;
    }
//    // ����request
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
	// �p�⦨�\�����|��
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
    // �C�L����
    printf("%d\n", path_num);
    for(int count = 0; count < req_num; count++){
        if(req_ID[count] == 1){
            printf("%d ", count);
            printPath(parent[count], req_list[count][2]);
            printf("\n");
        }
    }
////###########################################################################################
    // �M���ʺA�ŧi���Ŷ�
    for(int count=0; count<node_num; count++){
        free(graph[count]), free(graph_cpy[count]);
    }
    free(graph), free(graph_cpy);
    // ����{��
	return 0;
}
