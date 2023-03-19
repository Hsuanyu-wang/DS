#include<stdio.h>
#include<stdlib.h>
#define height 4
//宣告 struct shape
struct shape{
    int ID;
    int freq;
    int time;
    struct shape *prior;
    struct shape *later;
};
////print 用來debug的
//void printdata(struct shape *array[], int MAX){
//    printf("\n");
//    for(int i = 0; i < MAX; i++){
//        printf("%d ", array[i]->ID);
//        for(struct shape *cur = array[i]; cur != NULL; cur = cur->later){
//            printf("%dx%d ", cur->freq, cur->time);
//        }
//        printf("\n");
//    }
//}
//
void printlist(struct shape *data_head[], int user_num, int list_x[], int list_y[]){
    for(int i = 0; i < user_num; i++){
        if(list_x[i] >= 0){
            printf("%d %dx%d %d %d\n", data_head[i]->ID, data_head[i]->freq, data_head[i]->time, list_y[i], list_x[i]);
        }
    }
}

int main(){
    //c  用來存取一個char為了後面if判斷用
    char c;
    //rsrc = resource
    int rsrc_y, rsrc_x, user_num;
    //讀input
    scanf("%d %d %d", &rsrc_y, &rsrc_x, &user_num);
    //宣告一個指標陣列準備存取資料(每個ID的head)
    struct shape *data_head[user_num];
    for(int i = 0; i < user_num; i++){
        data_head[i] = NULL;
    }
    //宣告會用到的變數
    int temp_ID, temp_f, temp_t;
    struct shape *head = NULL;
    struct shape *new_data = NULL;
    //讀資料，從每個id的指標陣列做 candidate shape 的 linked list
    for(int i = 0; i < user_num; i++){
        scanf("%d", &temp_ID);
        while(1){
            c = getchar();
            if(c == ' '){
                scanf("%dx%d", &temp_f, &temp_t);
            }
            else if(c == '\n'){
                break;
            }
            new_data = malloc(sizeof(struct shape));
            new_data->ID = temp_ID;
            new_data->freq = temp_f;
            new_data->time = temp_t;
            new_data->later = NULL;
            new_data->prior = NULL;
            if(data_head[i] == NULL){
                data_head[i] = new_data;
            }
            else{
                head = data_head[i];
                //接在尾巴
                while(head->later != NULL)
                    head = head->later;
                head->later = new_data;
                new_data->prior = head;
            }
        }
    }
//    printdata(data_head, user_num);
    //宣告list用來存每個block的time座標，初始為-1
    int accept_num = 0;
    int list_x[user_num], list_y[user_num];
    //初始化
    for(int i = 0; i < user_num; i++){
        list_x[i] = -1;
        list_y[i] = -1;
    }
    int edge_right = rsrc_x, edge_left = 0;
    // check special case : 16 * 1, 8 * 2 put at left
    for(int i = 0; i < user_num; i++){
        struct shape *current = data_head[i];
        while(current != NULL){
            if(current->freq == 16 && current->time == 1 && (edge_right - edge_left >= 1) && current->freq <= rsrc_y){// 16 * 1, set position
                list_x[i] = edge_left++;
                list_y[i] = 0;
                accept_num++;
                data_head[i] = current;
                break;
            }
            else if(current->freq == 8 && current->time == 2 && (edge_right - edge_left >= 2) && current->freq <= rsrc_y){// 16 * 1, set position
                list_x[i] = edge_left++;
                edge_left++;
                list_y[i] = 0;
                accept_num++;
                data_head[i] = current;
                break;
            }
            current = current->later;
        }
    }
    //remember used left space
    int edge_left_fixed = edge_left;

    //剩下case最高4 分區堆疊
    int layer = rsrc_y/height;
    for(int f = 0; f < layer; f++){
        for(int i = f; i < user_num; i += layer){
            //判斷 other case 直接落下
            if( ((edge_right - edge_left) >= data_head[i]->time) && (list_x[i] == -1) && (data_head[i]->freq <= 4)){// accept
                list_x[i] = edge_left;
                edge_left += data_head[i]->time;
                list_y[i] = f*height;
                accept_num++;
            }
            else{// reject
                continue;
            }
        }
        edge_left = edge_left_fixed;
    }
    //用accept_num, list列印結果
    printf("%d\n", accept_num);
    printlist(data_head, user_num, list_x, list_y);
    return 0;
}
