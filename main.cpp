#include <iostream>
#include <ostream>
#include <string>
#include "hashmap.hpp"

int main() {
    HashMap<std::string, int> map;

    map.insert("one", 1);
    map.insert("two", 2);
    map.insert("three", 3);

    // get values
    if (auto value = map.get("two")) {
        std::cout << "two = " << *value << std::endl;
    }

    std::cout << "Size before remove: " << map.size() << std::endl;

    map.remove("one");

    std::cout << "Size after remove: " << map.size() << std::endl;

    return 0;
}