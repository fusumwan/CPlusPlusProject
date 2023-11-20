#include <vector>
#include <list>
#include <map>
#include <set>

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>

using namespace std;

/*

Problem (5) FairWorkload:

Initial thoughts:

On this issue, many workers need to check many filing cabinets. 

The cabinets vary in size and we will tell you how many folders each cabinet contains. 

I have to be  asked to find a most work so that each staff member could access a series of cabinets in turn,

minimizing the maximum number of folders a staff member must browse.


Pseudo-Code:

class FairWorkload {
public procedure getMostWork(vector<int> folders, int workers ) {
       int n =(int)folders.size();
       int max_find = max(folders);
       int accum = sum( folders.begin(), folders.end(), 0 );
       while ( max_find < accum ) {
           int i=0;
           int need = 1;
           int load = 0;
           int half= (accum-max_find)/2;
           int x = max_find + half;
           while(i<n){
               if ( load + folders[i] <= x ) {
                  load += folders[i];
               }
               else {
                  ++need;
                  load = folders[i];
               }
               i+=1;
           }
          if ( need <= workers )
             accum = x;
          else
             max_find = x+1;
       }
       return max_find;
    endprocedure
endclass

Reflection:

Once I were familiar with this problem,  I need a certain amount of creativity. 

Imagine that I have an unlimited number of workers available. 

The key observation is that for a certain number of maximum, 

I can calculate the minimum number of workers required so that each worker can only check up to maximum folders (if possible). 

Let see what I should do. Some workers need to check the first cabinet, so I have assigned any workers.

However, since the cabinets must be distributed in order because the workers can't check the cabinets 1 and 3 without checking 2, 

it is best to assign them to the second cabinet at the same time. 

If this does not exceed our limit, push to the maximum. 

If he is to be allowed to exceed the limit, 

I conclude that his work has been completed and a new worker has been assigned to the second cabinet. 

I operate in a similar manner until all cabinets are assigned, 

then assert that we have used the minimum number of workers possible and introduced artificial restrictions. 
*/





class FairWorkload {
public:
    int max(vector<int> f){
        int ans=-99999999;
        for(int i=0;i<(int)f.size();i++){
            if(f[i]>ans)
                ans=f[i];
        }
        return ans;
    }
    int sum(std::vector<int>::iterator first, std::vector<int>::iterator last, int init){
        for (; first != last; ++first) {
            init = std::move(init) + *first;
        }
        return init;
    }

    int getMostWork(vector<int> folders, int workers ) {
       int n =(int)folders.size();
       int max_find = max(folders);
       int accum = sum( folders.begin(), folders.end(), 0 );
       while ( max_find < accum ) {
           int i=0;
           int need = 1;
           int load = 0;
           int half= (accum-max_find)/2;
           int x = max_find + half;
           while(i<n){
               if ( load + folders[i] <= x ) {
                  load += folders[i];
               }
               else {
                  ++need;
                  load = folders[i];
               }
               i+=1;
           }
          if ( need <= workers )
             accum = x;
          else
             max_find = x+1;
       }
       return max_find;
    }
};

int main(int argc, char *argv[]) {
    FairWorkload f;
    cout<<f.getMostWork({ 10, 20, 30, 40, 50, 60, 70, 80, 90 }, 3)<<endl;
    cout<<f.getMostWork({ 10, 20, 30, 40, 50, 60, 70, 80, 90 }, 5)<<endl;
    return 0;
}
