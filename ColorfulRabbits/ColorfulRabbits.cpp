#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

//g++ -std=c++11 ColorfulRabbits.hpp -o ColorfulRabbits
//./ColorfulRabbits

/*
 Problem (3) ColorfulRabbits:
 Initial thoughts:
 
 First, we must consider how many rabbits there are in this town.
 Therefore, if a rabbit answers only one color, the total number of rabbits should be one plus the same color (1 + 1 = 2).
 On the other hand, if a rabbit's answer is two rabbits of different colors, it should be a rabbit with two different colors (1 + 2 = 3).
 In the end, 2 + 3 = 5 is just 5 rabbits.
 
 
 Pseudo-Code:
 
 class ColorfulRabbits
 public procedure getMinimum(vector <int> rs) {
 len=(int)rs.size();
 arr=new int[rs.size()];
 Repeat(int i=0;i<rs.size();i++)
 *(arr+i)=rs[i];
 EndRepeat
 arr=insertionSort(arr,len);
 int ans = 0;
 Repeat(int i=0;i<len;)
 int value = arr[i]+1;
 i+=1;
 int c = 1;
 i=getIndex(i,&c);
 int tmp = c / value;
 if (c % value != 0) {
 tmp+=1;
 }
 ans += value*tmp;
 EndRepeat
 return ans;
 endprocedure
 endclass
 
 Reflection:
 When I get the list of arrays, I have to create a function insertSort that works in a way that sorts the number of rabbits in our opponent. Then, if there are two identical answers, we can find rabbits of the same color, then we can add one.
 
 This problem is definitely O(n) because you have to check every answer in the list, so if I can find the O(n) brute force solution, I can iterate through the array of numbers and check if the answer is equal to next. The answer is then set to add 1. Repeat the action. I will find the number of rabbits.
 
 */

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


int main(){
    ColorfulRabbits c;
    cout<<c.getMinimum({ 1, 1, 2, 2 })<<endl;
}
