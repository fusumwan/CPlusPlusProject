
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

//g++ -std=c++11 AlternateColors.cpp -o AlternateColors
//./AlternateColors
/*



Problem (1) AlternateColors:

Initial thoughts:

Apparently, there’s also need not complex mathematical concepts. I should probably look for sorting solutions.

Bob is playing his smashing robot. Initially, Bob had r red balls, g green balls and b blue balls. 

The robot will repeat the following three steps until there is no ball. So I have to be very organized in my reasoning.

I need to create a Node to save the detail of color and then I have to sort the vector colors in two ways.

The first way is to sort the vector colors by number of balls of that particular colour. And the second way is to sort by order of destruction.



Pseudo-Code:
class AlternateColors {
	public prodecure getColor(long r,long g,long b,long k) {
        init(r,g,b,k);
        sort(colors.begin(),colors.end(),order_color);
        if(k<=get_step(3,2)) {
            return order_of_destruction(k);
        }
        else{
            return order_of_color(k);
        }
    endprodecure
endclass

Reflection:

I forgot something. When the system is running, 

it will use the previously recorded data for the next run. 

In fact, I should reset the program into initialization.

I would like to choose sorting solution.

Because I want to solves a problem in the most simple, direct or obvious way. 

And there is not distinguished by structure or form in the instructions.

*/

struct node {
    long index;
    string color_name;
    int order;
    node(long i,string name,int o) {
        index=i; color_name=name; order=o; }
};
bool order_color (const node &color1, const node &color2) {
    if(color1.index>color2.index)
    {
        return true;
    }
    return false;
}

bool order_dest (const node &color1, const node &color2) {
    if(color1.index<color2.index)
    {
        return true;
    }
    return false;
}

class AlternateColors {
public:
    vector<node> colors;
    void init(long r,long g,long b,long k){
        colors.clear();
        colors.push_back(node(r,"RED",0));
        colors.push_back(node(g,"GREEN",1));
        colors.push_back(node(b,"BLUE",2));
    }
    long get_step(long s,long i){
        return s*colors[i].index;
    }
    string order_of_destruction(long k){
        sort(colors.begin(),colors.end(),order_dest);
        long tmp=k-1;
        long index=tmp%3;
        return colors[index].color_name;
    }
    string order_of_color(long k){
        k-=colors[2].index*3;
        colors[0].index-=(colors[2].index);
        colors[1].index-=(colors[2].index);
        colors.erase(colors.begin()+2,colors.end());
        if(k<=get_step(2,1)) {
            sort(colors.begin(),colors.end(),order_dest);
            long tmp=k-1;
            long index=tmp%2;
            return colors[index].color_name;
        }
        colors.erase(colors.begin()+1,colors.end());
        return colors[0].color_name;
    }
    string getColor(long r,long g,long b,long k) {
        init(r,g,b,k);
        sort(colors.begin(),colors.end(),order_color);
        if(k<=get_step(3,2)) {
            return order_of_destruction(k);
        }
        else{
            return order_of_color(k);
        }
    }
};

int main() {
    AlternateColors A;
    cout<<A.getColor(1,1,1,3); cin.get();
    cout<<A.getColor(3,4,5,4); cin.get();
    cout<<A.getColor(7,7,1,7); cin.get();
    return 0;
}
