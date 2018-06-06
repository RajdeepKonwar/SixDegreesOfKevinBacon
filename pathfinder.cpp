/**
 @author Rajdeep Konwar (A53225609)
 
 @date Dec 09 2017
 
 @section OVERVIEW
 Implementation of pathfinder program.
 
 @section ASSIGNMENT NUMBER
 PA4
 */

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
