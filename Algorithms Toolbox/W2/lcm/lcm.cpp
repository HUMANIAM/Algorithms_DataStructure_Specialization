#include <iostream>

int gcd_naive(int a, int b) {

  if(b == 0) return a;

    int temp;
    while( a%b !=0)
    {
        temp = a;
        a = b;
        b = temp%a;
    }
    return b;
}

long long lcm_naive(int a, int b) {

  return (long long) a * b / gcd_naive(a,b);
}

int main() {
  int a, b;
  std::cin >> a >> b;
  std::cout << lcm_naive(a, b) << std::endl;
  return 0;
}
