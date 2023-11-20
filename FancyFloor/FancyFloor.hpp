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
//g++ -std=c++11 FancyFloor.cpp -o main
//./main


/*
A1 and 80%B1  100*o.5 + 80*0.4 =82
A1 and A2 100*0.5+100.*0.25 =75
B1 and A1 100*0.6 +100*0.25 = 85;
B1 and B2 100*0.6 +100*0.4 =100;
B1 and A1 and A2 100*(0.6+0.25+0.15)=100
*/


class FancyFloor{
public:
	bool findcols(vector<string> place,char pattern,int i){
		bool result=true;
		for(int j=0;j<place.size();j++){
			
			if(place[j][i]!=pattern){
				result=false;
				//printf("colfail: %c    %c\n",place[j][i],pattern);
				return result;
			}else{
				//printf("%c    %c\n", place[j][i],pattern);
			}
		}
		return result;
	}
	bool findrows(vector<string> place,char pattern,int i){
		bool result=true;
		string p=place[i];
		for(int j=0;j<p.length();j++){
			
			if(p[j]!=pattern){
				result=false;

				return result;
			}else{
				//printf("%c    %c\n", p[j],pattern);
			}
		}
		return result;
	}

	int findrows_ex(vector<string> place,char pattern,int r,int c){
		int count=0;
		string p=place[r];
		for(int c_i=c;c_i<p.length();c_i++){
			if(p[c_i]!=pattern){
				return count;
			}else{
				count+=1;
			}
		}
		return count;
	}

	vector<string> findrows_ex_wr(vector<string> place,char pattern,int r,int c){
		string p=place[r];
		for(int c_i=c;c_i<p.length();c_i++){
			if(p[c_i]!=pattern){
				return place;
			}else{
				place[r][c_i]='A';
			}
		}
		return place;
	}

	int findcols_ex(vector<string> place,char pattern,int r,int c){
		int count=0;
		for(int r_i=r;r_i<place.size();r_i++){
			
			if(place[r_i][c]!=pattern){
				//printf("colfail: %c    %c\n",place[j][i],pattern);
				return count;
			}else{
				count+=1;
			}
		}
		return count;
	}

	vector<string> findcols_ex_wr(vector<string> place,char pattern,int r,int c){
		for(int r_i=r;r_i<place.size();r_i++){
			
			if(place[r_i][c]!=pattern){
				//printf("colfail: %c    %c\n",place[j][i],pattern);
				return place;
			}else{
				place[r_i][c]='A';
			}
		}
		return place;
	}


	int howmanyBoards(vector<string> layout){
		if(layout.size()==0) return 0;
		int count=0;
		int collen=(int)layout[0].length();

		



		for(int r=0;r<(int)layout.size();r++){
			for(int c=0;c<layout[r].length();c++){
				char p=layout[r][c];
				if(p!='A' && p!='|'){
					int r_ex=findrows_ex(layout,'-',r,c);
					if(r_ex>0){
						layout=findrows_ex_wr(layout,'-',r,c);
						count+=1;
					}
				}
			}
		}

		for(int c=0;c<collen;c++){
			for(int r=0;r<(int)layout.size();r++){
				char p=layout[r][c];
				if(p!='A' && p!='|'){
					int c_ex=findcols_ex(layout,'|',r,c);
					if(c_ex>0){
						layout=findcols_ex_wr(layout,'|',r,c);
						count+=1;
					}
				}
			}
		}

		//printf("(int)layout.size():%d \n", (int)layout.size());
		for(int r=0;r<(int)layout.size();r++){
			if(findrows(layout,'-',r)){
				count+=1;
				//printf("rowfind%d    count:%d\n",r,count);
			}
		}

		
		//printf("collen:%d\n ", collen);
		for(int c=0;c<collen;c++){
			if(findcols(layout,'|',c)){
				count+=1;
				//printf("colfind%d     count:%d\n",c,count);
				
			}
		}

					

		/*
		for(int r=0;r<(int)layout.size();r++){
			string p=layout[r];
			for(int c=0;c<p.length();c++){
				if(layout[r][c]=='A'){
					count+=1;
				}
			}
		}
		*/
		return count;
	}	
};


/*
int main(){
	FancyFloor F;
	//cout<<F.howmanyBoards({"----","----","----","----"})<<endl;
	//cout<<F.howmanyBoards({"||||","||||","||||","||||"})<<endl;
	cout<<F.howmanyBoards(
		{"-||--",
		 "-||--",
		 "---||",
		 "|||--"})<<endl;
	return 0;
}
*/