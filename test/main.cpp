#include <iostream>

using namespace std;
int dp[100];

int fib(int n){
	if (n<2) return n;
	if (dp[n])return dp[n];
	return dp[n]=fib(n-1) + fib(n-2);
}
int main()
{
    cout << fib(35) << endl;
    return 0;
}
