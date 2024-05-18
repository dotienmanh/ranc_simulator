#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

void get_network_accuracy(){
    double max_mnist=10000;
    FILE *f_get_predict;
    f_get_predict=fopen("./predict/predict.txt","r");
    if (f_get_predict == NULL) {
        printf("Error opening file predict.txt\n");
        exit(1);
    }
    FILE *f_get_correct;
    f_get_correct=fopen("./testbench/tb_correct.txt","r");
    if (f_get_correct == NULL) {
        printf("Error opening file tb_correct.txt\n");
        exit(1);
    }
    double mnist_correct=0;
    int max[10]={0,0,0,0,0,0,0,0,0,0};
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
            max[predict]+=1;
        }
    }

    printf("Network Accuracy: %.2f ",mnist_correct/max_mnist *100);

    for(int i=0;i<10;i++){
        printf("network predict for class %d : %d\n",i,max[i]);
    }

    fclose(f_get_correct);
    fclose(f_get_predict);
}