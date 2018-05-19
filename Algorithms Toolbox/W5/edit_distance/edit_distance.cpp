#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>

using namespace std;
int edit_distance(const string &str1, const string &str2) {
  //write your code here
  vector< vector<int> > memotable( str1.size()+1, vector<int>(str2.size()+1) );
  memotable[0][0] = 0;

  for(int i=1; i<memotable.size(); i++)
    memotable[i][0] = i;

  for(int i=1; i<memotable[0].size(); i++)
    memotable[0][i] = i;

  for(int i=1; i<memotable.size(); i++)
    for(int j=1; j<memotable[i].size(); j++)
        if(str1[i-1] == str2[j-1])
            memotable[i][j] = memotable[i-1][j-1];
        else
            memotable[i][j] = min( min( memotable[i-1][j-1]+1 , memotable[i][j-1]+1 ) , memotable[i-1][j]+1  );

  return memotable[str1.size()][str2.size()];
}

int main() {
  string str1;
  string str2;
  std::cin >> str1 >> str2;
  std::cout << edit_distance(str1, str2) << std::endl;
  return 0;
}
