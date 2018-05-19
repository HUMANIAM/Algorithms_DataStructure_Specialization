#include <algorithm>
#include <iostream>
#include<stdlib.h>
#include <vector>
#include <map>

using namespace std;

int Major_element(int a[], int r, int l, int m1, int m2)
{
    int count1=0;
    int count2=0;
    for(int i=l;i<=r;i++){
        if(a[i] == m1)
        count1++;
        if(a[i]== m2)
        count2++;
   }
   if(count1 > (r-l+1)/2)
    return m1;
   else if(count2>(r-l+1)/2)
    return m2;
   else
    return -1;
}

int get_majority_element(int a[], int l, int r) {
    if(l>r) return -1;
   if(l==r)return a[l];
   int q = (l+r)/2;
   int m1 = get_majority_element(a,l,q);
   int m2 = get_majority_element(a,q+1,r);
   if(m1 == m2)
    return m1;
   else
    return Major_element(a,r,l,m1,m2);


}
//solve it by map<element , number of occurence>
void Majority_by_map()
{
     int n;
  map<int ,int>a;
  std::cin >> n;
  int input;

  for (size_t i = 0; i < n; ++i)
    {
        cin>>input;
        if ( a.find(input) == a.end() )
            a[input]=1;
        else
            a[input]++;
  }

bool found = false;
  for(  map<int,int>::iterator it = a.begin(); it != a.end(); it++){
        if((*it).second > n/2){
        cout<<1;
        found = true;
        }
    }
if(!found)
    cout<<0;
}


int main() {

  int n;
  std::cin >> n;
   int a[n];
  for (size_t i = 0; i < n; ++i)
    cin>>a[i];
 std::cout << (get_majority_element(a,0, n-1) != -1) << '\n';
}
