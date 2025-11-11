#include <stdio.h>
#include <stdlib.h>


void multiplyMAT (double *matrix1, double *matrix2, double *result, int rowof1, int col1, int col2){
    for(int i = 0; i< rowof1; i ++){
        for(int j = 0; j< col2; j++) {
            result[i * col2 +j] = 0;
            for(int k = 0; k < col1; k++){
                result[i * col2 +j] += matrix1[i *col1+k] * matrix2[k * col2 +j];
            }
        }
    }
}

void transposeMAT(double *matrix, double *transMax, int row, int col){
    for(int i = 0; i < row; i++){
        for(int j =0; j< col; j++){
            transMax[j*row+i] = matrix[i * col +j];
        }
    }
}

void inverseMAT(double *matrix, int size){
double *idenMat = malloc(size * size * sizeof(double));
    double scal;
//init matrix
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == j) {
                idenMat[i * size + j] = 1.0;
            } else {
                idenMat[i * size + j] = 0.0;
            }
        }
    }
    // gauss elim
    for (int i = 0; i < size; i++) {
        scal = matrix[i * size + i];
        for (int j = 0; j < size; j++) {
            matrix[i * size + j] /= scal;
            idenMat[i * size + j] /= scal;
        }


        for (int k = 0; k < size; k++) {
            if (k != i) {
                double multiplier = matrix[k * size + i];
                for (int j = 0; j < size; j++) {
                    matrix[k * size + j] -= multiplier * matrix[i * size + j];
                    idenMat[k * size + j] -= multiplier * idenMat[i * size + j];
                }
            }
        }
    }


    // Copy the inverse from idenMat to the final matrix
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i * size + j] = idenMat[i * size + j];
        }
    }
    free(idenMat);
}




int main(int argc, char *argv[]){
    if(argc != 3){
        printf("error");
        return 1;
    }
    //read files
    FILE *train = fopen(argv[1], "r");
    FILE *data = fopen(argv[2], "r");
    if (!train || !data){
        return 1;
    }
    int numofAtt;
    int trainRows;
    int inRows;
    fscanf(train, "train\n%d\n%d\n", &numofAtt, &trainRows );


    double *matrixX = (double *)malloc(trainRows * (numofAtt +1) * sizeof(double));
    double *matrixY = (double *)malloc(trainRows * sizeof(double));


    double *xTranspose = (double *)malloc((numofAtt + 1) * trainRows * sizeof(double));
    double *XTX = (double *)malloc((numofAtt + 1) * (numofAtt + 1) * sizeof(double));
    double *XTY = (double *)malloc((numofAtt + 1) * sizeof(double));
    double *weights = (double *)malloc((numofAtt + 1) * sizeof(double));


    for(int i = 0; i<trainRows; i++){
        matrixX[i * (numofAtt +1)] = 1.0;
    }


    // load data train x y numTrainrows numatt (new)
    for(int i = 0; i < trainRows; i++){
        for(int j = 1; j<= numofAtt; j ++){
            fscanf(train, "%lf", &matrixX[i*(numofAtt +1) +j]);
        }
        fscanf(train, "%lf", &matrixY[i]);
    }
    fclose(train);


    transposeMAT(matrixX, xTranspose, trainRows, numofAtt+1);
    multiplyMAT(xTranspose, matrixX, XTX, numofAtt + 1, trainRows, numofAtt +1);


    for(int i = 0; i<= numofAtt; i++){
        XTY[i] = 0;
        for(int j = 0; j < trainRows; j++){
            // new
            XTY[i] += xTranspose[i* trainRows +j] * matrixY[j];
        }
    }
    inverseMAT(XTX, numofAtt+1);

    multiplyMAT(XTX, XTY, weights, numofAtt+1, numofAtt+1, 1);


    fscanf(data, "data\n%d\n%d\n", &numofAtt, &inRows);
    double *inData = (double *)malloc(inRows * (numofAtt +1) *sizeof(double));
    for(int i = 0; i< inRows; i++){
        inData[i*(numofAtt+1)] = 1.0;
        for(int j = 1; j<= numofAtt; j++){
            fscanf(data, "%lf", &inData[i* (numofAtt+1) +j]);
        }
    }
    fclose(data);
    
    // predict price 
    for(int i = 0; i < inRows; i++){
        double predictVal = 0.0;
        for(int j = 0; j <= numofAtt; j++){
            // change
            predictVal += inData[i * ( numofAtt +1) + j] * weights[j];
        }
        printf("%.0f\n", predictVal);
    }
   
    free(matrixX);
    free(matrixY);
    free(xTranspose);
    free(XTX);
    free(XTY);
    free(weights);
    free(inData);
    return 0 ;
}

