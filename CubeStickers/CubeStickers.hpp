#include <sstream>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <cctype>
#include <cmath>
#include <utility>

using namespace std;

/*
Problem (2) CellRemoval:
According to the question, user will get same colored stickers and all sticker store into a String[]

This problem is definitely at most O(n) because you have to check each element in  6 faces with i-th sticker. 

So I should use a for loop to count each color of sticker in each face.


Pseudo-Code:

class CubeStickers
  public procedure isPossible(vector <string> sticker) {
    // -- main code --
    map<string, int> csp;
    csp=CountSticker(sticker,csp);
    vector<int> clist;
    clist=Apply(clist,csp);
    if(Vaild(clist)<=0){
      return "YES";
    }
    else{
      return "NO";
    }
  endprocedure
endclass


Reflection:

I would like to use brute force solution to find the answer.

Because it is very fast to find out the answer.

So this program I am 3 loops to check if possible.

*/




class CubeStickers {
  public:

  map<string, int> CountSticker(vector <string> sticker, map<string, int> csp){
    int i=0;
    while(i<sticker.size()){
      if(csp.find(sticker[i]) == csp.end()) {
        csp[sticker[i]] = 1;
      }else {
        csp[sticker[i]] += 1;
      }

      i+=1;
    }
    return csp;
  }

  vector<int> Apply(vector<int> cnt,map<string, int> mp){
      map<string, int>::iterator iter = mp.begin();
      while(iter != mp.end()) {
        cnt.push_back(iter->second);
        ++iter;
      }
      return cnt;
  }

  int Vaild(vector<int> clist){
    int faces = 6;
    int j=0;
    while(clist.size()>j){
      if(1 == clist[j]){
        faces -= 1;
      }
      if(2<= clist[j]){
        faces -= 2;
      }
      if(0>=faces){
        break;
      }
      j+=1;
    }
    return faces;
  }

  string isPossible(vector <string> sticker) {
    // -- main code --
    map<string, int> csp;
    csp=CountSticker(sticker,csp);
    vector<int> clist;
    clist=Apply(clist,csp);
    if(Vaild(clist)<=0){
      return "YES";
    }
    else{
      return "NO";
    }
  }


};
