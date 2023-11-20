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
#include <string>
#include <vector>


using namespace std;
//g++ -std=c++11 WordGame.cpp -o WordGame
//./WordGame

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

class Example{
public:
	int Fibonacci_C(int i){
		return Fibonacci(i);
	}
	int get(vector<int> inputs){
		return (int)inputs.size();
	}
};

std::string replace_all(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

class Node{
public:
	int count;
	string name;
	Node(){
		count=0;
		name="";
	}
};


class WordGame{
public:
	std::vector<Node> v;
	bool isFindNode(string s){
		for(int i=0;i<(int)v.size();i++){
			if(v[i].name==s){
				return true;
			}
		}
		return false;
	}
	Node FindNode(string s){
		for(int i=0;i<(int)v.size();i++){
			if(v[i].name==s){
				return v[i];
			}
		}
		Node empty;
		return empty;
	}
	int FindNodeIndex(string s){
		for(int i=0;i<(int)v.size();i++){
			if(v[i].name==s){
				return i;
			}
		}

		return -1;
	}
	int Total(){
		int count=0;
		for(int i=0;i<(int)v.size();i++){
			count+=v[i].count * v[i].count;

		}
		return count;
	}


	int process(string s){
		v.clear();
		int len=(int)s.length();
		for(int i=0;i<len;i++){
			char c=s[i];
			string str=string(1,c);
			if(isFindNode(str)){
				int index=FindNodeIndex(str);
				v[index].count+=1;
			}else{
				Node n;
				n.count=1;
				n.name=str;
				v.push_back(n);
			}
		}
		return Total();
	}

	int bestScore(string s){
		int max=-1;
		int len=(int)s.length();
		for(int i=0;i<len;i++){
			char c1=s[i];
			string str1=string(1,c1);
			for(int j=0;j<len;j++){
				char c2=s[j];
				string str2=string(1,c2);
				string tmp=replace_all(s,str1,str2);
				
				int res=process(tmp);
				if(res>max){
					max=res;
				}
			}
		}
		return max;
	}
	void print(){
		for(int i=0;i<(int)v.size();i++){
			printf("%s    %d\n",v[i].name.c_str(),v[i].count);
		}
	}
};
/*
int main(){
	WordGame w;
	cout<<w.bestScore("TOPCODERROCKS")<<endl;
	//w.print();
	cout<<w.bestScore("ABCDEFGHIJKLMNOPQRSTUVWXYZ")<<endl;
	return 0;
}
*/