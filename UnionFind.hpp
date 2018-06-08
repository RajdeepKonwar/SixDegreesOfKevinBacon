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
 * Declaration and implementation of class ActorConnect's member functions
 * required to find connections between a pair of actors.
 **/

#ifndef UNIONFIND_HPP
#define UNIONFIND_HPP

#include <set>

#include "ActorGraph.h"

//! Class for actor connections, inheriting publically from ActorGraph
class ActorConnect : public ActorGraph {
protected:
  std::set< int >            m_movieYears;   //! Set of movie years
  std::set< int >::iterator  m_mit;          //! Movie year iterator
  std::vector< int >         m_dsArr;        //! Disjoint Set array

  //! Extract years from the graph in ascending order
  void extractYearsInAscOrder();

  //! Construct edges in the graph for a given year
  void constructEdges( const int &i_year );

  //! Delete all edges in the graph
  void deleteEdges();

  //! Connect a pair of actors for a given algorithm
  std::string connectPair( const std::string &i_algorithm,
                           const int         &i_id1,
                           const int         &i_id2 );

  //! Perform BFS traversal to find actor connections
  bool pairBFSTraverse( const int &i_from,
                        const int &i_to );

  //! Find an actor in the disjoint set array
  int Find( const int &i_index,
                  int &o_height );

  //! Perform union of disjoint sets
  void Union( const int &i_id1,
              const int &i_id2 );

  //! Constructs the disjoint set array for a given year
  void constructArr( const int &i_year );

  //! Perform union-find traversal to finf actor connections
  bool pairUFindTraverse( const int &i_from,
                          const int &i_to );

public:
  //! Constructor
  ActorConnect( const std::string &i_outLine );

  //! Connect actors from input actor pair file
  bool connectActors( const char        *i_infile,
                      const std::string &i_algo );
};

/** Input params: First line to output in file
 *  Return param: None
 *  Description : Constructor
 */
ActorConnect::ActorConnect( const std::string &i_outLine ) : ActorGraph( i_outLine ) {}

/** Input params: None
 *  Return param: None
 *  Description : Constructor
 *
 *  Populates the set with movie years in an ascending order.
 */
void ActorConnect::extractYearsInAscOrder() {
  for( m_aim = m_actorsInMovie.begin(); m_aim != m_actorsInMovie.end(); ++m_aim )
    m_movieYears.insert( extractYear( m_aim->first ) ); //! Insert into set
}

/** Input params: Year
 *  Return param: None
 *  Description : Construct edges
 *
 *  Construct edges between actor nodes for movies of a particular year only.
 */
void ActorConnect::constructEdges( const int &i_year ) {
  Actor *l_actor;       //! Actor node
  Edge  *l_edge;        //! Edge between actors
  int    l_id1, l_id2;  //! Indices of the actors

  //! *moa is actor
  for( m_moa = m_moviesOfActor.begin(); m_moa != m_moviesOfActor.end(); ++m_moa ) {
    for( m_vit = (m_moa->second).begin(); m_vit != (m_moa->second).end(); ++m_vit ) {
      //! *vit is movie of actor (only consider movies matching year)
      if( extractYear( *m_vit ) != i_year )
        continue;

      m_aim = m_actorsInMovie.find( *m_vit );   //! *aim is movie
      if( m_aim != m_actorsInMovie.end() ) {
        for( m_sit = (m_aim->second).begin(); m_sit != (m_aim->second).end(); ++m_sit ) {
          //! *sit is list of actors in movie

          //! Skip self
          if( (m_moa->first).compare( *m_sit ) == 0 )
            continue;

          //! Get index of actor1
          m_ioa   = m_indexOfActor.find( m_moa->first );
          l_id1   = m_ioa->second;
          l_actor = m_theGraph[l_id1];

          //! Get index of actor2
          m_ioa   = m_indexOfActor.find( *m_sit );
          l_id2   = m_ioa->second;
          m_eit   = l_actor->m_adj.find( l_id2 );

          //! Construct a new edge if one doesn't exist between actor1-actor2
          if( m_eit == l_actor->m_adj.end() ) {
            l_edge                = new Edge;
            l_edge->m_movie       = *m_vit;
            l_edge->m_weight      = 1;
            l_actor->m_adj[l_id2] = l_edge;
          }

          l_actor = m_theGraph[l_id2];
          m_eit   = l_actor->m_adj.find( l_id1 );

          //! Edge needs to be two-way hence construct for actor2-actor1
          if( m_eit == l_actor->m_adj.end() ) {
            l_edge                = new Edge;
            l_edge->m_movie       = *m_vit;
            l_edge->m_weight      = 1;
            l_actor->m_adj[l_id1] = l_edge;
          }
        }
      }
    }
  }
}

/** Input params: None
 *  Return param: None
 *  Description : Delete edges
 *
 *  Clears the adjacency list of each actor in the graph.
 */
void ActorConnect::deleteEdges() {
  for( m_ait = m_theGraph.begin(); m_ait != m_theGraph.end(); ++m_ait ) {
    for( m_eit = (*m_ait)->m_adj.begin(); m_eit != (*m_ait)->m_adj.end(); ++m_eit )
      delete (*m_eit).second;

    (*m_ait)->m_adj.clear();  //! Clear the hash-map of all entries
  }
}

/** Input params: Traverse algorithm, indices of actors
 *  Return param: String of year the actors got connected
 *  Description : Connect pair of actors
 *
 *  Connects a pair of actors depending on algorithm used and returns the year.
 */
std::string ActorConnect::connectPair( const std::string  &i_algorithm,
                                       const int          &i_id1,
                                       const int          &i_id2 ) {
  if( i_algorithm == "bfs" )
    deleteEdges();  //! Clear hash-map of edges before venturing to next pair
  else
    m_dsArr = std::vector< int >( m_theGraph.size(), -1 ); //! Array of indices to -1

  //! *mit is pair of year and bool-flag in set (in ascending order)
  for( m_mit = m_movieYears.begin(); m_mit != m_movieYears.end(); ++m_mit ) {
    if( i_algorithm == "bfs" ) {
      //! Construct edges for a particular year
      constructEdges( *m_mit );

      //! Perform BFS traversal, if unsuccessful, increment year and try again
      if( !pairBFSTraverse( i_id1, i_id2 ) )
        continue;
      else
        return std::to_string( *m_mit );
    } else {
      //! Construct disjoint set array for each year
      constructArr( *m_mit );

      //! Perform union-find traversal till successful
      if( !pairUFindTraverse( i_id1, i_id2 ) )
        continue;
      else
        return std::to_string( *m_mit );
    }
  }

  /** If traversal failed after constructing edges/array for each year
   *  implies there exists no connection between the pair of actors
   */
  return "9999";
}

/** Input params: Indices in graph to traverse from and to
 *  Return param: Boolean
 *  Description : BFS traversal
 *
 *  Performs a BFS traversal between the actors and return success state.
 */
bool ActorConnect::pairBFSTraverse( const int &i_from,
                                    const int &i_to ) {
  std::queue< Actor * >  l_toExplore;    //! Queue to explore
  std::vector< Actor * > l_visited;      //! Visited nodes during traversal

  Actor *l_next, *l_neighbor;         //! Actor nodes
  bool l_found    = false;          //! Flag stating success of traversal

  Actor *l_start  = m_theGraph[i_from]; //! Initial node
  Actor *l_end    = m_theGraph[i_to];   //! End node

  //! Intial node conditioning
  l_start->m_dist   = 0;
  l_toExplore.push( l_start );
  l_visited.push_back( l_start );

  //! Traverse till queue isn't empty
  while( !l_toExplore.empty() ) {
    //! Get front element from queue
    l_next  = l_toExplore.front();
    l_toExplore.pop();

    //! Break if traverse was successful
    if( l_next == l_end ) {
      l_found = true;
      break;
    }

    //! Breadth-first search algorithm
    for( m_eit = l_next->m_adj.begin(); m_eit != l_next->m_adj.end(); ++m_eit ) {
      l_neighbor  = m_theGraph[m_eit->first];

      if( (l_next->m_dist + 1) < l_neighbor->m_dist ) {
        l_neighbor->m_dist  = l_next->m_dist + 1;
        l_neighbor->m_prev  = l_next->m_index;
        l_toExplore.push( l_neighbor );
        l_visited.push_back( l_neighbor );
      }
    }
  }

  //! Reset distances and previous info for all visited nodes during traversal
  for( m_ait = l_visited.begin(); m_ait != l_visited.end(); ++m_ait ) {
    (*m_ait)->m_dist = std::numeric_limits< int >::max();
    (*m_ait)->m_prev = -1;
  }

  //! Return success flag
  return l_found;
}

/** Input params: Actor index and height of disjoint set (passed by reference)
 *  Return param: Sentinel node index
 *  Description : Find actor in the disjoint set array
 *
 *  Finds an actor in the disjoint set array and returns index of sentinel node.
 */
int ActorConnect::Find( const int &i_index,
                              int &o_height ) {
  int l_val = m_dsArr[i_index]; //! Value of disjoint set at index
  int l_ind = i_index;        //! Stores sentinel node info
  std::vector< int > l_indices;  //! Nodes traversed during find (for path compression)

  while( l_val != -1 ) {
    l_indices.push_back( l_ind );
    l_ind = l_val;
    l_val = m_dsArr[l_val];
    o_height++;
  }

  //! Path-compression algorithm (for disjoint sets having height greater than 1)
  if( o_height > 1 )
    for( std::vector< int >::iterator l_it = l_indices.begin(); l_it != l_indices.end(); ++l_it )
      m_dsArr[*l_it] = l_ind;

  //! Return index of sentinel node
  return l_ind;
}

/** Input params: Indices of actor nodes to unite
 *  Return param: None
 *  Description : Perform union of two actors
 *
 *  Performs union operation on the disjoint set array.
 */
void ActorConnect::Union( const int &i_id1,
                          const int &i_id2 ) {
  int l_height1 = 0, l_height2 = 0;
  int l_find1 = Find( i_id1, l_height1 );   //! Sentinel node-index of actor1
  int l_find2 = Find( i_id2, l_height2 );   //! Sentinel node-index of actor2

  //! Perform union operation
  if( l_find1 != l_find2 ) {
    if( l_height1 < l_height2 )
      m_dsArr[l_find1] = l_find2;
    else
      m_dsArr[l_find2] = l_find1;
  }
}

/** Input params: Year
 *  Return param: None
 *  Description : Constructs disjoint set array for a given year
 *
 *  Performs union operation on actors connected by movie of given year.
 */
void ActorConnect::constructArr( const int &i_year ) {
  int l_id1, l_id2;

  for( m_moa = m_moviesOfActor.begin(); m_moa != m_moviesOfActor.end(); ++m_moa ) {
    for( m_vit = (m_moa->second).begin(); m_vit != (m_moa->second).end(); ++m_vit ) {
      //! *vit is movie of actor
      if( extractYear( *m_vit ) != i_year )
        continue;

      m_aim = m_actorsInMovie.find( *m_vit );   //! *aim is movie
      if( m_aim != m_actorsInMovie.end() ) {
        for( m_sit = (m_aim->second).begin(); m_sit != (m_aim->second).end(); ++m_sit ) {
          //! *sit is list of actors in movie

          //! Skip self
          if( (m_moa->first).compare( *m_sit ) == 0 )
            continue;

          //! Get index of actor1
          m_ioa   = m_indexOfActor.find( m_moa->first );
          l_id1   = m_ioa->second;

          //! Get index of actor2
          m_ioa   = m_indexOfActor.find( *m_sit );
          l_id2   = m_ioa->second;

          //! Perform union operation for the actors
          Union( l_id1, l_id2 );
        }
      }
    }
  }
}

/** Input params: Indices to traverse graph from and to
 *  Return param: Boolean state
 *  Description : Traverse graph using union-find
 *
 *  Performs a union-find traversal between the actors and return success state).
 */
bool ActorConnect::pairUFindTraverse( const int &i_from,
                                      const int &i_to ) {
  int l_height1 = 0, l_height2 = 0;
  int l_find1 = Find( i_from, l_height1 );  //! Sentinel index of initial node
  int l_find2 = Find( i_to, l_height2 );    //! Sentinel index of end node

  //! UFind traversal was successfull if both sentinel indices were equal
  return (l_find1 == l_find2);
}

/** Input params: Input filename and traversal algorithm
 *  Return param: Boolean
 *  Description : Connect actors in input actor-pair file
 */
bool ActorConnect::connectActors( const char        *i_infile,
                                  const std::string &i_algo ) {
  bool l_flag = false;
  int l_id1, l_id2;
  std::string l_s;

  if( i_algo == "bfs" )
    std::cout << "Running BFS\n";
  else
    std::cout << "Running UFind\n";

  extractYearsInAscOrder();

  //! Open the test_pairs_file
  std::ifstream l_in( i_infile );

  //! Read and ignore first line (i.e. Actor1	Actor2)
  if( l_in )
    std::getline( l_in, l_s );

  while( l_in ) {
    //! Get the next line
    if( !std::getline( l_in, l_s ) )
      break;

    std::istringstream l_ss( l_s );
    std::vector< std::string > l_pairs;

    while( l_ss ) {
      std::string l_next;

      //! Get the next string before hitting a tab character and put it in next
      if( !std::getline( l_ss, l_next, '\t' ) )
        break;

      l_pairs.push_back( l_next );
    }

    if( l_pairs.size() != 2 ) {
      //! We should have exactly 2 columns
      continue;
    }

    //! Get actor names
    std::string l_actor1( l_pairs[0] );
    std::string l_actor2( l_pairs[1] );

    //! Get index of actor1
    m_ioa = m_indexOfActor.find( l_actor1 );
    if( m_ioa == m_indexOfActor.end() )
      l_flag = true;
    else
      l_id1 = m_ioa->second;

    //! Get index of actor2
    m_ioa = m_indexOfActor.find( l_actor2 );
    if( m_ioa == m_indexOfActor.end() )
      l_flag = true;
    else
      l_id2 = m_ioa->second;

    std::string l_line = l_actor1 + "\t" + l_actor2 + "\t";

    //! Try and find connection between the pair
    if( !l_flag )
      l_line += connectPair( i_algo, l_id1, l_id2 );
    else
      l_line += "9999";

    m_outLines.push_back( l_line );   //! Store line info to be written out later
  }

  return true;
}

#endif //! UNIONFIND_HPP