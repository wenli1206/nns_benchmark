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

#ifndef _UTILS_H_
#define _UTILS_H_

#include <cctype>
#include <cstddef>
#include <cmath>
#include <cstdint>
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <map>
#include <typeinfo>
#include <random>
#include <climits>
#include <stdexcept>

// compiler_warning.h
#define STRINGISE_IMPL(x) #x
#define STRINGISE(x) STRINGISE_IMPL(x)

/*
 * This solution for generating
 * cross-platform warnings
 * is taken from http://stackoverflow.com/a/1911632/2120401
 * Use: #pragma message WARN("My message")
 *
 * Note: We may need other other definitions for other compilers,
 *       but so far it worked for MSVS, GCC, CLang, and Intel.
 */
#   define FILE_LINE_LINK __FILE__ "(" STRINGISE(__LINE__) ") : "
#   define WARN(exp) (FILE_LINE_LINK "WARNING: " exp)

#ifdef _MSC_VER
#define PATH_SEPARATOR "\\"
#else 
#define PATH_SEPARATOR "/"
#endif


#ifdef _MSC_VER
#define ISNAN _isnan
#define __func__ __FUNCTION__ 
#else
#define ISNAN std::isnan
#endif

#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

#define FIELD_DELIMITER ':'

namespace similarity {

using std::string;
using std::vector;
using std::stringstream;
using std::random_device;
using std::mt19937;

using namespace std;


const char* GetFileName(const char* fullpath);

bool CreateDir(const char* name, int mode = 0777);

bool DoesFileExist(const char* filename);

inline bool DoesFileExist(const string& filename) { return DoesFileExist(filename.c_str()); }

inline int RandomInt() {
    // Static is thread-safe in C++ 11
    static random_device rdev;
    static mt19937 gen(rdev());
    static std::uniform_int_distribution<int> distr(0, std::numeric_limits<int>::max());
  
    return distr(gen); 
}

template <class T>
inline T RandomReal() {
    // Static is thread-safe in C++ 11
    static random_device rdev;
    static mt19937 gen(rdev());
    static std::uniform_real_distribution<T> distr(0, 1);

    return distr(gen); 
}

void RStrip(char* str);

double Mean(const double* array, const unsigned size);

double Variance(const double* array, const unsigned size);

double Variance(const double* array, const unsigned size, const double mean);

double StdDev(const double* array, const unsigned size);

/*
 * A maximum number of random operations (e.g. while searching
 * for a pivot with given properties) before giving up and firing
 * an exception.
 */
#define MAX_RAND_ITER_BEFORE_GIVE_UP 100000

/*
 * We want to avoid an overflow in the case when the distance is an integer type.
 */
template <typename dist_t>
inline dist_t DistMax() {
  return std::numeric_limits<dist_t>::max() / 2;
}

/* 
 * 1. For floating-point numbers let's consider numbers to be 
 * equal if they are within 4 units in the last place (ULPs)
 * Or, if the numbers are smaller than 2*numeric_limits<T>::epsilon()
 * 2. For integral types an approximate equality is the same as an exact equality.
 */
#define MAX_ULPS 4

template <typename T>
bool ApproxEqual(const T& x, const T& y, unsigned maxUlps = MAX_ULPS);

inline double round1(double x) { return round(x*10.0)/10.0; }
inline double round2(double x) { return round(x*100.0)/100.0; }
inline double round3(double x) { return round(x*1000.0)/1000.0; }
inline double round6(double x) { return round(x*1000000.0)/1000000.0; }
/*
 * This function will only work for strings without spaces
 * TODO(@leo) replace, perhaps, it with a more generic version
 * Another TODO is to get rid of all streams all together,
 * because they are horribly slow. Does it affect our performance?
 *
 * See, e.g. Leo's rant here:
 * http://searchivarius.org/blog/branchless-code-would-leave-you-speechless-c-streams-are-super-expensive
 */
template <typename ElemType>
inline bool SplitStr(const std::string& str_, vector<ElemType>& res, const char SplitChar) {
  res.clear();
  std::string str = str_;

  for (auto it = str.begin(); it != str.end(); ++it) {
    if (*it == SplitChar) *it = ' ';
  }
  std::stringstream inp(str);

  while (!inp.eof()) {
    ElemType token;
    if (!(inp >> token)) {
      return false;
    }
    res.push_back(token);
  }

  return true;
}

  /*
   * "fields" each occupy a single line, they are in the format:
   * fieldName:fieldValue.
   */

// Returns false if the line is empty
inline void ReadField(istream &in, const string& fieldName, string& fieldValue) {
  string s;
  if (!getline(in, s)) throw runtime_error("Error reading a field value");
  if (s.empty()) {
    throw runtime_error("Empty field!");
  }
  string::size_type p = s.find(FIELD_DELIMITER);
  if (string::npos == p)
    throw runtime_error("Wrong field format, no delimiter: '" + s + "'");
  string gotFieldName = s.substr(0, p);
  if (gotFieldName != fieldName) {
    throw runtime_error("Expected field '" + fieldName + "' but got: '"
                        + gotFieldName + "'");
  }
  fieldValue = s.substr(p + 1);
}

inline void WriteField(ostream& out, const string& fieldName, const string& fieldValue) {
  if (!(out << fieldName << ":" << fieldValue << std::endl)) {
    throw
      runtime_error("Error writing to an output stream, field name: " + fieldName);
  }
}

template <typename obj_type>
string ConvertToString(const obj_type& o) {
  std::stringstream str;
  str << o;
  return str.str();
}

template <typename obj_type>
void ConvertFromString(const string& s, obj_type& o) {
  std::stringstream str(s);
  if (!(str >> o) || !str.eof()) {
    throw runtime_error("Cannot convert '" + s +
                        "' to the type:" + string(typeid(obj_type).name()));
  }
}

inline void ToLower(string &s) {
  for (size_t i = 0; i < s.size(); ++i) s[i] = std::tolower(s[i]);
}

inline bool StartsWith(const std::string& str, const std::string& prefix) {
  return str.length() >= prefix.length() &&
         str.substr(0, prefix.length()) == prefix;
}

// Don't remove period here
inline void ReplaceSomePunct(string &s) {
  for (size_t i = 0; i < s.size(); ++i)
    if (s[i] == ',' || s[i] == FIELD_DELIMITER) s[i] = ' ';
}

template <class T>
struct AutoVectDel {
  AutoVectDel(typename std::vector<T *>& Vector) : mVector(Vector) {}
  ~AutoVectDel() {
    for (auto i: mVector) { delete i; }
  }
  std::vector<T *>& mVector;
};

template <class U, class V>
struct AutoMapDel {
  AutoMapDel(typename std::map<U, V*>& Map) : mMap(Map) {}
  ~AutoMapDel() {
    for (auto i: mMap) { delete i.second; }
  }
    typename std::map<U, V*>& mMap;
};

template <class U, class V>
struct AutoMultiMapDel {
  AutoMultiMapDel(typename std::multimap<U, V*>& Map) : mMap(Map) {}
  ~AutoMultiMapDel() {
    for (auto i: mMap) { delete i.second; }
  }
    typename std::multimap<U, V*>& mMap;
};

}  // namespace similarity

#endif   // _UTILS_H_

