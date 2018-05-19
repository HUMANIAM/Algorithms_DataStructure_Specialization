#include <iostream>

int get_change(int m) {
  //write your code here
  int c10 = m/10;
  int c5 = (m - c10 * 10) / 5 ;
  int c1 = m - ( c5 * 5) - ( c10 * 10);

  return ( c1 + c5 + c10 );
}

int main() {
  int m;
  std::cin >> m;
  std::cout << get_change(m) << '\n';
}
