#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>


void get_des_packet(FILE *f_input,int *des_core, int *des_axon){
    int des_dy = 0;
    int des_dx = 0;
    char line[30];
    if (fscanf(f_input, "%s", line) != 1){
        exit(1);
    }

    int dx[9];
    for (int i=0; i<9;i++){
        dx[i] = line[i] - '0';
    }
    int dy[9];
    for (int i=0;i<9;i++){
        dy[i] = line[i+9] - '0';
    }
    des_dx=binaryToDecimalSigned(dx,sizeof(dx)/sizeof(dx[0]));
    des_dy=binaryToDecimalSigned(dy,sizeof(dy)/sizeof(dy[0]));
    *des_core = des_dx + 4*des_dy;

    int des_axon_temp[8];
    for (int i=0;i<8;i++){
        des_axon_temp[i]=line[i+18] - '0';
    }
    *des_axon = binaryToDecimalUnsigned(des_axon_temp,sizeof(des_axon_temp)/sizeof(des_axon_temp[0]));
}

int get_num_inputs(FILE *f_num_inputs){
    int num_inputs = 0;
    if (fscanf(f_num_inputs,"%d",&num_inputs) !=1) exit(1);
    return num_inputs;
}