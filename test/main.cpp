#include <iostream>

using namespace std;

typedef long long int lint;
lint dp[100];

lint fib(int n){
	if (n<2) return n;
	if (dp[n])return dp[n];
	return dp[n]=fib(n-1) + fib(n-2);
}
int main()
{
    cout << fib(65) << endl;
    return 0;
}
