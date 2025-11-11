#include <stdio.h>
#include <stdlib.h>
// mult matx
void mult(int **val1, int **val2, int **result, int k) {
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            result[i][j] = 0;
            for (int l = 0; l < k; l++) {
                result[i][j] += val1[i][l] * val2[l][j];
            }
        }
    }
}
//transfer matx
void tempMax(int **val1, int **val2, int k) {
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            val1[i][j] = val2[i][j];
        }
    }
}
int main(int argc, char *argv[]) {
    if (argc != 2) {
        return 1;
    }
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        return 1;
    }
    int k;
    if (fscanf(file, "%d", &k) != 1 || k <= 0) {
        fclose(file);
        return 1;
    }
    int **tempMaxt = (int **)malloc(k * sizeof(int *));
    int **matrix = (int **)malloc(k * sizeof(int *));
    int **result = (int **)malloc(k * sizeof(int *));

    
    for (int i = 0; i < k; i++) {
        matrix[i] = (int *)malloc(k * sizeof(int));
        result[i] = (int *)malloc(k * sizeof(int));
        tempMaxt[i] = (int *)malloc(k * sizeof(int));
    }
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            if (fscanf(file, "%d", &matrix[i][j]) != 1) {
                fclose(file);
                return 1;
            }
        }
    }
    int n;
    if (fscanf(file, "%d", &n) != 1 || n < 0) {
        
        fclose(file);
        return 1;
    }
    fclose(file); 
    if (n == 0) {
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < k; j++) {
                if(i == j ){
                    result[i][j] =1;
                } else{
                result[i][j] =0;
                }
               
            }
        }
    } else {
        tempMax(result, matrix, k);
        for (int exp = 1; exp < n; exp++) {       
            mult(result, matrix, tempMaxt, k); 
            tempMax(result, tempMaxt, k);      
        }
    }
    // print
     for(int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            if (j != 0) {
                printf(" "); 
            }
            printf("%d", result[i][j]);
        }
        printf("\n");
    }
    // free mem
    for (int i = 0; i < k; i++) {
        free(matrix[i]);
        free(result[i]);
        free(tempMaxt[i]);
    }
     free(result);
    free(matrix);
    free(tempMaxt);

    return 0;
}
