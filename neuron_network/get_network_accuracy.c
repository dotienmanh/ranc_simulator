#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include "param.c"

/*** Tính phần trăm đúng của network ***/
void get_network_accuracy(){
    FILE *f_get_predict;
    char file_get_predict[100]=PATH"/predict/predict.txt";
    f_get_predict=fopen(PATH"/predict/predict.txt","r");
    if (f_get_predict == NULL) {
        printf("Error opening file predict.txt\n");
        exit(1);
    }
    FILE *f_get_correct;
    char file_get_correct[100]=PATH"/testbench/tb_correct.txt";
    f_get_correct=fopen(file_get_correct,"r");
    if (f_get_correct == NULL) {
        printf("Error opening file tb_correct.txt\n");
        exit(1);
    }
    char file_get_debug[100]=PATH"/predict/debug.txt";
    FILE *f_get_debug=fopen(file_get_debug,"w");
    int debug[11][11] = {0};
    double mnist_correct=0;
    for(int i=0;i<max_mnist;i++){
        char predict_temp[1];
        fscanf(f_get_predict,"%d",predict_temp);

        int predict=predict_temp[0];

        char line[8];
        fscanf(f_get_correct,"%s",&line);
        int correct_temp[8];
        for(int k=0;k<8;k++){
            correct_temp[k]=line[k]-'0';
        }
        int correct = binaryToDecimalUnsigned(correct_temp,sizeof(correct_temp)/sizeof(correct_temp[0]));

        if(predict == correct){
            mnist_correct += 1;
        }
        else {
            debug[correct][predict] +=1;
        }
    }
    if(save_to_file){
        printf("Network Accuracy: %.2f \n",mnist_correct/max_mnist *100);
        for(int i=0;i<10;i++){
            fprintf(f_get_debug,"So %d bi nham voi: \n",i);
            for(int j=0;j<10;j++){
                fprintf(f_get_debug,"       So %d: %d lan\n",j,debug[i][j]);
            }
            fprintf(f_get_debug,"-----------------------------\n");
        }
    }
    fclose(f_get_correct);
    fclose(f_get_predict);
    fclose(f_get_debug);
}