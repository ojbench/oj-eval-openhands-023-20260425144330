#include <iostream>
#include <string>
#include "deque.hpp"

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    
    int n;
    std::cin >> n;
    
    sjtu::deque<int> dq;
    
    for (int i = 0; i < n; ++i) {
        std::string cmd;
        std::cin >> cmd;
        
        try {
            if (cmd == "push_back") {
                int x;
                std::cin >> x;
                dq.push_back(x);
            } else if (cmd == "push_front") {
                int x;
                std::cin >> x;
                dq.push_front(x);
            } else if (cmd == "pop_back") {
                dq.pop_back();
            } else if (cmd == "pop_front") {
                dq.pop_front();
            } else if (cmd == "front") {
                std::cout << dq.front() << std::endl;
            } else if (cmd == "back") {
                std::cout << dq.back() << std::endl;
            } else if (cmd == "size") {
                std::cout << dq.size() << std::endl;
            } else if (cmd == "empty") {
                std::cout << (dq.empty() ? "true" : "false") << std::endl;
            } else if (cmd == "clear") {
                dq.clear();
            } else if (cmd == "at") {
                int pos;
                std::cin >> pos;
                std::cout << dq.at(pos) << std::endl;
            }
        } catch (sjtu::exception &e) {
            std::cout << "error" << std::endl;
        }
    }
    
    return 0;
}
