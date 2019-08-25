#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>
#include <time.h>
using namespace std;

#define NUM_LETTER 256

void naiveMatching(string S, string P) {
	int m = P.length(); // pattern 길이
	int n = S.length(); // txt 길이
	int i = 0;
	clock_t before;
	double elapsed_time;

	cout << "Naive Matching Algorithm: " << endl;

	before = clock(); // 시간 측정 시작

	// 한 칸씩 움직여가면서 비교한다
	while(i <= (n - m)) {
		int j;
		// 현재 인덱스 i에서 pattern을 비교하여 맞지 않으면 다음 칸으로 넘어감
		for (j = 0; j < m; j++) {
			if (S[i + j] != P[j])
				break;
		}

		// if p[0 ~ m-1] = s[i ~ i+m-1]
		if (j == m)
			cout << "pattern found at index " << i << endl;
		i++;
	}
	elapsed_time = (double)(clock() - before) / CLOCKS_PER_SEC;	// 초 단위의 경과 시간을 측정함.
	fprintf(stdout, "Naive Matching 하는 데에 걸린 시간(초): %10.2f\n", elapsed_time);
}

// kmp 알고리즘을 위한 pi배열 만들기
int *preprocessing(string pattern) {
	int size = pattern.length(); // pattern 길이
	int *pi = new int[size];
	pi[0] = 0;
	int k = 0, j=1; // k: 패턴의 k번째 문자와의 비교

	for (int i = 1; i < size; i++){
		while (k > 0 && pattern[k] != pattern[i])
			k = pi[k - 1];
		// 패턴 비교하여 같으면 돌아갈 부분+1
		if (pattern[k] == pattern[i])
			k++;
		pi[i] = k;
	}

	return pi;
}

void KMP(string s, string p) {
	int P_length = p.length();
	int S_length = s.length();
	clock_t before;
	double elapsed_time;

	int* pi = preprocessing(p);
	int i = 0,  j = 0; // i: 텍스트 문자열 포인터, j: 패턴 문자열 포인터

	cout << "KMP algorithm: " << endl;

	before = clock();
	while (i < S_length) {
		if (p[j] == s[i]) {
			j++; i++;
		}
		else { // j번의 match 이후 mismatch가 나면
			if (j != 0) // pi 배열을 참조하여 어디서 부터 다시 matching을 비교 할지 가져온다.
				j = pi[j - 1];
			else
				i++;
		}
		if (j == P_length) {
			cout << "pattern found at index " << i-j << endl;
			j = pi[j - 1]; // 패턴 문자열 포인터 초기화
		} 
	}
	elapsed_time = (double)(clock() - before) / CLOCKS_PER_SEC;	// 초 단위의 경과 시간을 측정함.
	fprintf(stdout, "KMP Algorithm 계산하는 데에 걸린 시간(초): %10.2f\n", elapsed_time);
}


int *computeJump(string p){
	int *jump = new int[NUM_LETTER]; // jump배열 만들기
	int length, i, j;
	length = p.length();

	// jump 배열 초기화
	for (i = 0; i < NUM_LETTER; i++)
		jump[i] = length;

	// pattern 안에 해당하는 문자에 대한 점프 정보 저장
	for (j = 0; j < length - 1; j++)
		jump[(unsigned char)p[j]] = length - 1 - j;

	return jump;
}

void BoyerMoore(string s, string p) {
	int *jump = computeJump(p);
	int s_length = s.length(); // n
	int p_length = p.length(); // m
	int count, count_num = 0;
	int *countJump = new int[p_length + 1]; // 몇칸 점프하는지에 대한 정보 저장
	clock_t before;
	double elapsed_time;

	for (int j = 0; j < p_length; j++)
		countJump[j] = 0;

	int i = 0, j, k;

	cout << "Boyer Moore Algorithm: " << endl;
	before = clock();

	// i: txt에서 현 위치
	while (i <= s_length - p_length) {
		j = p_length - 1; k = i + p_length-1;

		// 패턴과 txt가 매칭하는 동안 j--
		while (j >= 0 && p[j] == s[k]) {
			j--; k--;
		} 
		// 위의 while루프를 돌면서 p와 s가 모두 일치하게 되면 -1이 됨
		// 따라서 일치하는 부분 리턴
		if (j < 0)
			cout << "pattern found at index " << i << endl;

		count = (int)jump[(unsigned char)s[i + p_length - 1]];
		countJump[count] += 1;
		count_num++;

		// jump 배열에 저장되어 있는 정보를 바탕으로 jump 후 그 위치에서부터 다시 비교 시작
		i += jump[(unsigned char)s[i + p_length-1]]; // 인덱스 update
	}
	elapsed_time = (double)(clock() - before) / CLOCKS_PER_SEC;	// 초 단위의 경과 시간을 측정함.
	fprintf(stdout, "Boyer Moore Algorithm 계산하는 데에 걸린 시간(초): %10.2f\n", elapsed_time);

	cout << "총 jump 수행 횟수: " << count_num << endl;
	cout << "칸별 jump 수행 횟수: " << endl;
	for (int j = 0; j < p_length; j++) {
		if (countJump[j] == 0)
			continue;
		else
			cout << j <<"칸 jump: " << countJump[j] << "번" << endl;
	}
}
	

int main() {
	string pattern = "Further Reading about Standards";


	cout << "2019-1 알고리즘입문 HW#8" << endl;
	cout << "IT공학전공 1612169 배현진" << endl << endl;
	ifstream inFile;
	inFile.open("sample.txt"); // "sample.txt"를 open

	stringstream strStream;
	strStream << inFile.rdbuf(); // 파일을 읽어서
	string str = strStream.str(); // str에 넣어줌

	naiveMatching(str, pattern);
	KMP(str, pattern);
	BoyerMoore(str, pattern);

	system("pause");
	return 0;
}
