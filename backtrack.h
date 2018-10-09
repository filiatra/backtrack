
#pragma once

#include<limits>

/*
Backtracking is a general algorithm for finding all (or some)
solutions to some computational problem, that incrementally builds
candidates to the solutions, and abandons each partial candidate c
("backtracks") as soon as it determines that c cannot possibly be
completed to a valid solution.
*/
template <class T, class I, class V>
class gsBacktrack
{
public:

    // precondition: first <= last
    gsBacktrack(const T& first, const T&  last, V pred)
    : size(0), left_child(first), right_child(last), Predicate(pred) { }

    // when the predicate is default-constructed
    gsBacktrack(const T& first, const T&  last)
    : size(0), left_child(first), right_child(last) { }

    // Finds the first solution to the problem.
    bool first(const I& begin, I end)
    {
        size = end - begin;

        end = begin;
        nextLevel(end);
        return nextSolution(begin,end);
    }

    // Finds the next solution to the problem. Repeated calls
    // will find all solutions to a problem if multiple solutions
    // exist.
    // Returns true if a solution was found.
    bool next(const I& begin, I end)
    {
        // assert first was called ?
        size = end - begin;

        // Need to visit new node since vector
        // contains the last solution.
        if ( !nextBranch(begin, end) )
            return false;
        return nextSolution(begin,end);
    }

    //bool good() const { return m_good; }

    const V & predicate() {return Predicate;} //const

    double optimize(const I& begin, I end);

private:

    // Finds the next solution
    // Returns true if a valid solution was found
    bool nextSolution(const I& begin, I & end);

    // Finds the next valid sibling of the leaf (end-1).
    // Returns true if a valid sibling was found.
    //bool nextSibling(const I& begin, const I& end);

    // Backtracks through the decision tree until a new promising
    // branch is found.
    // Returns true if an unvisited  node was found.
    bool nextBranch(const I& begin, I& end);

    // Advances deeper in the decision tree by one level
    void nextLevel(I& end) { *(end++) = left_child; }

private:
    size_t size;
    T left_child;
    T right_child;
    V Predicate;

    //Branch and bound
    T optval;
    std::vector<T> optsol;
};

template <class T, class I, class V> inline bool
gsBacktrack<T,I,V>::nextBranch(const I& begin, I& end)
{
    // ALGORITHM:
    //
    // If the current node is the rightmost child we must
    // backtrack one level because there are no more children at
    // this level. So we back up until we find a non-rightmost
    // child, then generate the child to the right. If we back
    // up to the top without finding an unvisted child, then all
    // nodes have been generated.

    // Back up as long as the node is the rightmost child of
    // its parent.
    while (end-begin > 0 && *(end-1) == right_child)
        --end;

    I back = end-1;
    if (end-begin > 0 && *back != right_child)
    {
        ++(*back);
        return true;
    } else
        return false;
}

/*
template <class T, class I, class V> inline bool
gsBacktrack<T,I,V>::nextSibling(const I& begin, const I& end)
{
    // Note that on entry to this routine the leaf has not yet
    // been used or tested for validity, so the leaf is
    // considered a "sibling" of itself.

    I back = end-1;
    while (true)
    {
        if ( Predicate(begin, end) ) // || Predicate(begin, end, optv)
            return true;
        else if (*back != right_child)
            ++(*back);
        else
            return false;
    }
}
*/

template <class T, class I, class V> inline bool
gsBacktrack<T,I,V>::nextSolution(const I& begin, I & end)
{
    while (true)
    {
//----------- nextSibling
        I back = end-1;
        while (true)
        {
            if ( Predicate(begin, end) ) // || Predicate.bound(begin, end)<optv
            {
                //return true;
                if (end - begin < size)
                    nextLevel(end);
                else
                    return true;
                break;
            }
            else if (*back != right_child)
                ++(*back);
            else
            {
                if ( !nextBranch(begin, end) )
                    return false; // tree exhausted
                break; // back=end-1
            }
        }
//-----------
/*
        if ( nextSibling(begin, end) )
        {
            if (end - begin < size)
                nextLevel(end);
            else
                return true;

        } else if ( !nextBranch(begin, end) )
            return false; // the tree has been exhausted,
        // so no solution exists.
//*/
    }
}

template <class T, class I, class V>
double gsBacktrack<T,I,V>::optimize(const I& begin, I end)
{
    Predicate.reset();
    bool Valid = first(begin, end);
    std::copy(begin, end, std::ostream_iterator<double>(std::cout, " ")); std::cout<< std::endl;
    if (!Valid) return -1;
    std::vector<T> opt(begin, end);
    double v, obj = std::numeric_limits<double>::min();
    while(Valid)
    {
        v = Predicate.value(begin, end);
        std::cout<< "(v="<<v<<")\n";
        if (v>obj)
        {
            std::cout<< "(opt)\n";
            obj = v;
            opt.assign(begin,end);
        }
        Valid = next(begin, end);
    }

    std::copy(opt.begin(), opt.end(), begin);
    return obj;
}

/*
template <class T, class I, class V>
bool gsBacktrack<T,I,V>::operator ()
    (const I& begin, I end, const bool first)
{
    const int size = end - begin;

    // If first time, need to create a root.
    // Otherwise need to visit new node since vector
    // contains the last solution.
    if (first)
    {
        first = false;
        end = begin;
        nextLevel(end);
    } else if (!nextBranch(begin, end))
        return false;

    while (true)
    {
        if (nextSibling(begin, end))
        {
            if (end - begin < size)
                nextLevel(end);
            else
                return true;

        } else if (!nextBranch(begin, end))
            return false; // the tree has been exhausted,
        // so no solution exists.
    }
}
*/
