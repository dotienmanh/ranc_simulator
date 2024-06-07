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

void get_packet_from_mnist(FILE *f_input, FILE *f_save_packet,FILE *f_num_inputs,int mnist,struct layer layers[], struct packet temp_packet, struct Queue *queue){
    int num_inputs = 0;
    fprintf(f_save_packet,"MNIST %d: num_inputs: %d\n",mnist+1,num_inputs=get_num_inputs(f_num_inputs));
    for(int packet=0;packet<num_inputs;packet++){
        get_des_packet(f_input,&temp_packet);
        //printf("dx: %d dy: %d axon: %d\n",temp_packet.des_core_dx,temp_packet.des_core_dy,temp_packet.des_axon);
        fprintf(f_save_packet,"packet %d: des_layer:%d; des_core: %d; ",packet+1,temp_packet.des_core_dy, temp_packet.des_core_dx);
        fprintf(f_save_packet,"des_axon: %d\n",temp_packet.des_axon);
        fflush(f_save_packet);
        enqueue(queue,temp_packet);
    }
}

void integrate(struct Queue *queue, int layer, struct packet temp_packet,struct layer layers[]){
    while (!isEmpty(queue)) {
        temp_packet = dequeue(queue);
        int des_layer = temp_packet.des_core_dy;
        int des_core = temp_packet.des_core_dx;
        int des_axon = temp_packet.des_axon;
        for (int neuron=0; neuron < layers[des_layer].cores[des_core].max_neurons; neuron++){
            if(layers[des_layer].cores[des_core].neurons[neuron].conns[des_axon]){
                layers[des_layer].cores[des_core].neurons[neuron].current_potential += layers[des_layer].cores[des_core].neurons[neuron].formatted_weight[layers[layer].cores[des_core].axons[des_axon].axon_type];
            }
        }
    }
}

void leaky(struct layer layers[],int layer){
    for (int core=0; core < (layers[layer].max_cores); core++){
        for (int neuron=0; neuron < layers[layer].cores[core].max_neurons; neuron++){
            layers[layer].cores[core].neurons[neuron].current_potential += layers[layer].cores[core].neurons[neuron].leak;
        }
    }
}

void fire(struct layer layers[], struct Queue *queue, struct packet temp_packet, int layer){
    for(int core=0;core<(layers[layer].max_cores);core++){
        for(int neuron=0; neuron < layers[layer].cores[core].max_neurons;neuron++){
            if(layers[layer].cores[core].neurons[neuron].current_potential >= layers[layer].cores[core].neurons[neuron].pos_threshold){
                layers[layer].cores[core].neurons[neuron].current_potential = layers[layer].cores[core].neurons[neuron].reset_potential;
                temp_packet.des_core_dx = core + layers[layer].cores[core].neurons[neuron].spike_des_core[0];
                temp_packet.des_core_dy = layer + layers[layer].cores[core].neurons[neuron].spike_des_core[1];
                temp_packet.des_axon = layers[layer].cores[core].neurons[neuron].spike_des_axon;
                enqueue(queue,temp_packet);
            }
            if(layers[layer].cores[core].neurons[neuron].current_potential < layers[layer].cores[core].neurons[neuron].neg_threshold){
                layers[layer].cores[core].neurons[neuron].current_potential = layers[layer].cores[core].neurons[neuron].reset_potential;
            }
        }
    }
}

void get_vote(struct layer layers[],struct Queue *queue, struct packet temp_packet, int number[]){
    while (!isEmpty(queue)) {
        temp_packet = dequeue(queue);
        number[(temp_packet.des_axon) % class] += 1;
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
    save_network_vote_class(f_save_network_predict,number,mnist,predict);
    fflush(f_save_network_predict);
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
    /*** Tạo đường dẫn lưu, đọc fiile ***/
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
    struct packet temp_packet;

    for(int mnist=0;mnist<max_mnist;mnist++){
        struct Queue* queue = createQueue();
        int number[class];
        for(int c=0;c<class;c++){
            number[c]=0;
        }
        if(mnist%1000==0 & mnist != 0){
            printf("Loading MNIST %d/10000\n", mnist);
        }
        get_packet_from_mnist(f_input,f_save_packet,f_num_inputs,mnist,layers,temp_packet,queue);
        for (int layer=0; layer<num_layer; layer++){
            integrate(queue,layer,temp_packet,layers);
            leaky(layers,layer);
            fire(layers,queue,temp_packet,layer);
        }
        // for (int x=0;x<4;x++){
        //     for (int y=0;y<64;y++){    
        //         printf("core %d neuron %d potential: %d\n",x,y,layers[0].cores[x].neurons[y].current_potential);
        //     }
        // }
        
        get_vote(layers,queue,temp_packet,number);
        clearQueue(queue);
        // for(int i=0;i<class;i++){
        //     printf("%d \n",number[i]);
        // }
        get_predict(f_predict,f_save_network_predict,number,mnist);
        reset_network(layers);
    }
    fclose(f_input);
    fclose(f_num_inputs);
    fclose(f_save_packet);
    fclose(f_predict);
    fclose(f_save_network_predict);
}        