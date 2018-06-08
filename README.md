# Six Degrees of Kevin Bacon [![FOSSA Status](https://app.fossa.io/api/projects/git%2Bgithub.com%2FRajdeepKonwar%2FSixDegreesOfKevinBacon.svg?type=shield)](https://app.fossa.io/projects/git%2Bgithub.com%2FRajdeepKonwar%2FSixDegreesOfKevinBacon?ref=badge_shield)
Based on the [Six Degrees of Separation](https://en.wikipedia.org/wiki/Six_degrees_of_separation) idea that all actors in Hollywood are six or fewer movies away from the actor [Kevin Bacon](https://www.imdb.com/name/nm0000102/). It is inspired by [The Oracle of Bacon](https://oracleofbacon.org/).

## Compile and Run Instructions
```
make all type=opt
```
* This will generate two programs - `actorconnections` and `pathfinder`.
* The `actorconnections` program answers the question: "After which year did actors X and Y become connected?"
* The `pathfinder` program computes the shortest path between 2 actors (weighted/unweighted where weight is how old the movie is from 2015; newer movies have lesser weight, i.e. higher priority). Weighted traversal uses Dijkstra's (greedy) algorithm whereas the unweighted traversal uses BFS (Breadth First Search) algorithm.
* Omit `type=opt` when compiling for debugging purposes.
* To clean executables, type `make clean`.

# actorconnections
```
./actorconnections movie_casts_file test_pairs_file out_file bfs/ufind
```
For example,
```
./actorconnections movie_casts.tsv test_pairs.tsv out_a.txt bfs
```
The last argument dictates which traversal algorithm to be used: bfs-BFS or ufind-Union Find.

# pathfinder
```
./pathfinder movie_casts_file u/w test_pairs_file out_file
```
For example,
```
./pathfinder movie_casts.tsv w test_pairs.tsv out_p.txt
```
The third argument decides which type of path traversal is performed: u-Unweighted (BFS) or w-Weigted (Dijkstra's).