#include <iostream>  
#include <algorithm>  
#include <vector> 
#include <climits>
#include <map>
#include <list>

using namespace std;

//g++ -std=c++98 RunLengthEncoding.hpp -o RunLengthEncoding
//./RunLengthEncoding



class Node{
	int _i=0;
	int _min;
	int _max;
	string _factor;
	int _base=0;
	string _data;
	
public:
	Node(){
		_i=0;
		_min=0;
		_max=0;
		_factor;
		_base;
		_data;
	}
	Node(int min,int max,string factor,int base,string data){
		_min=min;
		_max=max;
		_factor=factor;
		_base=base;
		_data=data;
	}
	
	int p_count(){
		int p = 0;
		while (_factor[_i] >= '0' &&
			   _factor[_i] <= '9'){
			int p1=p * 10;
			int p2=(_factor[_i] - '0');
			p = p1 +p2;
			_i++;
		}
		return p;
	}
	bool check(){
		if (_base > 50){
			_data="TOO LONG";
			return false;
		}
		return true;
	}
	
	void setMin(int min){
		_min=min;
	}
	int getMin(){
		return _min;
	}
	int getMax(){
		return _max;
	}
	string getFactor(){
		return _factor;
	}
	void setI(int i){
		_i=i;
	}
	int getI(){
		return _i;
	}
	void BaseUpdate(){
		get();
	}
	int setBase(string base){
		return _base;
	}
	int getBase(){
		return _base;
	}
	string getData(){
		return _data;
	}
	bool end(){
		if(_i>_max)
			return true;
		return false;
	}
	string get(){
		if(_i<_min || _i>_max)
			return _data;
		else if(_i<_factor.size()){
			if (_factor[_i] >= '0' && 
				_factor[_i] <= '9'){
				int p = p_count();
				_base += p;
				if (!check()){ return _data; }
				for (int j=0; j<p; j++)
					_data.push_back(_factor[_i]);
			}
			else
			{
				_data.push_back(_factor[_i]);
				_base++;
			}
			if (!check()){ return _data; }
		}
		return _data;
	}
};

class DP{
public:
	
	std::map<int, Node> store;
	string Solution(int i,Node n){
		string result;
		int len=n.getMax()-i;
		int def_len=1000;
		if(len<def_len){
			result= DecisionTree(i,n).getData();
		}else{
			int t=len/def_len;
			int r=len%def_len;
			int count=0;
			int turn=0;
			for(int x=i;x<n.getMax();x++){
				int base=0;
				string data="";
				Node nr(x,x,n.getFactor(),base,data);
				nr.setI(x);
				nr.BaseUpdate();
				store.clear();
				
				result.insert (result.begin(), DecisionTree(x,nr).getData().data(), DecisionTree(x,nr).getData().data()+3);
				turn+=1;
			}
		}
		return result;
	}
	Node DecisionTree(int i,Node n){
		if (n.end() || n.getData()=="TOO LONG"){
			return n;
		}
		else if (store.find(i)!= store.end()) {
			return (Node) store[i];
		}else{
			Node next(n.getMin(),n.getMax(),n.getFactor(),n.getBase(),n.getData());
			next.setI(n.getI()+1);
			next.BaseUpdate();
			Node result = DecisionTree(n.getI()+1,next);
			store[i]=result;
			return result;
		}
	}
};


class RunLengthEncoding {
public:
	string decode(string text)
	{
		int i=0;
		int length = 0;
		string ret;
		string data="";
		DP dp;
		Node n(0,(int)text.size(),text,0,data);
		n.setI(0);
		n.BaseUpdate();
		ret=dp.Solution(n.getMin(), n);
		if(ret=="" && text!="")
			ret="TOO LONG";
		
		return ret;
	}
};
