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
    int *used; //0 - free, 1 - active, 2-deleted
    table_elem *table;
    int max_size;
    int count;

    FixedHashTable(int size) {
        max_size = size;
        used = new int[size];
        for (int i = 0; i < size; ++i) used[i] = 0;
        table = new table_elem[size];
        count = 0;
    }

    ~FixedHashTable() {
        delete[] used;
        delete[] table;
    }

    bool contains(const std::string &s) {
        auto hashes = make_hp(s);
        auto index = hashes.first % max_size;
        for (int i = 0; i < max_size; ++i) {
            if (used[index] == 0) return false;
            if (used[index] == 1) {
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
            if (used[index] != 1) {
                table[index] = std::make_pair(s, hashes);
                used[index] = 1;
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
            if (used[index] == 1) {
                if (table[index].second == hashes) {
                    if (table[index].first == s) {
                        used[index] = 2;
                        --count;
                        return;
                    };
                }
            }
            index = (index + hashes.second) % max_size;
        }
    }

    void to_another(FixedHashTable *ft) {
        for (int i = 0; i < max_size; ++i)
            if (used[i] == 1)
                ft->add_new_elem(table[i].first);
    }

    bool full() {
        return count * 4 > max_size * 3;
    }


};


class HashTable {
private:
    int size;
    FixedHashTable *table;
public:
    HashTable() {
        size = 8;
        table = new FixedHashTable(8);
    }

    ~HashTable() {
        delete table;
    }

    bool contains(const std::string &s) {
        return table->contains(s);
    }

    bool add(const std::string &s) {
        if (table->contains(s)) return false;
        table->add_new_elem(s);
        if (table->full()) {
            size *= 2;
            FixedHashTable *next = new FixedHashTable(size);
            table->to_another(next);
            delete table;
            table = next;
        }
        return true;
    }

    bool remove(const std::string &s) {
        if (!table->contains(s)) return false;
        table->remove_elem(s);
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
