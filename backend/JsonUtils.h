#ifndef ALGOVIS_JSON_UTILS_H
#define ALGOVIS_JSON_UTILS_H

#include <string>
#include <vector>

// Helpers for safely formatting values in the JSON responses sent to Node.js.
std::string escapeJson(const std::string& value);
std::string toJsonArray(const std::vector<std::string>& values);

#endif // ALGOVIS_JSON_UTILS_H
