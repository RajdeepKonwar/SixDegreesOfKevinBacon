/**
 @author Rajdeep Konwar (A53225609)
 
 @date Dec 09 2017
 
 @section OVERVIEW
 Declaration of class Actorgraph's member functions required to
 perform BFS and Djikstra's path-finding traversal between actors.
 
 @section ASSIGNMENT NUMBER
 PA4
 */

#ifndef ACTORGRAPH_H
#define ACTORGRAPH_H

#include <iostream>
#include <algorithm>
#include <fstream>
#include <limits>
#include <string>
#include <sstream>
#include <queue>
#include <set>
#include <unordered_map>

#include "ActorEdge.h"
#include "ActorNode.hpp"

using namespace std;

//! Comparator class to be used by the priority queue in Djikstra's traversal
class ActorComp {
public:
  bool operator () ( Actor * &lhs, Actor * &rhs ) const {
    return (*lhs < *rhs);
  }
};

//! Class representing the actual graph and its various functionalities
class ActorGraph {
protected:
  const string use_weighted_edges;  //! Use weighted edges or not

  vector< Actor * > theGraph;       //! The actual graph
  vector< string >  outLines;       //! Lines to be written out to file

  /** Hash-map of movies and the list of actors appearing in them
   *  < movie#@year, vector< actor > >
   */
  unordered_map< string, vector< string > > actorsInMovie;  //! Hash-map
  unordered_map< string, vector< string > >::iterator aim;  //! Iterator

  /** Hash-map of actors and the list of movies done by them
   * < actor, vector< movie#@year > >
   */
  unordered_map< string, vector< string > > moviesOfActor;  //! Hash-map
  unordered_map< string, vector< string > >::iterator moa;  //! Iterator

  //! Hash-map of global actor-index and the actor-name
  unordered_map< int, string > actorAtIndex;    //! Hash-map
  unordered_map< int, string >::iterator aai;   //!Iterator

  //! Hash-map of actor-name and global actor-index
  unordered_map< string, int > indexOfActor;    //! Hash-map
  unordered_map< string, int >::iterator ioa;   //! Iterator

  //! Various iterators
  unordered_map< int, Edge * >::iterator  eit;
  vector< Actor * >::iterator             ait;
  vector< string >::iterator              vit, sit;

  //! Helper function to deallocate memory from destructor
  void deleteGraph();

  //! Extract year (last 4 digits) from a movie of the form: movie#@year
  int extractYear( const string &movie );

  //! Find movie on an edge lying between 2 actors
  string findMovieEdge( Actor *node1, const int &index2 );

  //! Find neighbors and populate adjacency list for a node (actor)
  void findNeighbors( const string &actor,
                      unordered_map< int, Edge * > &adjEdges );

  //! Perform BFS traversal (unweighted) between 2 nodes (actors)
  void BFSTraverse( const int &from, const int &to );

  //! Perform Djikstra's traversal (weighted) between 2 nodes
  void DjikstraTraverse( const int &from, const int &to );

public:
  //! Constructor (by default, unweighted edges)
  ActorGraph( const string &outLine, const string &uwe = "u" );

  //! Destructor
  ~ActorGraph();

  //! Creates the graph
  void createGraph( const bool &createEdges = true );

  //! Loads the movie cast file
  bool loadFromFile( const char *infile );

  //! Loads the file containing pairs of actors
  bool loadTestPairs( const char *infile );

  //! Writes out final output lines to file
  bool writeOutLines( const char *outfile );
};

#endif // ACTORGRAPH_H
