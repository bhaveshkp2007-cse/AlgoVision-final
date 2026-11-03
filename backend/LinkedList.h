#ifndef ALGOVIS_LINKED_LIST_H
#define ALGOVIS_LINKED_LIST_H
#include <string>
#include <vector>

// A node stores one value and a pointer to the next node in the list.
struct Node {
    std::string data;
    Node* next;
    explicit Node(const std::string& value) : data(value), next(nullptr) {}
};

// Dynamically allocated singly linked list used by the Linked List visualizer.
class LinkedListDS {
private:
    Node* head = nullptr;
    int count = 0;
    std::vector<std::string> values() const;
    void clear();
public:
    LinkedListDS() = default;
    ~LinkedListDS();
    LinkedListDS(const LinkedListDS&) = delete;
    LinkedListDS& operator=(const LinkedListDS&) = delete;
    std::string run(const std::string& operation, const std::string& value = "", int position = 0);
};
#endif
