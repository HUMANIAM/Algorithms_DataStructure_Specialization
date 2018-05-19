#include <algorithm>
#include <iostream>
#include <vector>
#include <limits>
using namespace std;

//merge sort is a good example about divide and conquer technique
template<class T>
void Merge(vector<T>& arr, int p, int q, int r)
{
    int n1 = q - p + 1;
    int n2 = r - q ;
    T tmin = numeric_limits<T>::min();

    T leftarr [n1 + 1] ;
    T rightarr [n2 + 1] ;

    for(int i=0; i<n1; i++)
        leftarr[i] = arr[p + i];

    for(int i=0; i<n2; i++)
        rightarr[i] = arr[q + i +1];

    rightarr [n2 ] = tmin;
    leftarr [n1] = tmin;

    int i=0, j=0;

    for(int k=p ; k<=r; k++)
    {
        if(leftarr[i] >= rightarr[j])
            arr[k] = leftarr[i++];
        else
            arr[k] = rightarr[j++];
    }
}

template<class T>
void Merge_sort(vector<T>& arr , int p, int r)
{
    if(p < r)
    {
        int q = (r+p) / 2;
        Merge_sort(arr, p, q);
        Merge_sort(arr, q+1, r);
        Merge(arr, p, q, r);
    }
}


long long max_dot_product(vector<int> a, vector<int> b , int n) {
  // write your code here
  Merge_sort(a, 0, n-1);
  Merge_sort(b, 0, n-1);
  long long result = 0;
  for (size_t i = 0; i < a.size(); i++) {
    result += ((long long) a[i]) * b[i];
  }
  return result;
}

int main() {
  size_t n;
  std::cin >> n;
  vector<int> a(n), b(n);
  for (size_t i = 0; i < n; i++) {
    std::cin >> a[i];
  }
  for (size_t i = 0; i < n; i++) {
    std::cin >> b[i];
  }
  std::cout << max_dot_product(a, b, n) << std::endl;
}
