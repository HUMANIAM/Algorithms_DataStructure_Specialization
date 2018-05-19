#include <iostream>
#include <vector>
using std::vector;

int fibonacci_sum_rem10(long long n) {
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


long long get_fibonacci_partial_sum_naive(long long from, long long to) {
    if (to <= 1)
        return to;

    int sumfrom = fibonacci_sum_rem10(from-1);   //sum of finbonacci till Fm-1
    int sumto = fibonacci_sum_rem10(to);       // sum of fibonacci till Fn

    if ( sumto >= sumfrom )
       return (sumto - sumfrom);
    return (sumto - sumfrom + 10);

}

int main() {
    long long from, to;
    std::cin >> from >> to;
    std::cout << get_fibonacci_partial_sum_naive(from, to) << '\n';
}
