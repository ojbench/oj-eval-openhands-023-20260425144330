#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"
#include <cstddef>

namespace sjtu {

template<class T>
class deque {
private:
    static const int BLOCK_SIZE = 512;
    
    struct Block {
        T* data;
        int start;
        int end;
        int capacity;
        Block* prev;
        Block* next;
        
        Block() : start(0), end(0), capacity(BLOCK_SIZE), prev(nullptr), next(nullptr) {
            data = (T*)::operator new(sizeof(T) * BLOCK_SIZE);
        }
        
        ~Block() {
            for (int i = start; i < end; ++i) {
                data[i].~T();
            }
            ::operator delete(data);
        }
        
        int size() const {
            return end - start;
        }
        
        bool full() const {
            return end >= capacity;
        }
        
        bool empty() const {
            return start >= end;
        }
    };
    
    Block* head;
    Block* tail;
    size_t len;
    
    void init() {
        head = new Block();
        tail = head;
        len = 0;
    }
    
    void clear_blocks() {
        Block* cur = head;
        while (cur) {
            Block* next = cur->next;
            delete cur;
            cur = next;
        }
        head = tail = nullptr;
        len = 0;
    }
    
public:
    class const_iterator;
    class iterator {
    private:
        Block* block;
        int index;
        const deque* dq;
        
    public:
        friend class deque;
        friend class const_iterator;
        
        iterator(Block* b = nullptr, int i = 0, const deque* d = nullptr) 
            : block(b), index(i), dq(d) {}
        
        iterator operator+(const int &n) const {
            iterator it = *this;
            it += n;
            return it;
        }
        
        iterator operator-(const int &n) const {
            iterator it = *this;
            it -= n;
            return it;
        }
        
        int operator-(const iterator &rhs) const {
            if (dq != rhs.dq) throw invalid_iterator();
            
            int result = 0;
            if (block == rhs.block) {
                return index - rhs.index;
            }
            
            Block* b = rhs.block;
            int idx = rhs.index;
            while (b != block) {
                result += b->end - idx;
                b = b->next;
                if (!b) throw invalid_iterator();
                idx = b->start;
            }
            result += index - idx;
            return result;
        }
        
        iterator& operator+=(const int &n) {
            if (n == 0) return *this;
            
            if (n > 0) {
                int remaining = n;
                while (remaining > 0) {
                    int available = block->end - index;
                    if (remaining < available) {
                        index += remaining;
                        return *this;
                    }
                    remaining -= available;
                    block = block->next;
                    if (!block) throw invalid_iterator();
                    index = block->start;
                }
            } else {
                int remaining = -n;
                while (remaining > 0) {
                    int available = index - block->start;
                    if (remaining <= available) {
                        index -= remaining;
                        return *this;
                    }
                    remaining -= available;
                    block = block->prev;
                    if (!block) throw invalid_iterator();
                    index = block->end;
                }
            }
            return *this;
        }
        
        iterator& operator-=(const int &n) {
            return *this += (-n);
        }
        
        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        
        iterator& operator++() {
            ++index;
            while (index >= block->end && block->next) {
                block = block->next;
                index = block->start;
            }
            return *this;
        }
        
        iterator operator--(int) {
            iterator tmp = *this;
            --(*this);
            return tmp;
        }
        
        iterator& operator--() {
            while (index <= block->start && block->prev) {
                block = block->prev;
                index = block->end;
            }
            if (index > block->start) {
                --index;
            }
            return *this;
        }
        
        T& operator*() const {
            return block->data[index];
        }
        
        T* operator->() const noexcept {
            return &(block->data[index]);
        }
        
        bool operator==(const iterator &rhs) const {
            return block == rhs.block && index == rhs.index;
        }
        
        bool operator==(const const_iterator &rhs) const {
            return block == rhs.block && index == rhs.index;
        }
        
        bool operator!=(const iterator &rhs) const {
            return !(*this == rhs);
        }
        
        bool operator!=(const const_iterator &rhs) const {
            return !(*this == rhs);
        }
    };
    
    class const_iterator {
    private:
        const Block* block;
        int index;
        const deque* dq;
        
    public:
        friend class deque;
        friend class iterator;
        
        const_iterator(const Block* b = nullptr, int i = 0, const deque* d = nullptr) 
            : block(b), index(i), dq(d) {}
        
        const_iterator(const iterator &other) 
            : block(other.block), index(other.index), dq(other.dq) {}
        
        const_iterator operator+(const int &n) const {
            const_iterator it = *this;
            it += n;
            return it;
        }
        
        const_iterator operator-(const int &n) const {
            const_iterator it = *this;
            it -= n;
            return it;
        }
        
        int operator-(const const_iterator &rhs) const {
            if (dq != rhs.dq) throw invalid_iterator();
            
            int result = 0;
            if (block == rhs.block) {
                return index - rhs.index;
            }
            
            const Block* b = rhs.block;
            int idx = rhs.index;
            while (b != block) {
                result += b->end - idx;
                b = b->next;
                if (!b) throw invalid_iterator();
                idx = b->start;
            }
            result += index - idx;
            return result;
        }
        
        const_iterator& operator+=(const int &n) {
            if (n == 0) return *this;
            
            if (n > 0) {
                int remaining = n;
                while (remaining > 0) {
                    int available = block->end - index;
                    if (remaining < available) {
                        index += remaining;
                        return *this;
                    }
                    remaining -= available;
                    block = block->next;
                    if (!block) throw invalid_iterator();
                    index = block->start;
                }
            } else {
                int remaining = -n;
                while (remaining > 0) {
                    int available = index - block->start;
                    if (remaining <= available) {
                        index -= remaining;
                        return *this;
                    }
                    remaining -= available;
                    block = block->prev;
                    if (!block) throw invalid_iterator();
                    index = block->end;
                }
            }
            return *this;
        }
        
        const_iterator& operator-=(const int &n) {
            return *this += (-n);
        }
        
        const_iterator operator++(int) {
            const_iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        
        const_iterator& operator++() {
            ++index;
            while (index >= block->end && block->next) {
                block = block->next;
                index = block->start;
            }
            return *this;
        }
        
        const_iterator operator--(int) {
            const_iterator tmp = *this;
            --(*this);
            return tmp;
        }
        
        const_iterator& operator--() {
            while (index <= block->start && block->prev) {
                block = block->prev;
                index = block->end;
            }
            if (index > block->start) {
                --index;
            }
            return *this;
        }
        
        const T& operator*() const {
            return block->data[index];
        }
        
        const T* operator->() const noexcept {
            return &(block->data[index]);
        }
        
        bool operator==(const iterator &rhs) const {
            return block == rhs.block && index == rhs.index;
        }
        
        bool operator==(const const_iterator &rhs) const {
            return block == rhs.block && index == rhs.index;
        }
        
        bool operator!=(const iterator &rhs) const {
            return !(*this == rhs);
        }
        
        bool operator!=(const const_iterator &rhs) const {
            return !(*this == rhs);
        }
    };
    
    deque() {
        init();
    }
    
    deque(const deque &other) {
        init();
        for (Block* b = other.head; b; b = b->next) {
            for (int i = b->start; i < b->end; ++i) {
                push_back(b->data[i]);
            }
        }
    }
    
    ~deque() {
        clear_blocks();
    }
    
    deque &operator=(const deque &other) {
        if (this == &other) return *this;
        clear_blocks();
        init();
        for (Block* b = other.head; b; b = b->next) {
            for (int i = b->start; i < b->end; ++i) {
                push_back(b->data[i]);
            }
        }
        return *this;
    }
    
    T& at(const size_t &pos) {
        if (pos >= len) throw index_out_of_bound();
        
        size_t current_pos = 0;
        Block* b = head;
        while (b) {
            size_t block_size = b->end - b->start;
            if (current_pos + block_size > pos) {
                return b->data[b->start + (pos - current_pos)];
            }
            current_pos += block_size;
            b = b->next;
        }
        throw index_out_of_bound();
    }
    
    const T& at(const size_t &pos) const {
        if (pos >= len) throw index_out_of_bound();
        
        size_t current_pos = 0;
        Block* b = head;
        while (b) {
            size_t block_size = b->end - b->start;
            if (current_pos + block_size > pos) {
                return b->data[b->start + (pos - current_pos)];
            }
            current_pos += block_size;
            b = b->next;
        }
        throw index_out_of_bound();
    }
    
    T& operator[](const size_t &pos) {
        return at(pos);
    }
    
    const T& operator[](const size_t &pos) const {
        return at(pos);
    }
    
    const T& front() const {
        if (empty()) throw container_is_empty();
        return head->data[head->start];
    }
    
    const T& back() const {
        if (empty()) throw container_is_empty();
        return tail->data[tail->end - 1];
    }
    
    iterator begin() {
        return iterator(head, head->start, this);
    }
    
    const_iterator cbegin() const {
        return const_iterator(head, head->start, this);
    }
    
    iterator end() {
        return iterator(tail, tail->end, this);
    }
    
    const_iterator cend() const {
        return const_iterator(tail, tail->end, this);
    }
    
    bool empty() const {
        return len == 0;
    }
    
    size_t size() const {
        return len;
    }
    
    void clear() {
        clear_blocks();
        init();
    }
    
    iterator insert(iterator pos, const T &value) {
        if (pos.dq != this) throw invalid_iterator();
        
        if (pos == begin()) {
            push_front(value);
            return begin();
        }
        if (pos == end()) {
            push_back(value);
            iterator result(tail, tail->end - 1, this);
            return result;
        }
        
        // Count position from beginning
        size_t insert_pos = 0;
        iterator it = begin();
        while (it != pos) {
            ++insert_pos;
            ++it;
        }
        
        // Rebuild
        deque tmp;
        it = begin();
        for (size_t i = 0; i < insert_pos; ++i, ++it) {
            tmp.push_back(*it);
        }
        tmp.push_back(value);
        while (it != end()) {
            tmp.push_back(*it);
            ++it;
        }
        *this = tmp;
        
        // Return iterator to inserted element
        it = begin();
        for (size_t i = 0; i < insert_pos; ++i) {
            ++it;
        }
        return it;
    }
    
    iterator erase(iterator pos) {
        if (pos.dq != this || pos == end()) throw invalid_iterator();
        
        if (pos == begin()) {
            pop_front();
            return begin();
        }
        
        iterator next = pos;
        ++next;
        if (next == end()) {
            pop_back();
            return end();
        }
        
        // For simplicity, rebuild
        deque tmp;
        iterator it = begin();
        while (it != end()) {
            if (it != pos) {
                tmp.push_back(*it);
            }
            ++it;
        }
        *this = tmp;
        
        return begin();
    }
    
    void push_back(const T &value) {
        if (tail->full()) {
            Block* new_block = new Block();
            tail->next = new_block;
            new_block->prev = tail;
            tail = new_block;
        }
        new (tail->data + tail->end) T(value);
        ++tail->end;
        ++len;
    }
    
    void pop_back() {
        if (empty()) throw container_is_empty();
        
        --tail->end;
        tail->data[tail->end].~T();
        --len;
        
        if (tail->empty() && tail->prev) {
            Block* old_tail = tail;
            tail = tail->prev;
            tail->next = nullptr;
            delete old_tail;
        }
    }
    
    void push_front(const T &value) {
        if (head->start == 0) {
            Block* new_block = new Block();
            new_block->next = head;
            head->prev = new_block;
            head = new_block;
            head->start = head->end = head->capacity;
        }
        --head->start;
        new (head->data + head->start) T(value);
        ++len;
    }
    
    void pop_front() {
        if (empty()) throw container_is_empty();
        
        head->data[head->start].~T();
        ++head->start;
        --len;
        
        if (head->empty() && head->next) {
            Block* old_head = head;
            head = head->next;
            head->prev = nullptr;
            delete old_head;
        }
    }
};

}

#endif
