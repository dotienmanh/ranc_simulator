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
