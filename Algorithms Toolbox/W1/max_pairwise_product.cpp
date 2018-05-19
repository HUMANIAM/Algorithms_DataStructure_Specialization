#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;
using std::cin;
using std::cout;
/*
int Maxvalue(const vector<int>& numbers, int sz)
{
    int maxindex = 0;

    for(int i=1; i<sz; i++)
        if(numbers[maxindex] < numbers[i])
            maxindex = i;

    return maxindex;
}

long long MaxPairwiseProduct( vector<int>& numbers)
{
  int n = numbers.size();

  int maxindex1 = Maxvalue(numbers , n);
  int maxvalue1 = numbers[maxindex1];
  numbers.erase(numbers.begin() + maxindex1);

  int maxindex2 = Maxvalue(numbers , n-1);
  int maxvalue2 = numbers[maxindex2];

  return (1LL * maxvalue1 * maxvalue2);
}
*/

long long MaxPairwiseProduct(const vector<int>& numbers) {
  int n = numbers.size();

  int max_index1 = 0;
  for (int i = 1; i < n; ++i)
    if ((numbers[i] > numbers[max_index1]))
      max_index1 = i;

  int max_index2 = -1;
   for (int j = 0; j < n; ++j)
    if ((numbers[j] != numbers[max_index1]) && ((max_index2 == -1) || (numbers[j] > numbers[max_index2])))
      max_index2 = j;

  return ((long long)(numbers[max_index1])) * numbers[max_index2];
}
int main() {

       int n;
    cin >> n;
    vector<int> numbers(n);
    for (int i = 0; i < n; ++i) {
        cin >> numbers[i];
    }

    long long result =  MaxPairwiseProduct(numbers);
    cout << result << "\n";


    return 0;
}
