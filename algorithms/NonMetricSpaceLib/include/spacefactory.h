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

#ifndef SPACE_FACTORY_H
#define SPACE_FACTORY_H

#include <string>

#include "space.h"
#include "space/space_lp.h"
#include "space/space_bregman.h"
#include "params.h"

namespace similarity {

#define REGISTER_SPACE_CREATOR(type, name, func)\
      SpaceFactoryRegistry<type>::Instance().Register(name, func);
 

template <typename dist_t>
class SpaceFactoryRegistry {
public:
  typedef Space<dist_t>* (*CreateFuncPtr)(const AnyParams&);
  static SpaceFactoryRegistry& Instance() {
    static SpaceFactoryRegistry elem;

    return elem;
  }

  void Register(const string& SpaceType, CreateFuncPtr func) {
    LOG(LIB_INFO) << "Registering at the factory, space: " << SpaceType << " distance type: " << DistTypeName<dist_t>();
    Creators_[SpaceType] = func;
  }

  Space<dist_t>* CreateSpace(const string& SpaceType, const AnyParams& SpaceParams) {
    if (Creators_.count(SpaceType)) {
      return Creators_[SpaceType](SpaceParams);
    } else {
      LOG(LIB_FATAL) << "It looks like the space " << SpaceType << 
                    " is not defined for the distance type : " << DistTypeName<dist_t>();
    }
    return NULL;
  }
private:
  map<string, CreateFuncPtr>  Creators_;
};

};

#endif
