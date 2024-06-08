#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include "layer.c"
// #include "convert.c"

/*** Đọc file Csram ***/
void readCsram(struct layer layers[]){
    FILE *f;
    char filename_csram[100];
    for (int layer=0;layer<num_layer;layer++){
        for(int core=0;core<layers[layer].max_cores;core++){
            snprintf(filename_csram, sizeof(filename_csram),PATH"/streaming/csram_%03d.mem",(layer*4+core));
            f= fopen(filename_csram,"r");
            if (f==NULL){
                printf("\n invalid csram file");
                exit(1);
            }
            for (int j = 0; j < layers[layer].cores[core].max_neurons; j++) {
                char line[368];
                if (fscanf(f, "%s", line) != 1){
                    exit(1);
                }
                else {
                    for (int k = 0; k < 256; k++) {
                        layers[layer].cores[core].neurons[j].conns[k] = line[255-k] - '0';
                    }
                    
                    int current_potential_temp [9];
                    for (int k = 0; k < 9; k++){
                        current_potential_temp[k]=line[k+256] - '0';
                    }
                    layers[layer].cores[core].neurons[j].current_potential = binaryToDecimalSigned(current_potential_temp,sizeof(current_potential_temp)/ sizeof(current_potential_temp[0]));
                    
                    int reset_potential_temp [9];
                    for (int k=0; k<9;k++){
                        reset_potential_temp[k]=line[k+265] -'0';
                    }
                    layers[layer].cores[core].neurons[j].reset_potential = binaryToDecimalSigned(reset_potential_temp,sizeof(reset_potential_temp)/sizeof(reset_potential_temp[0]));

                    int formatted_weight_1_temp [9];
                    for (int k = 0; k < 9;k++){
                        formatted_weight_1_temp[k]=line[k+274] - '0';
                    }
                    layers[layer].cores[core].neurons[j].formatted_weight[0] = binaryToDecimalSigned(formatted_weight_1_temp,sizeof(formatted_weight_1_temp)/sizeof(formatted_weight_1_temp[0]));
                    
                    int formatted_weight_2_temp [9];
                    for (int k = 0; k < 9;k++){
                        formatted_weight_2_temp[k]=line[k+283] - '0';
                    }
                    layers[layer].cores[core].neurons[j].formatted_weight[1] = binaryToDecimalSigned(formatted_weight_2_temp,sizeof(formatted_weight_2_temp)/sizeof(formatted_weight_2_temp[0]));
                    
                    int formatted_weight_3_temp [9];
                    for (int k = 0; k < 9;k++){
                        formatted_weight_3_temp[k]=line[k+292] - '0';
                    }
                    layers[layer].cores[core].neurons[j].formatted_weight[2] = binaryToDecimalSigned(formatted_weight_3_temp,sizeof(formatted_weight_3_temp)/sizeof(formatted_weight_3_temp[0]));

                    int formatted_weight_4_temp [9];
                    for (int k = 0; k < 9;k++){
                        formatted_weight_4_temp[k]=line[k+301] - '0';
                    }
                    layers[layer].cores[core].neurons[j].formatted_weight[3] = binaryToDecimalSigned(formatted_weight_4_temp,sizeof(formatted_weight_4_temp)/sizeof(formatted_weight_2_temp[0]));
                    
                    int leak_temp [9];
                    for (int k = 0; k < 9;k++){
                        leak_temp[k]=line[k+310] - '0';
                    }
                    layers[layer].cores[core].neurons[j].leak = binaryToDecimalSigned(leak_temp,sizeof(leak_temp)/sizeof(leak_temp[0]));

                    int pos_threshold_temp [9];
                    for (int k = 0; k < 9;k++){
                        pos_threshold_temp[k]=line[k+319] - '0';
                    }
                    layers[layer].cores[core].neurons[j].pos_threshold = binaryToDecimalUnsigned(pos_threshold_temp,sizeof(pos_threshold_temp)/sizeof(pos_threshold_temp[0]));
                    
                    int neg_threshold_temp [9];
                    for (int k = 0; k < 9;k++){
                        neg_threshold_temp[k]=line[k+328] - '0';
                    }
                    layers[layer].cores[core].neurons[j].neg_threshold = binaryToDecimalSigned(neg_threshold_temp,sizeof(neg_threshold_temp)/sizeof(neg_threshold_temp[0]));
                    
                    layers[layer].cores[core].neurons[j].reset_mode = line[337] - '0';

                    int spike_des_core_x_temp [9];
                    for (int k = 0; k < 9;k++){
                        spike_des_core_x_temp[k]=line[k+338] - '0';
                    }
                    layers[layer].cores[core].neurons[j].spike_des_core[0] = binaryToDecimalSigned(spike_des_core_x_temp,sizeof(spike_des_core_x_temp)/sizeof(spike_des_core_x_temp[0]));
                    
                    int spike_des_core_y_temp [9];
                    for (int k = 0; k < 9;k++){
                        spike_des_core_y_temp[k]=line[k+347] - '0';
                    }
                    layers[layer].cores[core].neurons[j].spike_des_core[1] = binaryToDecimalSigned(spike_des_core_y_temp,sizeof(spike_des_core_y_temp)/sizeof(spike_des_core_y_temp[0]));
                    
                    int spike_des_axon_temp [8];
                    for (int k = 0; k < 8;k++){
                        spike_des_axon_temp[k]=line[k+356] - '0';
                    }
                    layers[layer].cores[core].neurons[j].spike_des_axon = binaryToDecimalUnsigned(spike_des_axon_temp,sizeof(spike_des_axon_temp)/sizeof(spike_des_axon_temp[0]));
                    
                    int spike_tick_temp [4];
                    for (int k = 0; k < 4;k++){
                        spike_tick_temp[k]=line[k+364] - '0';
                    }
                    layers[layer].cores[core].neurons[j].spike_tick = binaryToDecimalUnsigned(spike_tick_temp,sizeof(spike_tick_temp)/sizeof(spike_tick_temp[0]));  
                }
            }
            fclose(f);
        }
    }
}

/*** Đọc file TC ***/
void readTC(struct layer layers[]){
    char filename_tc[100];
    FILE *f;
    for (int layer=0;layer<num_layer;layer++){
        for(int core=0;core<layers[layer].max_cores ;core++){
            snprintf(filename_tc, sizeof(filename_tc),PATH"/streaming/tc_%03d.mem", (layer*4+core));
            f= fopen(filename_tc,"r");
            if (f==NULL){
                printf("\n invalid tc file");
                break;
            }
            for (int j=0;j<256;j++){
                char line[2];
                if (fscanf(f, "%s", line) != 1) break;
                int axon_type_temp [2];
                    for (int k = 0; k < 2;k++){
                        axon_type_temp[k]=line[k] - '0';
                    }
                layers[layer].cores[core].axons[j].axon_type=binaryToDecimalUnsigned(axon_type_temp,sizeof(axon_type_temp)/sizeof(axon_type_temp[0]));
                layers[layer].cores[core].axons[j].weight=layers[layer].cores[core].neurons[0].formatted_weight[layers[layer].cores[core].axons[j].axon_type];
                
            }
            fclose(f);
        }
    }
}

/*** Lưu tham số network vào file ***/
void save_parameter_to_file(struct layer layers[]){
    FILE *f;
    char filename_write[100];
    char filename_tc[100];
    mkdir(PATH"/neuron_parameter");
    mkdir(PATH"/axon_parameter");
    for (int layer=0;layer<num_layer;layer++){
        for(int core=0;core<layers[layer].max_cores;core++){
            snprintf(filename_write, sizeof(filename_write),PATH"/neuron_parameter/neuron_core_%03d.txt", (layer * 4 + core));
            f= fopen(filename_write,"w");
            for (int neuron = 0; neuron < layers[layer].cores[core].max_neurons; neuron++) {
                fprintf(f,"\n");
                fprintf(f,"LAYER %d CORE %d NEURON %d\n", layer, core, neuron);
                fprintf(f,"conns ");
                for (int conns = 0; conns < max_axon; conns++) {
                    fprintf(f,"%d", layers[layer].cores[core].neurons[neuron].conns[conns]);
                }
                fprintf(f,"\n");
                fprintf(f,"current_potential %d\n", layers[layer].cores[core].neurons[neuron].current_potential);
                fprintf(f,"reset_potential %d\n", layers[layer].cores[core].neurons[neuron].reset_potential);
                fprintf(f,"formatted_weight_1 %d\n", layers[layer].cores[core].neurons[neuron].formatted_weight[0]);
                fprintf(f,"formatted_weight_2 %d\n", layers[layer].cores[core].neurons[neuron].formatted_weight[1]);
                fprintf(f,"formatted_weight_3 %d\n", layers[layer].cores[core].neurons[neuron].formatted_weight[2]);
                fprintf(f,"formatted_weight_4 %d\n", layers[layer].cores[core].neurons[neuron].formatted_weight[3]);
                fprintf(f,"leak %d\n", layers[layer].cores[core].neurons[neuron].leak);
                fprintf(f,"pos_threshold %d\n", layers[layer].cores[core].neurons[neuron].pos_threshold);
                fprintf(f,"neg_threshold %d\n", layers[layer].cores[core].neurons[neuron].neg_threshold);
                fprintf(f,"reset_mode %d\n", layers[layer].cores[core].neurons[neuron].reset_mode);
                fprintf(f,"spike_des_core_x %d\n", layers[layer].cores[core].neurons[neuron].spike_des_core[0]);
                fprintf(f,"spike_des_core_y %d\n", layers[layer].cores[core].neurons[neuron].spike_des_core[1]);
                fprintf(f,"spike_des_axon %d\n", layers[layer].cores[core].neurons[neuron].spike_des_axon);
                fprintf(f,"spike_tick %d\n", layers[layer].cores[core].neurons[neuron].spike_tick);
            }
            fclose(f);
            snprintf(filename_tc, sizeof(filename_tc),PATH"/axon_parameter/axon__core_00%d.txt",(layer * 4 + core));
            f= fopen(filename_tc,"w");
            for (int axon=0;axon<max_axon;axon++){
                fprintf(f,"LAYER %d CORE %d AXON %d\n", layer, core, axon);
                fprintf(f,"axon type: %d\n",layers[layer].cores[core].axons[axon].axon_type);
                fprintf(f,"axon_weight: %d\n",layers[layer].cores[core].axons[axon].weight=layers[layer].cores[core].neurons[0].formatted_weight[layers[layer].cores[core].axons[axon].axon_type]);
                fprintf(f," \n");
            }
            fclose(f);
        }
    }
}

/*** Tạo network và lấy tham số cho network ***/
void get_parameter(struct layer layers[]){
    printf("Creating Network...\n");
    create_network(layers);
    printf("Installing parameter for network...\n");
    readCsram(layers);
    readTC(layers);
    save_parameter_to_file(layers);
}