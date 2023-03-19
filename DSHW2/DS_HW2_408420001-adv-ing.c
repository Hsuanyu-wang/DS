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
    int curr_x = 0, curr_y = 0, accept_num = 0;
    int list_x[user_num], list_y[user_num];
    //edge�O�i���{��
    int edge_top = rsrc_y, edge_down = 0, edge_right = rsrc_x, edge_left = 0;
    //��l��
    for(int i = 0; i < user_num; i++){
        list_x[i] = list_y[i] = -1;
    }
    //�P�_x, y��V���L�W�X�d��
    for(int i = 0; i < user_num; i++){
        // check special case : 1 * 16, 16 * 1
        struct shape *current = data_head[i];
        while(current != NULL){
            if(current->freq == 16 && (edge_right-1) >= edge_left && edge_right > (rsrc_x/4*3)){// 16 * 1, set position
                list_x[i] = --edge_right;
                list_y[i] = 0;
                accept_num++;
                data_head[i] = current;
                break;
            }
            else if(current->freq == 8 && (edge_right-1) >= edge_left && edge_right > (rsrc_x/4*3)){// 16 * 1, set position
                list_x[i] = --(--edge_right);
                list_y[i] = 0;
                accept_num++;
                data_head[i] = current;
                break;
            }
            else if(current->time == 1 && (edge_top-1) >= edge_down && edge_top > (rsrc_y/2)){// 1 * 16, set position
                list_x[i] = 0;
                list_y[i] = --edge_top;
                accept_num++;
                data_head[i] = current;
                break;
            }
            else{
                current = current->later;
            }
        }
        //�P�_ other case �������U
        if(data_head[i]->time <= edge_right && data_head[i]->freq <= edge_top && list_x[i] == -1){// accept
            list_x[i] = edge_left;
            list_y[i] = 0;
            edge_down += data_head[i]->freq;
            edge_left += data_head[i]->time;
            accept_num++;
        }
        else{// reject
            continue;
        }
    }
    //��accept_num, list�C�L���G
    printf("%d\n", accept_num);
    for(int i = 0; i < user_num; i++){
        if(list_x[i] >= 0){
            printf("%d %dx%d %d %d\n", data_head[i]->ID, data_head[i]->freq, data_head[i]->time, list_y[i], list_x[i]);
        }
    }
    return 0;
}
