#pragma once
#include <cstdint>
#include <cstddef>

class Bitset {
private:
    uint64_t* data_;
    size_t size_;
    
    static size_t number_of_blocks(size_t bits);

public:
    Bitset();
    explicit Bitset(size_t size__);
    ~Bitset();

    Bitset(const Bitset& other);
    Bitset& operator=(const Bitset& other);
    Bitset(Bitset&& other) noexcept;
    Bitset& operator=(Bitset&& other) noexcept;

    void set(size_t k, bool b);
    bool test(size_t k) const;
    
    bool operator[](size_t k) const;

    Bitset union_with(const Bitset& other) const;
    Bitset intersection(const Bitset& other) const;
    bool is_subset(const Bitset& other) const;

    size_t size() const;

    bool empty() const;
    void clear();
};