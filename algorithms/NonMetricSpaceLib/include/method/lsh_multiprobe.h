/**
 * Non-metric Space Library
 *
 * Authors: Bilegsaikhan Naidan (https://github.com/bileg), Leonid Boytsov (http://boytsov.info).
 * With contributions from Lawrence Cayton (http://lcayton.com/) and others.
 *
 * For the complete list of contributors and further details see:
 * https://github.com/searchivarius/NonMetricSpaceLib 
 * 
 * Copyright (c) 2014
 *
 * This code is released under the
 * Apache License Version 2.0 http://www.apache.org/licenses/.
 *
 */

#ifndef _LSH_MULTI_PROBE_H_
#define _LSH_MULTI_PROBE_H_

#include "index.h"
#include "space.h"
#include "lshkit.h"

#define METH_LSH_MULTIPROBE         "lsh_multiprobe"

namespace similarity {

// this class is a wrapper around lshkit and
// but lshkit can handle only float!

template <typename dist_t>
class Space;

template <typename dist_t>
class MultiProbeLSH : public Index<dist_t> {
 public:
  MultiProbeLSH(const Space<dist_t>* space,
                const ObjectVector& data,
                // for FitData()
                unsigned N1,          // number of points to use
                unsigned P,           // number of pairs to sample
                unsigned Q,           // number of queries to sample
                unsigned K,           // search for K neighbors neighbors
                unsigned F,           // divide the sample to F folds
                // for MPLSHTune()
                unsigned N2,          // dataset size
                double R,             // desired recall
                // other config
                unsigned L,           // # of hash tables
                unsigned T,           // # of bins probed in each hash table
                unsigned H,           // hash table size
                int      M,           // # of hash functions
                float  W              // width
                );
  ~MultiProbeLSH();

  const std::string ToString() const;
  void Search(RangeQuery<dist_t>* query);
  void Search(KNNQuery<dist_t>* query);

 private:
  typedef lshkit::MultiProbeLshIndex<unsigned> LshIndexType;

  const ObjectVector& data_;
  int dim_;
  lshkit::FloatMatrix* matrix_;
  LshIndexType* index_;
  unsigned T_;
  float R_;

  // disable copy and assign
  DISABLE_COPY_AND_ASSIGN(MultiProbeLSH);
};

}   // namespace similarity

#endif     // _LSH_MULTI_PROBE_H_
