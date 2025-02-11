#include "Queue.h"
#include <iostream>

// Constructor to initialize an empty queue
Queue::Queue() {
    front = -1;
    rear = -1;

}

// Adds a province to the end of the queue
void Queue::enqueue(int province) {
    // Implement circular structure
    // Add the province
    if ((rear + 1) % MAX_QUEUE_SIZE == front) {
        return;
    }
    if (isEmpty()) {
        front = rear = 0;
    } else {
        rear = (rear + 1) % MAX_QUEUE_SIZE;
    }
    data[rear] = province;
}


// Removes and returns the front province from the queue
int Queue::dequeue() {
    if (isEmpty()) {
        return -1;
    }
    int dequeuedValue = data[front];
    data[front] = -1;
    if (front == rear) {
        front = rear = -1;
    } else {
        front = (front + 1) % MAX_QUEUE_SIZE;
    }
    return dequeuedValue;
}

// Returns the front province without removing it
int Queue::peek() const {
    if (isEmpty()) {
        return -1;
    }
    return data[front];
}

// Checks if the queue is empty
bool Queue::isEmpty() const {
    return front == -1;
}

// Add a priority neighboring province in a way that will be dequeued and explored before other non-priority neighbors
void Queue::enqueuePriority(int province) {
    // TODO: Your code here
    if ((rear + 1) % MAX_QUEUE_SIZE == front) {
        return;
    }
    if (isEmpty()) {
        front = rear = 0;
        data[rear] = province;
    } else {
        front = (front - 1 + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;
        data[front] = province;
    }
}

