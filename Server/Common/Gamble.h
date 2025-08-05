#pragma once

#include <cstdlib> // rand()

#define GAMBLE( value ) ( ( (double)rand()/RAND_MAX ) < value )