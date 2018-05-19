#include <iostream>
#include <vector>
using namespace std;

int lcs3(vector<int> &a, vector<int> &b, vector<int> &c) {
    int n = a.size();
    int m = b.size();
    int k = c.size();
    int memotable[n+1][m+1][k+1];
 //   initialize_memotable
     for(int i=0; i<=m; i++)
       for(int j=0; j<=k; j++)
          memotable[0][i][j] = 0;

     for(int i=0; i<=n; i++)
       for(int j=0; j<=k; j++)
          memotable[i][0][j] = 0;

           for(int i=0; i<=n; i++)
       for(int j=0; j<=m; j++)
          memotable[i][j][0] = 0;

    //filling the memotable with results of subproblems
    for(int x=1; x<=n; x++)
       for(int y=1; y<=m; y++)
         for(int z=1; z<=k; z++)
            if( (a[x-1] == b[y-1]) && (a[x-1]== c[z-1]))
                memotable[x][y][z] = memotable[x-1][y-1][z-1] + 1;
            else
                memotable[x][y][z] = max( max(memotable[x-1][y][z], memotable[x][y-1][z]), memotable[x][y][z-1]);

    return memotable[n][m][k];
}

int main() {

  size_t an;
  std::cin >> an;
  vector<int> a(an);
  for (size_t i = 0; i < an; i++) {
    std::cin >> a[i];
  }
  size_t bn;
  std::cin >> bn;
  vector<int> b(bn);
  for (size_t i = 0; i < bn; i++) {
    std::cin >> b[i];
  }
  size_t cn;
  std::cin >> cn;
  vector<int> c(cn);
  for (size_t i = 0; i < cn; i++) {
    std::cin >> c[i];
  }
  std::cout << lcs3(a, b, c) << std::endl;
}
