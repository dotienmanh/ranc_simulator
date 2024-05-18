#include "active_network.c"

int main() {
    struct core cores[max_core];
    get_parameter(cores);
    active_network(cores);
    get_network_accuracy();
    return 0;
}
