#include <iostream>
#include <cassert>
#include <string>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <limits>
using namespace std;

long long eval(long long a, long long b, char op) {
  if (op == '*') {
    return a * b;
  } else if (op == '+') {
    return a + b;
  } else if (op == '-') {
    return a - b;
  } else {
    assert(0);
  }
}

long long get_maximum_value(const string &operations, const string& operators) {
  int n = operators.size();
  vector<vector<long long> >maxvals(n, vector<long long>(n) );
  vector<vector<long long> >minvals(n, vector<long long>(n) );
  for(int i=0; i<n; i++)
    maxvals[i][i] = minvals[i][i] = operators[i] - '0';

  int gap = 1;
  while(gap < n){
    for(int i=0; i < n-gap ; i++){
        int j = i+gap;
        maxvals[i][j] = numeric_limits<long long>::min();
        minvals[i][j] = numeric_limits<long long>::max();
        for(int k=i; k<j; k++)
        {
            long long a = eval(maxvals[i][k], maxvals[k+1][j], operations[k]);
            long long b = eval(minvals[i][k], maxvals[k+1][j], operations[k]);
            long long c = eval(maxvals[i][k], minvals[k+1][j], operations[k]);
            long long d = eval(minvals[i][k], minvals[k+1][j], operations[k]);
            maxvals[i][j] = max ( maxvals[i][j] , max( max(c,d), max(a,b)) );
            minvals[i][j] = min ( minvals[i][j] , min( min(c,d), min(a,b)) );
        }
    }
     gap++ ;
  }
  return maxvals[0][n-1];
}

void Operation_operator(string& operators, string& operations, const string& s)
{
    for(int i=0; i<s.size()-1; i +=2)
    {
      operators.append(1,s[i]);
      operations.append(1,s[i+1]);
    }
   operators.append(1,s[s.size()-1]);
}
int main() {

  string s;
  std::cin >> s;
  string operators, operations;
  Operation_operator(operators, operations, s);
 std::cout << get_maximum_value(operations, operators) << '\n';
}
