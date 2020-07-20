#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <iostream>
#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iomanip>

using namespace std;

#define MAX_SAMPLE 7000
#define MAX_TRACE 3125
#define MAX_CIPHER 16
#define ANS_TRACE 2663 // 3123(testing)
#define MAX_KEY 256
#define MAX_HEX 16

int cipher[MAX_SAMPLE];
int trace[MAX_SAMPLE];

int sInvBox[256] = { 0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D};
std::map<char, int> mapObject;

void initializeMap() {
	printf("initializeMap! \n");
	mapObject.insert(std::pair<char, int>('0', 0));
	mapObject.insert(std::pair<char, int>('1', 1));
	mapObject.insert(std::pair<char, int>('2', 2));
	mapObject.insert(std::pair<char, int>('3', 3));
	mapObject.insert(std::pair<char, int>('4', 4));
	mapObject.insert(std::pair<char, int>('5', 5));
	mapObject.insert(std::pair<char, int>('6', 6));
	mapObject.insert(std::pair<char, int>('7', 7));
	mapObject.insert(std::pair<char, int>('8', 8));
	mapObject.insert(std::pair<char, int>('9', 9));
	mapObject.insert(std::pair<char, int>('a', 10));
	mapObject.insert(std::pair<char, int>('b', 11));
	mapObject.insert(std::pair<char, int>('c', 12));
	mapObject.insert(std::pair<char, int>('d', 13));
	mapObject.insert(std::pair<char, int>('e', 14));
	mapObject.insert(std::pair<char, int>('f', 15));
}

void readTraceFiles() {
	printf("readTraceFiles! \n");
	FILE *traceText;
	traceText = fopen("traces.txt", "r");
	int temp;
	int j = 0;
	for (int i = 0; i < MAX_SAMPLE * MAX_TRACE; i++)
	{
		fscanf(traceText, "%d", &temp);
		if (i == ((j * MAX_TRACE) + ANS_TRACE))
		{
			trace[j] = temp;
			j++;
		}
	}
	fclose(traceText);
	printf("end readTraceFiles! \n");
}

void readCipherFiles() {
	printf("readCipherFiles! \n");
	FILE *cipherText;
	cipherText = fopen("ciphers.txt", "r");
	int temp;
	int j = 0;
	for (int i = 0; i < MAX_SAMPLE * MAX_CIPHER; i++)
	{
		fscanf(cipherText, "%d", &temp);
		if (i == j * MAX_CIPHER)
		{
			cipher[j] = temp;
			j++;
		}
	}
	fclose(cipherText);
	printf("end readCipherFiles! \n");
}

int getSInvBox(int input) {
	char hexval[5];
	sprintf(hexval, "%0x", input);

	int i = 0;
	for (; i < 5; i++) {
		if (hexval[i] == '\0')
			break;
		// printf("HEX Result[%d] : %c \n", i, hexval[i]);
	}
	int row;
	if (i == 1)
	{
		row = mapObject.find(hexval[0])->second;
	}
	else {
		row = mapObject.find(hexval[1])->second;
		row += MAX_HEX * (mapObject.find(hexval[0])->second);
	}
	//printf("S INV BOX = %d \n", sInvBox[row]);
	return  sInvBox[row];
}

int getHW(int input) {
	int binaryNum[32];

	int i = 0;
	while (input > 0) {
		binaryNum[i] = input % 2;
		input = input / 2;
		i++;
	}
	int count = 0;
	for (int j = i - 1; j >= 0; j--) {
		if (binaryNum[j] == 1) {
			count++;
		}
	}
	return count;
}

double sum(vector<double> a)
{
	double s = 0;
	for (int i = 0; i < a.size(); i++)
	{
		s += a[i];
	}
	return s;
}

double mean(vector<double> a)
{
	return sum(a) / a.size();
}

double sqsum(vector<double> a)
{
	double s = 0;
	for (int i = 0; i < a.size(); i++)
	{
		s += pow(a[i], 2);
	}
	return s;
}

double stdev(vector<double> nums)
{
	double N = nums.size();
	return pow(sqsum(nums) / N - pow(sum(nums) / N, 2), 0.5);
}

vector<double> operator-(vector<double> a, double b)
{
	vector<double> retvect;
	for (int i = 0; i < a.size(); i++)
	{
		retvect.push_back(a[i] - b);
	}
	return retvect;
}

vector<double> operator*(vector<double> a, vector<double> b)
{
	vector<double> retvect;
	for (int i = 0; i < a.size(); i++)
	{
		retvect.push_back(a[i] * b[i]);
	}
	return retvect;
}

double pearsoncoeff(vector<double> X, vector<double> Y)
{
	return sum((X - mean(X))*(Y - mean(Y))) / (X.size()*stdev(X)* stdev(Y));
}



// function that returns correlation coefficient. 
float correlationCoefficient(int X[], int Y[], int n)
{

	int sum_X = 0, sum_Y = 0, sum_XY = 0;
	int squareSum_X = 0, squareSum_Y = 0;

	for (int i = 0; i < n; i++)
	{
		// sum of elements of array X. 
		sum_X = sum_X + X[i];

		// sum of elements of array Y. 
		sum_Y = sum_Y + Y[i];

		// sum of X[i] * Y[i]. 
		sum_XY = sum_XY + X[i] * Y[i];

		// sum of square of array elements. 
		squareSum_X = squareSum_X + X[i] * X[i];
		squareSum_Y = squareSum_Y + Y[i] * Y[i];
	}

	// use formula for calculating correlation coefficient. 
	float corr = (float)(n * sum_XY - sum_X * sum_Y)
		/ sqrt((n * squareSum_X - sum_X * sum_X)
			* (n * squareSum_Y - sum_Y * sum_Y));

	return corr;
}

int main() {
	printf("start! \n");

	initializeMap();
	readCipherFiles();
	readTraceFiles();
	int HD[MAX_KEY][MAX_SAMPLE];
	double corr[MAX_KEY];
	int answer = 0;


	for (int key = 0; key < MAX_KEY; key++)
	{
		for (int m = 0; m < MAX_SAMPLE; m++)
		{
			HD[key][m] = getHW(getSInvBox(cipher[m] ^ key) ^ cipher[m]);		
		}
		
		vector<double> a, b;
		for (int i = 0; i < MAX_SAMPLE; i++)
		{
			a.push_back(HD[key][i]);
			b.push_back(trace[i]);
		}
		corr[key] = pearsoncoeff(a, b);
	}
	
	printf("\n ================================================== \n");
	float min = 2.0f;
	for (int i = 0; i < MAX_KEY; i++) {
		if (min > corr[i]) {
			min = corr[i];
			answer = i;
		}
		printf("Intermediate max = %lf, corr[%d] = %lf \n", min, i, corr[i]);
	}
	printf("\n ================================================== \n");
	printf("KEY VALUE = %d , %lf\n", answer, min);
	return 0;
}


