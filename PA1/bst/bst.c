#include <stdio.h>
#include <stdlib.h>


struct Node {
    int value;
    struct Node *left;
    struct Node *right; 
};
struct Node* createNode(int value){
    struct Node *newNode = (struct Node*) malloc (sizeof(struct Node));
    newNode -> value = value; 
    newNode -> left = NULL;
    newNode -> right = NULL;
    return newNode;
}
struct Node* inserts(struct Node *root, int value, int *insert){
    if(root == NULL){
        *insert = 1;
        return createNode(value);
    }
     if (value < root->value) {
        root->left = inserts(root->left, value, insert);
    } else if (value > root->value) {
        root->right = inserts(root->right, value, insert);
    } else {
        *insert = 0; // Value already present
    }

    return root; 
}
int search(struct Node *root, int value){
    if(root == NULL){
        return 0;
    }
    if(root->value == value){
        return 1; 
    }
    if(value < root->value){
        return search(root->left, value); 
    }
    return search (root->right, value);
}
 struct Node* findMax(struct Node *root){
    while(root->right !=NULL){
        root = root->right;
    }
    return root; 
}
// if error change <> signs 
struct Node* deletes(struct Node *root, int value, int *delete){
    if(root == NULL){
        *delete = 0;
        return root;
    }
    if(value > root->value){
        root->right = deletes(root->right, value,delete);
    } else if(value < root->value){
        root->left = deletes(root->left, value, delete);
    } else{
        *delete =1;
        if(root->right == NULL){
            struct Node *tempNode = root->left;
            free(root);
            return tempNode;
        } else if(root ->left == NULL){
            struct Node *tempNode = root->right;
            free(root);
            return tempNode;
        }
        struct Node *tempNode = findMax(root->left);
        root ->value = tempNode ->value;
        root -> left = deletes(root ->left, tempNode ->value, delete);
    }
    return root;
}



void print(struct Node*root){
    if(root == NULL){
        return; 
    }
    printf("(");
    print(root->left);
    printf("%d", root->value);
    print(root->right);
    printf(")");
}
void freeTree(struct Node *root){
    if(root == NULL){
        return; 
    }
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}
int main(){
    struct Node*root = NULL;
    char com;
    int value;
    int insert;
    int found;
    int deleted;


    while(scanf(" %c", &com) !=EOF){
        switch(com){
            case'i':
            scanf("%d", &value);
            insert = 0;
            root = inserts(root,value, &insert);
            if(insert){
                printf("inserted\n");
            } else {
                printf("not inserted\n");
            }
            break;
            case 's':
                
                scanf("%d", &value);
                found = search(root, value);
                if (found)
                    printf("present\n");
                else
                    printf("absent\n");
                break;
            case 'd':
                // Delete command
                scanf("%d", &value);
                deleted = 0;
                root = deletes(root, value, &deleted);
                if (deleted)
                    printf("deleted\n");
                else
                    printf("absent\n");
                break;




            case 'p':
                // Print command
                print(root);
                printf("\n");
                break;
            default:
                break;
        }
    }
    freeTree(root);
    return 0;


}