#ifndef ALGOVIS_STACK_H
#define ALGOVIS_STACK_H
#include <string>
#include <vector>

// Array-backed LIFO stack used by the Stack visualizer.
class StackDS {
private:
    std::vector<std::string> items;
    const std::size_t capacity = 8;
    std::string buildResponse(bool success, const std::string& message) const;
public:
    std::string run(const std::string& operation, const std::string& value = "");
};
#endif
