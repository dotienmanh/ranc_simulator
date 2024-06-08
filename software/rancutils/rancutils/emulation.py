# Utlities for the RANC FPGA Emulation
from .neuron import Neuron
from .core import Core

import os
import struct
import math

from bitstring import BitArray
import numpy as np

# Viết thông số của cores vào CSRAM
def write_cores(cores,
                max_xy=(64, 64),
                potential_width=9,
                leak_width=9,
                weight_width=9,
                threshold_width=9,
                num_reset_modes=2,
                max_dx=512,
                max_dy=512,
                num_axons=256,
                num_neurons=256,
                num_ticks=16,
                num_weights=4,
                output_path='streaming/',   #output
                max_file_number=100,
                verbose=False):

    assert len(str(max_file_number)) >= len(str(max_xy[0]*max_xy[1]))

    # Define default cores for those that are not specified
    default_cores = []                                                      # Mảng Cores default
    default_axons = [0]*num_axons                                           # Mảng Axons default 0
    default_neuron = Neuron(                                                # 1 Neuron default với các tham số từ Class Neuron
        0, [0]*num_weights, 0, 1, 0, [0, 0], 0, 0                           
    )
    default_neurons = [default_neuron]*num_neurons                          # Mảng Neurons default
    default_connections = np.zeros((num_axons,                              # Mảng Connections default 0
                                    num_neurons)).astype(int).tolist()      # số hàng = số axons; số cột = số neurons

    # Tạo hàng trong mảng Cores 
    for x in range(max_xy[0]):
        cores_row = []
        # Tạo các cột trong mảng Cores với các tham số từ Class Core
        for y in range(max_xy[1]):
            cores_row.append(Core(default_axons,        # Axons default
                                  default_neurons,      # Neurons default
                                  default_connections,  # Connections default
                                  [x, y]))              # [x,y] là tọa độ coordinate
        default_cores.append(cores_row)                 # Đưa từng hàng vào mảng Cores default

    # Loop qua từng Cores
    for core in cores:
        default_cores[core.coordinates[0]][core.coordinates[1]] = core      # Đặt từng core vào tọa độ của nó

    # Loop qua từng core trong 1 hàng của "default_cores" -> đưa 2D về 1D
    for core_num, core in enumerate([core for row in default_cores
                                     for core in row]):
        if verbose:
            print('writing csram and tc memory: core', core_num + 1, '/',           # Viết vào CSRAM và tc memory
                  max_xy[0]*max_xy[1], end='\r')
        
        # Mở file CSRAM
        with open(os.path.join(output_path, 'csram_{}.mem'.format(
            str(core.coordinates[0] + max_xy[0] * core.coordinates[1]).zfill(
                len(str(max_file_number))
            )
        )), 'w+') as csram_file:
            # Loop qua từng neuron trong 1 core
            for neuron_num in range(num_neurons):
                # If there is a neuron defined
                if neuron_num < len(core.neurons):
                    
                    # Chuyển connections thành 1 mảng
                    core_connections_np = np.array(core.connections)

                    # Tạo neuron string ghi các thông số lấy từ hardware_str            
                    neuron_str = core.neurons[neuron_num].hardware_str(
                        potential_width,
                        leak_width,
                        weight_width,
                        threshold_width,
                        num_reset_modes,
                        max_dx,
                        max_dy,
                        num_axons,
                        num_ticks
                    )
                    # If there are connections defined for this neuron
                    if neuron_num < core_connections_np.shape[0]:

                        #Chuyển connections có sẵn thành string độ dài theo số axons
                        conns = ''.join(
                            [str(conn) for conn in np.flip(
                                core.connections[neuron_num],
                                axis=0)]
                        ).zfill(num_axons)
                    else:
                        # Chuyển thành string gồm các số 0, độ dài theo số axons
                        conns = ''.join(['0']*num_axons)
                # If there are no more defined neurons, use default
                else:                                                           
                    neuron_str = default_neuron.hardware_str(
                        potential_width,
                        leak_width,
                        weight_width,
                        threshold_width,
                        num_reset_modes,
                        max_dx,
                        max_dy,
                        num_axons,
                        num_ticks
                    )
                    # Conns là 0 hết, độ dài số axons
                    conns = ''.join(['0']*num_axons)

                # Viết conns và neuron_str vào CSRAM
                csram_file.write('{}{}\n'.format(conns, neuron_str))

        # Mở file ghi tc memory            
        with open(os.path.join(output_path, 'tc_{}.mem'.format(
            str(core.coordinates[0] + max_xy[0] * core.coordinates[1]).zfill(
                len(str(max_file_number))
            )
        )), 'w+') as tc_file:
            # Loop qua từng axon trong 1 core và viết vào file theo dạng Bit
            for axon in core.axons:
                tc_file.write('{}\n'.format(
                    BitArray(uint=axon, length=num_weights.bit_length()-1).bin)
                )
            # Nếu số axons ít hơn so với tổng số axons, ghi 0 vào các axons còn lại
            for _ in range(num_axons - len(core.axons)):
                tc_file.write('{}\n'.format(
                    BitArray(uint=0, length=num_weights.bit_length()-1).bin)
                )
    # DONE
    if verbose:
        print('writing csram and tc memory: done', end='\r')
        print()

# Tạo output packets cho hardware trong vmm
def output_for_streaming(cores,
                         packets=None,
                         max_xy=(64, 64),
                         potential_width=9,
                         leak_width=9,
                         weight_width=9,
                         threshold_width=9,
                         num_reset_modes=2,
                         max_dx=512,
                         max_dy=512,
                         num_axons=256,
                         num_neurons=256,
                         num_ticks=16,
                         num_weights=4,
                         max_packet_xy=(512, 512),
                         output_path='streaming/',              # Output
                         packet_file_name='data.bin',           # File data packets
                         packet_count_file_name='count.bin',    # File count packets
                         max_file_number=100,
                         verbose=False):
    # Tạo output path nếu chưa có
    try:
        os.makedirs(output_path)
    except FileExistsError:
        pass

    # Nếu packets không rỗng
    if not (packets is None):
        # Mở 2 file "packet data" và "packet count"
        with open(os.path.join(output_path, packet_file_name),
                  'wb') as packet_file, \
            open(os.path.join(output_path, packet_count_file_name),
                 'wb') as packet_count_file:

            # Loop qua từng "tick" trong packets
            for tick_index, tick in enumerate(packets):
                # Loop qua từng packet trong 1 "tick"
                for packet_index, packet in enumerate(tick):

                    # Convert hardware_str của packet thành integer
                    # và ghi vào file packet data
                    packet_file.write(
                        struct.pack('i', int(packet.hardware_str(
                            max_dx=max_packet_xy[0],
                            max_dy=max_packet_xy[1],
                            num_axons=num_axons,
                            num_ticks=num_ticks), 2))
                    )
                # Ghi số packet tại "tick" đó vào file packet_count
                packet_count_file.write(struct.pack('i', len(tick)))
                # Cứ 100 ticks sẽ ghi qua trình generating
                if verbose and tick_index % 100 == 0:
                    print('generating packets: tick', tick_index + 1, '/',
                          len(packets), end='\r')
        #DONE
        if verbose:
            print('generating packets: done', end='\r')
            print()

    # Gọi hàm ghi các thông số Cores vào CSRAM
    write_cores(cores,
                max_xy=max_xy,
                potential_width=potential_width,
                leak_width=leak_width,
                weight_width=weight_width,
                threshold_width=threshold_width,
                num_reset_modes=num_reset_modes,
                max_dx=max_dx,
                max_dy=max_dy,
                num_axons=num_axons,
                num_neurons=num_neurons,
                num_ticks=num_ticks,
                num_weights=num_weights,
                output_path=output_path,
                max_file_number=max_file_number,
                verbose=verbose)

# tạo output cho testbench, 
def output_for_testbench(packets,
                         correct_outputs,                           # correct output
                         output_path='testbench/',                  # output 
                         input_filename='tb_input.txt',             # testbench input
                         correct_filename='tb_correct.txt',         # testbench correct
                         num_inputs_filename='tb_num_inputs.txt',   # number file inputs
                         num_outputs_filename='tb_num_outputs.txt', # number file outputs
                         max_packet_xy=(512, 512),
                         num_axons=256,
                         num_ticks=16,
                         num_outputs=250):
    # Tạo đường path cho output nếu chưa có
    try:
        os.makedirs(output_path)
    except FileExistsError:
        pass
    
    # Mở file input và number_inputs
    with open(os.path.join(output_path, input_filename), 'w') as f, \
        open(os.path.join(output_path, num_inputs_filename),
             'w') as num_inputs_f:

        # Loop qua từng tick và các packets trong tick đó
        for tick, packets_tick in enumerate(packets):
            # Ghi số packets_tick dưới dạng string vào number_inputs
            num_inputs_f.write(str(len(packets_tick)) + '\n')
            # Loop qua từng packet trong 1 ticks
            for packet_num, packet in enumerate(packets_tick):
                # Nếu có packets với tick khác 0
                if tick != 0 or packet_num != 0:
                    f.write('\n')
                # Ghi tham số packet từ hardware_str vào file input
                f.write(packet.hardware_str(max_dx=max_packet_xy[0],
                                            max_dy=max_packet_xy[1],
                                            num_axons=num_axons,
                                            num_ticks=num_ticks))

    # Mở file correct và number_outputs
    with open(os.path.join(output_path, correct_filename), 'w') as f, \
        open(os.path.join(output_path, num_outputs_filename),
             'w') as num_outputs_f:

        # Loop qua từng correct_tick trong correct_outputs
        for tick, correct_tick in enumerate(correct_outputs):
            # Ghi vào number_outputs số correct_tick không có giá trị 0
            num_outputs_f.write(str(len(np.nonzero(correct_tick)[0])) + '\n')
            
            for correct_num, correct in enumerate(np.nonzero(correct_tick)[0]):
                if tick != 0 or correct_num != 0:
                    f.write('\n')
                f.write(BitArray(
                    uint=int(correct),
                    length=math.ceil(math.log(num_outputs, 2))
                ).bin)
