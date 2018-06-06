/**
 @author Rajdeep Konwar (A53225609)
 
 @date Dec 09 2017
 
 @section OVERVIEW
 Declaration of struct Actor's member variables required define a node in graph.
 
 @section ASSIGNMENT NUMBER
 PA4
 */

#ifndef ACTORNODE_H
#define ACTORNODE_H

//! Node in graph representing an actor
struct Actor {
  int                           index;  //! Global actor index
  int                           dist;   //! Distance from source
  int                           prev;   //! Previous actor's index
  bool                          done;   //! Discovery flag (for Djikstra's)
  unordered_map< int, Edge * >  adj;    //! Adjacency list (neighbor-index, edge)

  /** '<' operator overloaded to compare actors based on their distance i.e.
   *  lesser distance has higher priority in the priority queue for Djikstra's
   */
  bool operator < ( const Actor &other ) {
    return (dist >= other.dist);
  }
};

#endif //! ACTORNODE_H
