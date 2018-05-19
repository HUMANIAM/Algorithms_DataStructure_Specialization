#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>
#include <limits>
#include <stdlib.h>
using namespace std;
struct point{
     int x, y;
};
//Quick sort
void partition3(vector<point> &a, int l, int r, int& m1, int& m2, char coordinate) {

  int k = l + rand() % (r - l + 1);
  swap(a[r], a[k]);
  point pivote = a[r];
  int index = 1;
  int j = l;
  int i = l;

  if(coordinate == 'x'){

    while( i < (r - index +1) ) {
            if (a[i].x < pivote.x)
              swap(a[i], a[j++]);
            else if(a[i].x == pivote.x){
              swap(a[i--], a[r-index]);
              index++;
            }
            i++;
        }
    }
  else
  {
     while( i < (r - index +1) ) {
            if (a[i].y < pivote.y)
                swap(a[i], a[j++]);
            else if(a[i].y == pivote.y){
                swap(a[i--], a[r-index]);
                index++;
            }
            i++;
        }
     }

  m1=j;
  for(int k=0; k<index; k++)
    swap(a[j++], a[r - k]);
  m2 = j - 1;
}

void randomized_quick_sort(vector<point> &a, int l, int r, char coordinate) {
  if (l >= r)
    return;

  int m1,m2;
  partition3(a, l, r, m1, m2, coordinate);
  randomized_quick_sort(a, l, m1 - 1, coordinate);
  randomized_quick_sort(a, m2 + 1, r, coordinate);
}

double Distance(point p1, point p2)
{
    return sqrt( pow( (p1.x - p2.x), 2 ) + pow( (p1.y - p2.y), 2));
}

double minimal_distance(vector<point>& points, int l, int r)
{
   if(l<r){
            int q = (l+r)/2;
            double minleft = minimal_distance(points, l, q);
            double minright = minimal_distance(points, q+1, r);

            double closetdist = min(minleft, minright);
            double gama = closetdist;

            vector<point> sortbyy;
            point p;
            for(int i=q; i>=l && ((points[q].x - points[i].x) <= gama); i--)
            {
                p.x = points[i].x;
                p.y = points[i].y;
                sortbyy.push_back(p);
            }

            for(int j=q+1; j<=r && ( (points[j].x - points[q+1].x) <= gama);j++)
            {
                p.x = points[j].x;
                p.y = points[j].y;
                sortbyy.push_back(p);
            }

            randomized_quick_sort(sortbyy, 0, sortbyy.size()-1, 'y');

            for(int i=0; i<sortbyy.size()-1; i++)
                for(int j=i+1; j<sortbyy.size(); j++)
                   if( (sortbyy[j].y - sortbyy[i].y) <= gama )
                   {
                       double dist = Distance(sortbyy[i], sortbyy[j]);
                       closetdist = min(dist, closetdist);
                   }
                   else
                    break;

            return closetdist;
        }
    else
            return numeric_limits<double>::max();
}

double Get_minimal_distance(vector<point>& points, int l, int r) {
 randomized_quick_sort(points, l, r, 'x');
 return minimal_distance(points, l, r);
}

int main() {
  int n ;
  std::cin >> n;
  vector<point> points(n);
  for (int i = 0; i < n; i++) {
    std::cin >> points[i].x >> points[i].y;
  }
  std::cout << std::fixed;
  std::cout << std::setprecision(9) << Get_minimal_distance(points, 0, n-1) << "\n";
}
