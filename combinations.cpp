
#include <assert.h>

#include <functional>
#include <vector>

#include <iostream>
#include <iterator>

#include "backtrack.h"

struct IsValid
{
    template<class iter> bool
    operator() (const iter& begin, const iter& end)
   {
       const iter last = end-1;
       return begin==last || *last>*(last-1);
   }
};


int main (int argc, char* argv [])
{
    const int N = 7;
    const int K = 4;

    std::vector<int> cc(K);
    typedef std::vector<int>::iterator iterator;

    gsBacktrack <int, iterator, IsValid> bt (0, N-1);

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

    std::cout<<"Combinations: "<<i<<std::endl;

   return 0;
}
