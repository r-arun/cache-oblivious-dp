#include <iostream>
#include <vector>
#include <string>
#include <cstring>

//recursive solution
#define SIZE 12
using namespace std;

string x;
string y;
string z;
int dp[SIZE][SIZE];

int lcs(int i, int j){
	if(i < 0 || j < 0)
		return 0;
	if(dp[i][j] >= 0)
		return dp[i][j];
	if(x[i] == y[j]){
		dp[i][j] = lcs(i - 1, j - 1) + 1;
		return dp[i][j];
	}
	else{
		int v1 = lcs(i - 1, j);
		int v2 = lcs(i, j - 1);
		dp[i][j] = max(v1, v2);
		return dp[i][j];
	}
}

void print_z(int i, int j){
	if(i < 0 || j < 0)
		return;
	if(x[i] == y[j]){
		print_z(i-1, j-1);
		cout<<x[i];
	}
	else if(dp[i][j-1] > dp[i-1][j])
		print_z(i, j-1);
	else
		print_z(i-1, j);
}
int main(){
	cin>>x;
	cin>>y;
	int xlen = x.size();
	int ylen = y.size();
	for(int i = 0; i < xlen; ++i){
		for(int j = 0; j < ylen; ++j){
			dp[i][j] = -1;
		}
	}
	cout<<lcs(xlen - 1, ylen -1)<<endl;
	cout<<"LCS: ";
	print_z(xlen - 1, ylen -1);
	cout<<endl;
}	
