
import numpy as np

# Thu thập votes sau khi mô phỏng
def collect_votes_from_simulator(filename, num_classes=10):
    """
    Given an output from a neural network's output core in the simulator, this function collapses all the votes into a score for each class
    See software/tealayers/tealayer2.0/additivepooling.py for more information about the strided layout of class labels 
    """
    votes = None
    # Mở file
    with open(filename) as file:

        lines = file.readlines()                        # Đọc từng dòng và lưu vào lines
        votes = np.zeros((len(lines), num_classes))     # Tạo ra ma trận cho votes
        
        #Loop qua từng line
        for linenum, line in enumerate(lines):
            # Mảng mỗi votes cho class ngăn nhau bởi ' '
            individual_votes = np.array(line.split(' '))
            # Loop qua từng class
            for class_num in range(num_classes):
                # Lọc số votes
                votes_for_class = list(filter(lambda vote: vote == '0' or vote == '1', individual_votes[class_num:len(individual_votes):num_classes]))
                # Chuyển votes thành integer và đưa vào list
                votes_for_class = list(map(lambda vote: int(vote), votes_for_class))
                # Tính tổng số votes cho class đố
                votes[linenum, class_num] = np.sum(votes_for_class)
    return votes

# Xác định predict lable cho input của network
def collect_classifications_from_simulator(filename, num_classes=10, verbose=False):
    """
    Given an output from a neural network's output core in the simulator, this function collapses all the votes into a score for each class
    And then collapses again by performing an argmax across a given row to determine the class label for a given input
    See software/tealayers/tealayer2.0/additivepooling.py for more information about the strided layout of class labels 
    """
    # Lấy số votes bằng hàm "collect_votes_from_simulator"
    votes = collect_votes_from_simulator(filename, num_classes=num_classes)
    # Tạo mảng có giá trị "-1" cho mỗi phần tử, độ dài bằng số hàng của "votes" (số class?)
    classifications = -1 * np.ones(votes.shape[0])
    # Loop qua từng hàng
    for i in range(votes.shape[0]):
        # Tìm index của class có số votes lớn nhất
        decision = np.where(votes[i, :] == max(votes[i, :]))[0]
        
        # Nếu có nhiều hơn 1 class có số votes mã -> Chọn class số bé nhất
        if verbose and len(decision) > 1:
            print(f"There's a tie in voting at index {i}. Possible classes are {decision}. Picking {decision[0]}")
        
        # Trả về dự đoán của network
        classifications[i] = decision[0]
    return classifications