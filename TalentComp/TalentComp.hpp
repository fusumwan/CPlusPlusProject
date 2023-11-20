#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <tuple>
#include <unordered_map>

using namespace std;
//g++ -std=c++11 TalentComp.hpp -o main
//./main


/*
A1 and 80%B1  100*o.5 + 80*0.4 =82
A1 and A2 100*0.5+100.*0.25 =75
B1 and A1 100*0.6 +100*0.25 = 85;
B1 and B2 100*0.6 +100*0.4 =100;
B1 and A1 and A2 100*(0.6+0.25+0.15)=100
*/


class TalentComp{
public:
	bool cand(int i,vector<int> talent,vector<int> skill){
		int t=talent[i];
		int s=skill[i];
		for(int i=0;i<(int)talent.size();i++){
			if(talent[i]>t && skill[i]>s){
				//printf("i:%d   talent[i]:%d t:%d   skill[i]:%d  t:%d\n",i,talent[i],t,skill[i],s );
				return false;
			}
		}
		return true;
	}
	int round2(vector<int> talent,vector<int> skill){
		if(talent.size()!=skill.size())
			return 0;
		if(talent.size()==0)
			return 0;
		if(skill.size()==0)
			return 0;
		int t0=talent[0];
		bool tsame=true;
		for(int i=0;i<(int)talent.size();i++){
			if(talent[i]!=t0){
				tsame=false;
			}
		}
		int s0=skill[0];
		bool ssame=true;
		for(int i=0;i<(int)skill.size();i++){
			if(skill[i]!=s0){
				ssame=false;
			}
		}
		if(tsame)
			return (int)talent.size();
		if(ssame)
			return (int)skill.size();

		int fail=0;
		for(int i=0;i<(int)talent.size();i++){
			if(!cand(i,talent,skill)){
				//printf("fail:%d\n", i);
				fail+=1;
			}
		}
		//printf("(int)talent.size():%d    fail:%d\n",(int)talent.size(),fail);
		return (int)talent.size()-fail;
		//return (int)place.size();
	}
};
