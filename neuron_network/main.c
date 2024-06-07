#include "active_network.c"

int main() {
    struct layer layers[num_layer];
    get_parameter(layers);
    active_network(layers);
    get_network_accuracy();
    return 0;
}
