#include <iostream>

int get_fibonacci_last_digit_naive(int n) {
    if (n <= 1)
        return n;

int f1=0, f2=1, temp;
   for(int i=2; i<=n; i++){
       temp = f2;
       f2 = (f1+f2) % 10;
       f1=temp;
   }
        return f2;
}

int main() {
    int n;
    std::cin >> n;
    int c = get_fibonacci_last_digit_naive(n);
    std::cout << c << '\n';
    }
