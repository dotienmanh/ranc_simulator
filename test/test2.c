#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>


int binaryToDecimalUnsigned(int binaryArray[], int size) {
    int decimalNumber = 0;
    int power = size - 1;
    for (int core = 0; core < size; core++) {
        decimalNumber += binaryArray[core] * pow(2, power);
        power--;
    }

    return decimalNumber;
}


int main(){
    double max_mnist=10000;
    FILE *f_get_predict;
    f_get_predict=fopen("predict.txt","r");
    if (f_get_predict == NULL) {
        printf("Error opening file predict.txt\n");
        exit(1);
    }
    FILE *f_get_correct;
    f_get_correct=fopen("tb_correct.txt","r");
    if (f_get_correct == NULL) {
        printf("Error opening file tb_correct.txt\n");
        exit(1);
    }
    double mnist_correct=0;
    for(int i=0;i<max_mnist;i++){
        char predict_temp[1];
        fscanf(f_get_predict,"%d",predict_temp);
        // for(int i=0;i<1;i++){
        //     printf("%d",predict_temp[i]);
        // }
        int predict=predict_temp[0];
        //printf(" predict: %d",predict);
        char line[8];
        fscanf(f_get_correct,"%s",&line);
        int correct_temp[8];
        for(int k=0;k<8;k++){
            correct_temp[k]=line[k]-'0';
        }
        int correct = binaryToDecimalUnsigned(correct_temp,sizeof(correct_temp)/sizeof(correct_temp[0]));
        //printf(" correct:%d\n",correct);

        if(predict == correct){
            mnist_correct += 1;
        }
    }
    // printf("%d",mnist_correct);
    printf("Network Accuracy: %.2f ",mnist_correct/max_mnist *100);

    fclose(f_get_correct);
    fclose(f_get_predict);
}