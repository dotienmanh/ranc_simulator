#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

struct neuron {
    int conns[256];
    int current_potential;
    int reset_potential;
    int formatted_weight[4];
    int leak;
    int pos_threshold;
    int neg_threshold;
    int reset_mode;
    int spike_des_core[2];
    int spike_des_axon;
    int spike_tick;
};

struct axon {
    int axon_type;
    int weight;
};

struct core{
    struct neuron *neurons;
    struct axon axons[256];
    int max_neurons;
};

int binaryToDecimalSigned(int binaryArray[], int size) {
    int decimalNumber = 0;
    int power = size - 1;

    if (binaryArray[0] == 1) {
        for (int i = 0; i < size; i++) {
            if (binaryArray[i] == 1) {
                binaryArray[i] = 0;
            } else {
                binaryArray[i] = 1;
            }
        }
        int carry = 1;
        for (int i = size - 1; i >= 0; i--) {
            binaryArray[i] += carry;
            if (binaryArray[i] > 1) {
                binaryArray[i] = 0;
                carry = 1;
            } else {
                carry = 0;
            }
        }
        decimalNumber =  binaryToDecimalSigned(binaryArray, size);
        decimalNumber = - decimalNumber;
    } else {
        for (int i = 0; i < size; i++) {
            decimalNumber += binaryArray[i] * pow(2, power);
            power --;
        }
    }

    return decimalNumber;
}

int binaryToDecimalUnsigned(int binaryArray[], int size) {
    int decimalNumber = 0;
    int power = size - 1;
    for (int i = 0; i < size; i++) {
        decimalNumber += binaryArray[i] * pow(2, power);
        power--;
    }

    return decimalNumber;
}

int readCsram(struct core cores[]){
    FILE *f;
    char filename_csram[100];
    for(int i=0;i<5;i++){
        //Đọc CSRAM core1 -> core5
        printf("READ CSRAM.MEM\n");
        snprintf(filename_csram, sizeof(filename_csram), "csram_00%d.mem", i);
        f= fopen(filename_csram,"r");
        if (f==NULL){
            printf("\n invalid file");
            exit(1);
        }
        //if(i==4) {cores[i].max_neurons=250;}
        //else {cores[i].max_neurons=64;}
        for (int j = 0; j < cores[i].max_neurons; j++) {
            char line[368];
            if (fscanf(f, "%s", line) != 1){
                printf("alo"); 
                exit(1);
            }
            else {
                printf("CORE %d NEURON %d\n", i+1, j+1);
                printf("conns ");
                for (int k = 0; k < 256; k++) {
                    cores[i].neurons[j].conns[k] = line[k] - '0';
                    printf("%d", cores[i].neurons[j].conns[k]);
                }
                printf("\n");
                
                int current_potential_temp [9];
                for (int k = 0; k < 9; k++){
                    current_potential_temp[k]=line[k+256] - '0';
                    printf("%d",current_potential_temp[k]);
                }
                printf("\n");
                printf("current_potential %d\n", cores[i].neurons[j].current_potential = binaryToDecimalSigned(current_potential_temp,sizeof(current_potential_temp)/ sizeof(current_potential_temp[0])));
                
                int reset_potential_temp [9];
                for (int k=0; k<9;k++){
                    reset_potential_temp[k]=line[k+265] -'0';
                    printf("%d",reset_potential_temp[k]);
                }
                printf("\n");
                printf("reset_potential %d\n", cores[i].neurons[j].reset_potential = binaryToDecimalSigned(reset_potential_temp,sizeof(reset_potential_temp)/sizeof(reset_potential_temp[0])));

                int formatted_weight_1_temp [9];
                for (int k = 0; k < 9;k++){
                    formatted_weight_1_temp[k]=line[k+274] - '0';
                    printf("%d",formatted_weight_1_temp[k]);
                }
                printf("\n");
                printf("formatted_weight_1 %d\n", cores[i].neurons[j].formatted_weight[0] = binaryToDecimalSigned(formatted_weight_1_temp,sizeof(formatted_weight_1_temp)/sizeof(formatted_weight_1_temp[0])));
                
                int formatted_weight_2_temp [9];
                for (int k = 0; k < 9;k++){
                    formatted_weight_2_temp[k]=line[k+283] - '0';
                    printf("%d",formatted_weight_2_temp[k]);
                }
                printf("\n");
                printf("formatted_weight_2 %d\n", cores[i].neurons[j].formatted_weight[1] = binaryToDecimalSigned(formatted_weight_2_temp,sizeof(formatted_weight_2_temp)/sizeof(formatted_weight_2_temp[0])));
                
                int formatted_weight_3_temp [9];
                for (int k = 0; k < 9;k++){
                    formatted_weight_3_temp[k]=line[k+292] - '0';
                    printf("%d",formatted_weight_3_temp[k]);
                }
                printf("\n");
                printf("formatted_weight_3 %d\n", cores[i].neurons[j].formatted_weight[2] = binaryToDecimalSigned(formatted_weight_3_temp,sizeof(formatted_weight_3_temp)/sizeof(formatted_weight_3_temp[0])));

                int formatted_weight_4_temp [9];
                for (int k = 0; k < 9;k++){
                    formatted_weight_4_temp[k]=line[k+301] - '0';
                    printf("%d",formatted_weight_4_temp[k]);
                }
                printf("\n");
                printf("formatted_weight_4 %d\n", cores[i].neurons[j].formatted_weight[3] = binaryToDecimalSigned(formatted_weight_4_temp,sizeof(formatted_weight_4_temp)/sizeof(formatted_weight_2_temp[0])));
                
                int leak_temp [9];
                for (int k = 0; k < 9;k++){
                    leak_temp[k]=line[k+310] - '0';
                    printf("%d",leak_temp[k]);
                }
                printf("\n");
                printf("leak %d\n", cores[i].neurons[j].leak = binaryToDecimalSigned(leak_temp,sizeof(leak_temp)/sizeof(leak_temp[0])));

                int pos_threshold_temp [9];
                for (int k = 0; k < 9;k++){
                    pos_threshold_temp[k]=line[k+319] - '0';
                    printf("%d",pos_threshold_temp[k]);
                }
                printf("\n");
                printf("pos_threshold %d\n", cores[i].neurons[j].pos_threshold = binaryToDecimalUnsigned(pos_threshold_temp,sizeof(pos_threshold_temp)/sizeof(pos_threshold_temp[0])));
                
                int neg_threshold_temp [9];
                for (int k = 0; k < 9;k++){
                    neg_threshold_temp[k]=line[k+328] - '0';
                    printf("%d",neg_threshold_temp[k]);
                }
                printf("\n");
                printf("neg_threshold %d\n", cores[i].neurons[j].neg_threshold = binaryToDecimalSigned(neg_threshold_temp,sizeof(neg_threshold_temp)/sizeof(neg_threshold_temp[0])));
                
                printf("reset_mode %d\n", cores[i].neurons[j].reset_mode = line[337] - '0');

                int spike_des_core_x_temp [9];
                for (int k = 0; k < 9;k++){
                    spike_des_core_x_temp[k]=line[k+338] - '0';
                    printf("%d",spike_des_core_x_temp[k]);
                }
                printf("\n");
                printf("spike_des_core_x %d\n", cores[i].neurons[j].spike_des_core[0] = binaryToDecimalSigned(spike_des_core_x_temp,sizeof(spike_des_core_x_temp)/sizeof(spike_des_core_x_temp[0])));
                
                int spike_des_core_y_temp [9];
                for (int k = 0; k < 9;k++){
                    spike_des_core_y_temp[k]=line[k+347] - '0';
                    printf("%d",spike_des_core_y_temp[k]);
                }
                printf("\n");
                printf("spike_des_core_y %d\n", cores[i].neurons[j].spike_des_core[1] = binaryToDecimalSigned(spike_des_core_y_temp,sizeof(spike_des_core_y_temp)/sizeof(spike_des_core_y_temp[0])));
                
                int spike_des_axon_temp [8];
                for (int k = 0; k < 8;k++){
                    spike_des_axon_temp[k]=line[k+356] - '0';
                    printf("%d",spike_des_axon_temp[k]);
                }
                printf("\n");
                printf("spike_des_axon %d\n", cores[i].neurons[j].spike_des_axon = binaryToDecimalUnsigned(spike_des_axon_temp,sizeof(spike_des_axon_temp)/sizeof(spike_des_axon_temp[0])));
                
                int spike_tick_temp [4];
                for (int k = 0; k < 4;k++){
                    spike_tick_temp[k]=line[k+364] - '0';
                    printf("%d",spike_tick_temp[k]);
                }
                printf("\n");
                printf("spike_tick %d\n", cores[i].neurons[j].spike_tick = binaryToDecimalUnsigned(spike_tick_temp,sizeof(spike_tick_temp)/sizeof(spike_tick_temp[0])));   
            }
        }
        fclose(f);
    }
}

int readTC(struct core cores[]){
    char filename_tc[100];
    FILE *f;
    for(int i=0;i<5;i++){
        //Đọc TC core1->core5
        printf("READ TC.MEM\n");
        snprintf(filename_tc, sizeof(filename_tc), "tc_00%d.mem", i);
        f= fopen(filename_tc,"r");
        if (f==NULL){
            printf("\n invalid file");
            break;
        }
        for (int j=0;j<256;j++){
            printf("CORE %d AXON %d\n",i+1,j+1);
            char line[2];
            if (fscanf(f, "%s", line) != 1) break;
            int axon_type_temp [2];
                for (int k = 0; k < 2;k++){
                    axon_type_temp[k]=line[k] - '0';
                    printf("%d",axon_type_temp[k]);
                }
                printf("\n");
            printf("axon type: %d\n",cores[i].axons[j].axon_type=binaryToDecimalSigned(axon_type_temp,sizeof(axon_type_temp)/sizeof(axon_type_temp[0])));
            printf("axon_weight: %d\n",cores[i].axons[j].weight=cores[i].neurons[0].formatted_weight[cores[i].axons[j].axon_type]);
        }
        fclose(f);
    }
}

int save_to_file(struct core cores[]){
    FILE *f;
    char filename_write[100];
    char filename_tc[100];
    for(int i=0;i<5;i++){
        snprintf(filename_write, sizeof(filename_write), "neuron_core_00%d.txt", i);
        f= fopen(filename_write,"w");

        //if(i==4) {cores[i].max_neurons=250;}
        //else {cores[i].max_neurons=64;}
        for (int j = 0; j < cores[i].max_neurons; j++) {
            fprintf(f,"\n");
            fprintf(f,"CORE %d NEURON %d\n", i+1, j+1);
            fprintf(f,"conns ");
            for (int k = 0; k < 256; k++) {
                fprintf(f,"%d", cores[i].neurons[j].conns[k]);
            }
            fprintf(f,"\n");
            fprintf(f,"current_potential %d\n", cores[i].neurons[j].current_potential);
            fprintf(f,"reset_potential %d\n", cores[i].neurons[j].reset_potential);
            fprintf(f,"formatted_weight_1 %d\n", cores[i].neurons[j].formatted_weight[0]);
            fprintf(f,"formatted_weight_2 %d\n", cores[i].neurons[j].formatted_weight[1]);
            fprintf(f,"formatted_weight_3 %d\n", cores[i].neurons[j].formatted_weight[2]);
            fprintf(f,"formatted_weight_4 %d\n", cores[i].neurons[j].formatted_weight[3]);
            fprintf(f,"leak %d\n", cores[i].neurons[j].leak);
            fprintf(f,"pos_threshold %d\n", cores[i].neurons[j].pos_threshold);
            fprintf(f,"neg_threshold %d\n", cores[i].neurons[j].neg_threshold);
            fprintf(f,"reset_mode %d\n", cores[i].neurons[j].reset_mode);
            fprintf(f,"spike_des_core_x %d\n", cores[i].neurons[j].spike_des_core[0]);
            fprintf(f,"spike_des_core_y %d\n", cores[i].neurons[j].spike_des_core[1]);
            fprintf(f,"spike_des_axon %d\n", cores[i].neurons[j].spike_des_axon);
            fprintf(f,"spike_tick %d\n", cores[i].neurons[j].spike_tick);
        }
        fclose(f);
        snprintf(filename_tc, sizeof(filename_tc), "axon__core_00%d.txt", i);
        f= fopen(filename_tc,"w");
        for (int j=0;j<256;j++){
            fprintf(f,"CORE %d AXON %d\n",i+1,j+1);
            fprintf(f,"axon type: %d\n",cores[i].axons[j].axon_type);
            fprintf(f,"axon_weight: %d\n",cores[i].axons[j].weight=cores[i].neurons[0].formatted_weight[cores[i].axons[j].axon_type]);
            fprintf(f," \n");
        }
        fclose(f);
    }
}
int main() {
    struct core cores[5];
    for (int i=0; i<4; ++i){
        cores[i].neurons = malloc(64 * sizeof(struct neuron));
        cores[i].max_neurons = 64;
    }
    cores[4].neurons = malloc(250 * sizeof(struct neuron));
    cores[4].max_neurons = 250;
    readCsram(cores);
    readTC(cores);
    save_to_file(cores);
    return 0;
}
