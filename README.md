# Six Degrees of Kevin Bacon [![FOSSA Status](https://app.fossa.io/api/projects/git%2Bgithub.com%2FRajdeepKonwar%2FSixDegreesOfKevinBacon.svg?type=shield)](https://app.fossa.io/projects/git%2Bgithub.com%2FRajdeepKonwar%2FSixDegreesOfKevinBacon?ref=badge_shield)
Based on the [Six Degrees of Separation](https://en.wikipedia.org/wiki/Six_degrees_of_separation) idea that all actors in Hollywood are six or fewer movies away from the actor [Kevin Bacon](https://www.imdb.com/name/nm0000102/). It is inspired by [The Oracle of Bacon](https://oracleofbacon.org/).

## Compile and Run Instructions
```
make all
```
This will compile the 2 programs `actorconnections` and `pathfinder`. The `actorconnections` program answers this question: "After which year did actors X and Y become connected?" whereas the `pathfinder` program prints the shortest path between 2 actors (weighted/unweighted where weight is how old the movie is from 2016; older movies having lesser weight).

```
./actorconnections movie_casts_file test_pairs_file out_file bfs/ufind
```

```
./pathfinder movie_casts_file u/w test_pairs_file out_file
```