#include <iostream>
#include <vector>

using namespace std;
//g++ -std=c++98 Barbecue.hpp -o Barbecue
//./Barbecue

/*
Problem (5) Barbecue:
Initial thoughts:
Due to there are n members in the Smith series, 
represented by an index between 0 and n-1. 
So I just use the for loop and inital two vector lists.
Each of them can vote for any number of people (from 0 to n-1). 
So I have to save the number of against and voted for by using two vectors.
And then compare any number of voting.

Reflection:
I would like to choose brute force. My functions have O(s*(n)) runtime, 
Sometimes denoting the number of Smiths attending the barbecue is an actual number 
for each iteration of outer loop.
I can eliminate and compare voting for any number of people, so I can 
return the index of the person that is going to be excluded,
So it works this way. Let make a example to explain.
*/

class Barbecue{
public:
	vector<int> vlist;
	vector<int> nlist;
	void inital(int size){
		int i=0;
		while( i< size){
			nlist.push_back(0);
			vlist.push_back(0);
			i+=1;
		}
	}
	void voteragainst(vector<int> voter, vector<int> excluded){
		int i=0;
		while( i< voter.size()){
			vlist[voter[i]]+=1;
			nlist[excluded[i]]+=1;
			i+=1;
		}
	}
	int eliminate(int n, vector<int> voter, vector<int> excluded){
		inital(n);
		voteragainst(voter,excluded);
		int result= 0;
		int i=1;
		int nval = nlist[0];
		while( i< n){
			if(nlist[i]>nval){
				nval = nlist[i];
				result = i;
			}
			else if(nlist[i]==nval){
				if(vlist[i]>vlist[result]){
					nval = nlist[i];
					result = i;
				}
			}
			i+=1;
		}
		return result;
	}
};


int main(){
	Barbecue b;
	cout<<b.eliminate(5,{4,3,1,0},{2,2,3,3})<<endl;
	return 0;
}
