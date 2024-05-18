#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "core.c"
#include "convert.c"

void readCsram(struct core cores[]){
    FILE *f;
    char filename_csram[100];
    for(int core=0;core<5;core++){
        //Đọc CSRAM core1 -> core5
        snprintf(filename_csram, sizeof(filename_csram), "./streaming/csram_00%d.mem", core);
        f= fopen(filename_csram,"r");
        if (f==NULL){
            printf("\n invalid file");
            exit(1);
        }
        //if(core==4) {cores[core].max_neurons=250;}
        //else {cores[core].max_neurons=64;}
        for (int j = 0; j < cores[core].max_neurons; j++) {
            char line[368];
            if (fscanf(f, "%s", line) != 1){ 
                exit(1);
            }
            else {
                for (int k = 0; k < 256; k++) {
                    cores[core].neurons[j].conns[k] = line[255-k] - '0';
                }
                
                int current_potential_temp [9];
                for (int k = 0; k < 9; k++){
                    current_potential_temp[k]=line[k+256] - '0';
                }
                cores[core].neurons[j].current_potential = binaryToDecimalSigned(current_potential_temp,sizeof(current_potential_temp)/ sizeof(current_potential_temp[0]));
                
                int reset_potential_temp [9];
                for (int k=0; k<9;k++){
                    reset_potential_temp[k]=line[k+265] -'0';
                }
                cores[core].neurons[j].reset_potential = binaryToDecimalSigned(reset_potential_temp,sizeof(reset_potential_temp)/sizeof(reset_potential_temp[0]));

                int formatted_weight_1_temp [9];
                for (int k = 0; k < 9;k++){
                    formatted_weight_1_temp[k]=line[k+274] - '0';
                }
                cores[core].neurons[j].formatted_weight[0] = binaryToDecimalSigned(formatted_weight_1_temp,sizeof(formatted_weight_1_temp)/sizeof(formatted_weight_1_temp[0]));
                
                int formatted_weight_2_temp [9];
                for (int k = 0; k < 9;k++){
                    formatted_weight_2_temp[k]=line[k+283] - '0';
                }
                cores[core].neurons[j].formatted_weight[1] = binaryToDecimalSigned(formatted_weight_2_temp,sizeof(formatted_weight_2_temp)/sizeof(formatted_weight_2_temp[0]));
                
                int formatted_weight_3_temp [9];
                for (int k = 0; k < 9;k++){
                    formatted_weight_3_temp[k]=line[k+292] - '0';
                }
                cores[core].neurons[j].formatted_weight[2] = binaryToDecimalSigned(formatted_weight_3_temp,sizeof(formatted_weight_3_temp)/sizeof(formatted_weight_3_temp[0]));

                int formatted_weight_4_temp [9];
                for (int k = 0; k < 9;k++){
                    formatted_weight_4_temp[k]=line[k+301] - '0';
                }
                cores[core].neurons[j].formatted_weight[3] = binaryToDecimalSigned(formatted_weight_4_temp,sizeof(formatted_weight_4_temp)/sizeof(formatted_weight_2_temp[0]));
                
                int leak_temp [9];
                for (int k = 0; k < 9;k++){
                    leak_temp[k]=line[k+310] - '0';
                }
                cores[core].neurons[j].leak = binaryToDecimalSigned(leak_temp,sizeof(leak_temp)/sizeof(leak_temp[0]));

                int pos_threshold_temp [9];
                for (int k = 0; k < 9;k++){
                    pos_threshold_temp[k]=line[k+319] - '0';
                }
                cores[core].neurons[j].pos_threshold = binaryToDecimalUnsigned(pos_threshold_temp,sizeof(pos_threshold_temp)/sizeof(pos_threshold_temp[0]));
                
                int neg_threshold_temp [9];
                for (int k = 0; k < 9;k++){
                    neg_threshold_temp[k]=line[k+328] - '0';
                }
                cores[core].neurons[j].neg_threshold = binaryToDecimalSigned(neg_threshold_temp,sizeof(neg_threshold_temp)/sizeof(neg_threshold_temp[0]));
                
                cores[core].neurons[j].reset_mode = line[337] - '0';

                int spike_des_core_x_temp [9];
                for (int k = 0; k < 9;k++){
                    spike_des_core_x_temp[k]=line[k+338] - '0';
                }
                cores[core].neurons[j].spike_des_core[0] = binaryToDecimalSigned(spike_des_core_x_temp,sizeof(spike_des_core_x_temp)/sizeof(spike_des_core_x_temp[0]));
                
                int spike_des_core_y_temp [9];
                for (int k = 0; k < 9;k++){
                    spike_des_core_y_temp[k]=line[k+347] - '0';
                }
                cores[core].neurons[j].spike_des_core[1] = binaryToDecimalSigned(spike_des_core_y_temp,sizeof(spike_des_core_y_temp)/sizeof(spike_des_core_y_temp[0]));
                
                int spike_des_axon_temp [8];
                for (int k = 0; k < 8;k++){
                    spike_des_axon_temp[k]=line[k+356] - '0';
                }
                cores[core].neurons[j].spike_des_axon = binaryToDecimalUnsigned(spike_des_axon_temp,sizeof(spike_des_axon_temp)/sizeof(spike_des_axon_temp[0]));
                
                int spike_tick_temp [4];
                for (int k = 0; k < 4;k++){
                    spike_tick_temp[k]=line[k+364] - '0';
                }
                cores[core].neurons[j].spike_tick = binaryToDecimalUnsigned(spike_tick_temp,sizeof(spike_tick_temp)/sizeof(spike_tick_temp[0]));   
                
            }
        }
        fclose(f);
        //printf("DONE READ CSRAM_00%d.MEM\n",core);
    }
}

void readTC(struct core cores[]){
    char filename_tc[100];
    FILE *f;
    for(int core=0;core<5;core++){
        //Đọc TC core1->core5
        snprintf(filename_tc, sizeof(filename_tc), "./streaming/tc_00%d.mem", core);
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
            cores[core].axons[j].axon_type=binaryToDecimalSigned(axon_type_temp,sizeof(axon_type_temp)/sizeof(axon_type_temp[0]));
            cores[core].axons[j].weight=cores[core].neurons[0].formatted_weight[cores[core].axons[j].axon_type];
            
        }
        fclose(f);
    }
}

void save_to_file(struct core cores[]){
    FILE *f;
    char filename_write[100];
    char filename_tc[100];
    for(int core=0;core<5;core++){
        snprintf(filename_write, sizeof(filename_write), "./neuron_parameter/neuron_core_00%d.txt", core);
        f= fopen(filename_write,"w");
        for (int neuron = 0; neuron < cores[core].max_neurons; neuron++) {
            fprintf(f,"\n");
            fprintf(f,"CORE %d NEURON %d\n", core, neuron);
            fprintf(f,"conns ");
            for (int conns = 0; conns < 256; conns++) {
                fprintf(f,"%d", cores[core].neurons[neuron].conns[conns]);
            }
            fprintf(f,"\n");
            fprintf(f,"current_potential %d\n", cores[core].neurons[neuron].current_potential);
            fprintf(f,"reset_potential %d\n", cores[core].neurons[neuron].reset_potential);
            fprintf(f,"formatted_weight_1 %d\n", cores[core].neurons[neuron].formatted_weight[0]);
            fprintf(f,"formatted_weight_2 %d\n", cores[core].neurons[neuron].formatted_weight[1]);
            fprintf(f,"formatted_weight_3 %d\n", cores[core].neurons[neuron].formatted_weight[2]);
            fprintf(f,"formatted_weight_4 %d\n", cores[core].neurons[neuron].formatted_weight[3]);
            fprintf(f,"leak %d\n", cores[core].neurons[neuron].leak);
            fprintf(f,"pos_threshold %d\n", cores[core].neurons[neuron].pos_threshold);
            fprintf(f,"neg_threshold %d\n", cores[core].neurons[neuron].neg_threshold);
            fprintf(f,"reset_mode %d\n", cores[core].neurons[neuron].reset_mode);
            fprintf(f,"spike_des_core_x %d\n", cores[core].neurons[neuron].spike_des_core[0]);
            fprintf(f,"spike_des_core_y %d\n", cores[core].neurons[neuron].spike_des_core[1]);
            fprintf(f,"spike_des_axon %d\n", cores[core].neurons[neuron].spike_des_axon);
            fprintf(f,"spike_tick %d\n", cores[core].neurons[neuron].spike_tick);
        }
        fclose(f);
        snprintf(filename_tc, sizeof(filename_tc), "./axon_parameter/axon__core_00%d.txt", core);
        f= fopen(filename_tc,"w");
        for (int axon=0;axon<256;axon++){
            fprintf(f,"CORE %d AXON %d\n",core+1,axon+1);
            fprintf(f,"axon type: %d\n",cores[core].axons[axon].axon_type);
            fprintf(f,"axon_weight: %d\n",cores[core].axons[axon].weight=cores[core].neurons[0].formatted_weight[cores[core].axons[axon].axon_type]);
            fprintf(f," \n");
        }
        fclose(f);
    }
}
void get_parameter(struct core cores[]){
    create_core(cores);
    readCsram(cores);
    readTC(cores);
    save_to_file(cores);
}