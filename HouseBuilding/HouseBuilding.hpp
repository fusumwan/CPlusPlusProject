#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <climits>
#include <cmath>
using namespace std;

//g++ -std=c++11 HouseBuilding.hpp -o HouseBuilding
//./HouseBuilding


class Node{
public:
	int i;
	int effort;
	int data;
	Node(){
		i=0;
		effort=0;
		data=0;
	}
	~Node(){
	}
	int ceffort(){
		if (i>= data) { effort += i-data; } else { if(0<data-i-1){ effort += (data-i-1);}}
		return effort;
	}
};

class HouseBuilding {
	public:
	int process(int number,vector< vector<Node> > house_area,int i){
		for (int j = 0; j < house_area.size(); j++) {
			for (int k = 0; k < house_area[0].size(); k++) {
				house_area[j][k].i=i;
				house_area[j][k].effort=number;
				number=house_area[j][k].ceffort();
			}
		}
		return number;
	}
	int getMinimum(vector <string> area) {
		int ans = 99999999;
		int len=area.size();
		int len2=area[0].length();
		vector< vector<Node> > house_area;
		for (int i = 0; i < len; i++) {
			vector<Node> house;
			for (int j = 0; j < len2; j++) {
				Node n;
				n.data=area[i][j]-'0';
				house.push_back(n);
			}
			house_area.push_back(house);
		}
		for (int i = 0; i <= 50; i++) {
			int number = 0;
			number=process(number,house_area,i);
			ans = (ans>number)? number:ans;
		}
		return ans;
	}
  
};