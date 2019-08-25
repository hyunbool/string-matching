#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>
#include <time.h>
using namespace std;

#define NUM_LETTER 256

void naiveMatching(string S, string P) {
	int m = P.length(); // pattern ����
	int n = S.length(); // txt ����
	int i = 0;
	clock_t before;
	double elapsed_time;

	cout << "Naive Matching Algorithm: " << endl;

	before = clock(); // �ð� ���� ����

	// �� ĭ�� ���������鼭 ���Ѵ�
	while(i <= (n - m)) {
		int j;
		// ���� �ε��� i���� pattern�� ���Ͽ� ���� ������ ���� ĭ���� �Ѿ
		for (j = 0; j < m; j++) {
			if (S[i + j] != P[j])
				break;
		}

		// if p[0 ~ m-1] = s[i ~ i+m-1]
		if (j == m)
			cout << "pattern found at index " << i << endl;
		i++;
	}
	elapsed_time = (double)(clock() - before) / CLOCKS_PER_SEC;	// �� ������ ��� �ð��� ������.
	fprintf(stdout, "Naive Matching �ϴ� ���� �ɸ� �ð�(��): %10.2f\n", elapsed_time);
}

// kmp �˰����� ���� pi�迭 �����
int *preprocessing(string pattern) {
	int size = pattern.length(); // pattern ����
	int *pi = new int[size];
	pi[0] = 0;
	int k = 0, j=1; // k: ������ k��° ���ڿ��� ��

	for (int i = 1; i < size; i++){
		while (k > 0 && pattern[k] != pattern[i])
			k = pi[k - 1];
		// ���� ���Ͽ� ������ ���ư� �κ�+1
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
	int i = 0,  j = 0; // i: �ؽ�Ʈ ���ڿ� ������, j: ���� ���ڿ� ������

	cout << "KMP algorithm: " << endl;

	before = clock();
	while (i < S_length) {
		if (p[j] == s[i]) {
			j++; i++;
		}
		else { // j���� match ���� mismatch�� ����
			if (j != 0) // pi �迭�� �����Ͽ� ��� ���� �ٽ� matching�� �� ���� �����´�.
				j = pi[j - 1];
			else
				i++;
		}
		if (j == P_length) {
			cout << "pattern found at index " << i-j << endl;
			j = pi[j - 1]; // ���� ���ڿ� ������ �ʱ�ȭ
		} 
	}
	elapsed_time = (double)(clock() - before) / CLOCKS_PER_SEC;	// �� ������ ��� �ð��� ������.
	fprintf(stdout, "KMP Algorithm ����ϴ� ���� �ɸ� �ð�(��): %10.2f\n", elapsed_time);
}


int *computeJump(string p){
	int *jump = new int[NUM_LETTER]; // jump�迭 �����
	int length, i, j;
	length = p.length();

	// jump �迭 �ʱ�ȭ
	for (i = 0; i < NUM_LETTER; i++)
		jump[i] = length;

	// pattern �ȿ� �ش��ϴ� ���ڿ� ���� ���� ���� ����
	for (j = 0; j < length - 1; j++)
		jump[(unsigned char)p[j]] = length - 1 - j;

	return jump;
}

void BoyerMoore(string s, string p) {
	int *jump = computeJump(p);
	int s_length = s.length(); // n
	int p_length = p.length(); // m
	int count, count_num = 0;
	int *countJump = new int[p_length + 1]; // ��ĭ �����ϴ����� ���� ���� ����
	clock_t before;
	double elapsed_time;

	for (int j = 0; j < p_length; j++)
		countJump[j] = 0;

	int i = 0, j, k;

	cout << "Boyer Moore Algorithm: " << endl;
	before = clock();

	// i: txt���� �� ��ġ
	while (i <= s_length - p_length) {
		j = p_length - 1; k = i + p_length-1;

		// ���ϰ� txt�� ��Ī�ϴ� ���� j--
		while (j >= 0 && p[j] == s[k]) {
			j--; k--;
		} 
		// ���� while������ ���鼭 p�� s�� ��� ��ġ�ϰ� �Ǹ� -1�� ��
		// ���� ��ġ�ϴ� �κ� ����
		if (j < 0)
			cout << "pattern found at index " << i << endl;

		count = (int)jump[(unsigned char)s[i + p_length - 1]];
		countJump[count] += 1;
		count_num++;

		// jump �迭�� ����Ǿ� �ִ� ������ �������� jump �� �� ��ġ�������� �ٽ� �� ����
		i += jump[(unsigned char)s[i + p_length-1]]; // �ε��� update
	}
	elapsed_time = (double)(clock() - before) / CLOCKS_PER_SEC;	// �� ������ ��� �ð��� ������.
	fprintf(stdout, "Boyer Moore Algorithm ����ϴ� ���� �ɸ� �ð�(��): %10.2f\n", elapsed_time);

	cout << "�� jump ���� Ƚ��: " << count_num << endl;
	cout << "ĭ�� jump ���� Ƚ��: " << endl;
	for (int j = 0; j < p_length; j++) {
		if (countJump[j] == 0)
			continue;
		else
			cout << j <<"ĭ jump: " << countJump[j] << "��" << endl;
	}
}
	

int main() {
	string pattern = "Further Reading about Standards";


	cout << "2019-1 �˰����Թ� HW#8" << endl;
	cout << "IT�������� 1612169 ������" << endl << endl;
	ifstream inFile;
	inFile.open("sample.txt"); // "sample.txt"�� open

	stringstream strStream;
	strStream << inFile.rdbuf(); // ������ �о
	string str = strStream.str(); // str�� �־���

	naiveMatching(str, pattern);
	KMP(str, pattern);
	BoyerMoore(str, pattern);

	system("pause");
	return 0;
}
