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
//g++ -std=c++11 SortingBoxes.cpp -o SortingBoxes
//./SortingBoxes

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


std::string replace_all(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}



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


class SBox{
public:
	std::vector<int> red;
	std::vector<int> green;
	std::vector<int> blue;
	int move(int b,int m){
		int count=0;
		if(b==0){
			if(m<red.size()){
				for(int i=0;i<(int)red.size();i++){
					if(i!=m){
						count+=red[i];
						red[i]=0;
					}
				}
				red[m]+=count;
			}
		}
		if(b==1){
			if(m<green.size()){
				for(int i=0;i<(int)green.size();i++){
					if(i!=m){
						count+=green[i];
						green[i]=0;
					}
				}
				green[m]+=count;
			}
		}
		if(b==2){
			if(m<blue.size()){
				for(int i=0;i<(int)blue.size();i++){
					if(i!=m){
						count+=blue[i];
						blue[i]=0;
					}
				}
				blue[m]+=count;
			}
		}
		return count;
	}
};

class SortingBoxes{
	public:
		SBox box;
		int minOp(std::vector<int> red,std::vector<int> green,std::vector<int> blue){

			if((int)red.size()==1)
				return -1;
			if((int)green.size()==1)
				return -1;
			if((int)blue.size()==1)
				return -1;

			int size=0;
			size=(int)red.size();
			if(size>(int)green.size())
				size=(int)green.size();
			if(size>(int)blue.size())
				size=(int)blue.size();
			

			box.red=red;
			box.green=green;
			box.blue=blue;

			int max=999999999;
			for(int i=0;i<size;i++){
				
				for(int j=0;j<size;j++){
					int count=0;
					count+=box.move(0,j);
					count+=box.move(1,(j+1)%size);
					count+=box.move(2,(j+2)%size);
					if(count<max)
						max=count;
				}
				
			}

			return max;
		}

};

/*
int main(){
	SortingBoxes box;
	cout<<box.minOp({1,1,1},{1,1,1},{1,1,1})<<endl;

	return 0;
}
*/