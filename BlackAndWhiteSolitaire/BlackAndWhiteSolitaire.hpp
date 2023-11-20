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


using namespace std;



class BlackAndWhiteSolitaire {
public:
    int minimumTurns(string cardFront) {
        char current = 'B';
        int b_c,w_c = 0;
        int i=0;
        while(i<cardFront.length())
        {
            if (current==cardFront[i]  )
                b_c++;
            else
                w_c++;
            
            if (current == 'B')
                current = 'W';
            else
                current = 'B';
            i++;
        }
        return (b_c>w_c)?w_c:b_c;
    }
};
