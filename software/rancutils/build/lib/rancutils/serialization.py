from .packet import Packet
from .neuron import Neuron
from .core import Core

import json
import numpy as np

# Lưu danh sách cores và packets vào file json
def save(filename, cores, packets, output_bus, indent=None):
    """Saves a list of cores and packets to a json file.

    Arguments:
        filename -- The name of the file to save to.
        cores -- A list of core objects to save.
        packets -- A 2D list of packet objects to save
                            (a list for each tick)

    Keyword Arguments:
        indent -- The amount to indent the json. If none, json is
                  written to one line. (default: {None})"""
    # Mở file và mã hóa các tham số rồi viết vào file
    with open(filename, 'w+') as f:
        f.write(encode(cores, packets, output_bus, indent=indent))

# Mã hóa cores và packets để viết vào file json
def encode(cores, packets, output_bus, indent=None):
    """Encodes a list of cores and packets to json

    Arguments:
        cores -- A list of core objects to save.
        packets -- A 2D list of packet objects to save
                   (a list for each tick)
        output_bus -- The core that collects output

    Keyword Arguments:
        indent -- The amount to indent the json. If none, json is
                  written to one line. (default: {None})

    Returns:
        A string of json.
    """
    d = {'packets': packets, 'output_bus': output_bus, 'cores': cores}
    # Convert các object thành object trong json file
    return json.dumps(d, default=_serialize, indent=indent)

# Giải mã file input parent_json để có được list packets 2D
def parse_packets(parent_json):
    """Parses a json string for a 2D list of packets.

    Arguments:
        parent_json -- The json string to parse. If it contains a 'packets'
                       object, this object should either be empty or be a 2D
                       list containing a list of packets for each tick.

    Returns:
        A 2D list of packet objects if packets were present in json,
            otherwise None.

    Raises:
        TypeError -- If packets is not empty but also not a 2D list, or if an
                     individual packet does not have the correct parameters.
    """
    # Kiểm tra packets có trong file json không?
    if 'packets' not in parent_json:
        return None
    # tạo packets_json từ 'packets' trong file input parent_json
    packets_json = parent_json['packets']
    if not packets_json:
        return None
    
    # Kiểm tra xem packet trong json có phải dạng 2D list không?
    if (not isinstance(packets_json, list) or
       not isinstance(packets_json[0], list)):
        raise(TypeError, 'Packets json object is not a 2D list.')

    packets = []            # Tạo mảng cho packets
    # Loop qua từng "tick" trong packets_json
    for packets_json_tick in packets_json:

        packets_tick = []   # Tạo mảng cho tick trong packets
        # Loop qua từng packet trong 1 "tick"
        for packet_json in packets_json_tick:
            packet = Packet(packet_json['destination_core'],    # Các thông số trong packets
                            packet_json['destination_axon'],
                            packet_json['destination_tick'])
            packets_tick.append(packet)       # Đưa từng packet vào mảng tick đó
        packets.append(packets_tick)          # Đưa từng mảng "tick" vào trong packets

    return packets

# Giải mã file input neuron_json để có được neuron
def parse_neuron(neuron_json):
    """Parses a json string for a neuron object.

    Arguments:
        neuron_json -- The json string for the neuron.

    Returns:
        The neuron object.
    """
    return Neuron(neuron_json['reset_potential'],      # Các thông số của 1 neuron
                  neuron_json['weights'],              # trong file neuron_json
                  neuron_json['leak'],
                  neuron_json['positive_threshold'],
                  neuron_json['negative_threshold'],
                  neuron_json['destination_core'],
                  neuron_json['destination_axon'],
                  neuron_json['destination_tick'],
                  neuron_json['current_potential'],
                  neuron_json['reset_mode'])

# Giải mã file input parent_json để có được list các cores
def parse_cores(parent_json):
    """Parses a json string for a list of cores.

    Arguments:
        parent_json -- The json string to parse. If it contains a 'cores'
                       object, this object should either be empty or be a
                       list of cores.

    Returns:
        A list of core objects if cores were present in json, otherwise None.

    Raises:
        TypeError -- If a core does not have the correct parameters."""
    # Kiểm tra xem file có chứa "cores" không
    if 'cores' not in parent_json:
        return None
    # Tạo cores_json từ 'cores' trong parent_json
    cores_json = parent_json['cores']
    if not cores_json:
        return None
    # Kiểm tra xem cores_json có phải là list không
    if not isinstance(cores_json, list):
        raise(TypeError, 'Cores json object is not a list.')


    cores = []              # Tạo mảng Cores

    # Loop qua từng core
    for core_json in cores_json:

        neurons = []        # Tạo mảng neurons
        # Loop qua từng neuron trong 1 core
        for neuron_json in core_json['neurons']:
            # Giải mã neuron bằng hàm "parse_neuron" rồi đưa vào mảng neurons
            neurons.append(parse_neuron(neuron_json))
        # Đưa vào mảng cores các thông số lưu tại core_json
        cores.append(Core(core_json['axons'],
                          neurons,
                          core_json['connections'],
                          core_json['coordinates']))

    return cores

# Đọc data từ file json -> giải mã -> ghi vào list packets và cores
def load(filename):
    """Loads a list of packets and cores from a json file.

    Arguments:
        filename -- The name of the file to read from.

    Returns:
         The packets and cores that were read.
    """
    # Mở và đọc file json
    with open(filename, 'r') as f:
        json_data = f.read()
    # lấy data từ file json
    data = json.loads(json_data)

    # Giải mã packets và cores
    packets = parse_packets(data)
    cores = parse_cores(data)

    # nếu list "packets" và "cores" rỗng -> Error
    if packets is None and cores is None:
        raise(ValueError, 'json file did not have packets or cores to parse.')

    return packets, cores

# Dùng trong json.dump ở hàm "encode"
def _serialize(obj):
    """Helper function so that numpy arrays can be serialized correctly."""
    if isinstance(obj, np.ndarray):
        return obj.tolist()
    if isinstance(obj, np.int32) or isinstance(obj, np.int64):
        return int(obj)
    else:
        return obj.__dict__
