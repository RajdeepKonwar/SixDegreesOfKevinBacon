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
 * Implementation of class Actorgraph's member functions required to
 * perform BFS and Djikstra's path-finding traversal between actors.
 **/

#include <algorithm>

#include "ActorGraph.h"

/** Input params: First line to output in file, use weighted edges or not
 *  Return param: None
 *  Description : Constructor
 *
 *  Populates outLines with first line to output (eg: Actor1	Actor2	Year).
 */
ActorGraph::ActorGraph( const std::string &i_outLine,
                        const std::string &i_uwe ) : m_useWeightedEdges(i_uwe) {
  m_outLines.push_back( i_outLine );
}

/** Input params: None
 *  Return param: None
 *  Description : Destructor
 *
 *  Calls deleteGraph function which deallocates memory
 */
ActorGraph::~ActorGraph() {
  deleteGraph();
}

/** Input params: Boolean stating whether to create edges between actors
 *  Return param: None
 *  Description : Creates the graph
 *
 *  Creates actor nodes, initilizes them and populates the adjacency list
 *  depending on input boolean flag.
 */
void ActorGraph::createGraph( const bool &i_createEdges ) {
  Actor *l_act;

  for( m_moa = m_moviesOfActor.begin(); m_moa != m_moviesOfActor.end(); ++m_moa ) {
    l_act           = new Actor;                            //! New vertex in graph
    m_ioa           = m_indexOfActor.find( m_moa->first );  //! Get global actor index
    l_act->m_index  = m_ioa->second;                        //! Set index
    l_act->m_dist   = std::numeric_limits< int >::max();    //! Set distance to infinity
    l_act->m_prev   = -1;                                   //! Set previous to null
    l_act->m_done   = false;                                //! Set done to false

    if( i_createEdges )
      findNeighbors( m_moa->first, l_act->m_adj );          //! Populate edge-weights

    m_theGraph.push_back( l_act );                          //! Push vertex to graph
  }
}

/** Input params: None
 *  Return param: None
 *  Description : Deletes the graph
 *
 *  Deallocates memory from nodes (Actor) and edges (Edge)
 */
void ActorGraph::deleteGraph() {
  //! Memory deallocation
  for( m_ait = m_theGraph.begin(); m_ait != m_theGraph.end(); ++m_ait ) {
    for( m_eit = (*m_ait)->m_adj.begin(); m_eit != (*m_ait)->m_adj.end(); ++m_eit )
      delete (*m_eit).second;   //! Deallocate Edges

    delete *m_ait;  //! Deallocate Actors
  }
}

/** Input params: Movie
 *  Return param: Year
 *  Description : Extract year (last 4 digits) from movie title
 *
 *  Movie title is of form: Movie#@year (eg: INCEPTION#@2010).
 */
int ActorGraph::extractYear( const std::string &i_movie ) {
  size_t l_pos;       //! String position tracker
  std::string l_year; //! String to hold year extracted from last 4 places of movie

  l_pos   = i_movie.find( "#@" );
  l_pos   += 2;
  l_year  = i_movie.substr( l_pos );

  return std::stoi( l_year );
}

/** Input params: Actor node and neighboring node's index
 *  Return param: Movie on the edge between the two actors
 *  Description : Finds a movie edge
 */
std::string ActorGraph::findMovieEdge(       Actor *i_node1,
                                       const int   &i_index2 ) {
  m_eit = i_node1->m_adj.find( i_index2 );  //! Find neighbor's index in adj list

  if( m_eit != i_node1->m_adj.end() )
    return m_eit->second->m_movie;      //! Return movie stored on edge
  else
    return "";
}

/** Input params: Actor name and adjacency list (passed by reference)
 *  Return param: None
 *  Description : Finds neighbors of an actor
 *
 *  Finds neighbor nodes, populates adjacency list with newly created edges
 *  for movies lying on them with their respective weights.
 */
void ActorGraph::findNeighbors( const std::string &i_actor,
                                      std::unordered_map< int, Edge * > &io_adjEdges ) {
  int l_ind;      //! Index of neighbor
  int l_weight;   //! Edge-weight

  //! Search for actor in respective hash-map
  m_moa = m_moviesOfActor.find( i_actor );
  if( m_moa != m_moviesOfActor.end() ) {
    for( m_vit = (m_moa->second).begin(); m_vit != (m_moa->second).end(); ++m_vit ) {
      m_aim = m_actorsInMovie.find( *m_vit );  //! *vit is movie

      if( m_aim != m_actorsInMovie.end() ) {
        for( m_sit = (m_aim->second).begin(); m_sit != (m_aim->second).end(); ++m_sit ) {
          //! Skip self
          if( i_actor.compare( *m_sit ) == 0 )  //! *sit is actor
            continue;

          //! Unweighted edges
          if( m_useWeightedEdges == "u" ) {
            l_weight = 1;

            //! Get index of neighbor and search in adjacency list
            m_ioa = m_indexOfActor.find( *m_sit );
            l_ind = m_ioa->second;
            m_eit = io_adjEdges.find( l_ind );

            if( m_eit != io_adjEdges.end() )
              continue;
            else {
              Edge *l_ed          = new Edge;
              l_ed->m_movie       = *m_vit;
              l_ed->m_weight      = l_weight;
              io_adjEdges[l_ind]  = l_ed;
            }
          }

          //! Weighted edges
          else {
            l_weight  = 1 + (2015 - extractYear( *m_vit ));   //! Weight-formula

            //! Get index of neighbor and search in adjacency list
            m_ioa = m_indexOfActor.find( *m_sit );
            l_ind = m_ioa->second;
            m_eit = io_adjEdges.find( l_ind );

            if( m_eit != io_adjEdges.end() ) {
              if( l_weight < m_eit->second->m_weight ) {
                //! Update edge with newer movies having lesser weights
                m_eit->second->m_weight = l_weight;
                m_eit->second->m_movie  = *m_vit;
              }
            } else {
              //! Construct new edge with edge info
              Edge *l_ed          = new Edge;
              l_ed->m_movie       = *m_vit;
              l_ed->m_weight      = l_weight;
              io_adjEdges[l_ind]  = l_ed;
            }
          }
        }
      }
    }
  }
}

/** Input params: Index in the graph to traverse from and to
 *  Return param: None
 *  Description : BFS Traversal
 *
 *  Performs BFS traversal between 2 nodes and if successful, stores the path
 *  information (actors & movies) in a particular format to outLines to later
 *  write out to the output file.
 */
void ActorGraph::BFSTraverse( const int &i_from,
                              const int &i_to ) {
  std::queue< Actor * >  l_toExplore;   //! Queue to explore
  std::vector< Actor * > l_visited;     //! Visited nodes during traversal

  Actor *l_next, *l_neighbor;           //! Actor nodes
  bool l_found    = false;              //! Flag stating success of traversal

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

  //! For successful traversals, store path information (movies and actors)
  if( l_found ) {
    int l_prevInd;
    std::vector< std::string > l_output;
    std::string l_actor, l_movie, l_result;

    Actor *l_curr = m_theGraph[l_end->m_prev];

    m_aai     = m_actorAtIndex.find( l_end->m_index );
    l_actor   = "(" + m_aai->second + ")";
    l_prevInd = l_end->m_index;
    l_output.push_back( l_actor );

    while( l_curr != l_start ) {
      l_movie   = "--[" + findMovieEdge( l_curr, l_prevInd ) + "]-->";
      l_output.push_back( l_movie );

      m_aai     = m_actorAtIndex.find( l_curr->m_index );
      l_actor   = "(" + m_aai->second + ")";
      l_output.push_back( l_actor );

      l_prevInd = l_curr->m_index;
      l_curr    = m_theGraph[l_curr->m_prev];
    }

    l_movie = "--[" + findMovieEdge( l_start, l_prevInd ) + "]-->";
    l_output.push_back( l_movie );

    m_aai   = m_actorAtIndex.find( l_start->m_index );
    l_actor = "(" + m_aai->second + ")";
    l_output.push_back( l_actor );

    std::reverse( l_output.begin(), l_output.end() );

    for( std::string l_s : l_output )
      l_result += l_s;

    m_outLines.push_back( l_result ); //! Store output line to be written out later
  }

  //! Reset distances and previous info for all visited nodes during traversal
  for( m_ait = l_visited.begin(); m_ait != l_visited.end(); ++m_ait ) {
    (*m_ait)->m_dist = std::numeric_limits< int >::max();
    (*m_ait)->m_prev = -1;
  }
}

/** Input params: Index in the graph to traverse from and to
 *  Return param: None
 *  Description : Djikstra's Traversal
 *
 *  Performs Djikstra's traversal between 2 nodes and if successful, stores the
 *  path information (actors & movies) in a particular format to outLines to
 *  later write out to the output file.
 */
void ActorGraph::DjikstraTraverse( const int &i_from,
                                   const int &i_to ) {
  //! Priority queue with custom comparator class (ActorComp)
  std::priority_queue< Actor *, std::vector< Actor * >, ActorComp > l_toExplore;
  std::vector< Actor * > l_visited;       //! Visited nodes

  Actor *l_next, *l_neighbor;             //! Actor nodes
  bool l_found    = false;                //! Flag stating success of traversal
  int l_cost      = 0;                    //! Cost of traversal

  Actor *l_start  = m_theGraph[i_from];   //! Initial node
  Actor *l_end    = m_theGraph[i_to];     //! End node

  //! Initial node conditioning
  l_start->m_dist   = 0;
  l_toExplore.push( l_start );
  l_visited.push_back( l_start );

  //! Traverse till priority queue isn't empty
  while( !l_toExplore.empty() ) {
    //! Get top element of priority queue
    l_next = l_toExplore.top();
    l_toExplore.pop();

    //! Break if traverse was successful
    if( l_next == l_end ) {
      l_found = true;
      break;
    }

    //! Djikstra's algorithm
    if( l_next->m_done == false ) {
      l_next->m_done  = true;

      for( m_eit = l_next->m_adj.begin(); m_eit != l_next->m_adj.end(); ++m_eit ) {
        l_neighbor  = m_theGraph[m_eit->first];
        l_cost      = (l_next->m_dist) + (m_eit->second->m_weight);

        if( l_cost < l_neighbor->m_dist ) {
          l_neighbor->m_dist  = l_cost;
          l_neighbor->m_prev  = l_next->m_index;
          l_toExplore.push( l_neighbor );
          l_visited.push_back( l_neighbor );
        }
      }
    }
  }

  //! For successful traversals, store path information (movies and actors)
  if( l_found ) {
    std::vector< std::string > l_output;
    int l_prevInd;
    std::string l_actor, l_movie, l_result;

    Actor *l_curr = m_theGraph[l_end->m_prev];

    m_aai     = m_actorAtIndex.find( l_end->m_index );
    l_actor   = "(" + m_aai->second + ")";
    l_prevInd = l_end->m_index;
    l_output.push_back( l_actor );

    while( l_curr != l_start ) {
      l_movie   = "--[" + findMovieEdge( l_curr, l_prevInd ) + "]-->";
      l_output.push_back( l_movie );

      m_aai     = m_actorAtIndex.find( l_curr->m_index );
      l_actor   = "(" + m_aai->second + ")";
      l_output.push_back( l_actor );

      l_prevInd = l_curr->m_index;
      l_curr    = m_theGraph[l_curr->m_prev];
    }

    l_movie = "--[" + findMovieEdge( l_start, l_prevInd ) + "]-->";
    l_output.push_back( l_movie );

    m_aai   = m_actorAtIndex.find( l_start->m_index );
    l_actor = "(" + m_aai->second + ")";
    l_output.push_back( l_actor );

    std::reverse( l_output.begin(), l_output.end() );

    for( std::string l_s : l_output )
      l_result += l_s;

    m_outLines.push_back( l_result ); //! Store output line to be written out later
  }

  //! Reset distances, done & prev info for all visited nodes during traversal
  for( m_ait = l_visited.begin(); m_ait != l_visited.end(); ++m_ait ) {
    (*m_ait)->m_dist = std::numeric_limits< int >::max();
    (*m_ait)->m_prev = -1;
    (*m_ait)->m_done = false;
  }
}

/** Input params: Input filename
 *  Return param: Boolean
 *  Description : Loads from input movie cast file
 *
 *  Reads the input movie cast file, populates the correct hash-maps to later
 *  construct the graph and use in the graph traversal.
 */
bool ActorGraph::loadFromFile( const char *i_infile ) {
  std::string l_movie, l_s;

  // Initialize the file stream
  std::ifstream l_in( i_infile );

  //! Read and ignore first line (i.e. Actor/Actress	Movie	Year)
  if( l_in )
    std::getline( l_in, l_s );

  std::cout << "Reading " << i_infile << ".. ";

  //! Keep reading lines until the end of file is reached
  while( l_in ) {
    //! Get the next line
    if( !std::getline( l_in, l_s ) )
      break;

    std::istringstream l_ss( l_s );
    std::vector< std::string > l_record;

    while( l_ss ) {
      std::string l_next;

      //! Get the next string before hitting a tab character and put it in next
      if( !std::getline( l_ss, l_next, '\t' ) )
        break;

      l_record.push_back( l_next );
    }

    if( l_record.size() != 3 ) {
      //! We should have exactly 3 columns
      continue;
    }

    std::string l_actorName( l_record[0] );
    l_movie = l_record[1] + "#@" + l_record[2];

    m_aim = m_actorsInMovie.find( l_movie );
    //! If movie doesnt exist as key in map, insert it
    if( m_aim == m_actorsInMovie.end() ) {
      std::vector< std::string > l_actorList{ l_actorName };
      m_actorsInMovie[l_movie] = l_actorList;
    }
    //! Otherwise add actor to list of actors in the movie
    else
      (m_aim->second).push_back( l_actorName );

    m_moa = m_moviesOfActor.find( l_actorName );
    //! If actor doesnt exist as key in map, insert it
    if( m_moa == m_moviesOfActor.end() ) {
      std::vector< std::string > l_movieList{ l_movie };
      m_moviesOfActor[l_actorName] = l_movieList;
    }
    //! Otherwise add movie to list of movies done by the actor
    else
      (m_moa->second).push_back( l_movie );
  }

  if( !l_in.eof() ) {
    std::cerr << "Failed to read " << i_infile << "!\n";
    return false;
  }

  l_in.close();

  //! Assign global index to each actor, create 2 hashmaps (id-name & vice-versa)
  int l_index = 0;
  for( m_moa = m_moviesOfActor.begin(); m_moa != m_moviesOfActor.end(); ++m_moa ) {
    m_actorAtIndex[l_index]       = m_moa->first;
    m_indexOfActor[m_moa->first]  = l_index++;
  }

  std::cout << "done\n";

  return true;
}

/** Input params: Input filename
 *  Return param: Boolean
 *  Description : Loads from input actor pair file
 *
 *  Reads the input actor pair file, and calls the corresponding graph
 *  traversal algorithm for weighted/unweighted graph.
 */
bool ActorGraph::loadTestPairs( const char *i_infile ) {
  bool l_flag = false;
  int l_id1, l_id2;
  std::string l_s;

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

    std::cout << "Computing path for (" << l_actor1 << ") -> (" << l_actor2 << ")\n";

    //! Get index of actor 1
    m_ioa = m_indexOfActor.find( l_actor1 );
    if( m_ioa == m_indexOfActor.end() ) {
      std::cout << "Failure to locate node '" << l_actor1 << "'\n";
      l_flag = true;
    } else
      l_id1 = m_ioa->second;

    //! Get index of actor 2
    m_ioa = m_indexOfActor.find( l_actor2 );
    if( m_ioa == m_indexOfActor.end() ) {
      std::cout << "Failure to locate node '" << l_actor2 << "'\n";
      l_flag = true;
    } else
      l_id2 = m_ioa->second;

    if( l_flag )
      continue;

    //! Perform proper traversal corresponding to weighted/unweighted edges
    if( m_useWeightedEdges == "u" )
      BFSTraverse( l_id1, l_id2 );
    else
      DjikstraTraverse( l_id1, l_id2 );
  }

  if( !l_in.eof() ) {
    std::cerr << "Failed to read " << i_infile << "!\n";
    return false;
  }

  //! Close file
  l_in.close();

  return true;
}

/** Input params: Output filename
 *  Return param: Boolean
 *  Description : Writes to output file
 *
 *  Writes out all the lines stored in outLines, which got populated (hopefully)
 *  during our graph traversal.
 */
bool ActorGraph::writeOutLines( const char *i_outfile ) {
  //! Open file for writing output
  std::ofstream l_out( i_outfile );
  if( !l_out.is_open() ) {
    std::cerr << "Failed to open " << i_outfile << "!\n";
    return false;
  }

  //! Write output
  for( m_vit = m_outLines.begin(); m_vit != m_outLines.end(); ++m_vit )
    l_out << *m_vit << std::endl;

  //! Close file
  l_out.close();

  return true;
}