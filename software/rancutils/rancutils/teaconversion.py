from .neuron import Neuron
from .core import Core
from .packet import Packet

import numpy as np

def custom_clip(arr):
    """
    Clips the input array such that values >= 0.8 are set to 1
    and values < 0.8 are set to 0.
    
    Parameters:
    arr (np.ndarray): Input array to be clipped.

    Returns:
    np.ndarray: Clipped array.
    """
    # Ensure the input is a NumPy array
    arr = np.asarray(arr)
    
    # Apply the custom clipping
    clipped_arr = np.where(arr > 0.7, 1, 0)
    
    return clipped_arr
    
# Lấy input là model của network và số layers của network
# trả về mảng của "connections" và "bias"
def get_connections_and_biases(model, num_layers):
    # Lấy weights từ model (??)
    weights = model.get_weights()

    names = [weight.name for layer in model.layers for weight in layer.weights]
    
    # Tạo mảng để lưu connections và bias cho từng layer
    connections = [[] for i in range(num_layers)]
    biases = [[] for i in range(num_layers)]

    # Loop qua từng weight và tên
    for weight, name in zip(weights, names):
        layer_id = int(name.split('/')[0].split('_')[1]) - 1
        data_type = name.split('/')[1]

        # Nếu có "connections" -> đưa weight vào
        if 'connection' in data_type:
            connections[layer_id].append(weight)
        # Nếu không có "connections" -> check "bias" -> nếu có đưa weights vào
        elif 'bias' in data_type:
            biases[layer_id].append(weight)

    return connections, biases


# Tạo cores
def create_cores(
    model, num_layers, core_coordinates=None,
    #sửa weight
    #weights=[2, -2, 1, -1],
    weights=[1, -1, 1, -1],
    num_classes=10, num_neurons=256,
    neuron_reset_type=0
):
    # Lấy connections và biases của core bằng hàm "get..."
    connections, biases = get_connections_and_biases(model, num_layers)

    # Khởi tạo shape của core và size của layer (??)
    core_shapes = [[core.shape for core in layer] for layer in connections]
    layer_sizes = [len(layer) for layer in connections]

    # If core coordinates are not specified, each layer is given a row
    if core_coordinates is None:
        core_coordinates = []

        # Gán tọa độ của core theo default của core_shapes và layer
        for i, layer in enumerate(core_shapes):
            layer_coordinates = []
            for j in range(len(layer)):
                layer_coordinates.append((j, i))
            core_coordinates.append(layer_coordinates)

    # Define core objects
    cores = []
    # Loop qua từng thông số layer
    for layer_coordinates, layer_shapes, layer_size, connections_layer, \
        bias_layer in zip(core_coordinates, core_shapes, layer_sizes,
                          connections, biases):

        layer_cores = []    # Mảng chứa cores trong 1 layer

        # Loop qua từng thông số core trong 1 layer
        for single_core_coordinates, core_shape, connections_core, bias_core \
            in zip(layer_coordinates,
                   layer_shapes,
                   connections_layer,
                   bias_layer):
            # FIXME: Simulator assumes connections are formatted as
            # (neurons, axons). Probably not intuitive

            #sửa clip
            connections_constrained = np.round(connections_core)
            connections_constrained = custom_clip(connections_constrained)

            # connections_constrained = np.round(connections_core)                # Làm tròn
            # connections_constrained = np.clip(connections_constrained, 0, 1)    # Xấp xỉ về 0 hoặc 1
            
            # Đưa vào core vào layer_cores
            layer_cores.append(
                # sửa weight
                #Core(axons=[0, 1, 2, 3]*int(core_shape[0]/4),
                Core(axons=[0, 1]*int(core_shape[0]/2),
                     neurons=None,
                     connections=connections_constrained.T.astype(
                         int
                     ).tolist(),
                     coordinates=single_core_coordinates)
            )
        # Đưa layer_cores vào list cores
        cores.append(layer_cores)

    # Define neuron objects
    layer_num = 0

    # Loop qua từng thông số của layer
    for layer_cores, layer_shapes, layer_size, connections_layer, \
        bias_layer in zip(cores, core_shapes, layer_sizes,
                          connections, biases):
        # The number of cores to offset the in the destination
        core_offset = 0     # Xác định vị trí core của neuron
        axon_offset = 0     # Xác định vị trí neuron trong 1 core

        # Loop qua từng thông số core trong layer
        for core, core_shape, connections_core, bias_core in zip(
            layer_cores, layer_shapes, connections_layer, bias_layer
        ):
            neurons = []
            # Iterate through neurons
            for connections_neuron, bias_neuron in zip(
                connections_core.T, bias_core
            ):
                # Xác định tọa độ core của neuron dựa vào core_offset
                if (layer_num != len(cores) - 1):
                    coords = (
                        cores[layer_num+1][core_offset].coordinates[0] -
                        core.coordinates[0],
                        cores[layer_num+1][core_offset].coordinates[1] -
                        core.coordinates[1])
                else:
                    # Output bus is given its own row
                    coords = (-core.coordinates[0], 1)
                # Đưa các neuron vào list neurons
                # sửa pos-neg_threshold 
                neurons.append(Neuron(reset_potential=0,
                                      weights=weights,
                                      leak=int(np.round(bias_neuron)),
                                      positive_threshold=1,
                                      negative_threshold=0,
                                      #positive_threshold=0,
                                      #negative_threshold=0,
                                      destination_core=coords,
                                      destination_axon=axon_offset,
                                      destination_tick=0,
                                      current_potential=0,
                                      reset_mode=neuron_reset_type))
                
                axon_offset += 1       # Tăng axon_offset

                # Nếu axon_offset đạt tới max trong 1 core -> reset axons offet về 0 và tăng core offset
                if layer_num < len(core_shapes) - 1:
                    if axon_offset == \
                       core_shapes[layer_num+1][core_offset][0]:
                        core_offset += 1
                        axon_offset = 0
            
            # Gán neurons vào trong core đang xét
            core.neurons = neurons
        # Tăng layer lên 1
        layer_num += 1

    # Flatten cores
    return [c for cl in cores for c in cl]

# Tạo packet
def create_packets(packets, core_coordinates=None):
    # If no coordinates specified, feed to the first row
    # Nếu không có tọa độ core, đưa các packet vào hàng đầu tiên
    if core_coordinates is None:
        core_coordinates = []
        for i in range(len(packets)):
            core_coordinates.append((i, 0))

    output_packets = []

    # Loop qua từng core trong list packets
    for core_num, core in enumerate(packets):

        # Loop qua từng "tick" trong 1 core
        for tick_num, tick in enumerate(core):
            packets_tick = []   # Mảng cho các packets tại tick đang xét
            # Loop qua từng packet trong 1 "tick"
            for packet_num, packet in enumerate(tick):

                # nếu như packet=1 (có spike)
                if (np.round(packet) == 1):
                    # Đưa packet vào trong list packets_tick
                    packets_tick.append(
                        Packet(destination_core=core_coordinates[core_num],
                               destination_axon=packet_num,
                               destination_tick=0)
                    )
            # nếu trong output chưa có "tick" cho packets_tick, thì sẽ thêm tiếp packets_tick vào output
            if len(output_packets) - 1 < tick_num:
                output_packets.append(packets_tick)
            # Nếu trong output đã có sẵn "tick" cho packets_tick đó rồi, thì sẽ thêm vào đúng "tick" đó
            else:
                output_packets[tick_num] += packets_tick

    return output_packets
