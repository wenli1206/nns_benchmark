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
#include <cstring>
#include <map>
#include <limits>
#include <iostream>
#include <stdexcept>

#include "utils.h"
#include "params.h"
#include "logging.h"
#include "space.h"

#include <cmath>

using namespace std;

namespace similarity {

using std::multimap;
using std::string;

void ParseSpaceArg(const string& descStr, string& SpaceType, vector<string>& SpaceDesc) {
  vector<string> tmp;
  if (!SplitStr(descStr, tmp, ':') || tmp.size() > 2  || !tmp.size()) {
    throw runtime_error("Wrong format of the space argument: '" + descStr + "'");
  }

  SpaceType = tmp[0];
  SpaceDesc.clear();

  if (tmp.size() == 2) {
    if (!SplitStr(tmp[1], SpaceDesc, ',')) {
      throw runtime_error("Cannot split space arguments in: '" + tmp[1] + "'");
    }
  }
}

void ParseMethodArg(const string& descStr, string& MethName, vector<string>& MethodDesc) {

  vector<string> tmp;
  if (!SplitStr(descStr, tmp, ':') || tmp.size() > 2  || !tmp.size()) {
    throw runtime_error("Wrong format of the method argument: '" + descStr + "'");
  }

  MethName = tmp[0];

  MethodDesc.clear();
  if (tmp.size() == 2) {
    if (!SplitStr(tmp[1], MethodDesc, ',')) {
      throw runtime_error("Cannot split method arguments in: '" + tmp[1] + "'");
    }
  }
}


};


