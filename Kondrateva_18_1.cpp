//
// Created by Yulia Kondratyeva on 12/30/19.
//

/* Задача 18. Построение выпуклой оболочки (5 баллов)
Дано множество точек на плоскости (x, y). Постройте выпуклую оболочку этого множества и вычислите ее периметр.
Вариант 1. С помощью алгоритма Грэхема.
*/

#include <stdio.h>
#include <vector>
#include <algorithm>
#include <math.h>


typedef  std::pair<double, double> Point;


double tSize(Point a, Point b, Point c){
    double xb = b.first - a.first;
    double yb = b.second - a.second;

    double xc = c.first - a.first;
    double yc = c.second - a.second;

    return 0.5*abs(xb*yc-xc*yb);
}

double dist(Point p1, Point p2){
    double x = p1.first - p2.first;
    double y = p1.second - p2.second;
    return sqrt(x*x + y*y);
}

bool inSegment(Point start, Point end, Point check){
    double s = tSize(start, end, check);
    if(s>1e-5) return false;
    return dist(start, end) > dist(start, check);
}

bool inTriangle(Point p1, Point p2, Point p3, Point i){
    return tSize(p1, p2, i) + tSize(p1,i, p3) + tSize(i, p2, p3) - tSize(p1, p2, p3)<1e-7;
}

void addToConv(std::vector<Point> & stack, Point point){
      while(stack.size()>2 && inTriangle(
                stack[0],
                point,
                stack[stack.size()-2],
                stack[stack.size()-1]

              )) stack.pop_back();
      stack.push_back(point);
}

int main() {
    int n;
    scanf("%d", &n);
    std::vector<Point > points;
    for(int i = 0; i<n; ++i){
        double  x, y;
        scanf("%lf%lf", &x, &y);
        points.push_back(std::make_pair(x, y));
    }
    for(int i = 1; i<n; ++i) {
        if (points[i].second < points[0].second
            ||
            (points[i].second == points[0].second and points[i].first < points[0].first)) {
            std::swap(points[0], points[i]);
        }
    }

    for(int i = 1; i<n; ++i){
        points[i].first -=points[0].first;
        points[i].second -=points[0].second;
    }
    points[0].first = 0;
    points[0].second = 0;

    auto it = ++points.begin();

    std::sort(it, points.end(), [] (const auto& lhs, const auto& rhs) {
        return  atan2(lhs.second, lhs.first)< atan2(rhs.second, rhs.first);
    });

    std::vector<Point> ast;
    ast.push_back(points[0]);
    ast.push_back(points[1]);

    double at = atan2(points[1].second, points[1].first);
    for(int i = 2; i<n; ++i){
       if(inSegment(ast[0], ast[ast.size()-1],  points[i])) continue;
        if(inSegment(ast[0], points[i], ast[ast.size()-1])) {
             ast[ast.size()-1] = points[i];
        }else{
            ast.push_back(points[i]);
        }
    }

     n = ast.size();
     std::vector<Point> stack;
     stack.push_back(ast[0]);
     stack.push_back(ast[1]);
     for(int i = 2; i<n; ++i){
         addToConv(stack, ast[i]);
     }

     stack.push_back(stack[0]);
     double ans = 0;
     for(int i = 1; i<stack.size(); ++i) ans += dist(stack[i], stack[i-1]);
     printf("%0.8lf\n", ans);
}