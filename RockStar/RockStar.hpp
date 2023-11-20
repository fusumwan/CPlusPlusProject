#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

//g++ RockStar.cpp -o RockStar
//./RockStar

class RockStar{ 
        public: 
        int getNumSongs(int ff, int fs, int sf, int ss) { 
           if(fs > sf)	return ff + 2*sf + ss + 1;
           else if(fs == 0) return ff > 0 ? ff : ss + min(sf, 1);
           else return ff + 2*fs + ss;
        } 
       
}; 
