#include <iostream>
#include "Heap.h"
#include "BinomialHeap.h"
#include "FibonacciHeap.h"

int main() {
    std::cout << "Choose heap to use: (h/b/f)";
    std::string dec;
    std::cin >> dec;
    if (dec == "h") {
        Heap<int> h;
        while (true) {
            std::cout << "Heap $ ";
            std::cin >> dec;
            if (dec == "insert") {
                int x;
                std::cin >> x;
                h.insert(x);
            }
            else if (dec == "extract_min") {
                std::cout << h.extract_min() << std::endl;
            }
            else if (dec == "get_min") {
                std::cout << h.get_min() << std::endl;
            }
            else if (dec == "exit") {
                return 0;
            }
            else {
                std::cout << "Unknown command" << std::endl;
            }
        }
    }
    else if (dec == "b") {
        BinomialHeap<int> h;
        while (true) {
            std::cout << "BinomialHeap $ ";
            std::cin >> dec;
            if (dec == "insert") {
                int x;
                std::cin >> x;
                h.insert(x);
            }
            else if (dec == "extract_min") {
                std::cout << h.extract_min() << std::endl;
            }
            else if (dec == "get_min") {
                std::cout << h.get_min() << std::endl;
            }
            else if (dec == "exit") {
                return 0;
            }
            else {
                std::cout << "Unknown command" << std::endl;
            }
        }
    }
    else if (dec == "f") {
        FibonacciHeap<int> h;
        while (true) {
            std::cout << "FibonacciHeap $ ";
            std::cin >> dec;
            if (dec == "insert") {
                int x;
                std::cin >> x;
                h.insert(x);
            }
            else if (dec == "extract_min") {
                std::cout << h.extract_min() << std::endl;
            }
            else if (dec == "get_min") {
                std::cout << h.get_min() << std::endl;
            }
            else if (dec == "exit") {
                return 0;
            }
            else {
                std::cout << "Unknown command" << std::endl;
            }
        }
    }
    else {
        std::cout << "Unknown heap" << std::endl;
        return 0;
    }

    return 0;
}
