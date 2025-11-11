#include <stdlib.h>
#include <stdio.h>
typedef struct Node {
    int info;
    struct Node *next;
} Node;


Node* createNode(int data) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->info = data;
    newNode->next = NULL;
    return newNode;
}


Node* insert(Node *head, int data) {
    Node *newNode = createNode(data);
    if (!head ||(data < head->info) ) {
        newNode->next = head;
        return newNode;
    }
    Node *curNode = head;
    while (curNode->next && curNode->next->info < data) {
        curNode = curNode->next;
    }
    if ( (curNode->next && curNode->next->info == data) || curNode->info == data ) {
        free(newNode);
        return head;
    }
    newNode->next = curNode->next;
    curNode->next = newNode;
    return head;
}


Node* delete(Node *head, int val) {
    // nothing in head
    if (!head) {
        return NULL;
    }
    if (head->info == val) {
        Node *ptr = head;
        head = head->next;
        free(ptr);
        return head;
    }
    Node *current = head;
    while (current->next && current->next->info != val) {
        current = current->next;
    }
    if (current->next && current->next->info == val) {
        Node *ptr = current->next;
        current->next = current->next->next;
        free(ptr);
    }
    return head;
}
void printList(Node *head) {
    Node *curNode = head;
    int count = 0;
    while (curNode) {
        count++;
        curNode = curNode->next;
    }
    printf("%d :", count);
    curNode = head;
    while (curNode) {
        printf(" %d", curNode->info);
        curNode = curNode->next;
    }
    printf("\n");
}
int main() {
    char in;
    int value;
    Node *head = NULL;

    while (scanf(" %c %d", &in, &value) != EOF) {
        if (in == 'i') {
            head = insert(head, value);
        } else if (in == 'd') {
            head = delete(head, value);
        }
        printList(head);
    }

    while (head) {
        Node *ptr = head;
        head = head->next;
        free(ptr);
    }

    return 0;
}