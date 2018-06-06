/**
 @author Rajdeep Konwar (A53225609)
 
 @date Dec 09 2017
 
 @section OVERVIEW
 Implementation of actorconnections program.
 
 @section ASSIGNMENT NUMBER
 PA4
 */

#include <chrono>
#include <string.h>
#include "UnionFind.hpp"

using namespace std;

//! Timer class to time operations (in nanoseconds)
class Timer {
private:
  /* start the timer */
  std::chrono::time_point< std::chrono::high_resolution_clock > start;

public:
  /*
   * Function called when starting the timer.
   */
  void begin_timer();

  /*
   * Function called when ending the timer. Returns duration in nanoseconds
   * PRECONDITION: begin_timer() must be called before this function
   */
  long long end_timer();
};

/*
 * Starts the timer. Saves the current time.
 */
void Timer::begin_timer() {
  start = std::chrono::high_resolution_clock::now();
}

/** Ends the timer. Compares end time with the start time and returns number of
 *  nanoseconds
 */
long long Timer::end_timer() {
  std::chrono::time_point< std::chrono::high_resolution_clock > end;
  end = std::chrono::high_resolution_clock::now();

  return ((long long) std::chrono::duration_cast< std::chrono::nanoseconds >
          (end - start).count());
}

//! Main function
int main( int argc, char** argv ) {
  //! Check input arguments
  if( argc < 4 || argc > 5 ) {
    cout << "Invalid number of arguments." << endl
         << "Usage: ./actorconnections movie_casts_file test_pairs_file \
            out_file bfs/ufind" << endl;
    return -1;
  }

  if( argc == 5 ) {
    if( (strcmp( argv[4], "bfs" ) != 0) && (strcmp( argv[4], "ufind" ) != 0) ) {
      cout << "Invalid last argument! Should be bfs/ufind." << endl;
      return -1;
    }
  }

  Timer tmr;        //! Timer object
  long long tt;     //! Time (long long format)
  string algorithm; //! Traversal algorithm

  if( argc == 4 )
    algorithm = "ufind";  //! By-default, ufind algorithm (if last arg missing)
  else
    algorithm = argv[4];

 //! ActorGraph object
  ActorConnect act( "Actor1\tActor2\tYear" );

  //! Load the movie_casts_file
  if( !act.loadFromFile( argv[1] ) )
    return -1;

  //! Create the graph with only actors as nodes (no edges)
  act.createGraph( false );

  //! Note begin time
  tmr.begin_timer();

  //! Try and find connections between actor pairs read from actor-pair file
  if( !act.connectActors( argv[2] , algorithm ) )
    return -1;

  //! Note end time and display difference in seconds
  tt = tmr.end_timer();
  cout << "Run time = " << tt / 1e9 << "sec" << endl;

  //! Write final output to file
  if( !act.writeOutLines( argv[3] ) )
    return -1;

  return 0;
}
