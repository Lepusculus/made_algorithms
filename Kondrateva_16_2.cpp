//
// Created by Yulia Kondratyeva on 12/23/19.
//

/* Задача 16. Поиск подстроки (5 баллов)
Найдите все вхождения шаблона в строку. Длина шаблона – p, длина строки ­– n. Время O(n + p), доп. память – O(p).
Вариант 2. С помощью z-функции.
*/

#include <iostream>
#include <string>
#include <vector>

class ConcatView{
private:
    const std::string& first_;
    const std::string& second_;
    int len_first;
    int len;
public:
    ConcatView(const std::string& first, const std::string& second): first_(first), second_(second){
        len_first = first_.size();
        len = len_first + second_.size();
    }

    int lenght() const{
        return len;
    }

    int lenghtFirst() const{
        return len_first;
    }

    char get(int i) const{
        return i<len_first?first_[i]:second_[i-len_first];
    }

};


int main(){
    std::string pattern;
    std::string str;
    std::cin >> pattern >> str;
    ConcatView view(pattern, str);

    int k = view.lenghtFirst();
    int n = view.lenght();

    int z = 0;
    int l = 0;
    int r = 0;

    std::vector<int> zcache;
    zcache.push_back(0);
    for (int i=1; i<n; ++i) {
        z = 0;
        if (i <= r)
            z = std::min (r-i+1, zcache[i-l]);
        while (i+z < n && view.get(z) == view.get(i+z))
            ++z;
        if (i+z-1 > r) {
            l = i;
            r = i + z - 1;
        }

        if(z>=k && i>=k){
            std::cout << i-k << ' ';
        }
        zcache.push_back(z);
    }
    std::cout << std::endl;
}

