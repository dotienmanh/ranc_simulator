#include <stdio.h>
#include <stdlib.h>
#include "neuron.c"
#include "axon.c"
#include "param.c"
#include "get_packet.c"


struct core{
    struct neuron *neurons;
    struct axon axons[max_axon];
    int max_neurons;
    struct Queue *queue;
};

void create_core(struct core* core, int max_neurons) {
    core->neurons = malloc(max_neurons * sizeof(struct neuron));
    core->max_neurons = max_neurons;
    core->queue = createQueue();
}