#include <iostream>

using namespace std;
long long FabonacciRec(int n)
{
    //base cases
    if(n==0)
        return 0;
    if(n==1)
        return 1;

    return FabonacciRec(n-1)+FabonacciRec(n-2);
}

long long Fabonacci(int n)
{
     if(n==0)
        return 0;
    if(n==1)
        return 1;

   int f1=0, f2=1, temp;

   for(int i=2; i<n; i++)
   {
       temp = f2;
       f2 = f1+f2;
       f1=temp;

   }
        return f1+f2;

}

int GCD(int a, int b)
{
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
int main()
{


    return 0;
}
