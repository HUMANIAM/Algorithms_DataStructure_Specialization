#include <iostream>
#include <vector>
#include <map>
#include <limits>
#include<cstdlib>
using namespace std;
//in this sorting problem i distinguish between end by r
//and start by l and point by its index
struct point{
    int value, type;
};

//merge sort from large to small
void Mergesort(vector<point> &Allpoints  , int p, int q, int r)
{
    int n1 = q - p + 1 ;
    int n2 = r - q ;
    int maxint = numeric_limits<int>::max();
    vector<point> leftvec(n1+1);
    vector<point> rightvec(n2 + 1);
    int i = 0;
    int j = 0;

    for(int i=0; i<n1 ; i++)
       leftvec[i] = Allpoints[p+i];
    for(int i=0; i<n2 ; i++)
       rightvec[i] = Allpoints[q+i+1];

    leftvec[n1].value = rightvec[n2].value= maxint;

    for(int k=p; k<=r ; k++)
    {
        if(leftvec[i].value < rightvec[j].value)
            Allpoints[k] = leftvec[i++];
        else if(leftvec[i].value > rightvec[j].value)
            Allpoints[k] = rightvec[j++];
        else
        {
            if(leftvec[i].type == -1)
               Allpoints[k] = leftvec[i++];
            else if(rightvec[i].type == -1)
                Allpoints[k] = rightvec[j++];
            else if(leftvec[i].type != 0)
                Allpoints[k] = leftvec[i++];
            else
                Allpoints[k] = rightvec[j++];
        }
    }
}

void Merge_sort(vector<point> &Allpoints , int p, int r)
{
    if(p < r)
    {
        int q= (p + r) / 2;
        Merge_sort(Allpoints , p , q);
        Merge_sort(Allpoints, q+1 , r);
        Mergesort(Allpoints, p , q , r);
    }
}
vector<int> fast_count_segments(vector<point> Allpoints, int m, int n) {
    vector<int> cnt(m);
    Merge_sort( Allpoints, 0, (m+2*n)-1 );

    int count=0;
    for(int i=0; i<(2*n+m); i++)
    {
        if(Allpoints[i].type == -1)
            count++;
        else if(Allpoints[i].type == 0)
            count--;
        else
            cnt[Allpoints[i].type - 2*n] = count;
    }

  return cnt;
}

int main() {

  int n, m;
  cin >> n >> m;

  vector<point>Allpoints(m+2*n);
  vector<int> points(m);
  //get segments
  for (size_t i = 0; i < 2*n; i++) {
    cin >> Allpoints[i].value;
    Allpoints[i].type = -1;
    cin >> Allpoints[++i].value;
    Allpoints[i].type = 0;
  }
  //get points
  for (size_t i = 2*n; i < 2*n+m; i++) {
    cin >> Allpoints[i].value;
    Allpoints[i].type = i;
  }
  //use fast_count_segments
  vector<int> cnt = fast_count_segments(Allpoints, m, n);
  for (size_t i = 0; i < cnt.size(); i++) {
    std::cout << cnt[i] << ' ';
  }
}
