#include <iostream>
#include <string>
#include <cstdlib>
#include <cassert>
#include <ctime>
#include <cmath>
#define TEST_PARTB 0
#define TEST_PARTC 0
#define PART_A_TESTS 0

#define TRIANGLE 0
#define SQUARE_MID 1
#define SQUARE_HORIZONTAL 2
#define SQUARE_VERTICAL 3
#define DEBUG 1

using namespace std;
#define MAX_VAL 500
string str;

int dp[MAX_VAL][MAX_VAL];
int arr[MAX_VAL][MAX_VAL];

int n = 0;
struct point{
	int x;
	int y;
};

typedef struct point pt;

void print_point(string str, pt I){
	cout<<str<<" "<<I.x<<" "<<I.y<<endl;
}

void midpoints(pt p1, pt p2, pt * result, int shape){
	//result an array of size 2.
	result[0].x = (p1.x + p2.x)/2;
	result[0].y = (p1.y + p2.y)/2;
	if(shape == TRIANGLE || shape == SQUARE_MID){
		result[1].x = result[0].x + 1;
		result[1].y = result[0].y + 1;
	}
	else if(shape == SQUARE_HORIZONTAL){
		result[1].x = result[0].x;
		result[1].y = result[0].y + 1;
	}
	else if(shape == SQUARE_VERTICAL){
		result[1].x = result[0].x + 1;
		result[1].y = result[0].y;
	}
}

bool equals(pt p1, pt p2){
	return p1.x == p2.x && p1.y == p2.y;
}
int score_fold(int i, int j, int k){
	int l = j-1;
	int r = j+2;
	//cout<<i<<" "<<j<<" "<<k<<" "<<str.size()<<endl;
	int cnt = 0;
	while(l >= i && r <=k){
		assert(l >= 0);
		assert(l < str.size());
		assert(r < str.size());
		if(str[l] == str[r] && str[l] == '1')
			cnt++;	
		l--;r++;
	}
	return cnt;
}
int score(int i, int j, int n){
	int v = 0;
	//cout<<"SCORE CALL "<<i<<" "<<j<<endl;
	//if(j >= k) return 0;
	for(int k = j+2; k < n; k++){
		v = max(v, score_fold(i,j,k)+ score(j + 1,k, n));
	}
	return v;
}

int score_mem(int i, int j, int n){
	if(dp[i][j] != -1) return dp[i][j];
	int v = 0;
	//cout<<"SCORE_MEM CALL "<<i<<" "<<j<<endl;
	//if(j >= k) return 0;
	for(int k = j+2; k < n; k++){
		v = max(v, score_fold(i,j,k)+ score_mem(j + 1,k, n));
	}
	dp[i][j] = v;
	return v;
}

int score_iterative(int n){
	for(int end = n-1; end > 0; --end){
		for(int start = end-1; start >= 0; --start){
			int val = 0;
			for(int k = end + 2; k < n; ++k){
				val = max(val, score_fold(start,end, k) + arr[end + 1][k]);
			}
			arr[start][end] = val;
		}
	}
	int result = 0;
	for(int j = 1; j < n-1; ++j)
		result =max(result, arr[0][j]);
	return result;
}

int score_iterative2(int n){
	int score_arr[n][n][n];
#if TEST_PARTB
	int score2_arr[n][n][n];
	for(int i = 0; i < n-1; ++i){
		for(int j = i+1; j < n; ++j){
			for(int k = j+2; k < n; ++k){
				score2_arr[i][j][k] = score_fold(i, j, k);
			}
		}
	}
#endif
	for(int i = 0; i < n-1; ++i){
		for(int j = i+1; j < n; ++j){
			score_arr[i][j][j] = 0;
			if(j+1 < n) score_arr[i][j][j+1]= 0;
			for(int l = 1; j+l+1 < n; ++l){
				score_arr[i][j][j+l+1] = score_arr[i][j][j+l];
				if(j-l >= i && (str[j-l] == str[j+l+1] && str[j-l] =='1'))
					score_arr[i][j][j+l+1]++;
			#if TEST_PARTB
				assert(score2_arr[i][j][j+l+1] == score_arr[i][j][j+l+1]);
			#endif
			}
		}
	}
	for(int end = n-1; end > 0; --end){
		for(int start = end-1; start >= 0; --start){
			int val = 0;
			for(int k = end + 2; k < n; ++k){
				val = max(val, score_arr[start][end][k] + arr[end + 1][k]);
			}
			arr[start][end] = val;
		}
	}
	int result = 0;
	for(int j = 1; j < n-1; ++j)
		result =max(result, arr[0][j]);
	return result;
}
int score_iterative3_d(int n){
#if TEST_PARTC
	int score_arr[n][n][n];
	for(int i = 0; i < n-1; ++i){
		for(int j = i; j < n; ++j){
			for(int k = j; k < n; ++k){
				score_arr[i][j][k] = score_fold(i, j, k);
			}
		}
	}
#endif
//Pre-compute score_arr //Apply values using a simpler iterative version //Aim O(n^2) space. O(n^3) time.
	int score2_arr[n][n];
	for(int i = 0; i < n; ++i){
		for(int j = 0; j < n; ++j){
			arr[i][j] = 0;
		}
	}
	for(int k = n-1; k >= 0; --k){
		for(int i = 0; i < k; ++i){
			for(int j = i+1; j < k; ++j){
				score2_arr[i][j] = score_fold(i,j,k);
			}
		}
		/*for(int j = k; j >= 0; --j){ //first fold
			int l = 1;
			score2_arr[j][j+1] = 0;
			while(j-l >= 0){
				score2_arr[j][j-l] = score2_arr[j][j-l+1];
				if(j+l <= k && str[j-l] == str[j+1+l] && str[j-l]=='1')
						++score2_arr[j][j-l];
				l++;
			}
		}
		*/
	#if TEST_PARTC
		for(int i = 0; i < k; ++i){
			for(int j = i+1; j < k; ++j){
				assert(score_arr[i][j][k] == score2_arr[i][j]);
			}
		}
	#endif	
		//now use score in opt-value calcuation
		/**
		*
		*/
		for(int j = k-2; j >= 0; --j){ //first fold
			for(int i = j - 1; i >= 0; --i){ //second fold
				arr[i][j]  =  max(score2_arr[i][j] + arr[j+1][k],
								arr[i][j]);
			}
		}
	}
	int ret = 0;
	for(int j = 0; j < n; ++j){
		ret = ret < arr[0][j]?arr[0][j]:ret;
	}
	return ret;
}


int score_iterative3_c(int n){
	//Pre-compute score_arr
	//Apply values using a simpler iterative version
	//Aim O(n^2) space. O(n^3) time.
	int score2_arr[n][n];
	for(int i = 0; i < n; ++i){
		for(int j = 0; j < n; ++j){
			arr[i][j] = 0;
		}
	}
	for(int i = n-1; i >= 0; --i){
		for(int j = i+1; j < n; ++j){ //first fold
			int l = 1;
			score2_arr[j][j+1] = 0;
			while(j+1+l < n){
				score2_arr[j][j+1+l] = score2_arr[j][j+l];
				if(j-l >= i && str[j-l] == str[j+1+l] && str[j-l]=='1')
						++score2_arr[j][j+1+l];
				l++;
			}
		}
		//now use score in opt-value calcuation
		/**
		*
		*/
		for(int j = n-1; j > i; --j){ //first fold
			arr[i][j] = 0;
			for(int k = n-1; k >= j+2; --k){ //second fold
				arr[i][j]  =  max(score2_arr[j][k] + arr[j+1][k],
								arr[i][j]);
			}
		}
	}
	int ret = 0;
	for(int j = 0; j < n; ++j){
		ret = ret < arr[0][j]?arr[0][j]:ret;
	}
	return ret;
}

int score_iterative3_b(int n){
	//Pre-compute score_arr
	//Apply values using a simpler iterative version
	//Aim O(n^2) space. O(n^3) time.
	int score2_arr[n][n];
	for(int i = 0; i < n; ++i){
		for(int j = 0; j < n; ++j){
			arr[i][j] = 0;
		}
	}
	for(int i = n-1; i >= 0; --i){
		for(int j = i+1; j < n; ++j){ //first fold
			int l = 1;
			score2_arr[j][j+1] = 0;
			while(j+1+l < n){
				score2_arr[j][j+1+l] = score2_arr[j][j+l];
				if(j-l >= i && str[j-l] == str[j+1+l] && str[j-l]=='1')
						++score2_arr[j][j+1+l];
				l++;
			}
		}
		//now use score in opt-value calcuation
		/**
		*
		*/
		for(int j = i+1; j < n; ++j){ //first fold
			arr[i][j] = 0;
			for(int k = j+2; k < n; ++k){ //second fold
				arr[i][j]  =  max(score2_arr[j][k] + arr[j+1][k],
								arr[i][j]);
			}
		}
	}
	int ret = 0;
	for(int j = 0; j < n; ++j){
		ret = ret < arr[0][j]?arr[0][j]:ret;
	}
	return ret;
}
int score_iterative3(int n){
#if TEST_PARTC
	int score2_arr[n][n][n];
	for(int i = 0; i < n-1; ++i){
		for(int j = i+1; j < n; ++j){
			for(int k = j+2; k < n; ++k){
				score2_arr[i][j][k] = score_fold(i, j, k);
			}
		}
	}
#endif
	for(int end = n-1; end > 0; --end){
		int score_arr[n][n];
		for(int start = end-1; start >= 0; --start){
			score_arr[start][end] = 0;
			if(end+1 < n)
				score_arr[start][end+1] = 0;
			for(int k = 1; k+end+1 < n; ++k){
				score_arr[start][k +end+ 1] = score_arr[start][end+k];
				if(end-k >= start && str[end-k] == str[end+k+1] && str[end-k] =='1')
					score_arr[start][end+k+1]++;
			#if TEST_PARTC
				//cout<<start<<" "<<end<<" "<<end+k+1<<" "<<score_arr[start][end+k+1]<<" "<<score2_arr[start][end][end+k+1]<<endl;
				assert(score_arr[start][end+k+1] == score2_arr[start][end][end+k+1]);
			#endif
			}
		}
		for(int start = end-1; start >= 0; --start){
			int val = 0;
			for(int k = end + 2; k < n; ++k){
				val = max(val, score_arr[start][k] + arr[end + 1][k]);
			}
			arr[start][end] = val;
		}
	}
	int result = 0;
	for(int j = 1; j < n-1; ++j)
		result =max(result, arr[0][j]);
	return result;
}
string generate_random_string(){
	int len = (rand() % 9) + 1;
	len = (int)pow(2.0, len);
	//len = 32;
	string s = "";
	for(int i = 0; i < len; ++i){
		int val = (rand() % 10);
		if(val < 5) s = s + '1';
		else s = s + '0';
	}
	return s;
}
void setPoint(pt * p, int x, int y){
	p->x = x;
	p->y = y;
}

void fss(pt I00, pt I22, pt D00, pt D22){
#if DEBUG
	cout<<"fss"<<endl;
	cout << I00.x <<" "<< I00.y<<" "<< I22.x <<" "<< I22.y <<" "<< D00.x <<" "<< D00.y <<" "<< D22.x <<" "<<D22.y<<endl;
#endif
	if(equals(I00,I22)){
		//cout<<"Base case"<<endl;
		assert(equals(D00,D22));
		if(D00.x == n-1){
			arr[I00.x][I00.y] = 0;
			return;
		}
		arr[I00.x][I00.y] = max(arr[I00.x][I00.y], arr[D00.x + 1][D00.y] + score_fold(I00.x, I00.y, D00.y));
		return;
	}
	pt I02;
	setPoint(&I02, I00.x, I22.y);
	pt I20;
	setPoint(&I20, I22.x, I00.y);

	pt I01[2];
	pt Im0[2];
	pt Im1[2];
	pt Im2[2];
	pt I21[2];

	midpoints(I00, I02, I01, SQUARE_HORIZONTAL);
	midpoints(I00, I20, Im0, SQUARE_VERTICAL);
	midpoints(I02, I22, Im2, SQUARE_VERTICAL);
	midpoints(I20, I22, I21, SQUARE_HORIZONTAL);
	midpoints(Im0[0], Im2[0], Im1, SQUARE_MID);

	pt D02;
	setPoint(&D02, D00.x, D22.y);
	pt D20;
	setPoint(&D20, D22.x, D00.y);

	pt D01[2];
	pt Dm0[2];
	pt Dm1[2];
	pt Dm2[2];
	pt D21[2];

	midpoints(D00, D02, D01, SQUARE_HORIZONTAL);
	midpoints(D00, D20, Dm0, SQUARE_VERTICAL);
	midpoints(D02, D22, Dm2, SQUARE_VERTICAL);
	midpoints(D20, D22, D21, SQUARE_HORIZONTAL);
	midpoints(Dm0[0], Dm2[0], Dm1, SQUARE_MID);

	fss(Im1[1], I22, Dm0[1], D21[0]);
	fss(Im1[1], I22, Dm1[1], D22);
	fss(I01[1], Im2[0], Dm0[1], D21[0]);
	fss(I01[1], Im2[0], Dm1[1], D22);
	fss(I00, Im1[0], D00, Dm1[0]);
	fss(I00, Im1[0], D01[1], Dm2[0]);
	fss(Im0[1], I21[0], D00, Dm1[0]);
	fss(Im0[1], I21[0], D01[1], Dm2[0]);
}

void fts(pt I00, pt I22, pt D00, pt D22){
#if DEBUG
	cout<<"fts"<<endl;
	cout << I00.x <<" "<< I00.y<<" "<< I22.x <<" "<< I22.y <<" "<< D00.x <<" "<< D00.y <<" "<< D22.x <<" "<<D22.y<<endl;
#endif
	if(equals(I00,I22)){
		//cout<<"Base case"<<endl;
		assert(equals(D00,D22));
		if(D00.x == n-1){
			arr[I00.x][I00.y] = 0;
			return;
		}
		arr[I00.x][I00.y] = max(arr[I00.x][I00.y], arr[D00.x + 1][D00.y] + score_fold(I00.x, I00.y, D00.y));
		return;
	}

	pt I02;
	setPoint(&I02, I00.x, I22.y);
	pt I01[2];
	pt Im2[2];
	pt Im1[2];
	midpoints(I00, I02, I01, SQUARE_HORIZONTAL);
	midpoints(I00, I22, Im1, SQUARE_MID);
	midpoints(I02, I22, Im2, SQUARE_VERTICAL);

	pt D02;
	setPoint(&D02, D00.x, D22.y);
	pt D20;
	setPoint(&D20, D22.x, D00.y);

	pt D01[2];
	pt Dm0[2];
	pt Dm1[2];
	pt Dm2[2];
	pt D21[2];

	midpoints(D00, D02, D01, SQUARE_HORIZONTAL);
	midpoints(D00, D20, Dm0, SQUARE_VERTICAL);
	midpoints(D02, D22, Dm2, SQUARE_VERTICAL);
	midpoints(D20, D22, D21, SQUARE_HORIZONTAL);
	midpoints(Dm0[0], Dm2[0], Dm1, SQUARE_MID);

	fts(Im1[1], I22, Dm0[1],D21[0]);
	fts(Im1[1], I22, Dm1[1],D22);
	fss(I01[1], Im2[0], Dm0[1],D21[0]);
	fss(I01[1], Im2[0], Dm1[1],D22);
	fts(I00, Im1[0], D00, Dm1[0]);
	fts(I00, Im1[0], D01[1], Dm2[0]);
}

void fst(pt I00, pt I22, pt D00, pt D22){
#if DEBUG
	cout<<"fst"<<endl;
	cout << I00.x <<" "<< I00.y<<" "<< I22.x <<" "<< I22.y <<" "<< D00.x <<" "<< D00.y <<" "<< D22.x <<" "<<D22.y<<endl;
#endif
	if(equals(I00,I22)){
		//cout<<"Base case"<<endl;
		assert(equals(D00,D22));
		if(D00.x == n-1){
			arr[I00.x][I00.y] = 0;
			return;
		}
		arr[I00.x][I00.y] = max(arr[I00.x][I00.y], arr[D00.x + 1][D00.y] + score_fold(I00.x, I00.y, D00.y));
		return;
	}
	pt I02;
	setPoint(&I02, I00.x, I22.y);
	pt I20;
	setPoint(&I20, I22.x, I00.y);

	pt I01[2];
	pt Im0[2];
	pt Im1[2];
	pt Im2[2];
	pt I21[2];

	midpoints(I00, I02, I01, SQUARE_HORIZONTAL);
	midpoints(I00, I20, Im0, SQUARE_VERTICAL);
	midpoints(I02, I22, Im2, SQUARE_VERTICAL);
	midpoints(I20, I22, I21, SQUARE_HORIZONTAL);
	midpoints(Im0[0], Im2[0], Im1, SQUARE_MID);

	pt D02;
	setPoint(&D02, D00.x, D22.y);
	pt D01[2];
	pt Dm2[2];
	pt Dm1[2];
	midpoints(D00, D02, D01, SQUARE_HORIZONTAL);
	midpoints(D00, D22, Dm1, SQUARE_MID);
	midpoints(D02, D22, Dm2, SQUARE_VERTICAL);

	fst(Im1[1], I22, Dm1[1], D22);
	fst(I01[1], Im2[0], Dm1[1], D22);
	fst(I00, Im1[0], D00, Dm1[0]);
	fss(I00, Im1[0], D01[1], Dm2[0]);
	fst(Im0[1], I21[0], D00, Dm1[0]);
	fss(Im0[1], I21[0], D01[1], Dm2[0]);
}

void ftt(pt I00, pt I22, pt D00, pt D22){
#if DEBUG
	cout<<"ftt"<<endl;
	cout << I00.x <<" "<< I00.y<<" "<< I22.x <<" "<< I22.y <<" "<< D00.x <<" "<< D00.y <<" "<< D22.x <<" "<<D22.y<<endl;
#endif
	if(equals(I00,I22)){
		//cout<<"Base case"<<endl;
		assert(equals(D00,D22));
		if(D00.x == n-1){
			arr[I00.x][I00.y] = 0;
			return;
		}
		arr[I00.x][I00.y] = max(arr[I00.x][I00.y], arr[D00.x + 1][D00.y] + score_fold(I00.x, I00.y, D00.y));
		return;
	}

	pt I02;
	setPoint(&I02, I00.x, I22.y);
	pt I01[2];
	pt Im2[2];
	pt Im1[2];

	midpoints(I00, I02, I01, SQUARE_HORIZONTAL);
	midpoints(I00, I22, Im1, SQUARE_MID);
	midpoints(I02, I22, Im2, SQUARE_VERTICAL);

	pt D02;
	setPoint(&D02, D00.x, D22.y);
	pt D01[2];
	pt Dm2[2];
	pt Dm1[2];
	midpoints(D00, D02, D01, SQUARE_HORIZONTAL);
	midpoints(D00, D22, Dm1, SQUARE_MID);
	midpoints(D02, D22, Dm2, SQUARE_VERTICAL);

	ftt(Im1[1], I22, Dm1[1], D22);
	fst(I01[1], Im2[0], Dm1[1], D22);
	ftt(I00, Im1[0], D00, Dm1[0]);
	fts(I00, Im1[0], D01[1], Dm2[0]);
}

int score_recursive(int n){
	pt I00; 
	pt I22; 
	pt D00; 
	pt D22; 

	setPoint(&I00, 0, 0);	
	setPoint(&I22, n-1, n-1);	
	setPoint(&I00, 0, 0);	
	setPoint(&I22, n-1, n-1);	

	setPoint(&D00, 0, 0);	
	setPoint(&D22, n-1, n-1);	
	setPoint(&D00, 0, 0);	
	setPoint(&D22, n-1, n-1);	
	
	cout << "Calling "<<n<<endl;
	ftt(I00, I22, D00, D22);
	int val = 0;
	for(int i = 0; i < n; ++i){
		if(val < arr[0][i])
			val = arr[0][i];
	}
	return val;
}

int main(){
	int trails = 100;
	srand(time(NULL));
	while(trails--){
		str = "001000010100001100010001100010";
		str = generate_random_string();
//		str = "00011100100100011110001110001101000101111100011101001000001001000010111110101111000000001111110011011000100110000100010100111011001100100111101110110101010110100110100000010101100000111101101110110010010010110011000111001111111001100110011100011100011000100000111110111011110000100000000101111010010111110110000001011100111100001000110101110010101010001100100010101110001000110010000010010001101010000100110101100001001010010100000010100000010010111100100111101100010000000101101010101101011001001111001010010100";
		cout<<str<<endl;
		n = str.size();
		cout<<n<<endl;
	#if PART_A_TESTS
		for(int i = 0; i < MAX_VAL; ++i)
			for(int j = 0; j < MAX_VAL; ++j)
				dp[i][j] = -1;
		for(int i = 0; i < MAX_VAL; ++i)
			for(int j = 0; j < MAX_VAL; ++j)
				arr[i][j] = -1;
		int v1 = 0;
		int v2 = 0;
		for(int j = 1; j < n -1; ++j){
			v1 = max(score(0,j, n), v1);
		}
		for(int j = 1; j < n -1; ++j){
			v2 = max(score_mem(0,j, n), v2);
		}
		int v4 = score_iterative2(n);
	#endif
		int v3 = score_iterative(n);
		int v5 = score_iterative3(n);
		int v6 = score_iterative3_b(n);
		int v7 = score_iterative3_c(n);
		int v8 = score_iterative3_d(n);
		n = str.size();
		int v9 = score_recursive(n);
	#if PART_A_TESTS
		assert(v1==v2);
		assert(v1==v3);
		assert(v3==v4);
	#endif
		assert(v3==v5);
		assert(v3==v6);
		assert(v3==v7);
		assert(v3==v8);
		cout << v9 << " " << v3 << endl;
		assert(v3==v9);
	}
}
