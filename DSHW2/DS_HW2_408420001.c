#include<stdio.h>
#include<stdlib.h>
#define height 4
//�ŧi struct shape
struct shape{
    int ID;
    int freq;
    int time;
    struct shape *prior;
    struct shape *later;
};
////print �Ψ�debug��
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
//    printdata(data_head, user_num);
    //�ŧilist�ΨӦs�C��block��time�y�СA��l��-1
    int accept_num = 0;
    int list_x[user_num], list_y[user_num];
    //��l��
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

    //�ѤUcase�̰�4 ���ϰ��|
    int layer = rsrc_y/height;
    for(int f = 0; f < layer; f++){
        for(int i = f; i < user_num; i += layer){
            //�P�_ other case �������U
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
    //��accept_num, list�C�L���G
    printf("%d\n", accept_num);
    printlist(data_head, user_num, list_x, list_y);
    return 0;
}
