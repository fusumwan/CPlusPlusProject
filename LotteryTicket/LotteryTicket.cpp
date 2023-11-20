#include <iostream>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <string>
#include <list>
#include <deque>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <utility>
#include <sstream>
#include <cstring>
using namespace std;

//g++ LotteryTicket.cpp -o LotteryTicket
//./LotteryTicket

/*
Problem (4) LotteryTicket:

According to the question. Nick has exactly four banknotes with values b1, b2, b3 and b4. 
Some of the values may be equal.
Hence, we need to find out all probability  of the combination of values b1 to b4.
Therefore, first of all, I have to make a vector to save all the values, 
and then find out the sum of all combination.
So this method can help me to find out all the possible to buy a single lotto ticket without getting any change back.

Pseudo-Code:

class LotteryTicket {
    public procedure  buy(int p, int b1, int b2, int b3, int b4) {
            vector<int> banknotes;
            banknotes.push_back(b1);
            banknotes.push_back(b2);
            banknotes.push_back(b3);
            banknotes.push_back(b4);
            Repeat(int a=0;a<banknotes.size();a++){
                if(banknotes[a]==p) return "POSSIBLE";
                Repeat(int b=0;b<banknotes.size();b++){
                    if(a!=b){
                        if(banknotes[a]+banknotes[b]==p) return "POSSIBLE";
                        Repeat(int c=0;c<banknotes.size();c++){
                            if(a!=b && b!=c){
                                if(banknotes[a]+banknotes[b]+banknotes[c]==p) return "POSSIBLE";
                                Repeat(int d=0;d<banknotes.size();d++){
                                    if(a!=b && b!=c && c!=d){
                                        if(banknotes[a]+banknotes[b]+banknotes[c]+banknotes[d]==p) return "POSSIBLE";
                                    }
                                EndRepeat
                            }
                        EndRepeat
                    }
                EndRepeat
            EndRepeat
            return "IMPOSSIBLE";
    endprocedure
endclass

Reflection:

I found a bit issue of my program, because one of combination is the sum of b1 and b2 twice time so the result is wrong.

In order to fix that issue. I make two new rules (a!=b && a!=c && b!=c) and (a!=b && a!=c && b!=c && a!=d && b!=d && c!=d) which can prevent to sum up duplicated value.

I would like to use brute force to solve the problem. Because this question  is simply find out all possible to buy a single lotto ticket without getting any change back.
Hence, brute force is a fastest way to do this question. Another way maybe hard code all the possible ways but is not good programming style.


*/

class LotteryTicket {
    public:
        string buy(int p, int b1, int b2, int b3, int b4) {
            vector<int> banknotes;
            banknotes.push_back(b1);
            banknotes.push_back(b2);
            banknotes.push_back(b3);
            banknotes.push_back(b4);
            for(int a=0;a<banknotes.size();a++){
                if(banknotes[a]==p){
                    return "POSSIBLE";
                }
                for(int b=0;b<banknotes.size();b++){
                    if(a!=b){
                        if(banknotes[a]+banknotes[b]==p){
                            return "POSSIBLE";
                        }
                        for(int c=0;c<banknotes.size();c++){
                            if(a!=b && a!=c && b!=c){
                                if(banknotes[a]+banknotes[b]+banknotes[c]==p){
                                   return "POSSIBLE";
                                }
                                for(int d=0;d<banknotes.size();d++){
                                    if(a!=b && a!=c && b!=c && a!=d && b!=d && c!=d){
                                        int sum=banknotes[a]+banknotes[b]+banknotes[c]+banknotes[d];
                                        if(sum==p){
                                            return "POSSIBLE";
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            return "IMPOSSIBLE";
        }
};


int main(){
    LotteryTicket L;
    std::cout<<L.buy(20,42,18,35,1)<<std::endl;

}
