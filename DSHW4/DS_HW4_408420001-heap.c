#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct heap_tree{
    int ID;
    int level;
    int order;
    unsigned long data;
};

unsigned long MurmurOAAT32 (char * key)
{
    unsigned long h = 3323198485ul;
    for (;*key;++key) {
        h ^= *key;
        h *= 0x5bd1e995;
        h ^= h >> 15;
    }
    return h;
}

int power(int a, int n){
    int ans = 1;
    for(int i = 0; i < n; i++){
        ans *= a;
    }
    return ans;
}

void print_heap(struct heap_tree* heap, int MAX){
//    printf("(%d ; %lu)\n", heap[0].ID, heap[0].data);
    for(int count = 0; count < MAX; count++){
//        printf("%d: ", count);
        printf("(%2d ; %d ; %d ; %lu)\n", heap[count].ID, heap[count].level, heap[count].order, heap[count].data);
    }
}

int check_error(struct heap_tree* heap, char** ori_data, char** error_data, int lowest_left, int ID, int error_num){
    printf("ID : %d ; error_num : %d\n", ID, error_num);
    if(ID >= lowest_left){
//        system("pause");
        strcpy(error_data[error_num], ori_data[ID-lowest_left]);
//        system("pause");
        printf("error data found : %s ; (level, order) : (%d, %d)\n", error_data[error_num], heap[ID].level, heap[ID].order);
//        system("pause");
        error_num++;
        return error_num;
    }
    else if(heap[ID * 2].data != 0){
        unsigned long comp;
        printf("leftchild : \n");
        printf("1 %d %d\n", heap[ID * 2].level, heap[ID * 2].order);
        scanf("%lu", &comp);
        if(heap[ID * 2].data != comp){
            error_num = check_error(heap, ori_data, error_data, lowest_left, ID*2, error_num);
        }
        printf("rightchild : \n");
        printf("1 %d %d\n", heap[ID * 2 + 1].level, heap[ID * 2 + 1].order);
        scanf("%lu", &comp);
        if(heap[ID * 2 + 1].data != comp){
            error_num = check_error(heap, ori_data, error_data, lowest_left, ID*2+1, error_num);
        }
    }
    return error_num;
}

//主程式
int main(){
    //宣告 & 讀取str_num, strlen_max;
    int str_num, strlen_max, layer_num, layer_no;;
    scanf("%d %d", &str_num, &strlen_max);

    //計算層數,tempa拿去除,tempb記餘數
    int tempa = str_num, tempb, count = 1;
    while(tempa != 1){
        tempb = tempa % 2;
        tempa /= 2;
        count++;
        if(tempb > 0 && tempa == 1){
            count++;
        }
    }
    layer_num = count;
    layer_no = layer_num - 1;
    printf("layer_num : %d\nlayer_no : %d\n", layer_num, layer_no);

    //建立heap tree, 宣告 & default
    int total_node = power(2, layer_num);
    int lowest_left = power(2, layer_no);
    printf("lowest left : %d\n", lowest_left);
    struct heap_tree heap[total_node];
    int count_order = 0, count_level = 0;
    for(int i = 1; i < total_node; i++){
        heap[i].data = 0;
        heap[i].ID = i;
        if(i == power(2, count_level+1)){
            count_level++;
            count_order = 0;
        }
        heap[i].level = count_level;
        heap[i].order = count_order;
        count_order++;
    }
    heap[0].ID = heap[0].level = heap[0].order = -1, heap[0].data = (unsigned long)str_num;

    print_heap(heap, total_node);

    //輸入最下層資料(經過hashing)
    char str[strlen_max];
    char **ori_data,  **error_data;
    error_data = malloc(sizeof(char*) * str_num);
    ori_data = malloc(sizeof(char*) * str_num);
    for(int count = 0; count < str_num; count++){
        ori_data[count] = malloc(sizeof(char) * strlen_max);
        error_data[count] = malloc(sizeof(char) * strlen_max);
    }

    for(int i = 0; i < str_num; i++){
        scanf("%s", str);
        printf("string : %s\n", str);
        heap[i + lowest_left].data = MurmurOAAT32(str);
        strcpy(ori_data[i], &str);
        strcpy(error_data[i], "none");
        printf("ori[%d] = %s ; heap[%2d] : %lu\n", i, ori_data[i], i + lowest_left, heap[i + lowest_left].data);
//        printf("(%2d ; %lu)\n", heap[i + lowest_left].ID, heap[i + lowest_left].data);
    }

    print_heap(heap, total_node);

    //建立merkle tree, temp往上填資料
    for(int i = lowest_left - 1; i >= 1; i--){
        unsigned long temp;
        if(heap[i*2 + 1].data == 0){
            temp = heap[i*2].data * 2;
        }
        else{
            temp = heap[i*2].data + heap[i*2 + 1].data;
        }
//        printf("temp : %lu\n", temp);
        sprintf(str, "%lu", temp);
//        printf("string : %s\n", string);
        heap[i].data = MurmurOAAT32(str);
        printf("heap[%d] : %lu\n", i, heap[i].data);
//        printf("data : %lu\nID : %d\n", heap[i + temp].data, heap[i + temp].ID);
    }

    print_heap(heap, total_node);

    printf("error_data : \n");
    for(int count = 0; count < str_num; count++){
        printf("%s\n", error_data[count]);
    }

    //開始檢查
    unsigned long accept;
    printf("ID : 1\n1 0 0\n");
    scanf("%lu", &accept);
    int error_num = 0;
    if(accept != heap[1].data){
        if(heap[2].data != 0)
            error_num = check_error(heap, ori_data, error_data, lowest_left, 2, error_num);
        if(heap[3].data != 0)
            error_num = check_error(heap, ori_data, error_data, lowest_left, 3, error_num);
    }

    //answer
    printf("2 %d\n", error_num);
    for(int count = 0; count < str_num; count++){
        if( strcmp(error_data[count], "none") != 0){
            printf("%s\n", error_data[count]);
        }
    }
	return 0;
}
