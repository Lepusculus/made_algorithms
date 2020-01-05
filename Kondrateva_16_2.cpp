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

std::vector<int> findPattern(const std::string& pattern, const std::string& str){
    int k = pattern.size();
    int n = k + str.size();
    int z = 0;
    int l = 0;
    int r = 0;
    std::vector<int> ret;
    
    std::vector<int> zcache;
    zcache.push_back(0);
    for (int i=1; i<n; ++i) {
        z = 0;
        if (i <= r)
            z = std::min (r-i+1, zcache[i-l]);
        while (i+z < n && (z<k?pattern[z]:str[z-k]) == (i+z<k?pattern[i+z]:str[i + z-k]))
            ++z;
        if (i+z-1 > r) {
            l = i;
            r = i + z - 1;
        }

        if(z>=k && i>=k){
            ret.push_back(i-k);
        }
        zcache.push_back(z);
    }
    return ret;
}


int main(){
    std::string pattern;
    std::string str;
    std::cin >> pattern >> str;
    for (auto pos: findPattern(pattern, str)){
       std::cout << pos << ' '; 
    }
    
    std::cout << std::endl;
}

