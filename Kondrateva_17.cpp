//
// Created by Yulia Kondratyeva on 12/25/19.
//

/* Задача 17. Шаблон с ?
Шаблон поиска задан строкой длины m, в которой кроме обычных символов могут встречаться символы “?”.
Найти позиции всех вхождений шаблона в тексте длины n. Каждое вхождение шаблона предполагает, что все обычные символы 
совпадают с соответствующими из текста, а вместо символа “?” в тексте встречается произвольный символ.
Гарантируется, что сам “?” в тексте не встречается.
*/

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>


struct HashableOrderedVector {
    std::vector<int> positions;
    int hash;

    HashableOrderedVector() {
        hash = 0;
        positions.clear();
    }

    void add(int value) {
        int size = positions.size() + 1;
        positions.push_back(value);
        hash += size * (value + 1);
        hash %= 10000007;
    }

    int last() const {
        if(positions.empty()) return -1;
        return positions[positions.size()-1];
    }
};


bool operator==(const HashableOrderedVector &first, const HashableOrderedVector &second) {
    if (first.hash != second.hash) return false;
    int size = first.positions.size();
    if (second.positions.size() != size) return false;
    for (int i = 0; i < size; ++i) {
        if (first.positions[i] != second.positions[i]) return false;
    }
    return true;
}


namespace  std {
    template<>
    struct hash<HashableOrderedVector> {
        std::size_t operator()(const HashableOrderedVector &k) const {
            return k.hash;
        }
    };
}


bool  char_eq(char pats, char strs){
    return pats == strs || pats == '?';
}


HashableOrderedVector next(const HashableOrderedVector& prev, const std::string&  pattern, char c){
    HashableOrderedVector ret;
    if(char_eq(pattern[0], c)) ret.add(0);
    for(const auto pos : prev.positions){
        if(pos!=pattern.size()-1)
            if(char_eq(pattern[pos+1], c))
                ret.add(pos + 1);
    }
    return ret;
}


void buildDFA(const std::string& pattern, const std::unordered_set<char>& alphabet, std::vector<std::unordered_map<char, int> >& dfa, std::vector<bool>& is_end ){
    std::vector<HashableOrderedVector> states;
    std::unordered_map<HashableOrderedVector, int>  stateToNumber;

    std::unordered_map<char, int> first;
    dfa.push_back(first);
    HashableOrderedVector empty;
    states.push_back(empty);
    stateToNumber[empty] = 0;

    for(int i = 0; i<dfa.size(); ++i){
        for(char c: alphabet){
            auto nextState = next(states[i], pattern, c);
            if(stateToNumber.count(nextState)){
                dfa[i][c] = stateToNumber[nextState];
            }else{
                states.push_back(nextState);
                auto size = stateToNumber.size();
                stateToNumber[nextState] = size;
                std::unordered_map<char, int> last;
                dfa.push_back(last);
                dfa[i][c] = size;
            }
        }
    }

    int k = pattern.size()-1;
    for(auto& st: states)
        is_end.push_back(k == st.last());
}


int main() {
    std::string pattern;
    std::string str;
    std::cin >> pattern >> str;

    std::unordered_set<char> alphabet;
    for(int i = 0; i<str.size(); ++i) alphabet.insert(str[i]);
    std::vector<std::unordered_map<char, int> > dfa;
    std::vector<bool> is_end;

    buildDFA(pattern, alphabet, dfa, is_end);
    int k = pattern.size()-1;
    int state = 0;

    for(int i = 0; i<str.size(); ++i){
        state = dfa[state][str[i]];
        if(is_end[state]){
            std::cout << i-k << ' ';
        }
    }
    std::cout << std::endl;
}
