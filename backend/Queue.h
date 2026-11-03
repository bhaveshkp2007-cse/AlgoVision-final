#ifndef ALGOVIS_QUEUE_H
#define ALGOVIS_QUEUE_H
#include <string>
#include <vector>

// Circular-array FIFO queue. frontIndex identifies the current front item.
class QueueDS {
private:
    static constexpr int capacity = 8;
    std::vector<std::string> items = std::vector<std::string>(capacity);
    int frontIndex = 0;
    int count = 0;
    std::vector<std::string> orderedItems() const;
public:
    std::string run(const std::string& operation, const std::string& value = "");
};
#endif
