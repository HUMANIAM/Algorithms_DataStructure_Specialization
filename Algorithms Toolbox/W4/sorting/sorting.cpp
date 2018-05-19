#include <iostream>
#include <vector>
#include <cstdlib>
using namespace std;

void partition3(vector<int> &a, int l, int r, int& m1, int& m2) {

  int k = l + rand() % (r - l + 1);
  swap(a[r], a[k]);
  int x = a[r];
  int index = 1;
  int j = l;
  int i = l;

while( i < (r - index +1) ) {

    if (a[i] < x)
      swap(a[i], a[j++]);

    else if(a[i] == x)
    {
        swap(a[i--], a[r-index]);
        index++;
    }
   i++;
  }

  m1=j;
  for(int k=0; k<index; k++)
    swap(a[j++], a[r - k]);
  m2 = j - 1;
}

void randomized_quick_sort(vector<int> &a, int l, int r) {
  if (l >= r)
    return;

  int m1,m2;
  partition3(a, l, r, m1, m2);
  randomized_quick_sort(a, l, m1 - 1);
  randomized_quick_sort(a, m2 + 1, r);
}

int main() {
  int n;
  std::cin >> n;
  vector<int> a(n);
  for (size_t i = 0; i < a.size(); ++i) {
    std::cin >> a[i];
  }
  randomized_quick_sort(a,0,n-1);
//  randomized_quick_sort(a, 0, a.size() - 1);
  for (size_t i = 0; i < a.size(); ++i) {
    std::cout << a[i] << ' ';
  }
}
