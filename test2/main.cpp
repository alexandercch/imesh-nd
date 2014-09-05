#include <iostream>
#include <algorithm>

using namespace std;

int main(){
	int arr[5]={4,3,6,8,87};
	sort(arr, arr+5);
	for(int i=0; i<5; ++i)
		cout<<arr[i]<<" ";
	cout<<endl;

}

