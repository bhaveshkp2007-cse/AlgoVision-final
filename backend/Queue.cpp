#include "Queue.h"
#include "JsonUtils.h"

// Convert the circular storage into front-to-rear order for the UI.
std::vector<std::string> QueueDS::orderedItems() const {
    std::vector<std::string> ordered;
    for (int i = 0; i < count; ++i)
        ordered.push_back(items[(frontIndex + i) % capacity]);
    return ordered;
}

std::string QueueDS::run(const std::string& operation, const std::string& value) {
    bool success = true;
    std::string message;

    if (operation == "enqueue") {
        if (count == capacity) {
            success = false;
            message = "Queue Overflow! Capacity is " + std::to_string(capacity);
        } else {
            const int rearIndex = (frontIndex + count) % capacity;
            items[rearIndex] = value;
            ++count;
            message = "Enqueued '" + value + "' at rear";
        }
    } else if (operation == "dequeue") {
        if (count == 0) {
            success = false;
            message = "Queue Underflow! Nothing to dequeue";
        } else {
            const std::string removed = items[frontIndex];
            items[frontIndex].clear();
            frontIndex = (frontIndex + 1) % capacity;
            --count;
            message = "Dequeued '" + removed + "' from front";
        }
    } else if (operation == "front") {
        if (count == 0) {
            success = false;
            message = "Queue is empty, no front element";
        } else {
            message = "Front element is '" + items[frontIndex] + "'";
        }
    } else if (operation == "clear") {
        for (std::string& item : items) item.clear();
        frontIndex = 0;
        count = 0;
        message = "Queue cleared";
    } else {
        message = "Queue state loaded";
    }

    const std::vector<std::string> ordered = orderedItems();
    const std::string frontValue = count == 0 ? "Empty" : items[frontIndex];
    return std::string("{\"ok\":") + (success ? "true" : "false") +
        ",\"message\":\"" + escapeJson(message) +
        "\",\"items\":" + toJsonArray(ordered) +
        ",\"size\":" + std::to_string(count) +
        ",\"capacity\":" + std::to_string(capacity) +
        ",\"front\":\"" + escapeJson(frontValue) +
        "\",\"isEmpty\":" + (count == 0 ? "true" : "false") +
        ",\"isFull\":" + (count == capacity ? "true" : "false") + "}";
}
