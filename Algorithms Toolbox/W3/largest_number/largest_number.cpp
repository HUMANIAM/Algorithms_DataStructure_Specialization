#include <algorithm>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

bool TheSame(string s1 , string s2)
{
    int i=0, j=0;
    int len1 = s1.length();
    int len2 = s2.length();

    while(i != len1 || j != len2)
    {
        i = i % len1;
        j = j % len2;
        if(s1[i++] != s2[j++] )
            return false;
    }
        return true;
}

bool IsGreater(string s1, string s2)
{
    if(TheSame(s1, s2))
        return true;

    int i=0, j=0;
    while( true )
    {
        if( s1[i] > s2[j] )
             return true;
        else if( s1[i] < s2[j] )
            return false;

        else
        {
            i = (++i) % s1.length();
            j = (++j) % s2.length();
        }
    }
}

//merge sort is a good example about divide and conquer technique
template<class T>
void Merge(vector<T>& arr, int p, int q, int r)
{
    int n1 = q - p + 1;
    int n2 = r - q ;
    T leftarr [n1] ;
    T rightarr [n2] ;

    for(int i=0; i<n1; i++)
        leftarr[i] = arr[p + i];

    for(int i=0; i<n2; i++)
        rightarr[i] = arr[q + i +1];

    int i=0, j=0, k=p;

    while(i != n1 && j != n2)
        {
            if(IsGreater( leftarr[i], rightarr[j] ))
                arr[k++] = leftarr[i++];
            else
                arr[k++] = rightarr[j++];
        }

    while(i != n1)
      arr[k++] = leftarr[i++];
    while( j != n2)
      arr[k++] = rightarr[j++];
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

string largest_number(vector<string> a, int n) {
  //write your code here
  Merge_sort(a, 0, n-1);
  std::stringstream ret;
  for (size_t i = 0; i < n; i++) {
    ret << a[i];
  }
  string result;
  ret >> result;
  return result;
}

int main() {
//while(true){
  int n;
  cin >> n;
  vector<string> a(n);

  for (size_t i = 0; i < a.size(); i++)
    cin >> a[i];

  cout << largest_number(a, n);
//}
return 0;
}
