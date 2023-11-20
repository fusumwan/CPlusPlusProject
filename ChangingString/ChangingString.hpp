#include <iostream>
#include <string>
#include <vector>
#include <stack>




using namespace std;


//g++ -std=c++98 ChangingString.hpp -o ChangingString
//./ChangingString


class ChangingString{
public:
	int* insertionSort(int a[], int n)  {  
		int i, index, j;  
		for (i = 1; i < n; i++) {  
			index = a[i];  
			j = i - 1;
			while (j >= 0 && a[j] > index) {
				a[j + 1] = a[j];  
				j = j - 1;  
			}  
			a[j + 1] = index;  
		}
		return a;
	}
	int distance(string A, string B, int K) {
		int *diffs = new int[A.length()];
		int diffs_len=A.length();
		for (int i = 0; i < diffs_len; i++) {
			diffs[i] = abs(A.at(i) - B.at(i));
		}
		diffs=insertionSort(diffs,diffs_len);
		for (int i = diffs_len - 1; i >= diffs_len - K; i--) {
			diffs[i] = (diffs[i] == 0) ? 1 : 0;
		}
		int sum = 0;
		for (int i=0;i<diffs_len;i++) {
			sum += diffs[i];
		}
		return sum;
	}
};

