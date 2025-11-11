#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    AND,
    OR,
    NAND,
    NOR,
    XOR,
    NOT,
    PASS,
    DECODER,
    MULTIPLEXER
} typeOfGate;

typedef struct {
    typeOfGate type;
    int size;
    int input[100];
    int outputs[100];
} Gate;

typedef struct {
    char name[17];
    int index;
} Var;

Var vars[100];
int numofVars = 0;
Gate gates[100];
int numOfGates = 0;

int inStart;
int inCount;
int outStart;
int outCount;

int getVar(const char *name) {
    for (int i = 0; i < numofVars; i++) {
        if (strcmp(vars[i].name, name) == 0) {
            return vars[i].index;
        }
    }
    // Add another var
    strcpy(vars[numofVars].name, name);
    vars[numofVars].index = numofVars;
    return numofVars++;
}

void evalRow(int *vals) {
    for (int i = 0; i < numOfGates; i++) {
        Gate *gate = &gates[i];
        switch (gate->type) {
            case AND:
                vals[gate->outputs[0]] = vals[gate->input[0]] & vals[gate->input[1]];
                break;
            case OR:
                vals[gate->outputs[0]] = vals[gate->input[0]] | vals[gate->input[1]];
                break;
            case NAND:
                vals[gate->outputs[0]] = !(vals[gate->input[0]] & vals[gate->input[1]]);
                break;
            case NOR:
                vals[gate->outputs[0]] = !(vals[gate->input[0]] | vals[gate->input[1]]);
                break;
            case XOR:
                vals[gate->outputs[0]] = vals[gate->input[0]] ^ vals[gate->input[1]];
                break;
            case NOT:
                vals[gate->outputs[0]] = !vals[gate->input[0]];
                break;
            case PASS:
                vals[gate->outputs[0]] = vals[gate->input[0]];
                break;
            case DECODER: {
                int in = 0;
                for (int j = 0; j < gate->size; j++) {
                    in = (in << 1) | vals[gate->input[j]];
                }
                for (int k = 0; k < (1 << gate->size); k++) {
                    if(k == in){
                       vals[gate->outputs[k]] = 1;
                    } else {
                        vals[gate->outputs[k]] = 0;
                    }
                }
                break;
            }
            case MULTIPLEXER: {
                int numIn = 1 << gate->size;
                int select = 0;
                for (int j = 0; j < gate->size; j++) {
                    select = (select << 1) | vals[gate->input[numIn + j]];
                }
                if (select < numIn) {
                    vals[gate->outputs[0]] = vals[gate->input[select]];
                } else {
                    exit(1);
                }
                break;
            }
        }
    }
}

void parseFile(const char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (!file) {
        exit(1);
    }

    char directive[17];
    while (fscanf(file, " %16s", directive) != EOF) {
        if (strcmp(directive, "INPUT") == 0) {
            fscanf(file, "%d", &inCount);
            inStart = numofVars;
            for (int i = 0; i < inCount; i++) {
                char name[17];
                fscanf(file, " %16s", name);
                getVar(name);
            }
        } else if (strcmp(directive, "OUTPUT") == 0) {
            fscanf(file, "%d", &outCount);
            outStart = numofVars;
            for (int i = 0; i < outCount; i++) {
                char name[17];
                fscanf(file, " %16s", name);
                getVar(name);
            }
        } else {
            Gate gate = {0};
            if (strcmp(directive, "DECODER") == 0) {
                gate.type = DECODER;
                fscanf(file, "%d", &gate.size);
                for (int i = 0; i < gate.size; i++) {
                    char name[17];
                    fscanf(file, " %16s", name);
                    gate.input[i] = getVar(name);
                }
                for (int i = 0; i < (1 << gate.size); i++) {
                    char name[17];
                    fscanf(file, " %16s", name);
                    gate.outputs[i] = getVar(name);
                }
            } else if (strcmp(directive, "MULTIPLEXER") == 0) {
                gate.type = MULTIPLEXER;
                fscanf(file, "%d", &gate.size);
                int numIN = 1 << gate.size;
                for (int i = 0; i < numIN; i++) {
                    char name[17];
                    fscanf(file, " %16s", name);
                    gate.input[i] = getVar(name);
                }
                for (int i = 0; i < gate.size; i++) {
                    char name[17];
                    fscanf(file, " %16s", name);
                    gate.input[numIN + i] = getVar(name);
                }
                char out[17];
                fscanf(file, " %16s", out);
                gate.outputs[0] = getVar(out);
            } else {
                char in1[17], in2[17], out[17];
                if (strcmp(directive, "AND") == 0) {
                    gate.type = AND;
                } else if (strcmp(directive, "OR") == 0) {
                    gate.type = OR;
                } else if (strcmp(directive, "NAND") == 0) {
                    gate.type = NAND;
                } else if (strcmp(directive, "NOR") == 0) {
                    gate.type = NOR;
                } else if (strcmp(directive, "XOR") == 0) {
                    gate.type = XOR;
                } else if (strcmp(directive, "NOT") == 0) {
                    gate.type = NOT;
                    fscanf(file, " %16s %16s", in1, out);
                    gate.input[0] = getVar(in1);
                    gate.outputs[0] = getVar(out);
                    gates[numOfGates++] = gate;
                    continue;
                }
                fscanf(file, " %16s %16s %16s", in1, in2, out);
                gate.input[0] = getVar(in1);
                gate.input[1] = getVar(in2);
                gate.outputs[0] = getVar(out);
            }
            gates[numOfGates++] = gate;
        }
    }
    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        return 1;
    }

    parseFile(argv[1]);

    int totalRows = 1 << inCount;
    int vals[100] = {0};

    for (int row = 0; row < totalRows; row++) {
        for (int col = 0; col < inCount; col++) {
            vals[inStart + col] = (row >> (inCount - col - 1)) & 1;
        }

        evalRow(vals);

        for (int i = 0; i < inCount; i++) {
            printf("%d", vals[inStart + i]);
            if (i < inCount - 1) {
                printf(" ");
            }
        }
        printf(" | ");
        for (int i = 0; i < outCount; i++) {
            printf("%d", vals[outStart + i]);
            if (i < outCount - 1) {
                printf(" ");
            }
        }
        printf("\n");
    }

    return 0;
}
