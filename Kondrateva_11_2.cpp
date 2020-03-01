//
// Created by Yulia Kondratyeva on 12/5/19.
//

/* Задача 11 (5 баллов)
11_2. Количество различных путей
Дан невзвешенный неориентированный граф. В графе может быть несколько кратчайших путей между какими-то вершинами.
Найдите количество различных кратчайших путей между заданными вершинами. Требуемая сложность O(V+E).
Ввод: v: кол-во вершин (макс. 50000), n: кол-во ребер (макс. 200000), n пар реберных вершин, пара вершин v, w для запроса.
Вывод: количество кратчайших путей от v к w.
ьного префиксного кода Хаффмана. При помощи алгоритма реализуйте две функции для создания архива из одного файла и
извлечения файла из архива.
*/

#include <stdio.h>
#include <vector>


void initLenAndOrder(std::vector<bool> &proceed,
                     std::vector<int> &len,
                     std::vector<int> &order,
                     const std::vector<std::vector<int>> &edges,
                     int start
) {
    order.push_back(start);
    proceed[start] = true;
    len[start] = 0;

    for (unsigned int i = 0; i < order.size(); ++i) {
        int l = len[order[i]];
        for (int j: edges[order[i]])
            if (!proceed[j]) {
                proceed[j] = true;
                len[j] = l + 1;
                order.push_back(j);
            }
    }
}

void initCount(std::vector<int> &count,
               const std::vector<int> &order,
               const std::vector<int> &len,
               const std::vector<std::vector<int>> &edges) {
    for (auto i: order)
        for (auto j: edges[i])
            if (len[i] < len[j])
                count[j] += count[i];
}


int findCountWaysInGraph(int size, const std::vector<std::vector<int>> &edges, int from, int to) {

    std::vector<bool> proceed; //флаг,  обойдена ли вершина
    std::vector<int> len;   // кратчайшее расстрояние от вершины до стартовой
    std::vector<int> count; // число путей до вершины от стартовой
    std::vector<int> order; // порядок обхода вершин


    for (int i = 0; i < size; ++i) {
        proceed.push_back(false);
        len.push_back(0);
        count.push_back(0);
    }

    initLenAndOrder(proceed, len, order, edges, from);

    count[from] = 1;
    initCount(count, order, len, edges);

    return count[to];

}


int main() {
    int m, n;
    scanf("%d%d", &m, &n);
     std::vector<std::vector<int> > edges;
    for (int i = 0; i < m; ++i) {
         std::vector<int> vc;
        edges.push_back(vc);
    }

    for (int i = 0; i < n; ++i) {
        int f, t;
        scanf("%d%d", &f, &t);
        edges[f].push_back(t);
        edges[t].push_back(f);
    }

    int v, w;
    scanf("%d%d", &v, &w);

    printf("%d\n", findCountWaysInGraph(m, edges, v, w));


}

