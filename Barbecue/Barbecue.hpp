#include <iostream>
#include <vector>

using namespace std;

class Barbecue{
public:
    int eliminate(int n, vector<int> voter, vector<int> excluded){
        vector<int> vlist;
        vector<int> nlist;
        int i=0;
        while(i<n){
        	nlist.push_back(0);
            vlist.push_back(0);
            i+=1;
        }
        i=0;
        while(i<voter.size()){
        	nlist[excluded[i]]+=1;
        	vlist[voter[i]]+=1;
        	i+=1;
        }
        int nval = nlist[0];
        int result = 0;
        for(int i=1; i<n; i++){
            if(nval==nlist[i]){
                if(vlist[i]>vlist[result]){
                    nval = nlist[i];
                    result = i;
                }
            }
            else if(nlist[i]>nval){
                nval = nlist[i];
                result = i;
            }
        }
        return result;
    }
};
