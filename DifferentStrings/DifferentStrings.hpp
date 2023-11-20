#include <vector>
#include <list>
#include <map>
#include <set>

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>

using namespace std;


class DifferentStrings {
    public:
    string _A;
    string _B;

    int difference(int i) {
        int count = 0;
        for(int j=0; j<_A.length();j++) {
            if (_A[j] != _B[i + j])
                count ++;
        }
        return count;
    }
    int minimize(string A, string B) {
        this->_A = A;
        this->_B = B;
        int ans = 999999999;
        int len=(int)(_B.length() - _A.length() + 1);
        int i=0;
        while(i<len){
            int count = 0;
            int j=0;
            while(j<_A.length()){
                if (_A[j] != _B[i + j]){
                    count ++;
                }
                j+=1;
            }
            ans = min(ans,count);
            i+=1;
        }
        return ans;
    }
};



