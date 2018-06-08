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

#include "ActorGraph.h"

int main( int i_argc, char** i_argv ) {
  //! Start time
  clock_t l_t   = clock();

  //! Check input arguments
  if( i_argc != 5 ) {
    std::cout << "Invalid number of arguments.\n"
              << "Usage: ./pathfinder movie_casts_file u/w test_pairs_file out_file\n";
    return EXIT_FAILURE;
  }

  if( (strcmp( i_argv[2], "u" ) != 0) && (strcmp( i_argv[2], "w" ) != 0) ) {
    std::cout << "Invalid second argument! Should be u/w.\n";
    return EXIT_FAILURE;
  }

 //! ActorGraph object
  ActorGraph l_act( "(actor)--[movie#@year]-->(actor)--...", i_argv[2] );

  //! Load the movie_casts_file
  if( !l_act.loadFromFile( i_argv[1] ) )
    return EXIT_FAILURE;

  //! Create the graph with actors as nodes and movies as edges
  l_act.createGraph();

  //! Load actor pairs from input file and perform path find operations
  if( !l_act.loadTestPairs( i_argv[3] ) )
    return EXIT_FAILURE;

  //! Write final output to file
  if( !l_act.writeOutLines( i_argv[4] ) )
    return EXIT_FAILURE;

  //! Finish time
  l_t = clock() - l_t;
  std::cout << "Time taken: " << (float) l_t / CLOCKS_PER_SEC << "s\n";

  return EXIT_SUCCESS;
}