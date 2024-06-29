#include <stdio.h>
#include <stdlib.h>
#include "core.c"

struct layer{
    struct core *cores;
    int max_cores;
};

/*** Tạo layer ***/
void create_layer(struct layer* layer, int num_cores, int max_neurons) {
    layer->cores = malloc(num_cores * sizeof(struct core));
    layer->max_cores = num_cores;
    for (int i = 0; i < num_cores; ++i) {
        create_core(&layer->cores[i], max_neurons);
    }
}

/*** Tạo network ***/
void create_network(struct layer layers[]) {
    int num_core_layers[] = {num_core_layer_1, num_core_layer_2, num_core_layer_3, num_core_layer_4};
    int max_neuron_layers[] = {max_neuron_layer_1, max_neuron_layer_2, max_neuron_layer_3, max_neuron_layer_4};
    
    for (int i = 0; i < num_layer; ++i) {
        create_layer(&layers[i], num_core_layers[i], max_neuron_layers[i]);
    }

    // for (int i = 0; i < num_layer; ++i) {
    //     switch (i) {
    //         case 0:
    //             create_layer(&layers[i], num_core_layer_1, max_neuron_layer_1);
    //             break;
    //         case 1:
    //             create_layer(&layers[i], num_core_layer_2, max_neuron_layer_2);
    //             break;
    //         case 2:
    //             create_layer(&layers[i], num_core_layer_3, max_neuron_layer_3);
    //             break;
    //         case 3:
    //             create_layer(&layers[i], num_core_layer_4, max_neuron_layer_4);
    //             break;
    //     }
    // }
}
