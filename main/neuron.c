struct neuron {
    int conns[256];
    int current_potential;
    int reset_potential;
    int formatted_weight[4];
    int leak;
    int pos_threshold;
    int neg_threshold;
    int reset_mode;
    int spike_des_core[2];
    int spike_des_axon;
    int spike_tick;
};
