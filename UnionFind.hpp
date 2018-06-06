/**
 @author Rajdeep Konwar (A53225609)
 
 @date Dec 09 2017
 
 @section OVERVIEW
 Declaration and implementation of class ActorConnect's member functions
 required to find connections between a pair of actors.
 
 @section ASSIGNMENT NUMBER
 PA4
 */

#ifndef UNIONFIND_HPP
#define UNIONFIND_HPP

#include "ActorGraph.h"

//! Class for actor connections, inheriting publically from ActorGraph
class ActorConnect : public ActorGraph {
protected:
  set< int >            movieYears;   //! Set of movie years
  set< int >::iterator  mit;          //! Movie year iterator
  vector< int >         dsArr;        //! Disjoint Set array

  //! Extract years from the graph in ascending order
  void extractYearsInAscOrder();

  //! Construct edges in the graph for a given year
  void constructEdges( const int &year );

  //! Delete all edges in the graph
  void deleteEdges();

  //! Connect a pair of actors for a given algorithm
  string connectPair( const string &algorithm, const int &id1, const int &id2 );

  //! Perform BFS traversal to find actor connections
  bool pairBFSTraverse( const int &from, const int &to );

  //! Find an actor in the disjoint set array
  int Find( const int &index, int &height );

  //! Perform union of disjoint sets
  void Union( const int &id1, const int &id2 );

  //! Constructs the disjoint set array for a given year
  void constructArr( const int &year );

  //! Perform union-find traversal to finf actor connections
  bool pairUFindTraverse( const int &from, const int &to );

public:
  //! Constructor
  ActorConnect( const string &outLine );

  //! Connect actors from input actor pair file
  bool connectActors( const char *infile, const string &algo );
};

/** Input params: First line to output in file
 *  Return param: None
 *  Description : Constructor
 */
ActorConnect::ActorConnect( const string &outLine ) : ActorGraph( outLine ) {}

/** Input params: None
 *  Return param: None
 *  Description : Constructor
 *
 *  Populates the set with movie years in an ascending order.
 */
void ActorConnect::extractYearsInAscOrder() {
  for( aim = actorsInMovie.begin(); aim != actorsInMovie.end(); ++aim )
    movieYears.insert( extractYear( aim->first ) ); //! Insert into set
}

/** Input params: Year
 *  Return param: None
 *  Description : Construct edges
 *
 *  Construct edges between actor nodes for movies of a particular year only.
 */
void ActorConnect::constructEdges( const int &year ) {
  Actor *actor;   //! Actor node
  Edge  *edge;    //! Edge between actors
  int   id1, id2; //! Indices of the actors

  //! *moa is actor
  for( moa = moviesOfActor.begin(); moa != moviesOfActor.end(); ++moa ) {
    for( vit = (moa->second).begin(); vit != (moa->second).end(); ++vit ) {
      //! *vit is movie of actor (only consider movies matching year)
      if( extractYear( *vit ) != year )
        continue;

      aim = actorsInMovie.find( *vit );   //! *aim is movie
      if( aim != actorsInMovie.end() ) {
        for( sit = (aim->second).begin(); sit != (aim->second).end(); ++sit ) {
          //! *sit is list of actors in movie

          //! Skip self
          if( (moa->first).compare( *sit ) == 0 )
            continue;

          //! Get index of actor1
          ioa   = indexOfActor.find( moa->first );
          id1   = ioa->second;
          actor = theGraph[id1];

          //! Get index of actor2
          ioa   = indexOfActor.find( *sit );
          id2   = ioa->second;
          eit   = actor->adj.find( id2 );

          //! Construct a new edge if one doesn't exist between actor1-actor2
          if( eit == actor->adj.end() ) {
            edge            = new Edge;
            edge->movie     = *vit;
            edge->weight    = 1;
            actor->adj[id2] = edge;
          }

          actor = theGraph[id2];
          eit   = actor->adj.find( id1 );

          //! Edge needs to be two-way hence construct for actor2-actor1
          if( eit == actor->adj.end() ) {
            edge            = new Edge;
            edge->movie     = *vit;
            edge->weight    = 1;
            actor->adj[id1] = edge;
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
  for( ait = theGraph.begin(); ait != theGraph.end(); ++ait ) {
    for( eit = (*ait)->adj.begin(); eit != (*ait)->adj.end(); ++eit )
      delete (*eit).second;

    (*ait)->adj.clear();  //! Clear the hash-map of all entries
  }
}

/** Input params: Traverse algorithm, indices of actors
 *  Return param: String of year the actors got connected
 *  Description : Connect pair of actors
 *
 *  Connects a pair of actors depending on algorithm used and returns the year.
 */
string ActorConnect::connectPair( const string &algorithm,
                                  const int &id1, const int &id2 ) {
  if( algorithm == "bfs" )
    deleteEdges();  //! Clear hash-map of edges before venturing to next pair
  else
    dsArr = vector< int >( theGraph.size(), -1 ); //! Array of indices to -1

  //! *mit is pair of year and bool-flag in set (in ascending order)
  for( mit = movieYears.begin(); mit != movieYears.end(); ++mit ) {
    if( algorithm == "bfs" ) {
      //! Construct edges for a particular year
      constructEdges( *mit );

      //! Perform BFS traversal, if unsuccessful, increment year and try again
      if( !pairBFSTraverse( id1, id2 ) )
        continue;
      else
        return to_string( *mit );
    } else {
      //! Construct disjoint set array for each year
      constructArr( *mit );

      //! Perform union-find traversal till successful
      if( !pairUFindTraverse( id1, id2 ) )
        continue;
      else
        return to_string( *mit );
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
bool ActorConnect::pairBFSTraverse( const int &from, const int &to ) {
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

  //! Reset distances and previous info for all visited nodes during traversal
  for( ait = visited.begin(); ait != visited.end(); ++ait ) {
    (*ait)->dist = numeric_limits< int >::max();
    (*ait)->prev = -1;
  }

  //! Return success flag
  return found;
}

/** Input params: Actor index and height of disjoint set (passed by reference)
 *  Return param: Sentinel node index
 *  Description : Find actor in the disjoint set array
 *
 *  Finds an actor in the disjoint set array and returns index of sentinel node.
 */
int ActorConnect::Find( const int &index, int &height ) {
  int val = dsArr[index]; //! Value of disjoint set at index
  int ind = index;        //! Stores sentinel node info
  vector< int > indices;  //! Nodes traversed during find (for path compression)

  while( val != -1 ) {
    indices.push_back( ind );
    ind = val;
    val = dsArr[val];
    height++;
  }

  //! Path-compression algorithm (for disjoint sets having height greater than 1)
  if( height > 1 ) {
    vector< int >::iterator it; //! Iterator

    for( it = indices.begin(); it != indices.end(); ++it )
      dsArr[*it] = ind;
  }

  //! Return index of sentinel node
  return ind;
}

/** Input params: Indices of actor nodes to unite
 *  Return param: None
 *  Description : Perform union of two actors
 *
 *  Performs union operation on the disjoint set array.
 */
void ActorConnect::Union( const int &id1, const int &id2 ) {
  int height1 = 0, height2 = 0;
  int find1 = Find( id1, height1 );   //! Sentinel node-index of actor1
  int find2 = Find( id2, height2 );   //! Sentinel node-index of actor2

  //! Perform union operation
  if( find1 != find2 ) {
    if( height1 < height2 )
      dsArr[find1] = find2;
    else
      dsArr[find2] = find1;
  }
}

/** Input params: Year
 *  Return param: None
 *  Description : Constructs disjoint set array for a given year
 *
 *  Performs union operation on actors connected by movie of given year.
 */
void ActorConnect::constructArr( const int &year ) {
  int   id1, id2;

  for( moa = moviesOfActor.begin(); moa != moviesOfActor.end(); ++moa ) {
    for( vit = (moa->second).begin(); vit != (moa->second).end(); ++vit ) {
      //! *vit is movie of actor
      if( extractYear( *vit ) != year )
        continue;

      aim = actorsInMovie.find( *vit );   //! *aim is movie
      if( aim != actorsInMovie.end() ) {
        for( sit = (aim->second).begin(); sit != (aim->second).end(); ++sit ) {
          //! *sit is list of actors in movie

          //! Skip self
          if( (moa->first).compare( *sit ) == 0 )
            continue;

          //! Get index of actor1
          ioa   = indexOfActor.find( moa->first );
          id1   = ioa->second;

          //! Get index of actor2
          ioa   = indexOfActor.find( *sit );
          id2   = ioa->second;

          //! Perform union operation for the actors
          Union( id1, id2 );
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
bool ActorConnect::pairUFindTraverse( const int &from, const int &to ) {
  int height1 = 0, height2 = 0;
  int find1 = Find( from, height1 );  //! Sentinel index of initial node
  int find2 = Find( to, height2 );    //! Sentinel index of end node

  //! UFind traversal was successfull if both sentinel indices were equal
  return (find1 == find2);
}

/** Input params: Input filename and traversal algorithm
 *  Return param: Boolean
 *  Description : Connect actors in input actor-pair file
 */
bool ActorConnect::connectActors( const char *infile, const string &algo ) {
  bool flag = false;
  int id1, id2;
  string s;

  if( algo == "bfs" )
    cout << "Running BFS" << endl;
  else
    cout << "Running UFind" << endl;

  extractYearsInAscOrder();

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

    //! Get index of actor1
    ioa = indexOfActor.find( actor1 );
    if( ioa == indexOfActor.end() )
      flag = true;
    else
      id1 = ioa->second;

    //! Get index of actor2
    ioa = indexOfActor.find( actor2 );
    if( ioa == indexOfActor.end() )
      flag = true;
    else
      id2 = ioa->second;

    string line = actor1 + "\t" + actor2 + "\t";

    //! Try and find connection between the pair
    if( !flag )
      line += connectPair( algo, id1, id2 );
    else
      line += "9999";

    outLines.push_back( line );   //! Store line info to be written out later
  }

  return true;
}

#endif //! UNIONFIND_HPP
