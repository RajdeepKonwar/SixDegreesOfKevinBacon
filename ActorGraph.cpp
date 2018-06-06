/**
 @author Rajdeep Konwar (A53225609)
 
 @date Dec 09 2017
 
 @section OVERVIEW
 Implementation of class Actorgraph's member functions required to
 perform BFS and Djikstra's path-finding traversal between actors.
 
 @section ASSIGNMENT NUMBER
 PA4
 */

#include "ActorGraph.h"

/** Input params: First line to output in file, use weighted edges or not
 *  Return param: None
 *  Description : Constructor
 *
 *  Populates outLines with first line to output (eg: Actor1	Actor2	Year).
 */
ActorGraph::ActorGraph( const string &outLine, const string &uwe ) :
                                                      use_weighted_edges(uwe) {
  outLines.push_back( outLine );
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
void ActorGraph::createGraph( const bool &createEdges ) {
  Actor *act;

  for( moa = moviesOfActor.begin(); moa != moviesOfActor.end(); ++moa ) {
    act         = new Actor;                        //! New vertex in graph
    ioa         = indexOfActor.find( moa->first );  //! Get global actor index
    act->index  = ioa->second;                      //! Set index
    act->dist   = numeric_limits< int >::max();     //! Set distance to infinity
    act->prev   = -1;                               //! Set previous to null
    act->done   = false;                            //! Set done to false
    if( createEdges )
      findNeighbors( moa->first, act->adj );        //! Populate edge-weights

    theGraph.push_back( act );                      //! Push vertex to graph
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
  for( ait = theGraph.begin(); ait != theGraph.end(); ++ait ) {
    for( eit = (*ait)->adj.begin(); eit != (*ait)->adj.end(); ++eit )
      delete (*eit).second;   //! Deallocate Edges

    delete *ait;  //! Deallocate Actors
  }
}

/** Input params: Movie
 *  Return param: Year
 *  Description : Extract year (last 4 digits) from movie title
 *
 *  Movie title is of form: Movie#@year (eg: INCEPTION#@2010).
 */
int ActorGraph::extractYear( const string &movie ) {
  size_t pos;   //! String position tracker
  string year;  //! String to hold year extracted from last 4 places of movie

  pos   = movie.find( "#@" );
  pos   += 2;
  year  = movie.substr( pos );

  return stoi( year );
}

/** Input params: Actor node and neighboring node's index
 *  Return param: Movie on the edge between the two actors
 *  Description : Finds a movie edge
 */
string ActorGraph::findMovieEdge( Actor *node1, const int &index2 ) {
  eit = node1->adj.find( index2 );  //! Find neighbor's index in adj list

  if( eit != node1->adj.end() )
    return eit->second->movie;      //! Return movie stored on edge
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
void ActorGraph::findNeighbors( const string &actor,
                                unordered_map< int, Edge * > &adjEdges ) {
  int ind;      //! Index of neighbor
  int weight;   //! Edge-weight

  //! Search for actor in respective hash-map
  moa = moviesOfActor.find( actor );
  if( moa != moviesOfActor.end() ) {
    for( vit = (moa->second).begin(); vit != (moa->second).end(); ++vit ) {
      aim = actorsInMovie.find( *vit );  //! *vit is movie

      if( aim != actorsInMovie.end() ) {
        for( sit = (aim->second).begin(); sit != (aim->second).end(); ++sit ) {
          //! Skip self
          if( actor.compare( *sit ) == 0 )  //! *sit is actor
            continue;

          //! Unweighted edges
          if( use_weighted_edges == "u" ) {
            weight = 1;

            //! Get index of neighbor and search in adjacency list
            ioa = indexOfActor.find( *sit );
            ind = ioa->second;
            eit = adjEdges.find( ind );

            if( eit != adjEdges.end() ) {
              continue;
            } else {
              Edge *ed      = new Edge;
              ed->movie     = *vit;
              ed->weight    = weight;
              adjEdges[ind] = ed;
            }
          }

          //! Weighted edges
          else {
            weight  = 1 + (2015 - extractYear( *vit ));   //! Weight-formula

            //! Get index of neighbor and search in adjacency list
            ioa = indexOfActor.find( *sit );
            ind = ioa->second;
            eit = adjEdges.find( ind );

            if( eit != adjEdges.end() ) {
              if( weight < eit->second->weight ) {
                //! Update edge with newer movies having lesser weights
                eit->second->weight = weight;
                eit->second->movie  = *vit;
              }
            } else {
              //! Construct new edge with edge info
              Edge *ed      = new Edge;
              ed->movie     = *vit;
              ed->weight    = weight;
              adjEdges[ind] = ed;
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
void ActorGraph::BFSTraverse( const int &from, const int &to ) {
  queue< Actor * >  toExplore;    //! Queue to explore
  vector< Actor * > visited;      //! Visited nodes during traversal

  Actor *next, *neighbor;         //! Actor nodes
  bool found    = false;          //! Flag stating success of traversal

  Actor *start  = theGraph[from]; //! Initial node
  Actor *end    = theGraph[to];   //! End node

  //! Intial node conditioning
  start->dist   = 0;
  toExplore.push( start );
  visited.push_back( start );

  //! Traverse till queue isn't empty
  while( !toExplore.empty() ) {
    //! Get front element from queue
    next  = toExplore.front();
    toExplore.pop();

    //! Break if traverse was successful
    if( next == end ) {
      found = true;
      break;
    }

    //! Breadth-first search algorithm
    for( eit = next->adj.begin(); eit != next->adj.end(); ++eit ) {
      neighbor  = theGraph[eit->first];

      if( (next->dist + 1) < neighbor->dist ) {
        neighbor->dist  = next->dist + 1;
        neighbor->prev  = next->index;
        toExplore.push( neighbor );
        visited.push_back( neighbor );
      }
    }
  }

  //! For successful traversals, store path information (movies and actors)
  if( found ) {
    int prevInd;
    vector< string > output;
    string actor, movie, result;

    Actor *curr = theGraph[end->prev];

    aai     = actorAtIndex.find( end->index );
    actor   = "(" + aai->second + ")";
    prevInd = end->index;
    output.push_back( actor );

    while( curr != start ) {
      movie   = "--[" + findMovieEdge( curr, prevInd ) + "]-->";
      output.push_back( movie );

      aai     = actorAtIndex.find( curr->index );
      actor   = "(" + aai->second + ")";
      output.push_back( actor );

      prevInd = curr->index;
      curr    = theGraph[curr->prev];
    }

    movie = "--[" + findMovieEdge( start, prevInd ) + "]-->";
    output.push_back( movie );

    aai   = actorAtIndex.find( start->index );
    actor = "(" + aai->second + ")";
    output.push_back( actor );

    reverse( output.begin(), output.end() );

    for( string s : output )
      result += s;

    outLines.push_back( result ); //! Store output line to be written out later
  }

  //! Reset distances and previous info for all visited nodes during traversal
  for( ait = visited.begin(); ait != visited.end(); ++ait ) {
    (*ait)->dist = numeric_limits< int >::max();
    (*ait)->prev = -1;
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
void ActorGraph::DjikstraTraverse( const int &from, const int &to ) {
  //! Priority queue with custom comparator class (ActorComp)
  priority_queue< Actor *, vector< Actor * >,
                  ActorComp > toExplore;
  vector< Actor * >           visited;  //! Visited nodes

  Actor *next, *neighbor;               //! Actor nodes
  bool found    = false;                //! Flag stating success of traversal
  int cost      = 0;                    //! Cost of traversal

  Actor *start  = theGraph[from];       //! Initial node
  Actor *end    = theGraph[to];         //! End node

  //! Initial node conditioning
  start->dist   = 0;
  toExplore.push( start );
  visited.push_back( start );

  //! Traverse till priority queue isn't empty
  while( !toExplore.empty() ) {
    //! Get top element of priority queue
    next = toExplore.top();
    toExplore.pop();

    //! Break if traverse was successful
    if( next == end ) {
      found = true;
      break;
    }

    //! Djikstra's algorithm
    if( next->done == false ) {
      next->done  = true;

      for( eit = next->adj.begin(); eit != next->adj.end(); ++eit ) {
        neighbor  = theGraph[eit->first];
        cost      = (next->dist) + (eit->second->weight);

        if( cost < neighbor->dist ) {
          neighbor->dist  = cost;
          neighbor->prev  = next->index;
          toExplore.push( neighbor );
          visited.push_back( neighbor );
        }
      }
    }
  }

  //! For successful traversals, store path information (movies and actors)
  if( found ) {
    vector< string > output;
    int prevInd;
    string actor, movie, result;

    Actor *curr = theGraph[end->prev];

    aai     = actorAtIndex.find( end->index );
    actor   = "(" + aai->second + ")";
    prevInd = end->index;
    output.push_back( actor );

    while( curr != start ) {
      movie   = "--[" + findMovieEdge( curr, prevInd ) + "]-->";
      output.push_back( movie );

      aai     = actorAtIndex.find( curr->index );
      actor   = "(" + aai->second + ")";
      output.push_back( actor );

      prevInd = curr->index;
      curr    = theGraph[curr->prev];
    }

    movie = "--[" + findMovieEdge( start, prevInd ) + "]-->";
    output.push_back( movie );

    aai   = actorAtIndex.find( start->index );
    actor = "(" + aai->second + ")";
    output.push_back( actor );

    reverse( output.begin(), output.end() );

    for( string s : output )
      result += s;

    outLines.push_back( result ); //! Store output line to be written out later
  }

  //! Reset distances, done & prev info for all visited nodes during traversal
  for( ait = visited.begin(); ait != visited.end(); ++ait ) {
    (*ait)->dist = numeric_limits< int >::max();
    (*ait)->prev = -1;
    (*ait)->done = false;
  }
}

/** Input params: Input filename
 *  Return param: Boolean
 *  Description : Loads from input movie cast file
 *
 *  Reads the input movie cast file, populates the correct hash-maps to later
 *  construct the graph and use in the graph traversal.
 */
bool ActorGraph::loadFromFile( const char *infile ) {
  string movie, s;

  // Initialize the file stream
  ifstream in( infile );

  //! Read and ignore first line (i.e. Actor/Actress	Movie	Year)
  if( in )
    getline( in, s );

  cout << "Reading " << infile << ".. ";

  //! Keep reading lines until the end of file is reached
  while( in ) {
    //! Get the next line
    if( !getline( in, s ) )
      break;

    istringstream ss( s );
    vector< string > record;

    while( ss ) {
      string next;

      //! Get the next string before hitting a tab character and put it in next
      if( !getline( ss, next, '\t' ) )
        break;

      record.push_back( next );
    }

    if( record.size() != 3 ) {
      //! We should have exactly 3 columns
      continue;
    }

    string actor_name( record[0] );
    movie = record[1] + "#@" + record[2];

    aim = actorsInMovie.find( movie );
    //! If movie doesnt exist as key in map, insert it
    if( aim == actorsInMovie.end() ) {
      vector< string > actorList{ actor_name };
      actorsInMovie[movie] = actorList;
    }
    //! Otherwise add actor to list of actors in the movie
    else
      (aim->second).push_back( actor_name );

    moa = moviesOfActor.find( actor_name );
    //! If actor doesnt exist as key in map, insert it
    if( moa == moviesOfActor.end() ) {
      vector< string > movieList{ movie };
      moviesOfActor[actor_name] = movieList;
    }
    //! Otherwise add movie to list of movies done by the actor
    else
      (moa->second).push_back( movie );
  }

  if( !in.eof() ) {
    cerr << "Failed to read " << infile << "!" << endl;
    return false;
  }

  in.close();

  //! Assign global index to each actor, create 2 hashmaps (id-name & vice-versa)
  int index = 0;
  for( moa = moviesOfActor.begin(); moa != moviesOfActor.end(); ++moa ) {
    actorAtIndex[index]       = moa->first;
    indexOfActor[moa->first]  = index++;
  }

  cout << "done" << endl;

  return true;
}

/** Input params: Input filename
 *  Return param: Boolean
 *  Description : Loads from input actor pair file
 *
 *  Reads the input actor pair file, and calls the corresponding graph
 *  traversal algorithm for weighted/unweighted graph.
 */
bool ActorGraph::loadTestPairs( const char *infile ) {
  bool flag = false;
  int id1, id2;
  string s;

  //! Open the test_pairs_file
  ifstream in( infile );

  //! Read and ignore first line (i.e. Actor1	Actor2)
  if( in )
    getline( in, s );

  while( in ) {
    //! Get the next line
    if( !getline( in, s ) )
      break;

    istringstream ss( s );
    vector< string > pairs;

    while( ss ) {
      string next;

      //! Get the next string before hitting a tab character and put it in next
      if( !getline( ss, next, '\t' ) )
        break;

      pairs.push_back( next );
    }

    if( pairs.size() != 2 ) {
      //! We should have exactly 2 columns
      continue;
    }

    //! Get actor names
    string actor1( pairs[0] );
    string actor2( pairs[1] );

    cout << "Computing path for (" << actor1 << ") -> (" << actor2 << ")\n";

    //! Get index of actor 1
    ioa = indexOfActor.find( actor1 );
    if( ioa == indexOfActor.end() ) {
      cout << "Failure to locate node '" << actor1 << "'" << endl;
      flag = true;
    } else
      id1 = ioa->second;

    //! Get index of actor 2
    ioa = indexOfActor.find( actor2 );
    if( ioa == indexOfActor.end() ) {
      cout << "Failure to locate node '" << actor2 << "'" << endl;
      flag = true;
    } else
      id2 = ioa->second;

    if( flag )
      continue;

    //! Perform proper traversal corresponding to weighted/unweighted edges
    if( use_weighted_edges == "u" )
      BFSTraverse( id1, id2 );
    else
      DjikstraTraverse( id1, id2 );
  }

  if( !in.eof() ) {
    cerr << "Failed to read " << infile << "!\n";
    return false;
  }

  //! Close file
  in.close();

  return true;
}

/** Input params: Output filename
 *  Return param: Boolean
 *  Description : Writes to output file
 *
 *  Writes out all the lines stored in outLines, which got populated (hopefully)
 *  during our graph traversal.
 */
bool ActorGraph::writeOutLines( const char *outfile ) {
  //! Open file for writing output
  ofstream out( outfile );
  if( !out.is_open() ) {
    cerr << "Failed to open " << outfile << "!\n";
    return false;
  }

  //! Write output
  for( vit = outLines.begin(); vit != outLines.end(); ++vit )
    out << *vit << endl;

  //! Close file
  out.close();

  return true;
}
