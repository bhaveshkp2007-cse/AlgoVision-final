#include "JsonUtils.h"

std::string escapeJson(const std::string& value) {
    std::string escaped;
    for (char ch : value) {
        switch (ch) {
            case '"': escaped += "\\\""; break;
            case '\\': escaped += "\\\\"; break;
            case '\n': escaped += "\\n"; break;
            case '\r': escaped += "\\r"; break;
            case '\t': escaped += "\\t"; break;
            default:
                // JSON strings cannot contain unescaped control characters.
                if (static_cast<unsigned char>(ch) >= 32) escaped += ch;
        }
    }
    return escaped;
}

std::string toJsonArray(const std::vector<std::string>& values) {
    std::string json = "[";
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (i > 0) json += ",";
        json += "\"" + escapeJson(values[i]) + "\"";
    }
    return json + "]";
}
