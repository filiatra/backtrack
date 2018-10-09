
#include <assert.h>

#include <functional>
#include <vector>

#include <iostream>
#include <iterator>

#include "backtrack.h"


enum State
{ME, NH, VT, MA, CT, RI, NY, PA, NJ, DE, MD, DC,
 VA, NC, WV, SC, GA, FL, AL, TN, KY, OH, IN, MI,
 MS, LA, AR, MO, IL, WI, IA, MN, ND, SD, NE, KS,
 OK, TX, NM, CO, WY, MT, ID, UT, AZ, NV, CA, OR, WA};

const int NumberStates = 49;
const int MaxNeighbors = 8;

enum Color {Blue, Yellow, Green, Red};

inline Color& operator++ (Color& c)
{
    c = Color (c + 1);
    return c;
}

inline State& operator++ (State& c)
{
    c = State (c + 1);
    return c;
}

typedef std::vector<Color> Map;
typedef Map::iterator MapIter;


// store neighbor's of each state.
// Neighbor [i][0] == # of neighbors  of state i
// Neighbor [i][j] == jth neighbor of state i
State Neighbor [NumberStates][MaxNeighbors+1];

inline void Connect (State s1, State s2)
{
    int count = ++Neighbor [s1][0];
    Neighbor [s1][count] = s2;

    count = ++Neighbor [s2][0];
    Neighbor [s2][count] = s1;

    assert (Neighbor [s1][0] <= MaxNeighbors);
    assert (Neighbor [s2][0] <= MaxNeighbors);
}


void BuildMap ()
{
    for (int i = 0; i < NumberStates; i++)
        Neighbor [i][0] = State(0);


    Connect (ME,NH);
    Connect (NH,VT); Connect (NH,MA);
    Connect (VT,MA); Connect (VT,NY);
    Connect (MA,NY); Connect (MA,CT); Connect (MA,RI);
    Connect (CT,RI); Connect (CT,NY);
    Connect (NY,NJ); Connect (NY,PA); Connect (NY,OH);
    Connect (PA,NJ); Connect (PA,DE); Connect (PA,MD);
    Connect (PA,WV); Connect (PA,OH);

    // ... omitted to save space -- full source code available
    // on CUJ ftp site (see p. 3 for downloading instructions)

    Connect (UT,NV); Connect (UT,AZ);
    Connect (AZ,NV); Connect (AZ,CA);
    Connect (NV,OR); Connect (NV,CA);
    Connect (CA,OR);
    Connect (OR,WA);
}


struct IsValid :
    public std::binary_function<MapIter, MapIter, bool>
{
    bool operator() (const MapIter& begin, const MapIter& end)
    {
        State LastState = State (end-begin-1);
        Color LastColor = *(end-1);
        // For all edges around LastColor
        for (int i = 1; i <= Neighbor [LastState][0]; i++)
        {
            State NeighborState = Neighbor [LastState][i];
            if (NeighborState < LastState &&
                *(begin+NeighborState) == LastColor)
                return false;
        }
        return true;
    }
};



int main (int argc, char* argv [])
{
    Map tree (NumberStates);
    gsBacktrack<Color, MapIter, IsValid> bt (Blue, Red);
    BuildMap ();

    bool Valid = bt.first(tree.begin(), tree.end());
    // find first 10 valid colorings of the U.S.
    for (int i = 0; i < 10; i++)
    {
        Valid = bt.next(tree.begin(), tree.end());

        std::copy(tree.begin(), tree.end(),
                  std::ostream_iterator<Color>(std::cout, " "));
        std::cout<<std::endl;
    }

    return 0;
}
