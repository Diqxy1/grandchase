/******************************************************************
/* PrimeSearch.h

This class enables you to visit each and every member of an array
exactly once in an apparently random order.

NOTE: If you want the search to start over at the beginning again -
you must call the Restart()method,OR call GetNext(true).
*******************************************************************/

#pragma once
#include "CRandom.h"

class PrimeSearch
{
    static int prime_array [];

    int skip;
    int currentPosition;
    int maxElements;
    int *currentPrime;
    int searches;

    CRandom r;

public:
    PrimeSearch(int elements);
    int GetNext(bool restart=false);
    bool Done() { return (searches==*currentPrime); }
    void Restart() { currentPosition=0; searches=0; }
};
