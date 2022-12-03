# Graphs

## Data structures

Ways of representing the data:
- Adjacency list
- Adjacency matrix (v^2 with cells containing edge data)
- Edge list (2 arrays for vertices and links)

### Adjacency list

Vector of vertices with pointers to vectors of edges

```cpp
struct edge_t { int target_id; double distance; };
struct vertex_t { std::vector<edge_t> edges; std::string name; };
struct graph_t { std::vector<vertex_t> vertices; };
```


### Edge list

```cpp
struct edge_t { int source_id; int target_id; double distance; } 
struct graph_t { std::vector<edge_t> edges; std::vector<std::string> vertices;}
```


### Templated example

```cpp
template <class EV = void,          // edge value type
          class VV = void,          // vertex value type
          class GV = void,          // graph value type
          integral VId = uint32_t>  // vertex id type
class csr_graph;

using G = csr_graph<double, str_view, std::string>;
```

## Algorithms

Depth first search

Breadth first search

Topological sort 

Dijkstra's Shortest Path: 
    Given starting point, finds total distances to all possible targets.

Bellman-Ford Shortest Path

Connected Components

Strongly Connected Components

Minimum Spanning Tree




## Refs

https://www.youtube.com/watch?v=jCnBFjkVuN0