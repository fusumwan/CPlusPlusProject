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


