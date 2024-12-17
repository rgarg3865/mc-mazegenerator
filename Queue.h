#ifndef QUEUE
#define QUEUE

template <typename Type>
class Queue
{
   public:
    Queue(int capacity) : capacity(capacity), count(0), front(-1), rear(-1)
    {
        array = new Type[capacity];
    }

    Queue() : capacity(8), count(0), front(-1), rear(-1)
    {
        array = new Type[capacity];
    }

    Queue(const Queue& other)
        : capacity(other.capacity),
          count(other.count),
          front(other.front),
          rear(other.rear)
    {
        array = new Type[capacity];
        for (int i = 0; i < capacity; i++) {
            array[i] = other.array[i];
        }
    }

    ~Queue()
    {
        if (array != nullptr) {
            delete[] array;
        }
    }

    int getCount() { return this->count; }

    // Adds provided element to the back of the queue
    void push(const Type& val)
    {
        if (isFull()) {
            increaseSize();
        }
        // Allows for circular queue to utilize memory efficiently
        if (front == -1) {
            front = 0;
        }
        rear = (rear + 1) % capacity;
        array[rear] = val;
        count++;
    }

    // Removes and returns the element at front
    Type pop()
    {
        Type value;
        if (isEmpty()) {
            // std::cout << "Empty queue" << std::endl;
        } else {
            value = array[front];
            if (front == rear) {
                front = -1;
                rear = -1;
            } else {
                front = (front + 1) % capacity;
            }
            count--;
        }
        return value;
    }

    bool isFull() { return capacity == count; }

    bool isEmpty() { return count == 0; }

   private:
    int capacity;
    int count;
    int front;
    int rear;
    Type* array;

    // Makes a new queue with increased size if the current queue becomes full and deallocates the memory for the current queue
    void increaseSize()
    {
        int new_size = capacity * 2;
        Type* temp = array;
        array = new Type[new_size];

        for (int i = 0; i < count; i++) {
            array[i] = temp[(i + front) % capacity];
        }
        front = 0;
        rear = count - 1;
        delete[] temp;
        capacity = new_size;
    }
};
#endif
