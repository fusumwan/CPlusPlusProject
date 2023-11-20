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
//g++ -std=c++11 TalentComp.cpp -o main
//./main


/*
A1 and 80%B1  100*o.5 + 80*0.4 =82
A1 and A2 100*0.5+100.*0.25 =75
B1 and A1 100*0.6 +100*0.25 = 85;
B1 and B2 100*0.6 +100*0.4 =100;
B1 and A1 and A2 100*(0.6+0.25+0.15)=100
*/




template <typename T>
string to_string(T value)
{
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}

//It is the right way to split a string into a vector of strings
vector<string> split(string str, string token){
    vector<string>result;
    while(str.size()){
        unsigned int index = str.find(token);
        if(index <str.size() && index>=0){
            result.push_back(str.substr(0,index));
            str = str.substr(index+token.size());
            if(str.size()==0){
                result.push_back(str);
                break;
            }
        }else{
            result.push_back(str);
            break;
        }
    }
    return result;
}

int Fibonacci_R(int n){
	if (n <= 0){return 0;}
	else if (n == 1){return n;}
	return Fibonacci_R(n-1) + Fibonacci_R(n-2);
}

int Fibonacci_I(int n){
	if (n <= 0){return 0;}
	else if (n == 1){return n;}
	int f[n+2];
	int i;
	f[0] = 0;
	f[1] = 1;
	for (i = 2; i <= n; i++){ f[i] = f[i-1] + f[i-2]; }
	return f[n];
}

class HashMap {
    std::unordered_map<const void *, const void *> htmap;
public:
	//int one=1;
	//ht.put("one", &one);
    void put(const void *key, const void *value) {
            htmap[key] = value;
    }
    bool containsKey(const void *key){
    	std::unordered_map<const void *, const void *>::const_iterator got = htmap.find(key);
	  if ( got == htmap.end() )
	    return false;
	  else
	    return true;
    }
    //*(int *)ht.get(1);
    const void *get(const void *key) {
            return htmap[key];
    }
};


static HashMap store;
static int Fibonacci(int n){
	if (n <= 0){return 0;}
	else if(n==1)
		return n;
	else if(store.containsKey(&n)){
		return *(int *)store.get(&n);
	}else{
		int result = Fibonacci(n-1) + Fibonacci(n-2);
		store.put(&n,&result);
		return result;
	}
}

static std::vector<int> denoms={5,10,20,50,100,200};

static int countChange(int amount,int denomPointer){
	if(amount==0){
		return 1;
	}else if((amount<0) || (denomPointer >= (int)denoms.size())){
		return 0;
	}else{
		return countChange(amount-denoms[denomPointer],
			denomPointer)+countChange(amount,++denomPointer);
	}
}
static int countChange(int amount){
	return countChange(amount,0);
}

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

int main(){
	TalentComp T;
	//cout<<T.round2({10,20,30},{30,20,10})<<endl;
	//cout<<T.round2({10,20,30},{10,20,30})<<endl;
	//cout<<T.round2({10,10,10},{1,100,10000})<<endl;
	cout<<T.round2({46,70,39,53,10,47,73,95,95,27},{81,47,64,65,34,55,75,16,80,69})<<endl;
	return 0;
}