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

#ifndef _SPACE_SPARSE_VECTOR_H_
#define _SPACE_SPARSE_VECTOR_H_

#include <string>
#include <map>
#include <stdexcept>
#include <algorithm>
#include <ostream>
#include <functional>

#include <string.h>
#include "global.h"
#include "object.h"
#include "utils.h"
#include "space.h"
#include "distcomp.h"

namespace similarity {

using std::vector;
using std::fill;

template <typename dist_t>
struct SparseVectElem {  
  uint32_t  id_;
  dist_t    val_;
  SparseVectElem(uint32_t id = 0, dist_t val = 0) : id_(id), val_(val) {}
  bool operator<(const SparseVectElem<dist_t>& that) const {
    return id_ < that.id_;
  }
  bool operator==(const SparseVectElem<dist_t>& that) const {
    return id_ == that.id_ && val_ == that.val_;
  }
  bool operator!=(const SparseVectElem<dist_t>& that) const {
    return !operator==(that);
  }
};

template <typename dist_t>
ostream& operator<<(ostream& out, SparseVectElem<dist_t> e) {
  return out << "[" << e.id_ << ": " << e.val_ << "]";
}

/* 
 * The maximum number of sparse elements that will be kept on the stack
 * by the function ComputeDistanceHelper. 
 *
 * TODO:@leo If there are too many threads, we might run out stack memory.
 *           but it is probably extremely unlikely with the buffer of this size.
 *
 */
#define MAX_BUFFER_QTY  8192

template <typename dist_t>
class SpaceSparseVector : public Space<dist_t> {
public:
  typedef SparseVectElem<dist_t> ElemType;

  virtual ~SpaceSparseVector() {}

  virtual void ReadDataset(ObjectVector& dataset,
                      const ExperimentConfig<dist_t>* config,
                      const char* inputfile,
                      const int MaxNumObjects) const;

  virtual Object* CreateObjFromVect(IdType id, LabelType label, const vector<ElemType>& InpVect) const;
  // Sparse vectors have no fixed dimensionality
  virtual size_t GetElemQty(const Object* object) const {return 0;}
  virtual void CreateVectFromObj(const Object* obj, dist_t* pVect,
                                   size_t nElem) const = 0;

  virtual dist_t ScalarProduct(const Object* obj1, const Object* obj2) const = 0;
protected:
  virtual Space<dist_t>* HiddenClone() const = 0;
  void ReadSparseVec(std::string line, IdType& label, std::vector<ElemType>& v) const;
};

template <typename dist_t>
class SpaceSparseVectorSimpleStorage : public SpaceSparseVector<dist_t> {
public:
  typedef SparseVectElem<dist_t> ElemType;

  virtual ~SpaceSparseVectorSimpleStorage() {}
  virtual void CreateVectFromObj(const Object* obj, dist_t* pVect,
                                 size_t nElem) const {
    static std::hash<size_t>   indexHash;
    fill(pVect, pVect + nElem, static_cast<dist_t>(0));
    const ElemType* beg = reinterpret_cast<const ElemType*>(obj->data());
    const ElemType* const end =
        reinterpret_cast<const ElemType*>(obj->data() + obj->datalength());
    for (;beg < end;++beg) {
      size_t idx = indexHash(beg->id_) % nElem;
      pVect[idx] += beg->val_;
    }
  }

  virtual dist_t ScalarProduct(const Object* obj1, const Object* obj2) const {
    SpaceNormScalarProduct distObjNormSP;
    return SpaceSparseVectorSimpleStorage<dist_t>::
                            ComputeDistanceHelper(obj1, obj2, distObjNormSP);
  }
protected:
  virtual Space<dist_t>* HiddenClone() const = 0;

  struct SpaceNormScalarProduct {
    dist_t operator()(const dist_t* x, const dist_t* y, size_t length) const {
     return NormScalarProduct(x, y, length);
    }
  };

  
  virtual dist_t HiddenDistance(const Object* obj1, const Object* obj2) const = 0;

  /* 
   * This helper function converts a dense vector to a sparse one and then calls a generic distance function.
   * It can be used only with simple-storage sparse vector spaces, not with
   * children of SpaceSparseVectorInter.
   */
  template <typename DistObjType> 
  static dist_t ComputeDistanceHelper(const Object* obj1, const Object* obj2,  
                               const DistObjType distObj, dist_t missingValue = 0) {
    dist_t *mem1 = NULL, *mem2 = NULL;
    dist_t buf1[MAX_BUFFER_QTY], buf2[MAX_BUFFER_QTY];
    dist_t *vect1 = buf1;
    dist_t *vect2 = buf2;

    dist_t res = 0;

    CHECK(obj1->datalength() > 0);
    CHECK(obj2->datalength() > 0);
    const ElemType* beg1 = reinterpret_cast<const ElemType*>(obj1->data());
    const ElemType* beg2 = reinterpret_cast<const ElemType*>(obj2->data());
    const ElemType* end1 = reinterpret_cast<const ElemType*>(obj1->data() + obj1->datalength());
    const ElemType* end2 = reinterpret_cast<const ElemType*>(obj2->data() + obj2->datalength());
    const size_t qty1 = obj1->datalength() / sizeof(ElemType);
    const size_t qty2 = obj2->datalength() / sizeof(ElemType);
    const size_t maxQty = qty1 + qty2;

    /* 
     * We hope not to allocate memory frequently, b/c memory allocation can be expensive
     * TODO:@leo check how expensive are memory allocations, perhaps, this fear is not substantiated
     */      
    try {
      if (maxQty > MAX_BUFFER_QTY) {
        vect1 = mem1 = new dist_t[maxQty];
        vect2 = mem2 = new dist_t[maxQty];
      }

      size_t qty = 0;
   
      const ElemType* it1 = beg1, *it2 = beg2; 

      while (it1 < end1 && it2 < end2) {
        if (it1->id_ == it2->id_) {
          vect1[qty] = it1->val_;
          vect2[qty] = it2->val_;
          ++it1;
          ++it2;
          ++qty; 
        } else if (it1->id_ < it2->id_) {
          vect1[qty] = it1->val_;
          vect2[qty] = missingValue;
          ++qty;
          ++it1;
        } else {
          vect1[qty] = missingValue;
          vect2[qty] = it2->val_;
          ++qty;
          ++it2;
        }
      }
      
      while (it1 < end1) {
        vect1[qty] = it1->val_;
        vect2[qty] = missingValue;
        ++qty;
        ++it1;
      }
      
      while (it2 < end2) {
        vect1[qty] = missingValue;
        vect2[qty] = it2->val_;
        ++qty;
        ++it2;
      }

      if (qty > maxQty) {
        LOG(LIB_ERROR) << qty1;
        LOG(LIB_ERROR) << qty2;
        LOG(LIB_ERROR) << qty;
      }
      CHECK(qty <= maxQty);

      res = distObj(vect1, vect2, qty);

    } catch (...) {
      delete [] mem1;
      delete [] mem2;
    }

    delete [] mem1;
    delete [] mem2;

    return res;
  }
};

}  // namespace similarity

#endif
