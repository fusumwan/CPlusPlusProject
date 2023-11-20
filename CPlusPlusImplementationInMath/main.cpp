#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>

using namespace std;
//g++ main.cpp -o main
//./main

/*

C++ Implementation of Interger Addition, Multiplication, and Division


cd /Users/timothy/desktop/documents/a1714470/2019/s2/adsa/assignment1


Output From Web Submission Script:
For input  826404321640458300701875635273207656246618554520487562 68048186511 9
Output received  826404321640458300701875635273207656246618633568685173 63218638315643763023765241184825232701704887504740657650871282382 0
Output shouldbe  826404321640458300701875635273207656246618633568685173 63218638315643763023765241184825232701704887504740657650871282382 11745878866572654004686661074668301867321108
Sum and multiplication correct

// command to test my program

./main  
and then input the following number

826404321640458300701875635273207656246618554520487562 68048186511 9

Example

./main
input: 10 9 10
output 19 90 0

x=10
y=9

x+y=19
x*y=90
base=10
*/

// Gobal variables
//default of the total number of digit is 256
const int total_num=256;

template <typename T>
string to_string(T value)
{
	std::ostringstream os ;
	os << value ;
	return os.str() ;
}

//It is the right way to split a string into a vector of strings
vector<string> split(string str, string token){
	vector<string>result;
	while(str.size()){
		unsigned int index = str.find(token);
		if(index <str.size() && index>=0){
			result.push_back(str.substr(0,index));
			str = str.substr(index+token.size());
			if(str.size()==0){
				result.push_back(str);
				break;
			}
		}else{
			result.push_back(str);
			break;
		}
	}
	return result;
}

//This function use SchoolMethod
int* SchoolMethod(int x1[],int y1[],vector<string> list, bool add, int base){
	int *sum1=new int[total_num];
	int carry1[total_num];
	int borrow1[total_num];
	for(int i=0;i<total_num;i++){
		sum1[i]=0;
		carry1[i]=0;
		borrow1[i]=0;
	}
	for(int i=total_num-1;i>=0;i--){
		if(add){
			int s=x1[i]+y1[i]+carry1[i];
			if(s>=base){
				sum1[i]=s-base;
				if(i-1>=0){
					carry1[i-1]=1;
				}
			}else{
				sum1[i]=s;
			}
		}else{
			int s=x1[i]-y1[i]-borrow1[i];
			if(s<0){
				s=base+s;
				borrow1[i-1]=1;
			}
			sum1[i]=s;
		}
	}
	return sum1;
}

//This function use Karatsuba Algorithm
int *Karatsuba(int x1[],int y1[],vector<string> list,int base){
	int *sum1=new int[total_num];
	
	int carry1[total_num];
	
	for(int i=0;i<total_num;i++){
		sum1[i]=0;
		carry1[i]=0;
		
	}
	
	int count=0;
	for(int i=total_num-1;i>=0;i--){
		
		for(int j=total_num-1;j>=0;j--){
			if(j-count>=0){
				int s=x1[j]*y1[i]+carry1[j-count]+sum1[j-count];
				string numstr = to_string(s);
				if(s>=base){
					
					int base_num=0;
					
					while(s-base>=0){
						s=s-base;
						base_num+=1;
					}
					
					sum1[j-count]=s;
					if(j-1-count>=0){
						carry1[j-1-count]=base_num;
					}
					
				}else{
					sum1[j-count]=s;
					
				}
				
			}
		}
		count+=1;
		for(int i=0;i<total_num;i++){
			carry1[i]=0;
		}
	}
	return sum1;
	
}

//Convert array to int 
long arraytoint(int *arr,int len,int base){
	long count=1;
	long sum=0;
	for(int i=len-1;i>=0;i--){
		sum+=arr[i]*count;
		count*=base;
	}
	return sum;
}

//Convert number to base number
int* inttoarray(int num,vector<string> list,int base){
	int *sum1=new int[total_num];
	int *sum2=new int[total_num];
	int *carry=new int[total_num];
	int count=0;
	for(int i=0;i<total_num;i++){
		sum1[i]=0;
		sum2[i]=0;
		carry[i]=0;
	}
	sum2[total_num-1]=1;
	while(count+1<=num){
		count+=1;
		sum1=SchoolMethod(sum1,sum2,list,true,base);
	}
	return sum1;
}

// The main function will perform the required initializations and start the event processing loop.
// The first part of the main function code will initialize all object.
int main(int argc,char **argv)
{
	//code goes here ...
	string input;
	//input the string
	getline(cin,input);
	int l1=0;
	vector<string> list=split(input," ");
	string *arr=new string[list.size()];
	int *x1=new int[total_num];
	int *y1=new int[total_num];
	//initialization of all array
	for(int i=0;i<total_num;i++){
		x1[i]=0;
		y1[i]=0;
	}
	for(int i=0;i<(signed)list.size();i++){
		arr[i]=list[i];
	}
	
	//if the string hasn't contain enough input will return.
	if(list.size()<3){ return 0;}
	
	l1=total_num-arr[0].length();
	for(int i=arr[0].length()-1;i>=0;i--){
		x1[i+l1]=atoi(arr[0].substr(i,1).c_str());
	}

	l1=total_num-arr[1].length();
	for(int i=arr[1].length()-1;i>=0;i--){
		y1[i+l1]=atoi(arr[1].substr(i,1).c_str());
	}
	//Call the School Method
	int *sum_r1=SchoolMethod(x1,y1,list,true,atoi(arr[2].c_str()));
	//Call the Karatsuba Algorithm
	int *sum_r2=Karatsuba(x1,y1,list,atoi(arr[2].c_str()));
	int firstzero=1;
	
	//Printing out the result of operation with school method
	for(int i=0;i<total_num;i++){
		if(firstzero==1 && *(sum_r1+i)==0){
			
		}else{
			printf("%d",*(sum_r1+i));
			firstzero=0;
		}
	}
	//Printing out the result of operation with Karatsuba Algorithm
	printf(" ");
	firstzero=1;
	for(int i=0;i<total_num;i++){
		if(firstzero==1 && *(sum_r2+i)==0){
			
		}else{
			printf("%d",*(sum_r2+i));
			firstzero=0;
		}
	}
	printf(" ");
	//I am an undergraduate student, so simply output 0 as the division result.
	printf("%d",0);
	/*
	int div=0;
	int *div_arr=x1;
	long div_arr_num=arraytoint(div_arr,total_num,atoi(arr[2].c_str()));
	long divider_num=arraytoint(y1,total_num,atoi(arr[2].c_str()));
	while(div_arr_num>=divider_num){
		div_arr=SchoolMethod(div_arr,y1,list,false,atoi(arr[2].c_str()));
		div_arr_num=arraytoint(div_arr,total_num,atoi(arr[2].c_str()));
		div+=1;
	}
	int *divider_arr=inttoarray(div,list,atoi(arr[2].c_str()));
	
	printf(" ");
	firstzero=1;
	for(int i=0;i<total_num;i++){
		if(firstzero==1 && *(divider_arr+i)==0){
			if(i==total_num-1){
				printf("%d",*(divider_arr+i));
			}
		}
		else if(i==total_num-1){
			printf("%d",*(divider_arr+i));
		}
		else{
			printf("%d",*(divider_arr+i));
			firstzero=0;
		}
	}
	
	*/
	
	
	delete[] sum_r1;
	delete[] sum_r2;
	delete[] arr;
	delete[] x1;
	delete[] y1;
	
	//system("pause");
	return 0;
}
