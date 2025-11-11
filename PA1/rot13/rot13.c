#include <stdio.h>
#include <ctype.h>
//compile: gcc rot13.c -o rot13
// run: ./rot1cdd
// steps needed, iterate throught string
// check if its a letter and if it is lower or upper case
// change leter value
// check to make sure it is only taking one argument and not two.
void rot13(const char *str){
    while(*str){
        char letter = *str;
        if(isalpha(letter)){
            if(isupper(letter)){
                putchar(((letter - 'A' + 13) % 26) + 'A');
            } else{
                putchar(((letter-'a' + 13) % 26 ) + 'a');
			}
        } else{
          putchar(letter);  
        }
        str++;
    }
    putchar('\n');
}
int main(int argc, char *argv[]){
    rot13(argv[1]);
    return 0;
}
