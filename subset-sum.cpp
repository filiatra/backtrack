
#include <assert.h>

#include <algorithm>
#include <functional>
#include <numeric>
#include <vector>

#include <iostream>
#include <iterator>

#include "backtrack.h"

typedef std::vector<int> Map; // why not bool? ++ op ?

//http://wireless.cs.tku.edu.tw/~kpshih/course/Algorithms/Chapter%205.pdf
struct IsValid
{
    const Map & w;
    int target;
    explicit IsValid(const Map & _w, int _t) : w(_w), target(_t) { }

    template<class iter> bool
    operator() (const iter& begin, const iter& end)
    {
        iter last = end;
        int s = 0;
        for (iter it = begin; it!=last; ++it)
            s += (*it ? w[it-begin] : 0);

        // sum is higher than target
        if (s>target) return false;

        s = std::accumulate(w.begin()+(end-begin),w.end(), s);
        // any further completion cannot achieve target
        if (s<target) return false;

        // at this point s==target
        return true;
    }
};


int main (int argc, char* argv [])
{
    Map w(5);
    w[0] = 2;
    w[1] = 2;
    w[2] = 4;
    w[3] = 6;
    w[4] = 8;
    std::sort(w.begin(), w.end());
    int target = 10; // requested sum

    Map tree(w.size());

    typedef std::vector<int>::iterator iterator;
    gsBacktrack <bool, iterator, IsValid> bt (0, 1, IsValid(w,target));

    bool Valid = bt.first(tree.begin(), tree.end() );
    int i = 0;
    while(Valid)
    {
        ++i;
        std::copy(tree.begin(), tree.end(),
                  std::ostream_iterator<int>(std::cout, " "));
        std::cout<<std::endl;

        Valid = bt.next(tree.begin(), tree.end());
     }

    std::cout<<"Solutions: "<<i<<std::endl;

   return 0;
}
