#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;


int* insertionSort(int a[], int n)  {
    int i, index, j;
    for (i = 1; i < n; i++) {
        index = a[i];
        j = i - 1;
        while (j >= 0 && a[j] > index) {
            a[j + 1] = a[j];
            j = j - 1;
        }
        a[j + 1] = index;
    }
    return a;
}

class ColorfulRabbits {
public:
    int len;
    int *arr;
    int getIndex(int i,int *c){
        for (;i < len;) {
            int tmp1=arr[i-1];
            int tmp2=arr[i];
            if(tmp1 == tmp2){
                (*c)+=1;
                i+=1;
            }
            else{
                break;
            }
        }
        return i;
    }
    int getMinimum(vector <int> rs) {
        len=(int)rs.size();
        arr=new int[rs.size()];
        for(int i=0;i<rs.size();i++){
            *(arr+i)=rs[i];
        }
        arr=insertionSort(arr,len);
        int ans = 0;
        for(int i=0;i<len;){
            int value = arr[i]+1;
            i+=1;
            int c = 1;
            i=getIndex(i,&c);
            int tmp = c / value;
            if (c % value != 0) {
                tmp+=1;
            }
            ans += value*tmp;
        }
        return ans;
    }
};



