//
// Created by Yulia Kondratyeva on 11/19/19.
//

/* Задача № 6. Порядок обхода (3 балла)
Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N.
Требуется построить бинарное дерево, заданное наивным порядком вставки.
Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
Рекурсия запрещена.
*/

#include <stdio.h>
#include <vector>

class TreeElem{
public:
    TreeElem* left;
    TreeElem* right;
    int val;

    TreeElem(int val){
        this->val = val;
        left = right = nullptr;
    }
};


int main(){
    int N;
    scanf("%d", &N);
    if(N == 0) return 0;
    int p;
    scanf("%d", &p);
    auto* root = new TreeElem(p);
    for(int i = 1; i<N; ++i){
        scanf("%d", &p);
        auto* next = new TreeElem(p);
        auto * parent = root;

        while (parent != nullptr){
            if (p < parent->val) {
                if(parent->left == nullptr){
                    parent->left = next;
                    break;
                } else {
                    parent = parent->left;
                }
            }else{
                if(parent->right == nullptr){
                    parent->right = next;
                    break;
                } else {
                    parent = parent->right;
                }
            }
        }
    }

    std::vector<TreeElem* > stack;
    stack.push_back(root);
    while (stack.size()){
            auto* item = stack[stack.size() - 1];
            if(item->right == nullptr && item->left == nullptr) {
                stack.pop_back();

                printf("%d ", item->val);
                delete item;
                continue;
            }
            if(item->right!= nullptr) stack.push_back(item->right);
            if(item->left!= nullptr) stack.push_back(item->left);
            item->right = item->left = nullptr;

    }
    printf("\n");
}