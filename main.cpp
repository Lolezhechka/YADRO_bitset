#include <iostream>
#include <string>
#include "bitset.h" 

void print_set(const std::string& name, const Bitset& bitset) {
    std::cout << name << " (size: " << bitset.size() << ", empty: ";
    if (bitset.empty()) {
        std::cout << "true";
    } else {
        std::cout << "false";
    }
    std::cout << "): {";
    bool first = true;
    for (size_t i = 0; i < 150; ++i) {
        if (bitset[i]) {
            if (!first) {
                std::cout << ", ";
            }
            std::cout << i;
            first = false;
        }
    }
    std::cout << "}\n";
}

int main() {
    std::cout << "Проверка set, test, []\n";
    Bitset b1(10);
    b1.set(2, true);
    b1.set(5, true);
    b1.set(9, true);
    print_set("b1", b1);

    std::cout << "\nПроверяем реаллокацию добавляя 100 в b1\n";
    b1.set(100, true);
    print_set("b1 после расширения", b1);

    std::cout << "\nПроверка конструкторов из правила пяти\n";
    Bitset b2 = b1;
    b2.set(100, false);
    print_set("b2 (копия b1, удален 100)", b2);
    print_set("b1 (оригинал не изменился)", b1);

    Bitset b3 = std::move(b2);
    print_set("b3 (перемещен из b2)", b3);
    std::cout << "Емкость b2 после move: " << b2.size() << "\n\n";

    std::cout << "Проверка теоретико-множественных операций\n";
    Bitset A(32);
    A.set(1, true); A.set(3, true); A.set(10, true);
    
    Bitset B(64);
    B.set(3, true); B.set(10, true); B.set(20, true);

    print_set("Множество A", A);
    print_set("Множество B", B);

    Bitset U = A.union_with(B);
    print_set("Объединение (A union B)", U);

    Bitset I = A.intersection(B);
    print_set("Пересечение (A intersect B)", I);

    std::cout << "\nПроверка is_subset\n";
    Bitset sub(10);
    sub.set(3, true); sub.set(10, true);
    print_set("Множество sub", sub);
    
    std::cout << "sub является подмножеством A? : " << (sub.is_subset(A) ? "Да" : "Нет") << "\n";
    std::cout << "A является подмножеством sub? : " << (A.is_subset(sub) ? "Да" : "Нет") << "\n\n";

    std::cout << "Проверка clear\n";
    A.clear();
    print_set("Множество A после clear()", A);

    return 0;
}