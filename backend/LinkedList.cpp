#include "LinkedList.h"
#include "JsonUtils.h"

// Collect node values in traversal order for the frontend response.
std::vector<std::string> LinkedListDS::values() const {
    std::vector<std::string> result;
    for (Node* current = head; current != nullptr; current = current->next)
        result.push_back(current->data);
    return result;
}

// Release every dynamically allocated node. Also used by the destructor.
void LinkedListDS::clear() {
    while (head != nullptr) {
        Node* nodeToDelete = head;
        head = head->next;
        delete nodeToDelete;
    }
    count = 0;
}

LinkedListDS::~LinkedListDS() { clear(); }

std::string LinkedListDS::run(const std::string& operation, const std::string& value, int position) {
    bool success = true;
    std::string message;

    if (operation == "insertBeginning") {
        Node* newNode = new Node(value);
        newNode->next = head;
        head = newNode;
        ++count;
        message = "Inserted '" + value + "' at beginning (index 0)";
    } else if (operation == "insertEnd") {
        Node* newNode = new Node(value);
        if (head == nullptr) head = newNode;
        else {
            Node* current = head;
            while (current->next != nullptr) current = current->next;
            current->next = newNode;
        }
        ++count;
        message = "Inserted '" + value + "' at end (index " + std::to_string(count - 1) + ")";
    } else if (operation == "insertAt") {
        if (position < 0 || position > count) {
            success = false;
            message = "Invalid position. Must be 0 to " + std::to_string(count);
        } else if (position == 0) {
            Node* newNode = new Node(value);
            newNode->next = head;
            head = newNode;
            ++count;
            message = "Inserted '" + value + "' at position 0";
        } else {
            Node* current = head;
            for (int i = 0; i < position - 1; ++i) current = current->next;
            Node* newNode = new Node(value);
            newNode->next = current->next;
            current->next = newNode;
            ++count;
            message = "Inserted '" + value + "' at position " + std::to_string(position);
        }
    } else if (operation == "deleteBeginning") {
        if (head == nullptr) { success = false; message = "List is empty"; }
        else {
            Node* nodeToDelete = head;
            const std::string removed = nodeToDelete->data;
            head = head->next;
            delete nodeToDelete;
            --count;
            message = "Deleted '" + removed + "' from beginning";
        }
    } else if (operation == "deleteEnd") {
        if (head == nullptr) { success = false; message = "List is empty"; }
        else if (head->next == nullptr) {
            const std::string removed = head->data;
            delete head;
            head = nullptr;
            --count;
            message = "Deleted '" + removed + "' from end";
        } else {
            Node* current = head;
            while (current->next->next != nullptr) current = current->next;
            const std::string removed = current->next->data;
            delete current->next;
            current->next = nullptr;
            --count;
            message = "Deleted '" + removed + "' from end";
        }
    } else if (operation == "deleteAt") {
        if (position < 0 || position >= count) {
            success = false;
            message = count == 0 ? "List is empty" :
                "Invalid position. Must be 0 to " + std::to_string(count - 1);
        } else if (position == 0) {
            Node* nodeToDelete = head;
            const std::string removed = nodeToDelete->data;
            head = head->next;
            delete nodeToDelete;
            --count;
            message = "Deleted '" + removed + "' from position 0";
        } else {
            Node* current = head;
            for (int i = 0; i < position - 1; ++i) current = current->next;
            Node* nodeToDelete = current->next;
            const std::string removed = nodeToDelete->data;
            current->next = nodeToDelete->next;
            delete nodeToDelete;
            --count;
            message = "Deleted '" + removed + "' from position " + std::to_string(position);
        }
    } else if (operation == "search") {
        Node* current = head;
        int index = 0;
        int foundIndex = -1;
        while (current != nullptr) {
            if (current->data == value) { foundIndex = index; break; }
            current = current->next;
            ++index;
        }
        if (foundIndex == -1) { success = false; message = "Value '" + value + "' not found in list"; }
        else message = "Found '" + value + "' at index " + std::to_string(foundIndex);
    } else if (operation == "clear") {
        clear();
        message = "List cleared (HEAD = NULL)";
    } else {
        message = "Linked List state loaded";
    }

    const std::vector<std::string> currentValues = values();
    const std::string headValue = currentValues.empty() ? "NULL" : escapeJson(currentValues.front());
    return std::string("{\"ok\":") + (success ? "true" : "false") +
        ",\"message\":\"" + escapeJson(message) +
        "\",\"items\":" + toJsonArray(currentValues) +
        ",\"size\":" + std::to_string(count) +
        ",\"head\":\"" + headValue + "\"}";
}
