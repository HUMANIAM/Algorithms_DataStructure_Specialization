#include <iostream>
#include <vector>
using namespace std;
int fibonacci_sum_naive(long long n) {
    if (n <= 1)
        return n;

    int preremind   = 0;    // previous reminder
    int currremind  = 1;    // current reminder
    int temp;
    long long i;
    vector<int> a;
    a.push_back(0);
    a.push_back(1);

    for (i = 2; i <= n ; ++i) {
            //calculate Fn%10
        temp = currremind;
        currremind = (preremind + currremind) % 10;
        preremind = temp;
        a.push_back((currremind + a[i-1]) % 10);

        if( preremind == 0 && currremind == 1)
            break;
    }
    if(--i == n)
      return a[i];

    return a[n % i];
}

int main() {
    long long n = 0;

    std::cin >> n;
    std::cout << fibonacci_sum_naive(n);
}
