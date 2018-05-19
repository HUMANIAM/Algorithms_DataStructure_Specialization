#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <iomanip>
using namespace std;

//merge sort from large to small
void Mergesort(vector<int>& weight ,vector<double>& unitvalue , int p, int q, int r)
{
    int n1 = q - p + 1 ;
    int n2 = r - q ;
    int minint = numeric_limits<int>::min();
    double leftvec[n1 + 1][2];
    double rightvec[n2 + 1][2];
    int i = 0;
    int j = 0;

    for(int i=0; i<n1 ; i++)
        {
            leftvec[i][0] = unitvalue[p+i];
            leftvec[i][1] = weight[p+i];
        }
    for(int i=0; i<n2 ; i++){
        rightvec[i][0] = unitvalue[q+i+1];
        rightvec[i][1] = weight[q+i+1];
    }

    leftvec[n1][0] = rightvec[n2][0] = minint;
    leftvec[n1][1] = rightvec[n2][1] = 0;

    for(int k=p; k<=r ; k++)
    {
        if(leftvec[i][0] <= rightvec[j][0]){
            unitvalue[k] = rightvec[j][0];
            weight[k] = rightvec[j++][1];
            }
        else{
                unitvalue[k] = leftvec[i][0];
                weight[k] = leftvec[i++][1];
            }
    }
}

void Merge_sort(vector<int>& weight ,vector<double>& unitvalue, int p, int r)
{

    if(p < r)
    {
        int q= (p + r) / 2;
        Merge_sort(weight , unitvalue , p , q);
        Merge_sort(weight , unitvalue , q+1 , r);
        Mergesort(weight , unitvalue , p , q , r);
    }
}


double get_optimal_value(int capacity, vector<int> weights, vector<int> values , int n) {
  double value = 0.0;

  vector<double>unitvalues(n);
  for(int i=0; i<n; i++)
    unitvalues[i] =double(values[i]) / double(weights[i]);
    //sort the values
    Merge_sort(weights,unitvalues,0,n-1);

    int k = 0;
  while(capacity != 0 && k <n)
  {
      int a = min(capacity , weights[k]);
      value += a * unitvalues[k++];
      capacity -= a;
  }

  return value;
}


int main() {
  int n ;

  int capacity;
  std::cin >> n >> capacity;
  vector<int> values(n);
  vector<int> weights(n);
  for (int i = 0; i < n; i++) {
    std::cin >> values[i] >> weights[i];
  }

  double optimal_value = get_optimal_value(capacity, weights, values , n);


  std::cout <<setprecision(4)<<fixed<<optimal_value << std::endl;
  return 0;
}
