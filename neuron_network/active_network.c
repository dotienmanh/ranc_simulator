#include <unistd.h>
#include "get_parameter.c"
#include "get_network_accuracy.c"
#include "param.c"

/*** Lấy packet từ mnist */
void get_packet_from_mnist(FILE *f_input, FILE *f_save_packet,FILE *f_num_inputs,int mnist,struct layer layers[]){
    int num_inputs = 0;
    num_inputs=get_num_inputs(f_num_inputs);
    if(save_to_file){
        fprintf(f_save_packet,"MNIST %d: num_inputs: %d\n",mnist+1,num_inputs);
    }
    for(int packet=0;packet<num_inputs;packet++){
        int des_layer;
        int des_core;
        int des_axon;
        get_des_packet(f_input,&des_layer,&des_core,&des_axon);
        if(save_to_file){
            (f_save_packet,"packet %d: des_layer:%d; des_core: %d; ",packet+1,des_layer, des_core);
            fprintf(f_save_packet,"des_axon: %d\n",des_axon);
            fflush(f_save_packet);
        }
        enqueue(layers[des_layer].cores[des_core].queue,des_axon);
    }
}

/***  Neuron integrate potential ***/
void integrate(int layer,struct layer layers[]){
    for (int core = 0; core < layers[layer].max_cores; core++){
        while(!isEmpty(layers[layer].cores[core].queue)) {
            int des_axon;
            des_axon = dequeue(layers[layer].cores[core].queue);
            for (int neuron=0; neuron < layers[layer].cores[core].max_neurons; neuron++){
                if(layers[layer].cores[core].neurons[neuron].conns[des_axon]){
                    layers[layer].cores[core].neurons[neuron].current_potential += layers[layer].cores[core].neurons[neuron].formatted_weight[layers[layer].cores[core].axons[des_axon].axon_type];
                }
            }
        }
    }
}

/*** Neuron leak potential*/
void leaky(struct layer layers[],int layer){
    for (int core=0; core < (layers[layer].max_cores); core++){
        for (int neuron=0; neuron < layers[layer].cores[core].max_neurons; neuron++){
            layers[layer].cores[core].neurons[neuron].current_potential += layers[layer].cores[core].neurons[neuron].leak;
        }
    }
}

/*** Neuron bắn ***/
void fire(struct layer layers[], int layer){
    for(int core=0;core<(layers[layer].max_cores);core++){
        for(int neuron=0; neuron < layers[layer].cores[core].max_neurons;neuron++){
            if(layers[layer].cores[core].neurons[neuron].current_potential >= layers[layer].cores[core].neurons[neuron].pos_threshold){
                layers[layer].cores[core].neurons[neuron].current_potential = layers[layer].cores[core].neurons[neuron].reset_potential;
                int des_core = core + layers[layer].cores[core].neurons[neuron].spike_des_core[0];
                int des_layer = layer + layers[layer].cores[core].neurons[neuron].spike_des_core[1];
                int des_axon = layers[layer].cores[core].neurons[neuron].spike_des_axon;
                enqueue(layers[des_layer].cores[des_core].queue,des_axon);
            }
            if(layers[layer].cores[core].neurons[neuron].current_potential < layers[layer].cores[core].neurons[neuron].neg_threshold){
                layers[layer].cores[core].neurons[neuron].current_potential = layers[layer].cores[core].neurons[neuron].reset_potential;
            }
        }
    }
}

/*** Lấy vote ở output ***/
void get_vote(struct layer layers[], int number[]){
    for(int core = 0; core < layers[(num_layer-1)].max_cores; core++){
        for(int neuron = 0; neuron < layers[(num_layer-1)].cores[core].max_neurons;neuron++){
            if(layers[(num_layer-1)].cores[core].neurons[neuron].current_potential >= layers[(num_layer-1)].cores[core].neurons[neuron].pos_threshold){
                layers[(num_layer-1)].cores[core].neurons[neuron].current_potential = layers[(num_layer-1)].cores[core].neurons[neuron].reset_potential;
                number[(neuron)%class] += 1;
            }
            if(layers[(num_layer-1)].cores[core].neurons[neuron].current_potential < layers[(num_layer-1)].cores[core].neurons[neuron].neg_threshold);
                layers[(num_layer-1)].cores[core].neurons[neuron].current_potential = layers[(num_layer-1)].cores[core].neurons[neuron].reset_potential;
        }
    }
}

/*** Lưu vote của từng ảnh MNIST ***/
void save_network_vote_class(FILE *f_save_network_predict,int number[],int mnist,int predict){
    fprintf(f_save_network_predict,"Vote for image    %d\n",mnist+1);
    for(int num=0;num<class;num++){
        fprintf(f_save_network_predict,"Number %d: %d\n",num,number[num]);
    }
    fprintf(f_save_network_predict,"Network predict: number %d\n",predict);
    fprintf(f_save_network_predict,"------------------------\n");
}

/*** Chọn ra dự đoán cho network sau khi các neuron ở core đầu ra đã vote cho 10 class */
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
    fflush(f_predict);
    if(save_to_file){
        save_network_vote_class(f_save_network_predict,number,mnist,predict);
        fflush(f_save_network_predict);
    }
}

/*** Reset network sau mỗi lần đọc ảnh ***/
void reset_network(struct layer layers[]){
    for (int layer=0;layer < num_layer; layer++){
        for(int core=0; core < layers[layer].max_cores;core++){
            for(int neuron=0; neuron < layers[layer].cores[core].max_neurons; neuron++){
                layers[layer].cores[core].neurons[neuron].current_potential=0;
            }
        }
    }
}

void active_network(struct layer layers[]){
    /*** Tạo đường dẫn lưu, đọc file ***/
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
    
    printf("Activating Network...\n");

    int num_inputs = 0;     

    for(int mnist=0;mnist<max_mnist;mnist++){
        int number[class];
        for(int c=0;c<class;c++){
            number[c]=0;
        }
        if(mnist%1000==0 & mnist != 0){
            printf("Loading MNIST %d/10000\n", mnist);
        }
        get_packet_from_mnist(f_input,f_save_packet,f_num_inputs,mnist,layers);
        for (int layer=0; layer< num_layer; layer++){
            integrate(layer,layers);
            leaky(layers,layer);
            if (layer != (num_layer-1)){
                fire(layers,layer);
            }
        }
        get_vote(layers,number);
        get_predict(f_predict,f_save_network_predict,number,mnist);
        reset_network(layers);
    }
    fclose(f_input);
    fclose(f_num_inputs);
    fclose(f_save_packet);
    fclose(f_predict);
    fclose(f_save_network_predict);
}        