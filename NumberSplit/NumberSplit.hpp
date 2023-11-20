#include <iostream>
#include <algorithm>
#include <vector>
#include <climits>
#include <map>
#include <list>
#include <string>
#include <cmath>


using namespace std;

//g++ -std=c++98 NumberSplit.hpp -o NumberSplit
//./NumberSplit


class NumberSplit{
public:
    vector<int> gen;
    vector<int> numlist;
    void initalGen(){
        gen.push_back(1);
        gen.push_back(0);
        gen.push_back(0);
        gen.push_back(1);
    }
    void reset(){
        gen[0]=1;
        gen[1]=0;
        gen[2]=0;
        gen[3]=1;
    }
    void inital(){
        for(int i=0;i<500000;i++){
            numlist.push_back(-1);
        }
    }
    
    int longest(int num,int base1,int base2) {
        if (num < base1) { return 1; }
        if (numlist[num] > base2) { return numlist[num]; }
        int result = 0;
        for (int i = 1; i < toDecimal(num); i++) {
            result = max(result, 1 + longest(generate(num, i),base1,base2));
        }
        numlist[num] = result;
        return numlist[num];
    }
    void splitout(int i){
        if (((i >> gen[2]) & 1) == 1) {
            gen[0] *= gen[1];
            gen[3]  = 1;
            gen[1]  = 0;
        }
    }

    int generate(int n, int split) {
        reset();
        while (n > 0) {
            int d = n % 10;
            gen[1] += d * gen[3];
            gen[3] *= 10;
            splitout(split);
            n /= 10;
            gen[2]++;
        }
        if (gen[1] > 0) {
            gen[0]*=gen[1];
        }
        return gen[0];
    }
    
    int toDecimal(int n){
        int i = 0;
        while (n > 0) {
            n = n / 10;
            i++;
        }
        return 1 << (i-1);
    }
    int longestSequence(int start) {
        inital();
        initalGen();
        return longest(start,10,-1);
    }

};

