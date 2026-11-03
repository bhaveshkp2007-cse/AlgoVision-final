#ifndef ALGOVIS_HASH_MAP_H
#define ALGOVIS_HASH_MAP_H
#include <string>
#include <utility>
#include <vector>

// Hash table using separate chaining to resolve collisions.
class HashMapDS {
private:
    static constexpr int bucketCount = 8;
    std::vector<std::pair<std::string, std::string>> buckets[bucketCount];
    int hashKey(const std::string& key) const;
    std::string buildResponse(bool success, const std::string& message, int bucket) const;
public:
    std::string run(const std::string& operation, const std::string& key = "", const std::string& value = "");
};
#endif
