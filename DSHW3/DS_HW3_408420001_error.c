#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include <string.h>
int top = 0;
// construct structure
struct edge{
    int ID;
    int n1;
    int n2;
    int flag;
    double dist;
};
// count distance
double dist_count(double x1, double y1, double x2, double y2){
    return sqrt( (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2) );
}
// print function
void print_heap(struct edge* heap, int MAX){
    printf("(0) (%f ; %d ; %d)\n", heap[0].dist, heap[0].n1, heap[0].n2);
    for(int count = 1; count <= MAX; count++){
        printf("%d: ", count);
        printf("(%f ; %d ; %d)\n", heap[count].dist, heap[count].n1, heap[count].n2);
    }
}
void print_array(int* array, int MAX){
    for(int count = 0; count < MAX; count++){
        printf("%d ", array[count]);
    }
    printf("\n");
}
void print_2Darray(double** array, int row, int col){
    for(int count = 0 ; count < row ; count++){
        for(int count1 = 0; count1 < col ; count1++){
            printf("%9f ", array[count][count1]);
        }
        printf("\n");
    }
}
void print_2Darray_int(int** array, int row, int col){
    for(int count = 0 ; count < row ; count++){
        for(int count1 = 0; count1 < col ; count1++){
            printf("%d ", array[count][count1]);
        }
        printf("\n");
    }
}
// heap function
void adjust_heap(struct edge* heap,int chk_head){//Function to arrange the elements in the heap
    double temp;
    int last_pos, path_num, control = 1, temp1;
    path_num = heap[0].dist;
    while(2*chk_head <= path_num && control == 1)//left child
    {
        last_pos = 2*chk_head;// last_pos = 最後 left child
        if(last_pos+1 <= path_num && heap[last_pos+1].dist < heap[last_pos].dist){//如果最後right child大
            last_pos ++;//last_pos = 最後 right child
        }
        if(heap[last_pos].dist > heap[chk_head].dist){//child < parent -> break (correct)
            control = 0;
        }
        else{//child > parent -> change (wrong)
            temp = heap[chk_head].dist, heap[chk_head].dist = heap[last_pos].dist, heap[last_pos].dist = temp;
            temp1 = heap[chk_head].n1, heap[chk_head].n1 = heap[last_pos].n1, heap[last_pos].n1 = temp1;
            temp1 = heap[chk_head].n2, heap[chk_head].n2 = heap[last_pos].n2, heap[last_pos].n2 = temp1;
            temp = heap[chk_head].ID, heap[chk_head].ID = heap[last_pos].ID, heap[last_pos].ID = temp;
            temp = heap[chk_head].flag, heap[chk_head].ID = heap[last_pos].flag, heap[last_pos].flag = temp;
            chk_head = last_pos;
        }
    }
}
void min_heap(struct edge* heap){
    for(int chk_head = heap[0].dist/2; chk_head >= 1; chk_head--){
        adjust_heap(heap, chk_head);
    }
}
void weightedUnion(int* djtSet, int i, int j){//set new boss
    int numi = i, numj = j;
    for(; djtSet[numi] >= 0; numi = djtSet[numi]);
    for(; djtSet[numj] >= 0; numj = djtSet[numj]);
//    printf("numi : %d, numj : %d\n", numi, numj);
    int temp = djtSet[numi] + djtSet[numj];//negative number
//    printf("temp = %d\n", temp);
    if (djtSet[numi] > djtSet[numj]) {//parent[j] is greater
        djtSet[numi] = numj;
        djtSet[numj] = temp;
    }
    else {//parent[i] is greater
        djtSet[numj] = numi;
        djtSet[numi] = temp;
    }
}
void delete_heap(struct edge* heap){
    //delete n1, n2 from sptSet
    int last_pos, temp1, temp2;
    double temp;
    last_pos = heap[0].dist;
    temp = heap[1].dist, heap[1].dist = heap[last_pos].dist, heap[last_pos].dist = temp;
    temp1 = heap[1].n1, heap[1].n1 = heap[last_pos].n1, heap[last_pos].n1 = temp1;
    temp2 = heap[1].n2, heap[1].n2 = heap[last_pos].n2, heap[last_pos].n2 = temp2;
    heap[0].dist--;
}
int collapsingFind(int parent[], int i){//modified find(i)
    int root, trail, lead;
    for (root = i; parent[root] >= 0; root = parent[root]);
    for (trail = i; trail != root; trail = lead) {
        lead = parent[trail];
        parent[trail]= root;
    }
    return root;
}
int able_E(struct edge* heap, int path_num){
    int vac = 0;
    for(int count = 0; count < path_num; count++){
        if(heap[count].flag == 0){
            vac = 1;
            break;
        }
    }
    return vac;
}
//
void kruskal_MST(int node_num, struct edge* heap, int* djtSet, int* sptSet, int** graph, int path_num){
//    printf("kruskal srt\n");
    int set_count = 0;
    //while (set_count < node_num-1 && E is not empty)
    while (set_count < node_num-1 && able_E(heap, path_num) == 1){
//        printf("kruskal loop\n");
        //choose a least cost edge (n1,n2) from E;
        min_heap(heap);
//        printf("\nmin %d\n", set_count);
//        print_heap(heap, (int)heap[0].dist);
        //檢查是否形成cycle,若無則 modify djtSet,若行形成cycle則不採用
        if(collapsingFind(djtSet, heap[1].n1) != collapsingFind(djtSet, heap[1].n2)){// find of disjoint set{
//            printf("mark chosen edge, rem edge ID\n");
            heap[1].flag = 1, heap[1].ID = set_count;
//            printf("weight_union\n");
//            print_array(djtSet, node_num);
            weightedUnion(djtSet, heap[1].n1, heap[1].n2);//weighted union of disjoint set
//            print_array(djtSet, node_num);
//            printf("store MST graph\n");
            graph[heap[1].n1][heap[1].n2] = graph[heap[1].n2][heap[1].n1] = 1;
            //mark passed node & delete (n1,n2) from E;
//            printf("sptSet : mark passed node\n");
            sptSet[heap[1].n1]++, sptSet[heap[1].n2]++;
//            printf("delete %d\t", set_count);
//            printf("(%12.8f ; %d ; %d)\n", heap[1].dist, heap[1].n1, heap[1].n2);
            delete_heap(heap);
//            printf("set_count %d\n", set_count);
            set_count++;
//            printf("set_count %d\n", set_count);
        }
        else{//discard n1, n2
            heap[1].flag = 1;
//            printf("delete%d\t", set_count);
//            printf("(%12.8f ; %d ; %d)\n", heap[1].dist, heap[1].n1, heap[1].n2);
            delete_heap(heap);
//            printf("discard\n");
        }
    }
    if (set_count < node_num-1){
//        printf("No spanning tree\n");
    }
}
//
void push(int item, int* stack){/* add an item to the global stack */
    stack[++top] = item;
}
//
int pop (int* stack, int top){/* return the top element from the stack */
    return stack[top--];
}
//
void DFS(int* sptSet, int** graph, int src, int node_num, double** dist, int* order_stack){
//    printf("DFS - src : %d, level : %d\n", src, level);
    // 判斷本次DFS是否有走點
    // src是否有路可以走，有就印出點並走過去，傳graph給下個點，並做下個點當src的DFS
    for(int count = 0; count < node_num; count++){
        if(graph[src][count] == 1 || graph[count][src] == 1 && sptSet[count] == 0){
//            printf("%d ", count);
            push(count, order_stack);
            sptSet[count] = 1;
            graph[src][count] = graph[count][src] = -1;
            DFS(sptSet, graph, count, node_num, dist, order_stack);
        }
    }
}
// 主程式
int main(){
    //讀取點數量 、 限制
    int node_num = 0, B = 0;
    scanf("%d %d", &node_num, &B);
    //讀取點座標 & 初始化陣列
    int skipID = 0, heap_num = 1, UAVID = 0, path_num = 0;
    int* sptSet = malloc(sizeof(int) * node_num);
    int* djtSet = malloc(sizeof(int) * node_num);
    int* order_stack = malloc(sizeof(int) * node_num);
    double path_length = 0;
    double* node_x = malloc(sizeof(double) * node_num);
    double* node_y = malloc(sizeof(double) * node_num);
    path_num = node_num*(node_num-1)/2;
    // declare dist
    double** dist = malloc(sizeof(double*) * node_num);
    for(int count=0; count<node_num; count++){
        dist[count] = calloc(node_num, sizeof(double));
    }
    // declare path
    int **path = malloc(sizeof(int*) * node_num);
    for(int count=0; count<node_num; count++){
        path[count] = calloc(node_num, sizeof(int));
    }
    // declare heap
    struct edge *heap = malloc(sizeof(struct edge) * path_num);
    //input x,y & set default
    for(int count = 0; count < node_num; count++){
        scanf("%d %lf %lf", &skipID, &node_x[count], &node_y[count]);
    }
    for(int count = 0; count < node_num; count++){
        order_stack[count] = -1, sptSet[count] = 0, djtSet[count] = -1;
    }
    order_stack[0] = 0;
    sptSet[0] = 1;

    for(int count = 0; count < node_num; count++){
        for(int count1 = 0; count1 < node_num; count1++){
            dist[count][count1] = dist[count1][count] = dist_count(node_x[count], node_y[count], node_x[count1], node_y[count1]);
        }
    }
    //印出初始矩陣
//    print_2Darray(dist, node_num, node_num);
////    印出該有的路徑
//    printf("path_num\n");
//    for(int i=0;i<node_num;i++)
//    {
//        for(int k=0;k <= i;k++){
//            printf("          ");
//        }
//        for(int j=i+1;j<node_num;j++){
//            printf("%9f ", dist[i][j]);
//        }
//        printf("\n");
//    }
    //建立並初始化heap
    for(int count = 0; count < node_num; count++){
        for(int count1 = count+1; count1 < node_num; count1++){
            heap[heap_num].ID = -1, heap[heap_num].flag = 0;
            heap[heap_num].n1 = count, heap[heap_num].n2 = count1;
            heap[heap_num].dist = dist[count][count1];
//            printf("count, count1, heap, %d, %d, %f\n", count, count1, heap[heap_num].dist);
            heap_num++;
        }
    }
    heap[0].n1 = heap[0].n2 = 0;
    heap[0].dist = (double)path_num;//heap[0] store total number of heap
    printf("heap[0].dist %f\n", heap[0].dist);
    //印出stored data heap
//    print_heap(heap, (int)heap[0].dist);
    //進行kruskal選邊，做MST存到graph
    printf("kruskal\n");
//    print_2Darray_int(path, node_num, node_num);
    kruskal_MST(node_num, heap, djtSet, sptSet, path, path_num);
    print_2Darray_int(path, node_num, node_num);
    //對graph 做 DFS
    printf("DFS\n");
    DFS(sptSet, path, 0, node_num, dist, order_stack);
    print_array(order_stack, node_num);
    //answer
    printf("answer\n");
    for(int count = 0; count < node_num-1; count++){
        if(path_length + dist[order_stack[count]][order_stack[count+1]] > B/2){
            ++UAVID;
            path_length = dist[order_stack[count]][order_stack[count+1]];
        }
        path_length += dist[order_stack[count]][order_stack[count+1]];
    }
    printf("%d\n", UAVID+1);
    UAVID = 0, path_length = 0;
    printf("UAVID : 0\n0 ");
    for(int count = 0; count < node_num-1; count++){
        if(path_length + dist[order_stack[count]][order_stack[count+1]] > B/2){
            printf("\nUAVID : %d\n", ++UAVID);
            path_length = dist[order_stack[count]][order_stack[count+1]];
        }
        printf("%d ", order_stack[count+1]);
        path_length += dist[order_stack[count]][order_stack[count+1]];
    }

    for(int count = 0; count < node_num; count++){
        free(path[count]), free(dist[count]);
    }
    free(dist), free(path), free(heap), free(sptSet), free(order_stack), free(djtSet), free(node_x), free(node_y);
	return 0;
}
