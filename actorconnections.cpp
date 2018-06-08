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
 * Implementation of actorconnections program.
 **/

#include <chrono>
#include <cstring>
#include "UnionFind.hpp"

//! Timer class to time operations (in nanoseconds)
class Timer {
private:
  /* start the timer */
  std::chrono::time_point< std::chrono::high_resolution_clock > m_start;

public:
  /*
   * Function called when starting the timer.
   */
  void beginTimer();

  /*
   * Function called when ending the timer. Returns duration in nanoseconds
   * PRECONDITION: begin_timer() must be called before this function
   */
  long long endTimer();
};

/*
 * Starts the timer. Saves the current time.
 */
void Timer::beginTimer() {
  m_start = std::chrono::high_resolution_clock::now();
}

/** Ends the timer. Compares end time with the start time and returns number of
 *  nanoseconds
 */
long long Timer::endTimer() {
  std::chrono::time_point< std::chrono::high_resolution_clock > l_end;
  l_end = std::chrono::high_resolution_clock::now();

  return ((long long) std::chrono::duration_cast< std::chrono::nanoseconds >
          (l_end - m_start).count());
}

//! Main function
int main( int i_argc, char** i_argv ) {
  //! Check input arguments
  if( i_argc < 4 || i_argc > 5 ) {
    std::cout << "Invalid number of arguments.\n"
              << "Usage: ./actorconnections movie_casts_file test_pairs_file out_file bfs/ufind\n";
    return EXIT_FAILURE;
  }

  if( i_argc == 5 ) {
    if( (strcmp( i_argv[4], "bfs" ) != 0) && (strcmp( i_argv[4], "ufind" ) != 0) ) {
      std::cout << "Invalid last argument! Should be bfs/ufind.\n";
      return EXIT_FAILURE;
    }
  }

  Timer l_tmr;        //! Timer object
  long long l_tt;     //! Time (long long format)
  string l_algorithm; //! Traversal algorithm

  if( i_argc == 4 )
    l_algorithm = "ufind";  //! By-default, ufind algorithm (if last arg missing)
  else
    l_algorithm = i_argv[4];

 //! ActorGraph object
  ActorConnect l_act( "Actor1\tActor2\tYear" );

  //! Load the movie_casts_file
  if( !l_act.loadFromFile( i_argv[1] ) )
    return EXIT_FAILURE;

  //! Create the graph with only actors as nodes (no edges)
  l_act.createGraph( false );

  //! Note begin time
  l_tmr.beginTimer();

  //! Try and find connections between actor pairs read from actor-pair file
  if( !l_act.connectActors( i_argv[2] , l_algorithm ) )
    return EXIT_FAILURE;

  //! Note end time and display difference in seconds
  l_tt = l_tmr.endTimer();
  std::cout << "Run time = " << l_tt / 1e9 << "sec\n";

  //! Write final output to file
  if( !l_act.writeOutLines( i_argv[3] ) )
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}