# **ranc_simulator**
## Setup đường dẫn cho folder parameter
- Mỗi lần train python sẽ tạo ra bộ tham số khác nhau là 2 folder `streaming` và `testbench` để chạy code c.
- Để lưu lại mỗi lần chạy 1 bộ tham số, ta sẽ đưa 2 folder đó vào trong folder parameter1, parameter2,... trong folder parameter.
- Để code c chạy đúng, ta cần phải sửa đường dẫn tới folder parameter cần chạy, ta có thể sửa trong file `param.c` tại folder `neuron_network`:
```
#define PATH "D:/Code/RANC/c_code/parameter/parameter1"
```
- Hãy chỉnh lại đường dẫn theo đúng path mà bạn lưu trữ trên máy của mình, mỗi lần muốn đổi bộ tham số khác, hãy sửa `/parameter1` thành `/parameter2` hoặc `/parameterx` gì đó.

## Setup network
### Mạng 2 layers 5 cores
- Sử dụng 2 folder `parameter1` và `parameter2`
- Sửa tham số trong file `param.c` như sau:
  ```
  #define num_layer 2
  #define num_core_layer_1 4
  #define num_core_layer_2 1

  #define max_axon 256
  #define max_neuron_layer_1 64
  #define max_neuron_layer_2 250
  ```
- Trong hàm `create_network()` trong file `layer.c`, số `case` trong `switch(layer)` sẽ tương ứng với số layer, vậy nếu có 2 layer thì sẽ sử dụng `case 0` và `case 1`.
### Mạng 3 layers 7 cores
- Sử dụng folder `parameter3`
- Sửa tham số trong file `param.c` như sau:
  ```
  #define num_layer 3
  #define num_core_layer_1 4
  #define num_core_layer_2 2
  #define num_core_layer_3 1
  
  #define max_axon 256
  #define max_neuron_layer_1 64
  #define max_neuron_layer_2 128
  #define max_neuron_layer_3 250
  ```
- Trong hàm `create_network()` trong file `layer.c`, tương tự như trên thì 3 layers 7 cores sử dụng `case 0`, `case 1` và `case 2`.
### Mạng 4 layers 9 cores
- Sử dụng folder `parameter4`
- Sửa tham số trong file `param.c` như sau:
  ```
  #define num_layer 4
  #define num_core_layer_1 4
  #define num_core_layer_2 2
  #define num_core_layer_3 2
  #define num_core_layer_4 1
  
  #define max_axon 256
  #define max_neuron_layer_1 64
  #define max_neuron_layer_2 128
  #define max_neuron_layer_3 128
  #define max_neuron_layer_4 250
  ```
- Trong hàm `create_network()` trong file `layer.c`, tương tự như trên thì 4 layers 9 cores sử dụng `case 0`, `case 1`, `case 2` và `case 3`.
## Lưu ra file text
- Trong file `param.c`, `save_to_file` để ghi hoặc không ghi vào file text.
  ```
  #define save_to_file 0
  ```
- Nếu không ghi thì chạy nhanh hơn.
