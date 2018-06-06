/**
 @author Rajdeep Konwar (A53225609)
 
 @date Dec 09 2017
 
 @section OVERVIEW
 Declaration of struct Edge's member variables required define an edge in graph.
 
 @section ASSIGNMENT NUMBER
 PA4
 */

#ifndef ACTOREDGE_H
#define ACTOREDGE_H

using namespace std;

//! Edge in graph representing a movie between 2 actors
struct Edge {
  int     weight;   //! Edge-weight
  string  movie;    //! Movie-title
};

#endif //! ACTOREDGE_H
