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
        for (int core = 0; core < size; core++) {
            if (binaryArray[core] == 1) {
                binaryArray[core] = 0;
            } else {
                binaryArray[core] = 1;
            }
        }
        int carry = 1;
        for (int core = size - 1; core >= 0; core--) {
            binaryArray[core] += carry;
            if (binaryArray[core] > 1) {
                binaryArray[core] = 0;
                carry = 1;
            } else {
                carry = 0;
            }
        }
        decimalNumber =  binaryToDecimalSigned(binaryArray, size);
        decimalNumber = - decimalNumber;
    } else {
        for (int core = 0; core < size; core++) {
            decimalNumber += binaryArray[core] * pow(2, power);
            power --;
        }
    }

    return decimalNumber;
}

int binaryToDecimalUnsigned(int binaryArray[], int size) {
    int decimalNumber = 0;
    int power = size - 1;
    for (int core = 0; core < size; core++) {
        decimalNumber += binaryArray[core] * pow(2, power);
        power--;
    }

    return decimalNumber;
}

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
        
        //printf("DONE READ TC_00%d.MEM\n",core);
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

void get_des_packet(FILE *f_input,int *des_core, int *des_axon){
    int des_dy = 0;
    int des_dx = 0;
    char line[30];
    if (fscanf(f_input, "%s", line) != 1){
        exit(1);
    }

    int dx[9];
    for (int i=0; i<9;i++){
        dx[i] = line[i] - '0';
    }
    int dy[9];
    for (int i=0;i<9;i++){
        dy[i] = line[i+9] - '0';
    }
    des_dx=binaryToDecimalSigned(dx,sizeof(dx)/sizeof(dx[0]));
    des_dy=binaryToDecimalSigned(dy,sizeof(dy)/sizeof(dy[0]));
    *des_core = des_dx + 4*des_dy;

    int des_axon_temp[8];
    for (int i=0;i<8;i++){
        des_axon_temp[i]=line[i+18] - '0';
    }
    *des_axon = binaryToDecimalUnsigned(des_axon_temp,sizeof(des_axon_temp)/sizeof(des_axon_temp[0]));
}

int get_num_inputs(FILE *f_num_inputs){
    int num_inputs = 0;
    if (fscanf(f_num_inputs,"%d",&num_inputs) !=1) exit(1);
    return num_inputs;
}

void neuron_block_update_potential(struct core cores[], int *des_core, int *des_axon){
    // printf("%d",*des_axon);
    for(int neuron = 0; neuron < cores[*des_core].max_neurons; neuron++){
        //printf("neuron %d conns %d\n",neuron,cores[*des_core].neurons[neuron].conns[*des_axon]);
        //printf("neuron %d: %d\n",neuron,cores[*des_core].neurons[neuron].formatted_weight[cores[*des_core].axons[*des_axon].axon_type]);
        if(cores[*des_core].neurons[neuron].conns[*des_axon]){
            // printf("neuron %d\n",cores[*des_core].neurons[neuron].conns[*des_axon]);
            cores[*des_core].neurons[neuron].current_potential += cores[*des_core].neurons[neuron].formatted_weight[cores[*des_core].axons[*des_axon].axon_type];
        }
        //cores[*des_core].neurons[neuron].current_potential += cores[*des_core].neurons[neuron].conns[*des_axon] * cores[*des_core].neurons[neuron].formatted_weight[cores[*des_core].axons[*des_axon].axon_type];
    }
}

void check_threshold_and_fire(struct core cores[]){
    int des_core_packet=0;
    int des_axon_packet=0;
    int dx=0;
    int dy=0;
    for(int core=0;core<4;core++){
        for(int neuron=0; neuron < cores[core].max_neurons;neuron++){
            // if(core==0){
            //     //printf("neuron %d: %d\n",neuron,cores[core].neurons[neuron].pos_threshold);
            //     printf("neuron %d : %d\n",neuron, cores[core].neurons[neuron].current_potential);
            // }
            if(cores[core].neurons[neuron].current_potential >= cores[core].neurons[neuron].pos_threshold){
                cores[core].neurons[neuron].current_potential = cores[core].neurons[neuron].reset_potential;
                dx = core%4 + cores[core].neurons[neuron].spike_des_core[0];
                dy = core/4 + cores[core].neurons[neuron].spike_des_core[1];
                des_core_packet = dx + 4*dy;
                //printf("%d\n",des_core_packet);
                des_axon_packet = cores[core].neurons[neuron].spike_des_axon;
                // if(core==3){
                //     printf("core %d neuron %d des core: %d ",core,neuron,des_core_packet);
                //     printf("des axon: %d\n",des_axon_packet);
                // }
                neuron_block_update_potential(cores,&des_core_packet,&des_axon_packet);
                //printf("CORE: %d NEURONS: %d \n",core+1,neuron+1);
            }
            if(cores[core].neurons[neuron].current_potential < cores[core].neurons[neuron].neg_threshold){
                cores[core].neurons[neuron].current_potential = cores[core].neurons[neuron].reset_potential;
            }
        }
        // for(int neuron=0;neuron<cores[4].max_neurons;neuron++){
        //     if(cores[4].neurons[neuron].current_potential >= cores[4].neurons[neuron].pos_threshold){
        //         number[neuron/25] += 1;
        //         cores[4].neurons[neuron].current_potential = cores[4].neurons[neuron].reset_potential;
        //     }
        //     if(cores[4].neurons[neuron].current_potential < cores[4].neurons[neuron].neg_threshold){
        //         cores[4].neurons[neuron].current_potential = cores[4].neurons[neuron].reset_potential;
        //     }
        // }
    }
    //printf("DONE CHECK THRESHOLD\n");
}

void get_vote_core_5(struct core cores[],int number []){
    int i=0;
    for(int neuron=0;neuron<cores[4].max_neurons;neuron++){
        //printf("neuron %d: %d\n",neuron,cores[4].neurons[neuron].pos_threshold);
        //printf("neuron %d: %d \n",neuron,cores[4].neurons[neuron].current_potential);
        if(cores[4].neurons[neuron].current_potential >= cores[4].neurons[neuron].pos_threshold){
            number[i] += 1;
            cores[4].neurons[neuron].current_potential = cores[4].neurons[neuron].reset_potential;
            //printf("neuron %d\n",neuron);
        }
        if(cores[4].neurons[neuron].current_potential < cores[4].neurons[neuron].neg_threshold){
            cores[4].neurons[neuron].current_potential = cores[4].neurons[neuron].reset_potential;
        }
        if(i==9){
            i=0;
        }
        else{
            i=i+1;
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

void reset_core(struct core cores[]){
    for (int core=0;core<5;core++){
        for (int neuron=0;neuron<cores[core].max_neurons;neuron++){
            cores[core].neurons[neuron].current_potential=0;
        }
    }
}

void get_percentage(){
    double max_mnist=10000;
    FILE *f_get_predict;
    f_get_predict=fopen("predict.txt","r");
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

int main() {
    struct core cores[5];
    for (int core=0; core<4; ++core){
        cores[core].neurons = malloc(64 * sizeof(struct neuron));
        cores[core].max_neurons = 64;
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

    FILE *f_num_inputs=fopen("./testbench/tb_num_inputs.txt","r");
    if (f_num_inputs == NULL) {
        printf("Error opening file tb_num_inputs.txt\n");
        return 1;
    }
    FILE *f_input=fopen("./testbench/tb_input.txt","r");
    if (f_input == NULL) {
        printf("Error opening file tb_inputs.txt\n");
        return 1;
    }

    FILE *f_save_packet=fopen("./packet/packet_mnist.txt","w");
    FILE *f_predict=fopen("predict.txt","w");
    
    for(int mnist=0;mnist<max_MNIST;mnist++){
        int number[10]={0,0,0,0,0,0,0,0,0,0};
        fprintf(f_save_packet,"MNIST %d: num_inputs: %d\n",mnist+1,num_inputs=get_num_inputs(f_num_inputs));
        //printf("%d\n",num_inputs);
        //num_inputs =1;
        // Get packet from mnist
        for(int packet=0;packet<num_inputs;packet++){
            get_des_packet(f_input,&des_core,&des_axon);
            fprintf(f_save_packet,"packet %d: des_core: %d ",packet+1,des_core);
            fprintf(f_save_packet," des_axon: %d\n",des_axon);
            neuron_block_update_potential(cores,&des_core,&des_axon);
        }
        // for(int j=0;j<4;j++){
        //     for (int conns=0;conns<cores[j].max_neurons;conns++){
        //         printf("CORE %d NEURON %d: %d\n",j+1,conns+1,cores[j].neurons[conns].current_potential);
        //     }
        // }
        check_threshold_and_fire(cores);

        // for(int core=0;core<5;core++){
        //     for (int neuron=0;neuron<cores[core].max_neurons;neuron++){
        //         printf("CORE %d NEURON %d: %d\n",core+1,neuron+1,cores[core].neurons[neuron].current_potential);
        //     }
        // }

        get_vote_core_5(cores,number);

        // for(int i=0;i<10;i++){
        //     printf("number %d: %d\n",i,number[i]);
        // }

        get_predict(f_predict,number);
        reset_core(cores);
    }
    // for(int i=0;i<256;i++){
    //     printf("axon %d: %d\n",i,cores[0].axons[i].weight);
    // }
    //printf("DONE\n");
    
    // for(int core=0;core<5;core++){
    //     for (int neuron=0;neuron<cores[core].max_neurons;neuron++){
    //         printf("CORE %d NEURON %d: %d\n",core+1,neuron+1,cores[core].neurons[neuron].current_potential);
    //     }
    // }
    fclose(f_input);
    fclose(f_num_inputs);
    fclose(f_save_packet);
    fclose(f_predict);
    
    get_percentage();
    return 0;
}
