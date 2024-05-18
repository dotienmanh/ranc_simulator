#include <stdio.h>
#include "neuron.c"
#include "axon.c"

#define max_core 5
#define max_axon 256
#define max_neuron_layer_1 64
#define max_neuron_layer_2 250

struct core{
    struct neuron *neurons;
    struct axon axons[max_axon];
    int max_neurons;
};

void create_core(struct core cores[max_core]){
    for (int core = 0; core < (max_core-1); ++core){
        cores[core].neurons = malloc(max_neuron_layer_1 * sizeof(struct neuron));
        cores[core].max_neurons = max_neuron_layer_1;
    }
    cores[max_core-1].neurons = malloc(max_neuron_layer_2 * sizeof(struct neuron));
    cores[max_core-1].max_neurons = max_neuron_layer_2;
}
