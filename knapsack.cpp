
#include <assert.h>

#include <algorithm>
#include <functional>
#include <numeric>
#include <vector>

#include <iostream>
#include <iomanip>
#include <iterator>

#include "backtrack.h"

typedef std::vector<std::pair<double,double> > Items;

struct knapsack_compare
{
    typedef std::pair<double,double> kpair;
    bool operator() (const kpair& a, const kpair& b) const
    {   // note: decreasing order
        return (a.first/a.second) > (b.first/b.second);
    }
};

//http://wireless.cs.tku.edu.tw/~kpshih/course/Algorithms/Chapter%205.pdf
struct IsValid
{
    const Items & w;
    double capacity;
    double optv;
    std::vector<int> opt;
    explicit IsValid(const Items & _w, double _t)
    : w(_w), capacity(_t), optv(0), opt(_w.size()) { }

    template<class iter> bool
    operator() (const iter& begin, const iter& end)
    {
        // cost must be affordable (constraint)
        if( cost(begin,end)>capacity ) return false;

        // Filter using relaxation bound
        if( bound(begin,end)<optv )      // TO REMOVE
        {
            // std::cout<< "Upper bound: "<< v <<" < "<<optv<< " --- ";
            // std::copy(begin, end,
            //           std::ostream_iterator<int>(std::cout, " "));
            // std::cout<<", cost: "<<cost(begin,last)<< "\n";
            return false;
        }

        if( end-begin == w.size() ) // TO REMOVE
        {
            const double v = value(begin,end);
            if (v>optv)
            {
                //std::cout<< "New optimal: "<< v << "\n";
                optv = v;
                opt.assign(begin,end);
            }
        }
        return true;
    }

    template<class iter> bool
    operator() (const iter& begin, const iter& end, const double v)
    {
        return bound(begin,end)>=optv;
    }

    void reset()
    { optv = std::numeric_limits<double>::min(); }

    template<class iter> bool
    trivial(const iter& begin, const iter& end) const
    {
        // all weights > capacity
        // sum of all <= capcity
        return false;
    }

    template<class iter> double
    bound(const iter& begin, const iter& end) const
    {
        // Bound using fractional knapsack (relaxation)
        const iter last = end-1;
        double v = value(begin,last);
        double c =  cost(begin,last);
        for (int i = last-begin; i<w.size(); ++i)
            if (w[i].second+c<capacity)
            {
                v+= w[i].first;
                c+= w[i].second;
            }
            else
            {
                v+= ((capacity-c)/w[i].second)*w[i].first;
                break;
            }
    }

    template<class iter> double
    cost(const iter& begin, const iter& end) const
    {
        //return std::inner_product(begin,end,cc.begin(),0);
        double s = 0;
        for (iter it = begin; it!=end; ++it)
            s += (*it) * w[it-begin].second;
        return s;
    }

    template<class iter> double
    value(const iter& begin, const iter& end)  const
    {
        //return std::inner_product(begin,end,val.begin(),0);
        double s = 0;
        for (iter it = begin; it!=end; ++it)
            s += (*it) * w[it-begin].first;
        return s;
    }

};


//https://www.slideshare.net/mandlapure/01-knapsack-using-backtracking
// NEED BFS instead of DFS(=backtracking)
int main (int argc, char* argv [])
{
    Items choices(5);
    //Value                   Cost
    choices[0].first  = 2;    choices[0].second = 1;
    choices[4].first  = 2;    choices[4].second = 2;
    choices[3].first  = 4;    choices[3].second = 3;
    choices[2].first  = 6;    choices[2].second = 4;
    choices[1].first  = 8;    choices[1].second = 5;
    std::sort(choices.begin(),choices.end(),knapsack_compare());
/*
    for (Items::iterator it = choices.begin(); it!=choices.end(); ++it)
        std::cout<<it->first<<" ";
    std::cout<<std::endl;
    for (Items::iterator it = choices.begin(); it!=choices.end(); ++it)
        std::cout<<it->second<<" ";
    std::cout<<std::endl;
    for (Items::iterator it = choices.begin(); it!=choices.end(); ++it)
        std::cout<<(double)it->first/it->second<<" ";
    std::cout<<"\n------------"<<std::endl;
//*/
    double capacity = 10; // Knapsack size

    std::vector<int> cc(choices.size()); // why not bool?

    typedef std::vector<int>::iterator iterator;
    gsBacktrack <int, iterator, IsValid> bt (0, 1, IsValid(choices,capacity));
    const IsValid & pr = bt.predicate();

    bool Valid = bt.first(cc.begin(), cc.end() );

    int i = 0;
    while(Valid)
    {
        ++i;
        std::copy(cc.begin(), cc.end(),
                  std::ostream_iterator<int>(std::cout, " "));
        std::cout<< "( c="<<std::setw(2)<<pr.cost(cc.begin(), cc.end())
                 <<", v="<<std::setw(2)<<pr.value(cc.begin(), cc.end())<<" )"
                 <<std::endl;

        Valid = bt.next(cc.begin(), cc.end());
     }

    std::cout<<"Solution:\n";
    std::copy(pr.opt.begin(), pr.opt.end(),
              std::ostream_iterator<double>(std::cout, " "));

    std::cout<< "(v="<<pr.optv<<")\n";


    std::cout<<"Optimal solution ("<<bt.optimize(cc.begin(),cc.end())<<"): ";
    std::copy(cc.begin(), cc.end(), std::ostream_iterator<double>(std::cout, " ")); std::cout<< std::endl;

   return 0;
}
