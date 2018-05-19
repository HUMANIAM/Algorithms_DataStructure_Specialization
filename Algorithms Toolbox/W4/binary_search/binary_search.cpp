#include <iostream>
#include <cassert>
#include <vector>
#include <cstdlib>

using namespace std;

int binary_search(const vector<int> &a, int l, int r, int x) {

  if(l <= r)
  {
      int q = ((r - l) / 2)+l;
      if(a[q] == x)
        return q;

      else if(x > a[q])
        return binary_search(a, q+1, r, x);

      else
        return binary_search(a, l, q-1, x);
  }

    return -1;
}

int linear_search(const vector<int> &a, int x) {
  for (size_t i = 0; i < a.size(); ++i) {
    if (a[i] == x) return i;
  }
  return -1;
}

int main() {


  int n;
  std::cin >> n;
  vector<int> a(n);
  for (size_t i = 0; i < a.size(); i++)
    std::cin >> a[i];

  int m;
  std::cin >> m;
  vector<int> b(m);
  for (int i = 0; i < m; ++i)
    std::cin >> b[i];

  for (int i = 0; i < m; ++i) {
    //replace with the call to binary_search when implemented
    std::cout << binary_search(a, 0, n-1, b[i]) << ' ';
  }
}
