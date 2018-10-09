
#pragma once

/*
Backtracking is a general algorithm for finding all (or some)
solutions to some computational problem, that incrementally builds
candidates to the solutions, and abandons each partial candidate c
("backtracks") as soon as it determines that c cannot possibly be
completed to a valid solution.
*/
template <class T, class I, class V>
class gsBranchBound
{
public:

    // precondition: first <= last
    gsBranchBound(const T& first, const T&  last, V pred)
    : size(0), left_child (first), right_child (last), IsValid(pred) { }

    gsBranchBound(const T& first, const T&  last)
    : size(0), left_child (first), right_child (last) { }

    //bool operator() (const I& begin, I end, bool first);

    // Finds the first solution to the problem.
    bool first(const I& begin, I end)
    {
        // assert first was called ?
        size = end - begin;

        end = begin;
        CreateLeftLeaf(end);
        return step(begin,end);
    }

    // Finds the next solution to the problem. Repeated calls
    // will find all solutions to a problem if multiple solutions
    // exist.
    // Returns true if a solution was found.
    bool next(const I& begin, I end)
    {
        size = end - begin;

        // Need to visit new node since vector
        // contains the last solution.
        if ( !VisitNewNode(begin, end) )
            return false;
        return step(begin,end);
    }

    //bool good() const { return m_good; }

private:
    // Finds the next valid sibling of the leaf (end-1).
    // Returns true is if a valid sibling was found.
    bool FindValidSibling (const I& begin, const I& end);

    // Backtracks through the decision tree until it finds a node
    // that hasn't been visited. Returns true if an unvisited
    // node was found.
    bool VisitNewNode (const I& begin, I& end);

    void CreateLeftLeaf (I& end) { *(end++) = left_child; }

    bool step(const I& begin, I & end);

private:
    size_t size;
    T left_child;
    T right_child;
    V IsValid;
};

template <class T, class I, class V> inline bool
gsBranchBound<T,I,V>::VisitNewNode(const I& begin, I& end)
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

template <class T, class I, class V> inline bool
gsBranchBound<T,I,V>::FindValidSibling(const I& begin, const I& end)
{
    // Note that on entry to this routine the leaf has not yet
    // been used or tested for validity, so the leaf is
    // considered a "sibling" of itself.

    I back = end-1;
    while (true)
    {
        if ( IsValid(begin, end) )
            return true;
        else if (*back != right_child)
            ++(*back);
        else
            return false;
    }
}

template <class T, class I, class V> inline bool
gsBranchBound<T,I,V>::step(const I& begin, I & end)
{
    while (true)
    {
        if (FindValidSibling (begin, end))
        {
            if (end - begin < size)
                CreateLeftLeaf (end);
            else
                return true;

        } else if (!VisitNewNode (begin, end))
            return false; // the tree has been exhausted,
        // so no solution exists.
    }
}


template <class T, class I, class V>
bool gsBranchBound<T,I,V>::operator ()
    (const I& begin, I end, const bool first)
{
    const int size = end - begin;

    // If first time, need to create a root.
    // Otherwise need to visit new node since vector
    // contains the last solution.
    if (first)
    {
        //first = false;
        end = begin;
        CreateLeftLeaf (end);
    } else if (!VisitNewNode (begin, end))
        return false;

    while (true)
    {
        if (FindValidSibling (begin, end))
        {
            if (end - begin < size)
                CreateLeftLeaf (end);
            else
                return true;

        } else if (!VisitNewNode (begin, end))
            return false; // the tree has been exhausted,
        // so no solution exists.
    }
}
