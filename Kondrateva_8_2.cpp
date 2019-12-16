//
// Created by Yulia Kondratyeva on 11/26/19.
//

/* Задача 8. Хеш-таблица (10 баллов)
Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
Хранимые строки непустые и состоят из строчных латинских букв.
Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае,
 когда коэффициент заполнения таблицы достигает 3/4.
Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества и проверки
 принадлежности данной строки множеству.
1_1. Для разрешения коллизий используйте квадратичное пробирование. i-ая проба
g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.
1_2. Для разрешения коллизий используйте двойное хеширование.
*/

#include <string>
#include <iostream>
#include <memory>

using h_pair = std::pair<int, int>;
using table_elem = std::pair<std::string, h_pair>;


h_pair make_hp(const std::string &s) {
    int a1 = 0;
    int a2 = 0;
    for (char c: s)
        if (c) {
            a1 = (a1 * 1442 + c) % 100007;
            a2 = (a2 * 6329 + c) % 100009;
        }
    if ((a2 % 2) == 0) ++a2;
    return std::make_pair(a1, a2);
}

struct FixedHashTable {
    std::unique_ptr<bool[]> is_free;
    std::unique_ptr<bool[]> is_deleted;

    std::unique_ptr<table_elem []> table;
    int max_size;
    int count;

    FixedHashTable(int size) {
        max_size = size;
        is_free = std::unique_ptr<bool[]>(new bool[size]);
        is_deleted = std::unique_ptr<bool[]>(new bool[size]);
        for (int i = 0; i < size; ++i) {
            is_free[i] = true;
            is_deleted[i] = false;
        }
        table = std::unique_ptr<table_elem []>(new table_elem[size]);
        count = 0;
    }

    bool contains(const std::string &s) const{
        auto hashes = make_hp(s);
        auto index = hashes.first % max_size;
        for (int i = 0; i < max_size; ++i) {
            if (is_free[index]) return false;
            if (!is_deleted[index]) {
                if (table[index].second == hashes) {
                    if (table[index].first == s) return true;
                }
            }
            index = (index + hashes.second) % max_size;
        }
        return false;
    }

    void add_new_elem(const std::string &s) { // гарантированно отсутствует
        auto hashes = make_hp(s);
        auto index = hashes.first % max_size;
        for (int i = 0;; ++i) {
            if (is_free[index] || is_deleted[index] ) {
                table[index] = std::make_pair(s, hashes);
                is_free[index] = false;
                is_deleted[index] = false;

                ++count;
                return;
            }
            index = (index + hashes.second) % max_size;
        }
    }

    void remove_elem(const std::string &s) {// гарантированно присутствует
        auto hashes = make_hp(s);
        auto index = hashes.first % max_size;
        for (int i = 0;; ++i) {
            if (!(is_free[index]||is_deleted[index])) {
                if (table[index].second == hashes) {
                    if (table[index].first == s) {
                        is_deleted[index] = true;
                        --count;
                        return;
                    };
                }
            }
            index = (index + hashes.second) % max_size;
        }
    }

    void to_another(FixedHashTable *ft) const {
        for (int i = 0; i < max_size; ++i)
            if (!(is_free[i]||is_deleted[i]))
                ft->add_new_elem(table[i].first);
    }

    bool full() const {
        return count * 4 > max_size * 3;
    }

    bool  too_sparse() const {
        return max_size > 8 && count*4 >max_size;
    }


};


class HashTable {
private:
    int size;
    std::unique_ptr<FixedHashTable> table;
public:
    HashTable() {
        size = 8;
        table = std::unique_ptr<FixedHashTable>(new FixedHashTable(8));
    }



    bool contains(const std::string &s) const {
        return table->contains(s);
    }

    bool add(const std::string &s) {
        if (table->contains(s)) return false;
        table->add_new_elem(s);
        if (table->full()) {
            size *= 2;
            FixedHashTable *next = new FixedHashTable(size);
            table->to_another(next);

            table = std::unique_ptr<FixedHashTable>(next);
        }
        return true;
    }

    bool remove(const std::string &s) {
        if (!table->contains(s)) return false;
        table->remove_elem(s);
        if(table->too_sparse()){
            size /=2;
            FixedHashTable *next = new FixedHashTable(size);
            table->to_another(next);
            table = std::unique_ptr<FixedHashTable>(next);
        }
        return true;
    }
};

int main(int argc, const char *argv[]) {
    char c;
    std::string val;
    HashTable table;
    std::string ok = "OK\n";
    std::string fail = "FAIL\n";
    while (!std::cin.eof()) {
        std::cin >> std::ws >> c >> std::ws >> val >> std::ws;
        if (c == '+') {
            std::cout << ((table.add(val)) ? ok : fail);
        }
        if (c == '-') {
            std::cout << ((table.remove(val)) ? ok : fail);
        }
        if (c == '?') {
            std::cout << ((table.contains(val)) ? ok : fail);
        }
    }

    return 0;
}
