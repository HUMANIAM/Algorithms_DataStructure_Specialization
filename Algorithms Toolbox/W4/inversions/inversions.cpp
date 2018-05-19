#include <iostream>
#include <vector>
#include<limits>
#include<cstdlib>
using namespace std;
//merge sort is a good example about divide and conquer technique

long long num;
template<class T>
void Merge_sort(vector<T>& arr, int p, int q, int r)
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
    while(j<n2 && i<n1)
    {
        if(leftarr[i] <= rightarr[j])
            arr[k++] = leftarr[i++];
        else
            arr[k++] = rightarr[j++];
    }
    while(j<n2)
      arr[k++] = rightarr[j++];
    while(i<n1)
        arr[k++] = leftarr[i++];
}
//this algorithm suffer from time limit exceeds
long long get_number_of_inversions2(vector<int> &a, size_t left, size_t right) {
  long long number_of_inversions = 0;
  if (right <= left ) return number_of_inversions;
  size_t ave = (right + left) / 2;
  number_of_inversions += get_number_of_inversions2(a, left, ave);
  number_of_inversions += get_number_of_inversions2(a, ave+1, right);

  for(int i=left; i<=ave; i++)
    for(int j=ave+1; j<=right; j++)
    if(a[i]>a[j])
    number_of_inversions++;
    else break;
    Merge_sort(a, left, ave, right);

  return number_of_inversions;
}



//merge sort is a good example about divide and conquer technique
void Merge(vector<int>& arr, int p, int q, int r)
{
    int n1 = q - p + 1;
    int n2 = r - q ;
    int* leftarr = new int[n1] ;
    int* rightarr = new int[n2] ;

    for(int i=0; i<n1; i++)
        leftarr[i] = arr[p + i];
    for(int i=0; i<n2; i++)
        rightarr[i] = arr[q + i +1];
    int i=0, j=0, k=p;
    while(i<n1 && j<n2)
    {
      if(leftarr[i] < rightarr[j])
      {
          for(int g=0; g<j && leftarr[i] > rightarr[g]; g++)
            num++;
          arr[k++] = leftarr[i++];
      }


      else if(leftarr[i] == rightarr[j]){
               num += j;
               arr[k++] = leftarr[i++];
           }
      else{
            arr[k++] = rightarr[j++];
            num += j;
            while((j<n2 )&& (leftarr[i] > rightarr[j]) )
                {
                  num++;
                  arr[k++] = rightarr[j++];
                }
           if(j<n2)
            arr[k++] = leftarr[i++];
          }
    }
    while(j < n2)
    arr[k++] = rightarr[j++];

    while(i < n1){
                arr[k++] = leftarr[i++];
                if(i< n1)
                num += n2;
        }
}


void get_number_of_inversions1(vector<int> &a, int left, int right) {
  if (left<right ){

  int ave = (right + left) / 2;
  get_number_of_inversions1(a, left, ave);
  get_number_of_inversions1(a, ave+1, right);
  Merge(a, left, ave, right );
  }
}

void stresstest()
{
        while(true){
        int n = 1+rand()%100000;
        vector<int>a,b,c;
        for(int i=0;i<n;i++)
        {
            int v= rand()%1000000000;
            a.push_back(v);
            b.push_back(v);
            c.push_back(v);
        }
        num=0;
        get_number_of_inversions1(a, 0, n-1);
        long long num2;
        if(num==(num2 = get_number_of_inversions2(b, 0, n-1)) )
            cout<<"YES"<<endl;
        else
        {
            for(int i=0;i<n;i++)
                cout<<c[i]<<" ";
            cout<<endl;
            cout<<num<<"    "<<num2<<endl;
            exit(0);
        }
    }
}

int main() {
//stresstest();
  int n ;
  std::cin >> n;
  vector<int> a(n);
  for (size_t i = 0; i < n; i++)
   cin>> a[i];
  get_number_of_inversions1(a, 0, n-1);
  std::cout << num << '\n';

}
