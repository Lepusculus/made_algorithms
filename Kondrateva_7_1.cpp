//
// Created by Yulia Kondratyeva on 11/19/19.
//

/* Задача 7. Использование самобалансирующихся деревьев (7 баллов)
Солдаты. В одной военной части решили построить в одну шеренгу по росту. Т.к. часть была далеко не образцовая,
то солдаты часто приходили не вовремя, а то их и вовсе приходилось выгонять из шеренги за плохо начищенные сапоги.
Однако солдаты в процессе прихода и ухода должны были всегда быть выстроены по росту – сначала самые высокие,
а в конце – самые низкие. За расстановку солдат отвечал прапорщик, который заметил интересную
особенность – все солдаты в части разного роста. Ваша задача состоит в том, чтобы помочь прапорщику
правильно расставлять солдат, а именно для каждого приходящего солдата указывать,
перед каким солдатом в строе он должен становится.
*/

#include <stdlib.h>
#include <algorithm>
#include <stdio.h>

struct CTreeElem{
    CTreeElem* left;
    CTreeElem* right;
    int val;
    long priority;
    int count;

    CTreeElem(int val){
        this->val = val;
        priority = random();
        count = 1;
        left = right = nullptr;
    }
};

using TreePair = std::pair<CTreeElem*, CTreeElem*>;

int size(CTreeElem* elem){
    return elem == nullptr? 0: elem->count;
}

void patch_size(CTreeElem* elem){
    elem->count = 1 + size(elem->left) + size(elem->right);
}

std::pair<CTreeElem*, CTreeElem*> split(CTreeElem* root, int bound){
    if(root == nullptr) return  std::make_pair(nullptr, nullptr);
    if(root->val == bound) {
        auto ret = std::make_pair(root->left, root->right);
        delete root;
        return ret;
    }
    if(bound < root->val){
        auto left_split = split(root->left, bound);
        root->left = left_split.second;
        patch_size(root);
        return std::make_pair(left_split.first, root);
    } else {
        auto right_split = split(root->right, bound);
        root->right = right_split.first;
        patch_size(root);
        return std::make_pair(root, right_split.second);
    }
}

CTreeElem* merge(CTreeElem* small, CTreeElem* big){
    if(small == nullptr) return big;
    if(big == nullptr) return small;
    if(small->priority < big->priority) {
        small->right = merge(small->right, big);
        patch_size(small);
        return small;
    }else {
        big->left = merge(small, big->left);
        patch_size(big);
        return big;
    }
}

CTreeElem* add_val(CTreeElem* root, int val){
    const auto splitted = split(root, val);
    CTreeElem* v = new CTreeElem(val);
    return merge(splitted.first, merge(v, splitted.second));
}

CTreeElem* remove_val(CTreeElem* root, int val){
    const auto splitted = split(root, val);
    return merge(splitted.first, splitted.second);
}

int count_more(CTreeElem* root, int val){
    if(root == nullptr) return  0;
    if(root->val < val) return count_more(root->right, val);
    return  1 + size(root->right) + count_more(root->left, val);
}

int kth_val(CTreeElem* root, int k){
    if(root == nullptr) return  -1;
    auto t = size(root->right);
    if(k<=t) return  kth_val(root->right, k);
    if(k == t + 1) return root->val;
    return kth_val(root->left, k - t - 1);
}

void clean(CTreeElem* root){
    if(root == nullptr) return;
    clean(root->left);
    clean(root->right);
    delete root;
}

int main(){
    int n;
    scanf("%d", &n);
    CTreeElem* root = nullptr;
    for(int i = 0; i<n; ++i){
        int a, b;
        scanf("%d%d", &a, &b);
        if(a==1){
            printf("%d\n", count_more(root, b));
            root = add_val(root, b);
        }else{
            root = remove_val(root, kth_val(root, b+1));
        }
    }
    clean(root);
}