#include <algorithm>
#include <iostream>
#include <climits>
#include <limits>
#include <vector>
using namespace std;

struct Segment {
  int start, end;
};

//merge sort from large to small
void Mergesort(vector<Segment> &segments  , int p, int q, int r)
{
    int n1 = q - p + 1 ;
    int n2 = r - q ;
    int maxint = numeric_limits<int>::max();
    vector<Segment> leftvec(n1+1);
    vector<Segment> rightvec(n2 + 1);
    int i = 0;
    int j = 0;

    for(int i=0; i<n1 ; i++)
       leftvec[i] = segments[p+i];
    for(int i=0; i<n2 ; i++)
       rightvec[i] = segments[q+i+1];

    leftvec[n1].end = rightvec[n2].end= maxint;

    for(int k=p; k<=r ; k++)
    {
        if(leftvec[i].end <= rightvec[j].end)
            segments[k] = leftvec[i++];
        else
            segments[k] = rightvec[j++];
    }
}

void Merge_sort(vector<Segment> &segments , int p, int r)
{
    if(p < r)
    {
        int q= (p + r) / 2;
        Merge_sort(segments , p , q);
        Merge_sort(segments, q+1 , r);
        Mergesort(segments, p , q , r);
    }
}


vector<int> optimal_points(vector<Segment> &segments , int n) {
  vector<int> points;
  int m = 0;
  //write your code here
  Merge_sort(segments, 0, n-1);

  for (int i = 0; i < n; i++) {
    int j = i+1;
    while( j < n && segments[i].end >= segments[j].start)
        j++;
    points.push_back(segments[i].end);
    i = j-1;
     m++;
  }
  return points;
}

int main() {
  int n;
  std::cin >> n;

  vector<Segment> segments(n);

  for (size_t i = 0; i < segments.size(); ++i) {
    std::cin >> segments[i].start >> segments[i].end;
  }

  vector<int> points = optimal_points(segments, n);

  std::cout << points.size() << "\n";
  for (size_t i = 0; i < points.size(); ++i) {
    std::cout << points[i] << " ";
  }
}
