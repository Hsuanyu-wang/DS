#include<stdio.h>
#include<stdlib.h>
//�ŧi struct shape
struct shape{
    int ID;
    int freq;
    int time;
    struct shape *prior;
    struct shape *later;
};
//print �Ψ�debug��
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
    //c  �ΨӦs���@��char���F�᭱if�P�_��
    char c;
    //rsrc = resource
    int rsrc_y, rsrc_x, user_num;
    //Ūinput
    scanf("%d %d %d", &rsrc_y, &rsrc_x, &user_num);
    //�ŧi�@�ӫ��а}�C�ǳƦs�����(�C��ID��head)
    struct shape *data_head[user_num];
    for(int i = 0; i < user_num; i++){
        data_head[i] = NULL;
    }
    //�ŧi�|�Ψ쪺�ܼ�
    int temp_ID, temp_f, temp_t;
    struct shape *head = NULL;
    struct shape *new_data = NULL;
    //Ū��ơA�q�C��id�����а}�C�� candidate shape �� linked list
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
                //���b����
                while(head->later != NULL)
                    head = head->later;
                head->later = new_data;
                new_data->prior = head;
            }
        }
    }
//    //�C�L�s�������(degub��)
//    printdata(data_head, user_num);
    //�ŧilist�ΨӦs�C��block��time�y�СA��l��-1
    int sum = 0, temp, x = 0, y, accept_num = 0, list[user_num];
    for(int i = 0; i < user_num; i++){
        list[i] = -1;
    }
    //�P�_x, y��V���L�W�X�d��
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
    //��accept_num, list�C�L���G
    printf("%d\n", accept_num);
    for(int i = 0; i < user_num; i++){
        if(list[i] >= 0){
            printf("%d %dx%d 0 %d\n", data_head[i]->ID, data_head[i]->freq, data_head[i]->time, list[i]);
        }
    }
    return 0;
}
