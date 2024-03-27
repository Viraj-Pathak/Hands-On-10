#include <iostream>

// Node structure for doubly linked list
struct Node {
    int key;
    int value;
    Node* prev;
    Node* next;

    Node(int k, int v) : key(k), value(v), prev(nullptr), next(nullptr) {}
};

// Doubly linked list class
class DoublyLinkedList {
public:
    Node* head;
    Node* tail;

    DoublyLinkedList() : head(nullptr), tail(nullptr) {}

    // Function to insert a new node at the head of the list
    void insertHead(int key, int value) {
        Node* newNode = new Node(key, value);
        if (!head) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
    }

    // Function to remove a node from the list
    void remove(Node* node) {
        if (node == head && node == tail) {
            head = tail = nullptr;
        } else if (node == head) {
            head = head->next;
            head->prev = nullptr;
        } else if (node == tail) {
            tail = tail->prev;
            tail->next = nullptr;
        } else {
            node->prev->next = node->next;
            node->next->prev = node->prev;
        }
        delete node;
    }
};

// Hash table class
class HashTable {
private:
    int capacity; 
    int size;     
    int threshold; 
    DoublyLinkedList* table; 

    // Hash function using multiplication method
    int hash(int key) {
        const double A = 0.6180339887; 
        double frac = A * key - int(A * key); 
        return int(capacity * frac); 
    }

    // Function to resize the hash table
    void resize(int newCapacity) {
        DoublyLinkedList* newTable = new DoublyLinkedList[newCapacity];
        for (int i = 0; i < capacity; ++i) {
            Node* current = table[i].head;
            while (current) {
                int newIndex = hash(current->key);
                newTable[newIndex].insertHead(current->key, current->value);
                Node* temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] table;
        table = newTable;
        capacity = newCapacity;
        threshold = capacity / 2;
    }

public:
    HashTable(int initialCapacity = 8) : capacity(initialCapacity), size(0) {
        threshold = capacity / 2;
        table = new DoublyLinkedList[capacity];
    }

    ~HashTable() {
        delete[] table;
    }

    // Function to insert a key-value pair into the hash table
    void insert(int key, int value) {
        if (size >= threshold) {
            resize(capacity * 2); // Double the capacity
        }
        int index = hash(key);
        table[index].insertHead(key, value);
        ++size;
    }

    // Function to remove a key-value pair from the hash table
    void remove(int key) {
        int index = hash(key);
        Node* current = table[index].head;
        while (current) {
            if (current->key == key) {
                table[index].remove(current);
                --size;
                break;
            }
            current = current->next;
        }
        if (size < threshold / 4 && capacity > 8) {
            resize(capacity / 2); // Halve the capacity
        }
    }

    int get(int key) {
        int index = hash(key);
        Node* current = table[index].head;
        while (current) {
            if (current->key == key) {
                return current->value;
            }
            current = current->next;
        }
        return -1;
    }

    // Function to print the contents of the hash table
    void print() {
        for (int i = 0; i < capacity; ++i) {
            std::cout << "Bucket " << i << ": ";
            Node* current = table[i].head;
            while (current) {
                std::cout << "(" << current->key << ", " << current->value << ") ";
                current = current->next;
            }
            std::cout << std::endl;
        }
    }
};

int main() {
    HashTable hashTable;
    hashTable.insert(5, 10);
    hashTable.insert(15, 20);
    hashTable.insert(25, 30);
    hashTable.insert(35, 40);
    hashTable.insert(45, 50);

    std::cout << "Hash table after insertion:" << std::endl;
    hashTable.print();

    std::cout << "Value for key 15: " << hashTable.get(15) << std::endl;

    hashTable.remove(15);
    hashTable.remove(35);

    std::cout << "Hash table after removal:" << std::endl;
    hashTable.print();

    return 0;
}
