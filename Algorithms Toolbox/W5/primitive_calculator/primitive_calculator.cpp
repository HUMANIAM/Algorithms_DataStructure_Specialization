#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <stdlib.h>
using namespace std;

struct Minop{
    int minop;
    char op;
    Minop()
    {
        minop = 0;
        op = 'n';
    }
};

vector<Minop> optimal_sequence(int n) {
  std::vector<Minop> sequence(n+1);
  //using the bottom up approach
  for(int i=2; i<=n; i++)
  {
      sequence[i].minop = sequence[i-1].minop + 1;
      sequence[i].op = '+';

      int v;
      if(i%2 == 0)
        if(sequence[i].minop > ( v = sequence[i/2].minop +1 ))
      {
          sequence[i].minop = v;
          sequence[i].op = '2';
      }

      if(i%3 == 0)
        if(sequence[i].minop > ( v = sequence[i/3].minop +1 ))
      {
          sequence[i].minop = v;
          sequence[i].op = '3';
      }
  }

  return sequence;
}

void printoptimalsol( vector<Minop>& sequ )
{

    vector<int>optimalsol;
    for(int i=sequ.size()-1; i>0; i--)
    {
        optimalsol.push_back(i);

        if(sequ[i].op == '2')
            i = i/2 + 1;

        if(sequ[i].op == '3')
            i = i/3 + 1;
    }
    for(int i=optimalsol.size()-1; i>=0; i--)
        cout<<optimalsol[i]<<"  ";
}
int main() {
  int n;
  std::cin >> n;
  vector<Minop> sequence = optimal_sequence(n);

  std::cout << sequence[n].minop << std::endl;

  printoptimalsol(sequence);
}
