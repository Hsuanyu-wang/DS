#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct heap_tree{
    int ID;//heap, 1開始
    int level;//層數
    int order;//左到右
    int flag;//1為實點,0為虛點
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
    for(int count = 0; count < MAX; count++){
        printf("(%2d ; %d ; %d ; %lu)\n", heap[count].ID, heap[count].level, heap[count].order, heap[count].data);
    }
}

//檢查child*2
int check_error(struct heap_tree* heap, char** ori_data, char** error_data, int lowest_left, int ID, int error_num){
    if(heap[ID].flag == 1){
        if(ID >= lowest_left){
            strcpy(error_data[error_num], ori_data[ID-lowest_left]);
            error_num++;
            return error_num;
        }
        if(heap[ID*2].flag == 1){
            unsigned long comp;
            printf("1 %d %d\n", heap[ID * 2].level, heap[ID * 2].order);
            fflush(NULL);
            scanf("%lu", &comp);
            if(heap[ID * 2].data != comp){
                error_num = check_error(heap, ori_data, error_data, lowest_left, ID*2, error_num);
            }
        }
        if(heap[ID*2+1].flag == 1){
            unsigned long comp;
            printf("1 %d %d\n", heap[ID * 2 + 1].level, heap[ID * 2 + 1].order);
            fflush(NULL);
            scanf("%lu", &comp);
            if(heap[ID * 2 + 1].data != comp){
                error_num = check_error(heap, ori_data, error_data, lowest_left, ID*2+1, error_num);
            }
        }
    }
    return error_num;
}

//主程式
int main(){
    //宣告 & 讀取str_num, strlen_max;
    int str_num, strlen_max;
    scanf("%d %d", &str_num, &strlen_max);

    //計算層數,tempa拿去除,tempb記餘數
    int count = 1, layer_num = 1, layer_no;
    while(1){
        if(str_num <= count)
            break;
        layer_num ++;
        count *= 2;
    }
    layer_no = layer_num - 1;

    //建立heap tree, 宣告 & default
    int total_node = power(2, layer_num);
    int lowest_left = power(2, layer_no);
    struct heap_tree heap[total_node];
    int count_order = 0, count_level = 0;
    for(int i = 1; i < total_node; i++){
        heap[i].data = heap[i].flag = 0;
        heap[i].ID = i;
        if(i == power(2, count_level+1)){
            count_level++;
            count_order = 0;
        }
        heap[i].level = count_level;
        heap[i].order = count_order;
        count_order++;
    }
    heap[0].ID = heap[0].level = heap[0].order = heap[0].flag = -1, heap[0].data = (unsigned long)str_num;

    //宣告error_data, ori_data;
    char str[strlen_max];
    char **ori_data,  **error_data;
    error_data = malloc(sizeof(char*) * str_num);
    ori_data = malloc(sizeof(char*) * str_num);
    for(int count = 0; count < str_num; count++){
        ori_data[count] = malloc(sizeof(char) * strlen_max);
        error_data[count] = malloc(sizeof(char) * strlen_max);
    }

    //輸入最下層資料到heap最下層(經過hashing)，保留原本string到ori_data，error_data default為"none"
    for(int i = 0; i < str_num; i++){
        scanf("%s", str);
        heap[i + lowest_left].data = MurmurOAAT32(str);
        heap[i + lowest_left].flag = 1;
        strcpy(ori_data[i], str);
        strcpy(error_data[i], "none");
    }

    //建立merkle tree, temp往上填資料
    for(int i = lowest_left - 1; i >= 1; i--){
        unsigned long temp = 0;
        if(heap[i*2].flag == 0 && heap[i*2+1].flag == 0){
            heap[i].flag = 0;
        }
        else if(heap[i*2 + 1].flag == 0){
            temp = heap[i*2].data * 2;
            heap[i].flag = 1;
        }
        else{
            temp = heap[i*2].data + heap[i*2 + 1].data;
            heap[i].flag = 1;
        }
        if(heap[i].flag == 1){
            sprintf(str, "%lu", temp);
            heap[i].data = MurmurOAAT32(str);
        }
    }

    //先檢查root一樣就不用繼續做，不然就往下檢查
    unsigned long accept;
    printf("1 0 0\n");
    fflush(NULL);
    scanf("%lu", &accept);
    int error_num = 0;
    if(accept != heap[1].data){
        error_num = check_error(heap, ori_data, error_data, lowest_left, 1, error_num);
    }

    //輸出答案
    printf("2 %d\n", error_num);
    for(int count = 0; count < str_num; count++){
        if( strcmp(error_data[count], "none") != 0){
            printf("%s\n", error_data[count]);
        }
    }
	return 0;
}
