#include <iostream>
#include <vector>

using namespace std;

vector<int> optimal_summands(int n) {
  vector<int> summands;
  int nwrank = 1;

  while(n != 0)
  {
    if( (n -= nwrank) <= nwrank )
    {
        nwrank += n;
        n=0;
    }
    summands.push_back(nwrank++);
  }
  return summands;
}

int main() {

  int n  ;
  std::cin >> n;
  vector<int> summands = optimal_summands(n);
  std::cout << summands.size() << '\n';
  for (size_t i = 0; i < summands.size(); ++i) {
    std::cout << summands[i] << ' ';
  }

    return 0;
}
