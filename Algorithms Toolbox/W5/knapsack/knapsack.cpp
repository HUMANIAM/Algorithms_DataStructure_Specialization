#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h>

using namespace std;

int optimal_weight(int W, const vector<int> &w) {
  //write your code here
  vector< vector<int> > memotable( w.size()+1 , vector<int>(W+1) );

 //initialize the first row when wi = 0 and when W = 0

  for(int i=0; i<memotable.size(); i++)
    memotable[i][0] = 0;

  for(int i=0; i<memotable[0].size(); i++)
    memotable[0][i] = 0;

  for(int i=1; i<memotable.size(); i++)
      for(int weight=1; weight<memotable[i].size(); weight++)
      {
          memotable[i][weight] = memotable[i-1][weight];
          if(weight >= w[i-1])
            memotable[i][weight] = max(memotable[i][weight], ( memotable[i-1][weight-w[i-1]] + w[i-1]) );
      }

 return memotable[w.size()][W];
}

int main() {


  int n, W;
  std::cin >> W >> n;
  vector<int> w(n);
  for (int i = 0; i < n; i++) {
    std::cin >> w[i];
  }
  std::cout << optimal_weight(W, w) << '\n';
}
