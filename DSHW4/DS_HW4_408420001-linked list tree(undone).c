#include <stdio.h>
#include <math.h>

struct node{
    struct node* left;
    struct node* right;
    struct node* parent;
    int level;
    int order;
    int ID;
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

void insert(struct node* binary_tree) {
    struct node *tmp = NULL;
    if(!(binary_tree)) {
        tmp = (struct node *)malloc(sizeof(struct node));
        tmp->left = tmp->right = NULL;
        tmp->data = 0;
        binary_tree = tmp;
        return;
    }
}

//主程式
int main(){
    //宣告 % 讀取str_num, strlen_max;
    int str_num, strlen_max;
    scanf("%d %d", &str_num, &strlen_max);
    int temp = str_num, count = 1, layer;
    char string[strlen_max];
    unsigned long str_array[str_num];
    //計算層數
    while(temp != 1){
        temp /= 2;
        count++;
    }
    layer = count - 1;
    //讀取最下層資料
    for(int i = 0; i < str_num; i++){
        scanf("%s", string);
        printf("string[%d] : %s\n", i, string);
        str_array[i] = MurmurOAAT32(&string);
        printf("data : %lu\n", str_array[i]);
    }
    //建立root
    struct node* root;
    root = malloc(sizeof(struct node));
    root->level = 0, root->order = 0, root->data = 0, root->ID = 0;
    root->parent = NULL, root->left = NULL, root->right = NULL;
    printf("root:\ndata : %lu\n", root->data);
    printf("ID : %d\n", root->ID);
    //建立空樹
    struct node* current;
    current = root;

    for(int lev = 1; lev <= layer; lev--){
        for(int ord = 1; ord <= pow(2, (double)lev); ord++){
//            if(str_array[ord+1] == '\0'){
//                printf("lonely\n");
//            }
//            else{
                struct node* new_node;
                new_node = malloc(sizeof(struct node));
                new_node->level = lev;
                new_node->order = ord;
                new_node->parent = current;
                new_node->left = NULL;
                new_node->right = NULL;
                new_node->data = 0;
                printf("data : %lu\n", new_node->data);
                new_node->ID = pow(2, (double)(lev-1)) + ord;
                printf("ID : %d\n", new_node->ID);
//            }
                current->left = new_node;


        }
    }
    for(int lev = layer-1; lev >= 0; lev--){

    }
//    count = 0;
//    while(count < str_num){
//        struct node* current;
//        current = malloc(sizeof(struct node));
//        //current->level = ;
//        //current->order = ;
//        current->parent = NULL;
//
//        current->left_child = NULL;
//        current->right_child = NULL;
//
//        current->data = MurmurOAAT32(&string);
//        printf("data : %lu\n", current->data);
//        current->ID = i;
//        printf("ID : %d\n", current->ID);
//
//    }

	return 0;
}
