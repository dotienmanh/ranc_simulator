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

void readCsram(struct core cores[]){
    FILE *f;
    char filename_csram[100];
    for(int i=0;i<5;i++){
        //Đọc CSRAM core1 -> core5
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
                exit(1);
            }
            else {
                for (int k = 0; k < 256; k++) {
                    cores[i].neurons[j].conns[k] = line[k] - '0';
                }
                
                int current_potential_temp [9];
                for (int k = 0; k < 9; k++){
                    current_potential_temp[k]=line[k+256] - '0';
                }
                cores[i].neurons[j].current_potential = binaryToDecimalSigned(current_potential_temp,sizeof(current_potential_temp)/ sizeof(current_potential_temp[0]));
                
                int reset_potential_temp [9];
                for (int k=0; k<9;k++){
                    reset_potential_temp[k]=line[k+265] -'0';
                }
                cores[i].neurons[j].reset_potential = binaryToDecimalSigned(reset_potential_temp,sizeof(reset_potential_temp)/sizeof(reset_potential_temp[0]));

                int formatted_weight_1_temp [9];
                for (int k = 0; k < 9;k++){
                    formatted_weight_1_temp[k]=line[k+274] - '0';
                }
                cores[i].neurons[j].formatted_weight[0] = binaryToDecimalSigned(formatted_weight_1_temp,sizeof(formatted_weight_1_temp)/sizeof(formatted_weight_1_temp[0]));
                
                int formatted_weight_2_temp [9];
                for (int k = 0; k < 9;k++){
                    formatted_weight_2_temp[k]=line[k+283] - '0';
                }
                cores[i].neurons[j].formatted_weight[1] = binaryToDecimalSigned(formatted_weight_2_temp,sizeof(formatted_weight_2_temp)/sizeof(formatted_weight_2_temp[0]));
                
                int formatted_weight_3_temp [9];
                for (int k = 0; k < 9;k++){
                    formatted_weight_3_temp[k]=line[k+292] - '0';
                }
                cores[i].neurons[j].formatted_weight[2] = binaryToDecimalSigned(formatted_weight_3_temp,sizeof(formatted_weight_3_temp)/sizeof(formatted_weight_3_temp[0]));

                int formatted_weight_4_temp [9];
                for (int k = 0; k < 9;k++){
                    formatted_weight_4_temp[k]=line[k+301] - '0';
                }
                cores[i].neurons[j].formatted_weight[3] = binaryToDecimalSigned(formatted_weight_4_temp,sizeof(formatted_weight_4_temp)/sizeof(formatted_weight_2_temp[0]));
                
                int leak_temp [9];
                for (int k = 0; k < 9;k++){
                    leak_temp[k]=line[k+310] - '0';
                }
                cores[i].neurons[j].leak = binaryToDecimalSigned(leak_temp,sizeof(leak_temp)/sizeof(leak_temp[0]));

                int pos_threshold_temp [9];
                for (int k = 0; k < 9;k++){
                    pos_threshold_temp[k]=line[k+319] - '0';
                }
                cores[i].neurons[j].pos_threshold = binaryToDecimalUnsigned(pos_threshold_temp,sizeof(pos_threshold_temp)/sizeof(pos_threshold_temp[0]));
                
                int neg_threshold_temp [9];
                for (int k = 0; k < 9;k++){
                    neg_threshold_temp[k]=line[k+328] - '0';
                }
                cores[i].neurons[j].neg_threshold = binaryToDecimalSigned(neg_threshold_temp,sizeof(neg_threshold_temp)/sizeof(neg_threshold_temp[0]));
                
                cores[i].neurons[j].reset_mode = line[337] - '0';

                int spike_des_core_x_temp [9];
                for (int k = 0; k < 9;k++){
                    spike_des_core_x_temp[k]=line[k+338] - '0';
                }
                cores[i].neurons[j].spike_des_core[0] = binaryToDecimalSigned(spike_des_core_x_temp,sizeof(spike_des_core_x_temp)/sizeof(spike_des_core_x_temp[0]));
                
                int spike_des_core_y_temp [9];
                for (int k = 0; k < 9;k++){
                    spike_des_core_y_temp[k]=line[k+347] - '0';
                }
                cores[i].neurons[j].spike_des_core[1] = binaryToDecimalSigned(spike_des_core_y_temp,sizeof(spike_des_core_y_temp)/sizeof(spike_des_core_y_temp[0]));
                
                int spike_des_axon_temp [8];
                for (int k = 0; k < 8;k++){
                    spike_des_axon_temp[k]=line[k+356] - '0';
                }
                cores[i].neurons[j].spike_des_axon = binaryToDecimalUnsigned(spike_des_axon_temp,sizeof(spike_des_axon_temp)/sizeof(spike_des_axon_temp[0]));
                
                int spike_tick_temp [4];
                for (int k = 0; k < 4;k++){
                    spike_tick_temp[k]=line[k+364] - '0';
                }
                cores[i].neurons[j].spike_tick = binaryToDecimalUnsigned(spike_tick_temp,sizeof(spike_tick_temp)/sizeof(spike_tick_temp[0]));   
                
            }
        }
        fclose(f);
        printf("DONE READ CSRAM_00%d.MEM\n",i);
    }
}

void readTC(struct core cores[]){
    char filename_tc[100];
    FILE *f;
    for(int i=0;i<5;i++){
        //Đọc TC core1->core5
        snprintf(filename_tc, sizeof(filename_tc), "tc_00%d.mem", i);
        f= fopen(filename_tc,"r");
        if (f==NULL){
            printf("\n invalid file");
            break;
        }
        for (int j=0;j<256;j++){
            char line[2];
            if (fscanf(f, "%s", line) != 1) break;
            int axon_type_temp [2];
                for (int k = 0; k < 2;k++){
                    axon_type_temp[k]=line[k] - '0';
                }
            cores[i].axons[j].axon_type=binaryToDecimalSigned(axon_type_temp,sizeof(axon_type_temp)/sizeof(axon_type_temp[0]));
            cores[i].axons[j].weight=cores[i].neurons[0].formatted_weight[cores[i].axons[j].axon_type];
            
        }
        fclose(f);
        
        printf("DONE READ TC_00%d.MEM\n",i);
    }
}

void save_to_file(struct core cores[]){
    FILE *f;
    char filename_write[100];
    char filename_tc[100];
    for(int i=0;i<5;i++){
        snprintf(filename_write, sizeof(filename_write), "neuron_core_00%d.txt", i);
        f= fopen(filename_write,"w");
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

void get_des_packet(FILE *f_input,int *des_core, int *des_axon){
    int des_dy = 0;
    int des_dx = 0;
    char line[30];

    if (fscanf(f_input, "%s", line) != 1){
        exit(1);
    }

    int dx[9];
    for (int k=0; k<9;k++){
        dx[k] = line[k] - '0';
    }
    int dy[9];
    for (int k=0;k<9;k++){
        dy[k] = line[k+9] - '0';
    }
    des_dx=binaryToDecimalSigned(dx,sizeof(dx)/sizeof(dx[0]));
    des_dy=binaryToDecimalSigned(dy,sizeof(dy)/sizeof(dy[0]));
    *des_core = des_dx + 4*des_dy;

    int des_axon_temp[8];
    for (int k=0;k<8;k++){
        des_axon_temp[k]=line[k+18] - '0';
    }
    *des_axon = binaryToDecimalUnsigned(des_axon_temp,sizeof(des_axon_temp)/sizeof(des_axon_temp[0]));
}

int get_num_inputs(FILE *f_num_inputs){
    int num_inputs = 0;
    if (fscanf(f_num_inputs,"%d",&num_inputs) !=1) exit(1);
    return num_inputs;
}

void neuron_block_update_potential(struct core cores[], int *des_core, int *des_axon){
    for(int k=0;k<cores[*des_core].max_neurons;k++){
        if(cores[*des_core].neurons[k].conns[*des_axon]){
            cores[*des_core].neurons[k].current_potential += cores[*des_core].neurons[k].formatted_weight[cores[*des_core].axons[*des_axon].axon_type];
        }
    }
}

void check_threshold_and_fire(struct core cores[], int number[]){
    int des_core_packet=0;
    int des_axon_packet=0;
    int dx=0;
    int dy=0;
    for(int j=0;j<4;j++){
        for(int k=0; k < cores[j].max_neurons;k++){
            if(cores[j].neurons[k].current_potential >= cores[j].neurons[k].pos_threshold){
                cores[j].neurons[k].current_potential = cores[j].neurons[k].reset_potential;
                dx = j%4 + cores[j].neurons[k].spike_des_core[0];
                dy = j/4 + cores[j].neurons[k].spike_des_core[1];
                des_core_packet = dx + 4*dy;
                //printf("core %d neuron %d des core: %d ",j+1,k+1,des_core_packet+1);
                des_axon_packet = cores[j].neurons[k].spike_des_axon;
                //printf("des axon: %d\n",des_axon_packet+1);
                neuron_block_update_potential(cores,&des_core_packet,&des_axon_packet);
                //printf("CORE: %d NEURONS: %d \n",j+1,k+1);
            }
            else if(cores[j].neurons[k].current_potential < cores[j].neurons[k].neg_threshold){
                cores[j].neurons[k].current_potential = cores[j].neurons[k].reset_potential;
            }
        }
        for(int j=0;j<cores[4].max_neurons;j++){
            if(cores[4].neurons[j].current_potential >= cores[4].neurons[j].pos_threshold){
                number[j/25] += 1;
                cores[4].neurons[j].current_potential = cores[4].neurons[j].reset_potential;
            }
            else if(cores[4].neurons[j].current_potential < cores[4].neurons[j].neg_threshold){
                cores[4].neurons[j].current_potential = cores[4].neurons[j].reset_potential;
            }
        }
    }
    //printf("DONE CHECK THRESHOLD\n");
}

void get_vote_core_5(struct core cores[],int number []){
    for(int j=0;j<cores[4].max_neurons;j++){
        if(cores[4].neurons[j].current_potential >= cores[4].neurons[j].pos_threshold){
            number[j/25] += 1;
            cores[4].neurons[j].current_potential = cores[4].neurons[j].reset_potential;
        }
        else if(cores[4].neurons[j].current_potential < cores[4].neurons[j].neg_threshold){
            cores[4].neurons[j].current_potential = cores[4].neurons[j].reset_potential;
        }
    }
}

void get_predict(FILE *f_predict,int number[]){
    int get_max=0;
    int predict=0;
    for(int i=9;i>=0;i--){
        if(get_max <= number[i]){
            predict=i;
            get_max=number[i];
        }
    }
    fprintf(f_predict,"%d\n",predict);
}

int main() {
    struct core cores[5];
    for (int i=0; i<4; ++i){
        cores[i].neurons = malloc(64 * sizeof(struct neuron));
        cores[i].max_neurons = 64;
    }
    cores[4].neurons = malloc(250 * sizeof(struct neuron));
    cores[4].max_neurons = 250;

    int max_MNIST = 10000;
    int des_core = 0 ;
    int des_axon = 0;
    int num_inputs = 0;

    readCsram(cores);
    readTC(cores);
    save_to_file(cores);

    FILE *f_num_inputs=fopen("tb_num_inputs.txt","r");
    if (f_num_inputs == NULL) {
        printf("Error opening file tb_num_inputs.txt\n");
        return 1;
    }
    FILE *f_input=fopen("tb_input.txt","r");
    if (f_input == NULL) {
        printf("Error opening file tb_inputs.txt\n");
        return 1;
    }
    FILE *f_save_packet=fopen("packet_mnist.txt","w");
    FILE *f_predict=fopen("predict.txt","w");
    

    for(int i=0;i<max_MNIST;i++){
        int number[10]={0,0,0,0,0,0,0,0,0,0};
        num_inputs=get_num_inputs(f_num_inputs);
        fprintf(f_save_packet,"MNIST %d: num_inputs: %d\n",i+1,num_inputs);
        for(int j=0;j<num_inputs;j++){
            get_des_packet(f_input,&des_core,&des_axon);
            fprintf(f_save_packet,"packet %d: des_core: %d ",j+1,des_core);
            fprintf(f_save_packet," des_axon: %d\n",des_axon);
            neuron_block_update_potential(cores,&des_core,&des_axon);
        }
        // for(int j=0;j<4;j++){
        //     for (int k=0;k<cores[j].max_neurons;k++){
        //         printf("CORE %d NEURON %d: %d\n",j+1,k+1,cores[j].neurons[k].current_potential);
        //     }
        // }
        check_threshold_and_fire(cores,number);

        // for(int j=0;j<4;j++){
        //     for (int k=0;k<cores[j].max_neurons;k++){
        //         printf("CORE %d NEURON %d: %d\n",j+1,k+1,cores[j].neurons[k].current_potential);
        //     }
        // }

        //get_vote_core_5(cores,number);
        // for(int i=0;i<10;i++){
        //     printf("number %d: %d\n",i,number[i]);
        // }
        get_predict(f_predict,number);
    }
    printf("DONE\n");

    fclose(f_input);
    fclose(f_num_inputs);
    fclose(f_save_packet);
    fclose(f_predict);
    return 0;
}
