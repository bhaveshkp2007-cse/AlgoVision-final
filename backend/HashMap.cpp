#include "HashMap.h"
#include "JsonUtils.h"
#include <algorithm>

// Sum the key's byte values and map the result to one of eight buckets.
int HashMapDS::hashKey(const std::string& key) const {
    unsigned int sum = 0;
    for (unsigned char character : key) sum += character;
    return static_cast<int>(sum % bucketCount);
}

std::string HashMapDS::run(const std::string& operation, const std::string& key, const std::string& value) {
    bool success = true;
    std::string message;
    const int bucketIndex = hashKey(key);

    if (operation == "insert") {
        bool updated = false;
        for (auto& entry : buckets[bucketIndex]) {
            if (entry.first == key) { entry.second = value; updated = true; break; }
        }
        if (updated) message = "Updated key '" + key + "' at bucket " + std::to_string(bucketIndex);
        else {
            const bool collision = !buckets[bucketIndex].empty();
            buckets[bucketIndex].push_back({key, value});
            message = "Inserted (" + key + "," + value + ") at bucket " +
                std::to_string(bucketIndex) + (collision ? " (Collision handled by chaining)" : "");
        }
    } else if (operation == "search") {
        bool found = false;
        for (const auto& entry : buckets[bucketIndex]) {
            if (entry.first == key) {
                message = "Found key '" + key + "' at bucket " + std::to_string(bucketIndex) + " with value: " + entry.second;
                found = true;
                break;
            }
        }
        if (!found) { success = false; message = "Key '" + key + "' not found (checked bucket " + std::to_string(bucketIndex) + ")"; }
    } else if (operation == "remove") {
        auto& chain = buckets[bucketIndex];
        const auto entry = std::find_if(chain.begin(), chain.end(), [&](const auto& item) { return item.first == key; });
        if (entry == chain.end()) { success = false; message = "Key '" + key + "' not found, nothing deleted"; }
        else { chain.erase(entry); message = "Deleted key '" + key + "' from bucket " + std::to_string(bucketIndex); }
    } else if (operation == "clear") {
        for (auto& chain : buckets) chain.clear();
        message = "HashMap cleared, all buckets empty";
    } else if (operation == "loadExample") {
        for (auto& chain : buckets) chain.clear();
        const std::vector<std::pair<std::string, std::string>> examples = {
            {"Alice", "25"}, {"Bob", "30"}, {"Charlie", "22"}, {"Bhavesh", "20"}
        };
        for (const auto& entry : examples) buckets[hashKey(entry.first)].push_back(entry);
        message = "Loaded example: Alice, Bob, Charlie, Bhavesh";
    } else {
        message = "HashMap state loaded";
    }
    return buildResponse(success, message, bucketIndex);
}

// Serialize every bucket, preserving the JSON structure expected by hashmap.js.
std::string HashMapDS::buildResponse(bool success, const std::string& message, int bucketIndex) const {
    int size = 0;
    std::string bucketJson = "[";
    for (int i = 0; i < bucketCount; ++i) {
        if (i > 0) bucketJson += ",";
        bucketJson += "[";
        for (std::size_t j = 0; j < buckets[i].size(); ++j) {
            if (j > 0) bucketJson += ",";
            const auto& entry = buckets[i][j];
            bucketJson += "{\"key\":\"" + escapeJson(entry.first) +
                          "\",\"value\":\"" + escapeJson(entry.second) + "\"}";
            ++size;
        }
        bucketJson += "]";
    }
    bucketJson += "]";
    return std::string("{\"ok\":") + (success ? "true" : "false") +
        ",\"message\":\"" + escapeJson(message) +
        "\",\"buckets\":" + bucketJson +
        ",\"size\":" + std::to_string(size) +
        ",\"capacity\":8,\"bucket\":" + std::to_string(bucketIndex) + "}";
}
