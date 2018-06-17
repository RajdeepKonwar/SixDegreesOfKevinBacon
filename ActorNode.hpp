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
 * Declaration of struct Actor's member variables required define a node in graph.
 **/

#ifndef ACTOR_NODE_HPP
#define ACTOR_NODE_HPP

#include <unordered_map>

//! Node in graph representing an actor
struct Actor {
  int                               m_index;  //! Global actor index
  int                               m_dist;   //! Distance from source
  int                               m_prev;   //! Previous actor's index
  bool                              m_done;   //! Discovery flag (for Djikstra's)
  std::unordered_map< int, Edge * > m_adj;    //! Adjacency list (neighbor-index, edge)

  /** '<' operator overloaded to compare actors based on their distance i.e.
   *  lesser distance has higher priority in the priority queue for Djikstra's
   */
  bool operator < ( const Actor &i_other ) {
    return (this->m_dist >= i_other.m_dist);
  }
};

#endif //! ACTORNODE_H
