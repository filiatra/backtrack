
#include <assert.h>

#include <algorithm>
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
        std::cout<< "DFS index: ";
        std::copy(begin, end,
                  std::ostream_iterator<int>(std::cout, " "));
        std::cout<<std::endl;

        const iter last = end-1;
        return last==std::find(begin,last,*last);
    }
};


int main (int argc, char* argv [])
{
    const int N = 4;
    std::vector<int> cc(N);

    typedef std::vector<int>::iterator iterator;
    gsBacktrack <int, iterator, IsValid> bt (0, N-1);

    bool Valid = bt.first(cc.begin(), cc.end() );
    int i = 0;
    while(Valid)
    {
        ++i;
        std::copy(cc.begin(), cc.end(),
                  std::ostream_iterator<int>(std::cout, " "));
        std::cout<<std::endl;

        Valid = bt.next(cc.begin(), cc.end());
     }

    std::cout<<"Permutations: "<<i<<std::endl;

   return 0;
}
