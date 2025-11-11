#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct stack {
char *info ;
int ceiling;
int sizeOfStack;
} Stack;
Stack makeAStack(int sizeofStack){
    Stack newStack;
    newStack.info = (char*)malloc(sizeofStack * sizeof(char));
    newStack.sizeOfStack = sizeofStack; 
    newStack.ceiling = -1; 
    return newStack;
}
void freeStack(Stack *curStack){
    free(curStack->info);

}
char pop(Stack* curStack){
    if(curStack->ceiling >=0){
        return curStack->info[(curStack->ceiling)--]; 
    }
    return '\0';
}
void push(Stack *curStack, char symbol){
    if(curStack->ceiling == curStack->sizeOfStack -1){
        curStack->sizeOfStack *=2;
        curStack -> info = (char*)realloc(curStack->info, curStack->sizeOfStack * sizeof(char));
    }
    curStack->info[++(curStack->ceiling)] = symbol; 
}
int balance(const char *input) {
    int inputLen = strlen(input);
    Stack stack = makeAStack(inputLen);  


    for (int i = 0; i < inputLen; i++) {
        char ch = input[i];


        if (ch != '(' && ch != ')' && ch != '[' && ch != ']' && ch != '{' && ch != '}') {
            continue;
        }


        if (ch == '(' || ch == '[' || ch == '{') {
            push(&stack, ch);
        } else {
            if (stack.ceiling == -1) {
                printf("%d: %c\n", i, ch);
                freeStack(&stack);
                return EXIT_FAILURE;
            }
            char open = pop(&stack);
            if (!((open == '(' && ch == ')') || (open == '[' && ch == ']') || (open == '{' && ch == '}'))) {
                printf("%d: %c\n", i, ch);
                freeStack(&stack);  
                return EXIT_FAILURE;
            }
        }
    }


    if (stack.ceiling != -1) {
        printf("open: ");
        while (stack.ceiling != -1) {
            char open = pop(&stack);
            if (open == '(') printf(")");
            if (open == '[') printf("]");
            if (open == '{') printf("}");
        }
        printf("\n");
        freeStack(&stack);  
        return EXIT_FAILURE;
    }


    freeStack(&stack);  
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return EXIT_FAILURE;
    }


    return balance(argv[1]);
}

