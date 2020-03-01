//
// Created by Yulia Kondratyeva on 12/18/19.
//

/* Задача 14 (5 баллов)
14. «MST». Дан неориентированный связный граф. Требуется найти вес минимального остовного дерева в этом графе.
Вариант 1. С помощью алгоритма Прима.
*/

#include <stdio.h>
#include <queue>
#include <vector>

typedef std::pair<int, std::pair<int, int> > WEdge;

class Graph{
private:
    int size;
    std::vector<std::vector<std::pair<int, int>>> edges;


    public:
     Graph(int size){
         this->size = size;
         for(int i = 0; i<size; ++i){
             std::vector<std::pair<int, int>> empty;
             edges.push_back(empty);
         }
     }

     void addEdge(int from, int to, int w){
         if(from == to) return;
         edges[from].push_back(std::make_pair(to, w));
         edges[to].push_back(std::make_pair(from, w));
     }

     int minTreeWeight(){
         std::vector<bool> used;
         std::priority_queue<WEdge> edgesQ;

         for(int i = 0; i<size; ++i) used.push_back(false);
         used[0] = true;

         for(const auto& e: edges[0])
              edgesQ.push(makeWedge(0, e));


         int ans = 0;
         while (!edgesQ.empty()){
             auto edge = edgesQ.top();
             edgesQ.pop();
             if(used[edge.second.first] && used[edge.second.second]) continue;

             ans -= edge.first;
             int nextV = used[edge.second.first]?edge.second.second:edge.second.first;
             used[nextV] = true;
             for(const auto& e: edges[nextV])
                 edgesQ.push(makeWedge(nextV, e));
         }



         return ans;
     }

     private:
     WEdge makeWedge(int from, const std::pair<int, int>& toAndW){
        WEdge r;
        r.first = -toAndW.second;
        r.second = from<toAndW.first?std::make_pair(from, toAndW.first):std::make_pair(toAndW.first, from);
        return r;
     }
};

int main(){
    int n, m;
    scanf("%d%d", &n, &m);
    Graph g(n);
    for(int i = 0; i<m; ++i){
        int a, b, w;
        scanf("%d%d%d", &a, &b, &w);
        g.addEdge(a-1, b-1, w);
    }
    printf("%d\n", g.minTreeWeight());
}
