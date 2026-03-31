#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdint>

class Bitset {
private:
    uint64_t* data;
    size_t size;
    static size_t number_of_blocks (size_t bits) {
        return (bits + 63)/64;
    }
public:
    Bitset() : size(64) {
        data = new uint64_t[1]();
    }

    explicit Bitset(size_t size_) : size(size_) {
        if (size_ != 0) {
            data = new uint64_t[number_of_blocks(size_)]();
        } else {
            data = nullptr; 
        }
    }

    ~Bitset() {
        delete[] data;
    }

    Bitset(const Bitset& other) : size(other.size) {
        if (other.size != 0) {
            size_t blocks = number_of_blocks(other.size);
            data = new uint64_t[blocks]();
            std::copy(other.data,other.data+blocks,data);
        } else {
            data = nullptr;
        }
    }   
    Bitset& operator=(const Bitset other) {
        if (this != &other) {
            Bitset temp(other);
            std::swap(data,temp.data);
            std::swap(size,temp.size);
        }
        return *this;
    }

    Bitset(Bitset&& other) noexcept : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;    
    }

    Bitset& operator=(Bitset&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }

    void set(size_t k, bool b) {
        if (k >= size) {
            if (!b) {
                return;
            }
            size_t new_size = std::max(k+1,size*2);
            size_t old_blocks = number_of_blocks(size);
            size_t new_blocks = number_of_blocks(new_size);

            uint64_t* new_data = new uint64_t[new_blocks]();
            if (data) {
                std::copy(data,data+old_blocks,new_data);
                delete[] data;
            }
            data = new_data;
            size = new_size;
        }
        size_t block = k/64;
        size_t bit = k%64;

        if (b) {
            data[block] |= (1ULL << bit);
        } else {
            data[block] &= ~(1ULL << bit);
        }
    }

    bool test(size_t k) const {
        if (k >= size) {
            return false;
        }
        size_t block = k/64;
        size_t bit = k%64;
        return (data[block] & (1ULL << bit)) != 0;
    }

    bool operator[](size_t k) const {
        return test(k);
    }

    Bitset union_with(const Bitset& other) const {
        size_t max_bits = std::max(size,other.size);
        Bitset result(max_bits);

        size_t blocks1 = number_of_blocks(size);
        size_t blocks2 = number_of_blocks(other.size);
        size_t max_blocks = std::max(blocks1,blocks2);

        for (size_t i = 0; i != max_blocks; ++i) {
            uint64_t value1 = (i < blocks1) ? data[i] : 0;
            uint64_t value2 = (i < blocks2) ? other.data[i] : 0;
            result.data[i] = value1 | value2;
        }
        return result;
    }

    Bitset intersection(const Bitset& other) const {
        size_t min_bits = std::min(size,other.size);
        Bitset result(min_bits);

        size_t min_blocks = number_of_blocks(min_bits);
        for (size_t i = 0; i != min_blocks; ++i) {
            result.data[i] = data[i] & other.data[i];
        }
        return result;
    }

    bool is_subset(const Bitset& other) const {
        size_t blocks1 = number_of_blocks(size);
        size_t blocks2 = number_of_blocks(other.size);

        for (size_t i = 0; i != blocks1; ++i) {
            uint64_t value1 = data[i];
            uint64_t value2 = (i < blocks2) ? other.data[i] : 0;
            
            if ((value1 & ~value2) != 0) {
                return false;
            }
        }
        return true;
    }

    size_t size() const {
        return size;
    }

    bool empty() {
        size_t blocks = number_of_blocks(size);
        for (size_t i = 0; i != blocks; ++i) {
            if (data[i] != 0) return false;
        }
        return true;
    }

    void clear() {
        size_t blocks = number_of_blocks(size);
        delete[] data;
        data = new uint64_t[blocks]();    
    }
};