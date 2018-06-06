/**
 @author Rajdeep Konwar (A53225609)
 
 @date Dec 09 2017
 
 @section OVERVIEW
 Implementation of main program.
 
 @section ASSIGNMENT NUMBER
 PA4
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <vector>
#include <unordered_map>

using namespace std;

int main( int argc, char** argv ) {
  //! Start time
  clock_t t   = clock();

  //! Check for Arguments
  if( argc != 4 ) {
    cout << "Invalid number of arguments." << endl
         << "Usage: ./main movie_casts_file actors_file output_file" << endl;
    return -1;
  }

  string next, s;
  ifstream infile( argv[1] );

  //! < movie_year, vector< actor > >
  unordered_map< string, vector< string > > actorsInMovie;
  unordered_map< string, vector< string > >::iterator aim;

  //! < actor, vector< movie_year > >
  unordered_map< string, vector< string > > moviesOfActor;
  unordered_map< string, vector< string > >::iterator moa;

  //! Read and ignore first line (i.e. Actor/Actress	Movie	Year)
  if( infile )
    getline( infile, s );

  //! Keep reading lines until the end of file is reached
  while( infile ) {
    //! Get the next line
    if( !getline( infile, s ) )
      break;

    //! Convert line to a string stream
    istringstream ss( s );
    vector< string > record;

    while( ss ) {
      //! Get the next string before hitting a tab character and put it in next
      if( !getline( ss, next, '\t' ) )
        break;

      record.push_back( next );
    }

    //! We should have exactly 3 columns
    if( record.size() != 3 )
      continue;

    string actor_name( record[0] );               //! Actor name
    string movie = record[1] + "_" + record[2];   //! Movie title_year

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

  cout << "No. of movies = " << actorsInMovie.size() << endl;
  cout << "No. of actors = " << moviesOfActor.size() << endl << endl;

  if( !infile.eof() ) {
    cerr << "Failed to read " << argv[1] << "!\n";
    return false;
  }

  //! Close input file
  infile.close();
  infile.open( argv[2] );

  //! Read and ignore first line (i.e. Actor)
  if( infile )
    getline( infile, s );

  //! Variables
  size_t neighborCount;
  vector< string > neighbors;
  vector< string >::iterator it, iter;

  //! Keep reading lines until the end of file is reached
  while( infile ) {
    //! Get the next line
    if( !getline( infile, s ) )
      break;

    //! Populate actors to find unique neighbors for
    neighbors.push_back( s );
  }

  //! Open outfile file in ofstream
  ofstream outfile( argv[3] );
  if( !outfile.is_open() ) {
    cout << "Unable to open output file. Please try again." << endl;
    return -1;
  }

  cout << "Number of Neighbors" << endl;
  outfile << "Number of Neighbors" << endl;

  for( it = neighbors.begin(); it != neighbors.end(); ++it ) {
    neighborCount = 0;
    //! Maintain a set of unique neighbors for each actor
    set< string > uniqueNeighbors;
    //! Search for actor as key in map
    moa = moviesOfActor.find( *it );

    if( moa != moviesOfActor.end() ) {
      for( iter = (moa->second).begin(); iter != (moa->second).end(); ++iter ) {
        //! Search for movie as key in map
        aim = actorsInMovie.find( *iter );

        if( aim != actorsInMovie.end() ) {
          //! Uniquely copy vector of actors to set
          copy( (aim->second).begin(), (aim->second).end(),
                inserter( uniqueNeighbors, uniqueNeighbors.end() ) );
        }
      }
    }

    //! If unique neighbors exist
    if( !uniqueNeighbors.empty() )
      neighborCount = uniqueNeighbors.size() - 1;   //! Deduct self from set

    //! Write output
    cout << *it << ": " << neighborCount << endl;
    outfile << neighborCount << endl;
  }

  //! Close output file
  outfile.close();

  //! Finish time
  t = clock() - t;
  cout << "\nTime taken: " << (float) t / CLOCKS_PER_SEC << "s" << endl;

  return 0;
}
