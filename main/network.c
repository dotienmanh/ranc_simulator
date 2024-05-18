#include "active_network.c"

int main() {
    struct core cores[5];
    get_parameter(cores);
    active_network(cores);
    get_network_accuracy();
    return 0;
}
