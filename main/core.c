#include "neuron.c"
#include "axon.c"
struct core{
    struct neuron *neurons;
    struct axon axons[256];
    int max_neurons;
};

void create_core(struct core cores[5]){
    for (int core=0; core<4; ++core){
        cores[core].neurons = malloc(64 * sizeof(struct neuron));
        cores[core].max_neurons = 64;
    }
    cores[4].neurons = malloc(250 * sizeof(struct neuron));
    cores[4].max_neurons = 250;
}
