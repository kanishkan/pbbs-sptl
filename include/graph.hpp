
#include <iostream>
#include <algorithm>
#include "utils.hpp"
//typedef int vindex;

#ifndef _SPTL_GRAPH_INCLUDED
#define _SPTL_GRAPH_INCLUDED

namespace sptl {
namespace graph {
// **************************************************************
//    SPARSE ROW MAJOR REPRESENTATION
// **************************************************************

template <class ETYPE, class intT>
struct sparseRowMajor {
  intT numRows;
  intT numCols;
  intT nonZeros;
  intT* Starts;
  intT* ColIds;
  ETYPE* Values;
  void del() {free(Starts); free(ColIds); if (Values != NULL) free(Values);}
  sparseRowMajor(intT n, intT m, intT nz, intT* S, intT* C, ETYPE* V) :
  numRows(n), numCols(m), nonZeros(nz),
  Starts(S), ColIds(C), Values(V) {}
};

//typedef sparseRowMajor<double> sparseRowMajorD;

// **************************************************************
//    EDGE ARRAY REPRESENTATION
// **************************************************************

template <class intT>
struct edge {
  intT u;
  intT v;
  edge(intT f, intT s) : u(f), v(s) {}
};

template <class intT>
struct edgeArray {
  edge<intT>* E;
  intT numRows;
  intT numCols;
  intT nonZeros;
  void del() {free(E);}
  edgeArray(edge<intT> *EE, intT r, intT c, intT nz) :
  E(EE), numRows(r), numCols(c), nonZeros(nz) {}
  edgeArray() {}
};

// **************************************************************
//    WEIGHED EDGE ARRAY
// **************************************************************

template <class intT>
struct wghEdge {
  intT u, v;
  double weight;
  wghEdge() {}
  wghEdge(intT _u, intT _v, double w) : u(_u), v(_v), weight(w) {}
};

template <class intT>
struct wghEdgeArray {
  wghEdge<intT> *E;
  intT n; intT m;
  wghEdgeArray(wghEdge<intT>* EE, intT nn, intT mm) : E(EE), n(nn), m(mm) {}
  void del() { free(E);}
};

// **************************************************************
//    ADJACENCY ARRAY REPRESENTATION
// **************************************************************

template <class intT>
struct vertex {
  intT* Neighbors;
  intT degree;
  void del() {free(Neighbors);}
  vertex() { }
  vertex(intT* N, intT d) : Neighbors(N), degree(d) {}
};
  
template <class intT>
struct graph {
  vertex<intT> *V;
  intT n;
  intT m;
  intT* allocatedInplace;
  graph(vertex<intT>* VV, intT nn, intT mm)
  : V(VV), n(nn), m(mm), allocatedInplace(NULL) {}
  graph(vertex<intT>* VV, intT nn, intT mm, intT* ai)
  : V(VV), n(nn), m(mm), allocatedInplace(ai) {}
  graph copy() {
    vertex<intT>* VN = newA(vertex<intT>,n);
    intT* Edges = newA(intT,m);
    intT k = 0;
    for (intT i=0; i < n; i++) {
      VN[i] = V[i];
      VN[i].Neighbors = Edges + k;
      for (intT j =0; j < V[i].degree; j++)
        Edges[k++] = V[i].Neighbors[j];
    }
    return graph(VN, n, m, Edges);
  }
  void del() {
    if (allocatedInplace == NULL)
      for (intT i=0; i < n; i++) V[i].del();
    else free(allocatedInplace);
    free(V);
  }
};
  
template <class intT>
std::ostream& operator<<(std::ostream& out, const vertex<intT>& v) {
  out << "{";
  for (long i = 0; i < v.degree; i++) {
    out << " " << v.Neighbors[i] << ", ";
  }
  out << "}";
  return out;
}

} // end namespace
} // end namespace

#endif