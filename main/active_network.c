#include "get_parameter.c"
#include "get_packet.c"
#include "get_network_accuracy.c"


void neuron_block_update_potential(struct core cores[], int *des_core, int *des_axon){
    for(int neuron = 0; neuron < cores[*des_core].max_neurons; neuron++){
        if(cores[*des_core].neurons[neuron].conns[*des_axon]){
            cores[*des_core].neurons[neuron].current_potential += cores[*des_core].neurons[neuron].formatted_weight[cores[*des_core].axons[*des_axon].axon_type];
        }
    }
}

void check_threshold_and_fire(struct core cores[]){
    int des_core_packet=0;
    int des_axon_packet=0;
    int dx=0;
    int dy=0;
    for(int core=0;core<4;core++){
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
    for(int neuron=0;neuron<cores[4].max_neurons;neuron++){

        if(cores[4].neurons[neuron].current_potential >= cores[4].neurons[neuron].pos_threshold){
            number[i] += 1;
            cores[4].neurons[neuron].current_potential = cores[4].neurons[neuron].reset_potential;

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

void active_network(struct core cores[]){
    int max_MNIST = 10000;
    int des_core = 0 ;
    int des_axon = 0;
    int num_inputs = 0;
    FILE *f_num_inputs=fopen("./testbench/tb_num_inputs.txt","r");
    if (f_num_inputs == NULL) {
        printf("Error opening file tb_num_inputs.txt\n");
        exit(1);
    }
    FILE *f_input=fopen("./testbench/tb_input.txt","r");
    if (f_input == NULL) {
        printf("Error opening file tb_inputs.txt\n");
        exit(1);
    }

    FILE *f_save_packet=fopen("./packet/packet_mnist.txt","w");
    FILE *f_predict=fopen("./predict/predict.txt","w");

    for(int mnist=0;mnist<max_MNIST;mnist++){
        int number[10]={0,0,0,0,0,0,0,0,0,0};
        fprintf(f_save_packet,"MNIST %d: num_inputs: %d\n",mnist+1,num_inputs=get_num_inputs(f_num_inputs));

        for(int packet=0;packet<num_inputs;packet++){
            get_des_packet(f_input,&des_core,&des_axon);
            fprintf(f_save_packet,"packet %d: des_core: %d ",packet+1,des_core);
            fprintf(f_save_packet," des_axon: %d\n",des_axon);
            neuron_block_update_potential(cores,&des_core,&des_axon);
        }

        check_threshold_and_fire(cores);

        get_vote_core_5(cores,number);

        get_predict(f_predict,number);

        //reset_core(cores);
    }
    fclose(f_input);
    fclose(f_num_inputs);
    fclose(f_save_packet);
    fclose(f_predict);
}        