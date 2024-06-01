# **ranc_simulator**
## Lưu ý setup đường dẫn cho folder parameter
- Mỗi lần train python sẽ tạo ra bộ tham số khác nhau là 2 folder `streaming` và `testbench` để chạy code c.
- Để lưu lại mỗi lần chạy 1 bộ tham số, ta sẽ đưa 2 folder đó vào trong folder parameter1, parameter2,... trong folder parameter.
- Để code c chạy đúng, ta cần phải sửa đường dẫn tới folder parameter cần chạy, ta có thể sửa trong file `param.c` tại folder `neuron_network`:
```
#define PATH "D:/Code/RANC/c_code/parameter/parameter1"
```
- Hãy chỉnh lại đường theo đúng path mà bạn lưu trữ trên máy của mình, mỗi lần muốn đổi bộ tham số khác, hãy sửa `/parameter1` thành `/parameter2` hoặc `/parameterx` gì đó.
