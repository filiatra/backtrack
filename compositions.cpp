
#include <assert.h>

#include <algorithm>
#include <functional>
#include <numeric>
#include <vector>

#include <iostream>
#include <iterator>

#include "backtrack.h"

typedef std::vector<int> Map;
typedef Map::iterator MapIter;

const int N = 5;
int target = N; //


//http://wireless.cs.tku.edu.tw/~kpshih/course/Algorithms/Chapter%205.pdf
struct IsValid
{
    template<class iter> bool
    operator() (const iter& begin, const iter& end)
    {
        iter last = end;
        int s = 0;
        for (iter it = begin; it!=last; ++it)
            s += *it;

        if (s>N) return false;
        return true;
   }
};


int main (int argc, char* argv [])
{
    // compositions of N into K parts
    int K = 3;
    Map cc(K);// boolean

    gsBacktrack <int, MapIter, IsValid> bt (0, N-1);

    int i = 0;
    bool Valid = bt.first(cc.begin(), cc.end() );
    while(Valid)
    {
        ++i;
        std::copy(cc.begin(), cc.end(),
                  std::ostream_iterator<int>(std::cout, " "));
        std::cout<<std::endl;

        Valid = bt.next(cc.begin(), cc.end());
     }

    std::cout<<"Solutions: "<<i<<std::endl;//for N=5 they are 16

   return 0;
}
