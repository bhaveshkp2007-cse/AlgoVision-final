#include "Stack.h"
#include "JsonUtils.h"

std::string StackDS::run(const std::string& operation, const std::string& value) {
    bool success = true;
    std::string message;

    if (operation == "push") {
        if (items.size() >= capacity) {
            success = false;
            message = "Stack Overflow! Capacity is " + std::to_string(capacity);
        } else {
            items.push_back(value);
            message = "Pushed '" + value + "' to stack";
        }
    } else if (operation == "pop") {
        if (items.empty()) {
            success = false;
            message = "Stack Underflow! Nothing to pop";
        } else {
            const std::string removed = items.back();
            items.pop_back();
            message = "Popped '" + removed + "' from stack";
        }
    } else if (operation == "peek") {
        if (items.empty()) {
            success = false;
            message = "Stack is empty, nothing to peek";
        } else {
            message = "Top element is '" + items.back() + "' at index " +
                      std::to_string(items.size() - 1);
        }
    } else if (operation == "clear") {
        items.clear();
        message = "Stack cleared";
    } else {
        message = "Stack state loaded";
    }
    return buildResponse(success, message);
}

// Keep the response fields stable because the frontend relies on this format.
std::string StackDS::buildResponse(bool success, const std::string& message) const {
    const std::string top = items.empty() ? "Empty" : items.back();
    return std::string("{\"ok\":") + (success ? "true" : "false") +
        ",\"message\":\"" + escapeJson(message) +
        "\",\"items\":" + toJsonArray(items) +
        ",\"size\":" + std::to_string(items.size()) +
        ",\"capacity\":" + std::to_string(capacity) +
        ",\"top\":\"" + escapeJson(top) +
        "\",\"isEmpty\":" + (items.empty() ? "true" : "false") +
        ",\"isFull\":" + (items.size() >= capacity ? "true" : "false") + "}";
}
