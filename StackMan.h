#include </usr/local/include/mcpp/mcpp.h>

#ifndef STACKMAN
#define STACKMAN

#include <memory>

// Struct for maze generation to store coordinate points
struct Coord2D {
    int x, z;
};

// stack managing class for maze generation
// uses 'template' to be able to handle any type of object
// Contains functions that would be expected in a stack
template <typename Type>
class StackMan
{
public:
    StackMan() : size(0), max(16) {
    objects = std::make_unique<Type[]>(16);
    }
    ~StackMan() {}

    void push(Type& object)
    {
        if (size == max) {
            increase();
        }
        objects[size] = object;
        size++;
    }

    Type pop()
    {
        Type object = objects[size - 1];
        size--;
        return object;
    }

    Type peek()
    {
        Type object = objects[size - 1];
        return object;
    }

    int getSize() { return size; }
    bool isEmpty() { return (size == 0); }
    bool isFull() { return (size == max); }

private:
    int size;
    int max;
// Uses smart pointers so that c++ can manage the memory for this class
    std::unique_ptr<Type[]> objects;

// Function to dynamicly increase the stack size while retaining order
    void increase()
    {
    std::unique_ptr<Type[]> temp = std::make_unique<Type[]>(max);
        for (int i=0; i<max; i++) {
            temp[i] = objects[i];
        }
        objects.reset();
        objects = std::make_unique<Type[]>(2*max);

        for (int i = 0; i < max; i++) {
            objects[i] = temp[i];
        }

// reseting temp so it's data can be collected as garbage before the
// pointer is deleted (to avoid memory leaks)
        temp.reset();
        max *= 2;
    }
};

#endif
