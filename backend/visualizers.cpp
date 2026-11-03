#include "Stack.h"
#include "Queue.h"
#include "LinkedList.h"
#include "HashMap.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Decode the URL-encoded fields sent by server.js (one request per input line).
static std::string decodeField(const std::string& input) {
    auto hexValue = [](char ch) -> int {
        if (ch >= '0' && ch <= '9') return ch - '0';
        if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
        if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
        return -1;
    };

    std::string decoded;
    for (std::size_t i = 0; i < input.size(); ++i) {
        if (input[i] == '%' && i + 2 < input.size()) {
            const int high = hexValue(input[i + 1]);
            const int low = hexValue(input[i + 2]);
            if (high >= 0 && low >= 0) {
                decoded += static_cast<char>(high * 16 + low);
                i += 2;
            } else decoded += input[i];
        } else if (input[i] == '+') decoded += ' ';
        else decoded += input[i];
    }
    return decoded;
}

// Split a tab-separated request and decode each field independently.
static std::vector<std::string> parseRequest(const std::string& line) {
    std::vector<std::string> fields;
    std::stringstream stream(line);
    std::string field;
    while (std::getline(stream, field, '\t')) fields.push_back(decodeField(field));
    while (fields.size() < 5) fields.push_back("");
    return fields;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Keep one instance of each structure alive so state persists between requests.
    StackDS stack;
    QueueDS queue;
    LinkedListDS linkedList;
    HashMapDS hashMap;

    std::string line;
    while (std::getline(std::cin, line)) {
        const std::vector<std::string> fields = parseRequest(line);
        const std::string& module = fields[0];
        const std::string& operation = fields[1];
        const std::string& value = fields[2];
        const std::string& secondValue = fields[3];
        int position = 0;
        try { position = std::stoi(fields[4]); } catch (...) { position = 0; }

        std::string response;
        if (module == "stack") response = stack.run(operation, value);
        else if (module == "queue") response = queue.run(operation, value);
        else if (module == "linkedlist") response = linkedList.run(operation, value, position);
        else if (module == "hashmap") response = hashMap.run(operation, value, secondValue);
        else response = "{\"ok\":false,\"message\":\"Unknown module\"}";

        // server.js expects exactly one JSON response line for every request line.
        std::cout << response << '\n' << std::flush; // Flush so Node.js receives the response immediately.
    }
    return 0;
}
