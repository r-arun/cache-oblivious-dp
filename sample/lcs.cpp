#include <iostream>
#include <vector>
#include <string>
#include <cstring>

//Simple iterative dp

using namespace std;

int main(){
	string x;
	string y;
	string z;
	cin>>x;
	cin>>y;
	int xlen = x.size();
	int ylen = y.size();
	int dp[xlen][ylen];
	memset(dp, xlen * ylen, 0);
	for(int i = 0; i < ylen; ++i){
		dp[0][i] = (x[0] == y[i]) ? 1: dp[0][i-1];	
	}
	for(int i = 1; i < xlen; ++i){
		dp[i][0] = (x[i] == y[0]) ? 1: dp[i-1][0];
		for(int j = 1; j < ylen; ++j){
			if(x[i] == y[j])
				dp[i][j] = dp[i-1][j-1] + 1;
			else{
				dp[i][j] = (dp[i-1][j] > dp[i][j-1]) ? dp[i-1][j]: dp[i][j-1];
			}
		}
	}
	int ipos = xlen - 1;
	int jpos = ylen - 1;
	cout<<dp[ipos][jpos]<<endl;
	while(ipos >= 0 && jpos >= 0){
		if(x[ipos] == y[jpos]){
			z = x[ipos] + z;
			--ipos;
			--jpos;
		}
		else if(dp[ipos-1][jpos] > dp[ipos][jpos-1])
			--ipos;
		else
			--jpos;
	}
	cout<<z<<endl;
}
