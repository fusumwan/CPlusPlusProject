#include <string>
#include <vector>
#include <iostream>

using namespace std;





//g++ -std=c++98 MooingCows.hpp -o MooingCows
//./MooingCows


class MooingCows {
public:
    int compare(vector <string> f,int min_value,int x,int y,int n,int m){
        int sum = 0;
        for(int i=0;i<n;i++){
            for(int j=0;j<m;j++){
                if(f[i][j]=='C' &&
                   !(i==x && y==j))
                {
                    sum += (x-i)*(x-i) + (y-j)*(y-j);
                }
            }
        }
        return min(min_value, sum);
    }
    int dissatisfaction(vector <string> f){
        int n = (int)f.size();
        int m = (int)(f[0].size());
        int min_value = 2147483647; //max value of integer
        for(int x=0;x<n;x++){
            for(int y=0;y<m;y++){
                if(f[x][y] == 'C'){
                    min_value=compare(f,min_value,x,y,n,m);
                }
            }
        }
        return min_value;
    }
};
