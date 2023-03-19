#include<stdio.h>
#include<stdlib.h>
//宣告 struct shape
struct shape{
    int ID;
    int freq;
    int time;
    struct shape *prior;
    struct shape *later;
};
//print 用來debug的
void printdata(struct shape *array[], int MAX){
    for(int i = 0; i < MAX; i++){
        printf("%d ", array[i]->ID);
        for(struct shape *cur = array[i]; cur != NULL; cur = cur->later){
            printf("%dx%d ", cur->freq, cur->time);
        }
        printf("\n");
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
//    //列印存取的資料(degub用)
//    printdata(data_head, user_num);
    //宣告list用來存每個block的time座標，初始為-1
    int sum = 0, temp, x = 0, y, accept_num = 0, list[user_num];
    for(int i = 0; i < user_num; i++){
        list[i] = -1;
    }
    //判斷x, y方向有無超出範圍
    for(int i = 0; i < user_num; i++){
        y = data_head[i]->freq;
        temp = sum + data_head[i]->time;
        if(temp <= rsrc_x && y <= rsrc_y){
            list[i] = x;
            x += data_head[i]->time;
            sum += data_head[i]->time;
            accept_num++;
        }
        else{
            continue;
        }
    }
    //用accept_num, list列印結果
    printf("%d\n", accept_num);
    for(int i = 0; i < user_num; i++){
        if(list[i] >= 0){
            printf("%d %dx%d 0 %d\n", data_head[i]->ID, data_head[i]->freq, data_head[i]->time, list[i]);
        }
    }
    return 0;
}
