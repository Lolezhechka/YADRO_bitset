#include "bitset.h"
#include <algorithm>

size_t Bitset::number_of_blocks(size_t bits) {
    return (bits + 63) / 64;
}

Bitset::Bitset() : size_(64) {
    data_ = new uint64_t[1]();
}

Bitset::Bitset(size_t size__) : size_(size__) {
    if (size__ != 0) {
        data_ = new uint64_t[number_of_blocks(size__)]();
    } else {
        data_ = nullptr; 
    }
}

Bitset::~Bitset() {
    delete[] data_;
}

Bitset::Bitset(const Bitset& other) : size_(other.size_) {
    if (other.size_ != 0) {
        size_t blocks = number_of_blocks(other.size_);
        data_ = new uint64_t[blocks]();
        std::copy(other.data_, other.data_ + blocks, data_);
    } else {
        data_ = nullptr;
    }
}   

Bitset& Bitset::operator=(const Bitset& other) {
    if (this != &other) {
        Bitset temp(other);
        std::swap(data_, temp.data_);
        std::swap(size_, temp.size_);
    }
    return *this;
}

Bitset::Bitset(Bitset&& other) noexcept : data_(other.data_), size_(other.size_) {
    other.data_ = nullptr;
    other.size_ = 0;    
}

Bitset& Bitset::operator=(Bitset&& other) noexcept {
    if (this != &other) {
        delete[] data_;
        data_ = other.data_;
        size_ = other.size_;
        other.data_ = nullptr;
        other.size_ = 0;
    }
    return *this;
}

void Bitset::set(size_t k, bool b) {
    if (k >= size_) {
        if (!b) {
            return;
        }
        size_t new_size_ = std::max(k + 1, size_ * 2);
        size_t old_blocks = number_of_blocks(size_);
        size_t new_blocks = number_of_blocks(new_size_);

        uint64_t* new_data_ = new uint64_t[new_blocks]();
        if (data_) {
            std::copy(data_, data_ + old_blocks, new_data_);
            delete[] data_;
        }
        data_ = new_data_;
        size_ = new_size_;
    }
    size_t block = k / 64;
    size_t bit = k % 64;

    if (b) {
        data_[block] |= (1ULL << bit);
    } else {
        data_[block] &= ~(1ULL << bit);
    }
}

bool Bitset::test(size_t k) const {
    if (k >= size_) {
        return false;
    }
    size_t block = k / 64;
    size_t bit = k % 64;
    return (data_[block] & (1ULL << bit)) != 0;
}

Bitset Bitset::union_with(const Bitset& other) const {
    size_t max_bits = std::max(size_, other.size_);
    Bitset result(max_bits);

    size_t blocks1 = number_of_blocks(size_);
    size_t blocks2 = number_of_blocks(other.size_);
    size_t max_blocks = std::max(blocks1, blocks2);

    for (size_t i = 0; i != max_blocks; ++i) {
        uint64_t value1 = (i < blocks1) ? data_[i] : 0;
        uint64_t value2 = (i < blocks2) ? other.data_[i] : 0;
        result.data_[i] = value1 | value2;
    }
    return result;
}

Bitset Bitset::intersection(const Bitset& other) const {
    size_t min_bits = std::min(size_, other.size_);
    Bitset result(min_bits);

    size_t min_blocks = number_of_blocks(min_bits);
    for (size_t i = 0; i != min_blocks; ++i) {
        result.data_[i] = data_[i] & other.data_[i];
    }
    return result;
}

bool Bitset::is_subset(const Bitset& other) const {
    size_t blocks1 = number_of_blocks(size_);
    size_t blocks2 = number_of_blocks(other.size_);

    for (size_t i = 0; i != blocks1; ++i) {
        uint64_t value1 = data_[i];
        uint64_t value2 = (i < blocks2) ? other.data_[i] : 0;
        
        if ((value1 & ~value2) != 0) {
            return false;
        }
    }
    return true;
}

bool Bitset::empty() const {
    size_t blocks = number_of_blocks(size_);
    for (size_t i = 0; i != blocks; ++i) {
        if (data_[i] != 0) return false;
    }
    return true;
}

void Bitset::clear() {
    size_t blocks = number_of_blocks(size_);
    delete[] data_;
    data_ = new uint64_t[blocks]();
    size_ = 0;    
}

bool Bitset::operator[](size_t k) const {
    return test(k);
}

size_t Bitset::size() const {
    return size_;
}