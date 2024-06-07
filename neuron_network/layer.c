#include <stdio.h>
#include <stdlib.h>
#include "core.c"

struct layer{
    struct core *cores;
    int max_cores;
};

/*** Tạo network ***/
void create_network(struct layer layers[num_layer]){
    for (int layer=0; layer<num_layer; layer++){
        switch(layer){
            /** layer 0 **/
            case 0:
                layers[layer].cores = malloc(num_core_layer_1 * sizeof(struct core));
                layers[layer].max_cores = num_core_layer_1;
                for (int core = 0; core < num_core_layer_1; ++core){
                    layers[layer].cores[core].neurons = malloc(max_neuron_layer_1 * sizeof(struct neuron));
                    layers[layer].cores[core].max_neurons = max_neuron_layer_1;
                }
                break;
            /** layer 1 **/
            case 1:
                layers[layer].cores = malloc(num_core_layer_2 * sizeof(struct core));
                layers[layer].max_cores = num_core_layer_2;
                for (int core = 0; core < (num_core_layer_2); ++core){
                    layers[layer].cores[core].neurons = malloc(max_neuron_layer_2 * sizeof(struct neuron));
                    layers[layer].cores[core].max_neurons = max_neuron_layer_2;
                }
                break;
            /** layer 2 **/
            case 2:
                layers[layer].cores = malloc(num_core_layer_3 * sizeof(struct core));
                layers[layer].max_cores = num_core_layer_3;
                for (int core = 0; core < (num_core_layer_3); ++core){
                    layers[layer].cores[core].neurons = malloc(max_neuron_layer_3 * sizeof(struct neuron));
                    layers[layer].cores[core].max_neurons = max_neuron_layer_3;
                }
                break;
            /** layer 3 **/
            // case 3:
            //     layers[layer].cores = malloc(num_core_layer_4 * sizeof(struct core));
            //     layers[layer].max_cores = num_core_layer_4;
            //     for (int core = 0; core < (num_core_layer_4); ++core){
            //         layers[layer].cores[core].neurons = malloc(max_neuron_layer_4 * sizeof(struct neuron));
            //         layers[layer].cores[core].max_neurons = max_neuron_layer_4;
            //     }
            //     break;
        }
    }
}