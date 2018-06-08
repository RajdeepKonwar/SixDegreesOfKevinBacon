/**
 * @file This file is part of SixDegreesOfKevinBacon.
 *
 * @section LICENSE
 * MIT License
 *
 * Copyright (c) 2018 Rajdeep Konwar
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * @section DESCRIPTION
 * Implementation of pathfinder program.
 **/

#include <string.h>
#include "ActorGraph.h"

int main( int argc, char** argv ) {
  //! Start time
  clock_t t   = clock();

  //! Check input arguments
  if( argc != 5 ) {
    cout << "Invalid number of arguments." << endl
         << "Usage: ./pathfinder movie_casts_file u/w test_pairs_file out_file"
         << endl;
    return -1;
  }

  if( (strcmp( argv[2], "u" ) != 0) && (strcmp( argv[2], "w" ) != 0) ) {
    cout << "Invalid second argument! Should be u/w." << endl;
    return -1;
  }

 //! ActorGraph object
  ActorGraph act( "(actor)--[movie#@year]-->(actor)--...", argv[2] );

  //! Load the movie_casts_file
  if( !act.loadFromFile( argv[1] ) )
    return -1;

  //! Create the graph with actors as nodes and movies as edges
  act.createGraph();

  //! Load actor pairs from input file and perform path find operations
  if( !act.loadTestPairs( argv[3] ) )
    return -1;

  //! Write final output to file
  if( !act.writeOutLines( argv[4] ) )
    return -1;

  //! Finish time
  t = clock() - t;
  cout << "Time taken: " << (float) t / CLOCKS_PER_SEC << "s" << endl;

  return 0;
}