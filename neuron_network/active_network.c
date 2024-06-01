#include <unistd.h>
#include "get_parameter.c"
#include "get_packet.c"
#include "get_network_accuracy.c"
#include "param.c"

void neuron_block_update_potential(struct core cores[], int *des_core, int *des_axon){
    for(int neuron = 0; neuron < cores[*des_core].max_neurons; neuron++){
        if(cores[*des_core].neurons[neuron].conns[*des_axon]){
            cores[*des_core].neurons[neuron].current_potential += cores[*des_core].neurons[neuron].formatted_weight[cores[*des_core].axons[*des_axon].axon_type];
        }
    }
}

void integrate(FILE *f_input, FILE *f_save_packet,FILE *f_num_inputs,int mnist,struct core cores[]){
    int des_core = 0 ;
    int des_axon = 0;
    int num_inputs = 0;
    fprintf(f_save_packet,"MNIST %d: num_inputs: %d\n",mnist+1,num_inputs=get_num_inputs(f_num_inputs));
    for(int packet=0;packet<num_inputs;packet++){
            get_des_packet(f_input,&des_core,&des_axon);
            fprintf(f_save_packet,"packet %d: des_core: %d ",packet+1,des_core);
            fprintf(f_save_packet," des_axon: %d\n",des_axon);
            neuron_block_update_potential(cores,&des_core,&des_axon);
        }
}

void leaky(struct core cores[]){
    for (int core=0; core < (max_core-1); core++){
        for (int neuron=0; neuron < cores[core].max_neurons; neuron++){
            cores[core].neurons[neuron].current_potential += cores[core].neurons[neuron].leak;
        }
    }
}

void fire(struct core cores[]){
    int des_core_packet=0;
    int des_axon_packet=0;
    int dx=0;
    int dy=0;
    for(int core=0;core<(max_core-1);core++){
        for(int neuron=0; neuron < cores[core].max_neurons;neuron++){
            if(cores[core].neurons[neuron].current_potential >= cores[core].neurons[neuron].pos_threshold){
                cores[core].neurons[neuron].current_potential = cores[core].neurons[neuron].reset_potential;
                dx = core%4 + cores[core].neurons[neuron].spike_des_core[0];
                dy = core/4 + cores[core].neurons[neuron].spike_des_core[1];
                des_core_packet = dx + 4*dy;

                des_axon_packet = cores[core].neurons[neuron].spike_des_axon;

                neuron_block_update_potential(cores,&des_core_packet,&des_axon_packet);

            }
            if(cores[core].neurons[neuron].current_potential < cores[core].neurons[neuron].neg_threshold){
                cores[core].neurons[neuron].current_potential = cores[core].neurons[neuron].reset_potential;
            }
        }
    }
}

void get_vote_core_5(struct core cores[],int number []){
    int i=0;
    for(int neuron=0;neuron<cores[max_core-1].max_neurons;neuron++){
        if(cores[max_core-1].neurons[neuron].current_potential >= cores[max_core-1].neurons[neuron].pos_threshold){
            number[i] += 1;
            cores[max_core-1].neurons[neuron].current_potential = cores[max_core-1].neurons[neuron].reset_potential;

        }
        if(cores[max_core-1].neurons[neuron].current_potential < cores[max_core-1].neurons[neuron].neg_threshold){
            cores[max_core-1].neurons[neuron].current_potential = cores[max_core-1].neurons[neuron].reset_potential;
        }
        if(i==(class-1)){
            i=0;
        }
        else{
            i=i+1;
        }
    }
}
void save_network_vote_class(FILE *f_save_network_predict,int number[],int mnist,int predict){
    fprintf(f_save_network_predict,"Vote for image    %d\n",mnist+1);
    for(int num=0;num<class;num++){
        fprintf(f_save_network_predict,"Number %d: %d\n",num,number[num]);
    }
    fprintf(f_save_network_predict,"Network predict: number %d\n",predict);
    fprintf(f_save_network_predict,"------------------------\n");
}
void get_predict(FILE *f_predict,FILE *f_save_network_predict,int number[],int mnist){
    int get_max=0;
    int predict=0;
    for(int i=(class-1);i>=0;i--){
        if(get_max <= number[i]){
            predict=i;
            get_max=number[i];
        }
    }
    fprintf(f_predict,"%d\n",predict);
    save_network_vote_class(f_save_network_predict,number,mnist,predict);
}
void reset_core(struct core cores[]){
    for(int num_core=0; num_core < max_core;num_core++){
        for(int num_neuron=0; num_neuron < cores[num_core].max_neurons; num_neuron++){
            cores[num_core].neurons[num_neuron].current_potential=0;
        }
    }
}

void active_network(struct core cores[]){
    printf("Activating Network...\n");
    int des_core = 0 ;
    int des_axon = 0;
    int num_inputs = 0;
    char file_num_inputs[100]=PATH"/testbench/tb_num_inputs.txt";
    FILE *f_num_inputs=fopen(file_num_inputs,"r");
    if (f_num_inputs == NULL) {
        printf("Error opening file tb_num_inputs.txt\n");
        exit(1);
    }
    char file_input[100]=PATH"/testbench/tb_input.txt";
    FILE *f_input=fopen(file_input,"r");
    if (f_input == NULL) {
        printf("Error opening file tb_inputs.txt\n");
        exit(1);
    }
    mkdir(PATH"/predict");
    mkdir(PATH"/packet");
    char file_save_packet[100]=PATH"/packet/packet_mnist.txt";
    FILE *f_save_packet=fopen(file_save_packet,"w");
    char file_predict[100]=PATH"/predict/predict.txt";
    FILE *f_predict=fopen(file_predict,"w");
    char file_save_network_predict[100]=PATH"/predict/vote_class.txt";
    FILE *f_save_network_predict = fopen(file_save_network_predict,"w");

    for(int mnist=0;mnist<max_mnist;mnist++){
        int number[class];
        for(int c=0;c<class;c++){
            number[c]=0;
        }
        if(mnist%1000==0 & mnist != 0){
            printf("Loading MNIST %d/10000\n", mnist);
        }
        integrate(f_input, f_save_packet, f_num_inputs, mnist, cores);
        leaky(cores);
        fire(cores);
        get_vote_core_5(cores,number);
        get_predict(f_predict,f_save_network_predict,number,mnist);
        reset_core(cores);
    }
    fclose(f_input);
    fclose(f_num_inputs);
    fclose(f_save_packet);
    fclose(f_predict);
    fclose(f_save_network_predict);
}        