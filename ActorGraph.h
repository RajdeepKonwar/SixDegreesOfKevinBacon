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
 * Declaration of class Actorgraph's member functions required to
 * perform BFS and Djikstra's path-finding traversal between actors.
 **/

#ifndef ACTOR_GRAPH_H
#define ACTOR_GRAPH_H

#include <limits>
#include <queue>
#include <vector>
#include <fstream>
#include <sstream>

#include "ActorEdge.h"
#include "ActorNode.hpp"

//! Comparator class to be used by the priority queue in Djikstra's traversal
class ActorComp {
public:
  bool operator () ( Actor * &i_lhs, Actor * &i_rhs ) const {
    return (*i_lhs < *i_rhs);
  }
};

//! Class representing the actual graph and its various functionalities
class ActorGraph {
protected:
  const std::string           m_useWeightedEdges; //! Use weighted edges or not

  std::vector< Actor * >      m_theGraph;         //! The actual graph
  std::vector< std::string >  m_outLines;         //! Lines to be written out to file

  /** Hash-map of movies and the list of actors appearing in them
   *  < movie#@year, vector< actor > >
   */
  std::unordered_map< std::string, std::vector< std::string > >           m_actorsInMovie;
  std::unordered_map< std::string, std::vector< std::string > >::iterator m_aim;

  /** Hash-map of actors and the list of movies done by them
   * < actor, vector< movie#@year > >
   */
  std::unordered_map< std::string, std::vector< std::string > >           m_moviesOfActor;
  std::unordered_map< std::string, std::vector< std::string > >::iterator m_moa;

  //! Hash-map of global actor-index and the actor-name
  std::unordered_map< int, std::string >            m_actorAtIndex;
  std::unordered_map< int, std::string >::iterator  m_aai;

  //! Hash-map of actor-name and global actor-index
  std::unordered_map< std::string, int >            m_indexOfActor;
  std::unordered_map< std::string, int >::iterator  m_ioa;

  //! Various iterators
  std::unordered_map< int, Edge * >::iterator m_eit;
  std::vector< Actor * >::iterator            m_ait;
  std::vector< std::string >::iterator        m_vit, m_sit;

  //! Helper function to deallocate memory from destructor
  void deleteGraph();

  //! Extract year (last 4 digits) from a movie of the form: movie#@year
  int extractYear( const std::string &i_movie );

  //! Find movie on an edge lying between 2 actors
  std::string findMovieEdge(       Actor *i_node1,
                             const int   &i_index2 );

  //! Find neighbors and populate adjacency list for a node (actor)
  void findNeighbors( const std::string                       &i_actor,
                            std::unordered_map< int, Edge * > &io_adjEdges );

  //! Perform BFS traversal (unweighted) between 2 nodes (actors)
  void BFSTraverse( const int &i_from,
                    const int &i_to );

  //! Perform Djikstra's traversal (weighted) between 2 nodes
  void DjikstraTraverse( const int &i_from,
                         const int &i_to );

public:
  //! Constructor (by default, unweighted edges)
  ActorGraph( const std::string &i_outLine,
              const std::string &i_uwe = "u" );

  //! Destructor
  ~ActorGraph();

  //! Creates the graph
  void createGraph( const bool &i_createEdges = true );

  //! Loads the movie cast file
  bool loadFromFile( const char *i_infile );

  //! Loads the file containing pairs of actors
  bool loadTestPairs( const char *i_infile );

  //! Writes out final output lines to file
  bool writeOutLines( const char *i_outfile );
};

#endif // ACTORGRAPH_H