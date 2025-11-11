#include <stdio.h> 
#include <stdlib.h>
#include <string.h>


// compare has to work with any datra
int comp(const void *item1, const void *item2){
    const char **str1 = (const char **)item1;
    const char **str2 = (const char **)item2;
    
    return strcmp(*str1,*str2); 
}

int main(int argc, char *argv[]) {
    //checks if it is 0 arguements 
    if(argc<= 1){
        return 0; 
    }
    
    //qsort(array, num of elements, sizeof(char) or number of bites, )
    qsort(argv+1, argc-1, sizeof(char *), comp);

    //prints sorted word
    for(int i = 1; i < argc; i++){
        printf("%s\n", argv[i]);
    }

    return 0;
}