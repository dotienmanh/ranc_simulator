#include <stdio.h>
#include <stdlib.h>
#include "packet.c"

#define MAX_QUEUE_SIZE 1024 // Giới hạn kích thước hàng đợi

// Định nghĩa cấu trúc nút của hàng đợi
struct Node {
    struct packet data;
    struct Node* next;
};

// Định nghĩa cấu trúc hàng đợi
struct Queue {
    struct Node *front, *rear;
};

// Hàm tạo một hàng đợi trống
struct Queue* createQueue() {
    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
    return q;
}

// Hàm thêm phần tử vào hàng đợi
void enqueue(struct Queue* q, struct packet value) {
    // Đếm số phần tử trong hàng đợi
    int count = 0;
    struct Node* current = q->front;
    while (current != NULL) {
        count++;
        current = current->next;
    }

    if (count >= MAX_QUEUE_SIZE) {
        fprintf(stderr, "Queue is full, cannot enqueue more packets\n");
        return;
    }

    struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
    temp->data = value;
    temp->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = temp;
        return;
    }
    q->rear->next = temp;
    q->rear = temp;
}

// Hàm lấy phần tử khỏi hàng đợi
struct packet dequeue(struct Queue* q) {
    if (q->front == NULL) {
        printf("Queue is empty\n");
        exit(1); // Hàng đợi trống, thoát chương trình
    }
    struct Node* temp = q->front;
    struct packet data = temp->data;
    q->front = q->front->next;
    if (q->front == NULL)
        q->rear = NULL;
    free(temp);
    return data;
}

// Hàm kiểm tra hàng đợi có rỗng không
int isEmpty(struct Queue* q) {
    return q->front == NULL;
}

void clearQueue(struct Queue* q) {
    while (!isEmpty(q)) {
        dequeue(q);
    }
    free(q);
}