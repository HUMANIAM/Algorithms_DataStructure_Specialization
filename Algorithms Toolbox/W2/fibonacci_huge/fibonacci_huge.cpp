#include <iostream>
#include <vector>
using namespace std;

int get_fibonacci_huge_naive(long long n, int m) {
    if (n <= 1)
        return n;

    int previous = 0;
    int current  = 1;
    int temp;
    vector<int>arr;
    long long i;

   arr.push_back(0);
   arr.push_back(1);

    for (i = 2; i <= n ; ++i) {
            //calculate Fn%m
        temp  = current;
        current = (current  + previous ) % m ;
        previous = temp;
        arr.push_back(current);
        if(previous == 0 && current == 1)
            break;
      }

    if(--i == n)
      return arr[i];

    return arr[n % i];
}

int main() {

    long long n;
    int m;

    std::cin >> n >> m;
    std::cout << get_fibonacci_huge_naive(n, m) << '\n';



    return 0;

}
