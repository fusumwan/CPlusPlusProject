#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <utility>
#include <set>
#include <cctype>
#include <queue>
#include <stack>

using namespace std;
//g++ -std=c++98 Arrows.hpp -o Arrows
//./Arrows

class Arrows{
public:
	bool erase(string str,char ch){
		str.erase(remove(str.begin(), str.end(), ch), str.end());
		if(str=="" || str.empty()) return true;
		return false;
	}
	bool same(string str) {
		return erase(str, '-') || erase(str, '=');
	}
	bool arrow(string str) {
		char f = str.at(0);
		char l = str.at(str.length() - 1);
		return (f == '<' && same(str.substr(1)))
				|| (l == '>' && same(str.substr(0,
						str.length() - 1)));
	}
	int longestArrow(string s){
		for (int l = s.length(); l > 0; l--) {
			for (int i = 0; i + l <= s.length(); i++) {
				if (arrow(s.substr(i, l))) {
					return l;
				}
			}
		}
		return -1;
	}
};

