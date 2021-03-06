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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <cmath>
#include <limits>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <set>

#include "init.h"
#include "global.h"
#include "utils.h"
#include "memory.h"
#include "ztimer.h"
#include "experiments.h"
#include "experimentconf.h"
#include "space.h"
#include "spacefactory.h"
#include "logging.h"
#include "methodfactory.h"

#include "meta_analysis.h"
#include "params.h"

#include "test_integr_util.h"
#include "testdataset.h"

using namespace similarity;

using std::set;
using std::multimap;
using std::vector;
using std::string;
using std::stringstream;

//#define MAX_THREAD_QTY 4
#define MAX_THREAD_QTY 1
#define TEST_SET_QTY  "10"
#define MAX_NUM_QUERY "100"

vector<MethodTestCase>    vTestCaseDesc = {
  // *************** NEW versions of permutation & projection-based filtering method tests ******************** //
#if !defined(_MSC_VER)
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "nndes:NN=10,rho=0.5,delta=0.001",
                1 /* KNN-1 */, 0 /* no range search */ , 0.9, 1.0, 0, 1.0, 5, 12),  
#endif
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "small_world_rand:NN=10,initSearchAttempts=1,initIndexAttempts=1",
                1 /* KNN-1 */, 0 /* no range search */ , 0.9, 1.0, 0, 1.0, 40, 52),  
#if 1
  // 4 different types of projections
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "proj_incsort:projType=perm,projDim=4,dbScanFrac=1.0",
                1 /* KNN-1 */, 0 /* no range search */ , 0.999, 1.0, 0, 0.01, 0.99, 1.01),  
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "proj_incsort:projType=rand,projDim=4,dbScanFrac=1.0",
                1 /* KNN-1 */, 0 /* no range search */ , 0.999, 1.0, 0, 0.01, 0.99, 1.01),  
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "proj_incsort:projType=fastmap,projDim=4,dbScanFrac=1.0",
                1 /* KNN-1 */, 0 /* no range search */ , 0.999, 1.0, 0, 0.01, 0.99, 1.01),  
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "proj_incsort:projType=randrefpt,projDim=4,dbScanFrac=1.0",
                1 /* KNN-1 */, 0 /* no range search */ , 0.999, 1.0, 0, 0.01, 0.99, 1.01),  

  // Proj. VP-tree
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "proj_vptree:projType=perm,projDim=4,alphaLeft=2,alphaRight=2,dbScanFrac=1.0",
                1 /* KNN-1 */, 0 /* no range search */ , 0.999, 1.0, 0, 0.01, 0.99, 1.01),  

  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt","pp-index:numPivot=4,prefixLength=4,minCandidate=10000",
                1 /* KNN-1 */, 0 /* no range search */ , 0.999, 1.0, 0, 0.01, 0.99, 1.01),  
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "mi-file:numPivot=16,numPivotIndex=16,numPivotSearch=16,dbScanFrac=1.0",

                1 /* KNN-1 */, 0 /* no range search */ , 0.999, 1.0, 0, 0.01, 0.99, 1.01),  
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "napp:numPivot=8,numPivotIndex=8,numPivotSearch=8,chunkIndexSize=102",
                1 /* KNN-1 */, 0 /* no range search */ , 0.999, 1.0, 0, 0.01, 0.99, 1.01),  

  // Binarized permutations
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "perm_incsort_bin:numPivot=32,dbScanFrac=1.0",
                1 /* KNN-1 */, 0 /* no range search */ , 0.999, 1.0, 0, 0.01, 0.99, 1.01),  
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "perm_bin_vptree:numPivot=32,alphaLeft=2,alphaRight=2,dbScanFrac=1.0",
                1 /* KNN-1 */, 0 /* no range search */ , 0.999, 1.0, 0, 0.01, 0.99, 1.01),  


  // 4 different types of projections
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "proj_incsort:projType=perm,projDim=4,dbScanFrac=0.1",
                1 /* KNN-1 */, 0 /* no range search */ , 0.4, 0.7, 0.5, 4, 8, 12),  
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "proj_incsort:projType=rand,projDim=4,dbScanFrac=0.1",
                1 /* KNN-1 */, 0 /* no range search */ , 0.9, 1.01, 0.0, 0.2, 8, 12),  
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "proj_incsort:projType=fastmap,projDim=4,dbScanFrac=0.1",
                1 /* KNN-1 */, 0 /* no range search */ , 0.9, 1.01, 0.0, 0.2, 8, 12),  
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "proj_incsort:projType=randrefpt,projDim=4,dbScanFrac=0.1",
                1 /* KNN-1 */, 0 /* no range search */ , 0.9, 1.01, 0.0, 0.2, 8, 12),  

  // Proj. VP-tree
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "proj_vptree:projType=perm,projDim=4,alphaLeft=2,alphaRight=2,dbScanFrac=0.1",
                1 /* KNN-1 */, 0 /* no range search */ , 0.4, 0.7, 0.5, 4, 8, 12),  

  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt","pp-index:numPivot=4,prefixLength=4,minCandidate=100",
                1 /* KNN-1 */, 0 /* no range search */ , 0.8, 1.0, 0.1, 2, 3, 8),  
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "mi-file:numPivot=16,numPivotIndex=16,numPivotSearch=16,dbScanFrac=0.1",
                1 /* KNN-1 */, 0 /* no range search */ , 0.95, 1.0, 0, 0.5, 8, 12),  
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "napp:numPivot=32,numPivotIndex=8,numPivotSearch=8,chunkIndexSize=102",
                1 /* KNN-1 */, 0 /* no range search */ , 0.6, 0.8, 1, 4, 22, 37),

  // Binarized
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "perm_incsort_bin:numPivot=32,dbScanFrac=0.1",
                1 /* KNN-1 */, 0 /* no range search */ , 0.9, 1.0, 0.01, 0.3, 8, 12),  
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "perm_bin_vptree:numPivot=32,alphaLeft=2,alphaRight=2,dbScanFrac=0.1",
                1 /* KNN-1 */, 0 /* no range search */ , 0.9, 1.0, 0.01, 0.5, 8, 12),  


  // *************** omedrank tests ******************** //

  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "omedrank:numPivot=4,dbScanFrac=0.01,minFreq=0.5,chunkIndexSize=16536",
                1 /* KNN-1 */, 0 /* no range search */ , 0.7, 0.97, 0.1, 1, 70, 120),  
  MethodTestCase(DIST_TYPE_FLOAT, "kldivgenfast", "final8_10K.txt", "omedrank:numPivot=4,dbScanFrac=0.01,minFreq=0.5,chunkIndexSize=16536",
                1 /* KNN-1 */, 0 /* no range search */ , 0.6, 0.9, 0.2, 3, 70, 120),  

  // *************** VP-tree tests ******************** //
  // knn
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "vptree:chunkBucket=1,bucketSize=10", 
                1 /* KNN-1 */, 0 /* no range search */ , 1.0, 1.0, 0.0, 0.0, 40, 70),  
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "vptree:chunkBucket=1,bucketSize=10,alphaLeft=2,alphaRight=2", 
                1 /* KNN-1 */, 0 /* no range search */ , 0.93, 0.97, 0.03, 0.09, 120, 160),  
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final128_10K.txt", "vptree:chunkBucket=1,bucketSize=10", 
                1 /* KNN-1 */, 0 /* no range search */ , 1.0, 1.0, 0.0, 0.0, 1.5, 1.8),  
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final128_10K.txt", "vptree:chunkBucket=1,bucketSize=10,alphaLeft=2,alphaRight=2", 
                1 /* KNN-1 */, 0 /* no range search */ , 0.98, 1.0, 0.0, 0.02, 2.8, 3.4),  
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "vptree:chunkBucket=1,bucketSize=10", 
                10 /* KNN-10 */, 0 /* no range search */ , 1.0, 1.0, 0.0, 0.0, 20, 24),  
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "vptree:chunkBucket=1,bucketSize=10,alphaLeft=2,alphaRight=2", 
                10 /* KNN-10 */, 0 /* no range search */ , 0.93, 0.96, 0.0, 0.02, 56, 63),  
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final128_10K.txt", "vptree:chunkBucket=1,bucketSize=10", 
                10 /* KNN-10 */, 0 /* no range search */ , 1.0, 1.0, 0.0, 0.0, 1.1, 1.3),  
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final128_10K.txt", "vptree:chunkBucket=1,bucketSize=10,alphaLeft=2,alphaRight=2", 
                10 /* KNN-10 */, 0 /* no range search */ , 0.99, 0.999, 0.0, 0.01, 1.5, 2.5),  
  // range
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "vptree:chunkBucket=1,bucketSize=10", 
                0 /* no KNN */, 0.1 /* range search radius 0.1 */ , 1.0, 1.0, 0.0, 0.0, 23, 26),  
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "vptree:chunkBucket=1,bucketSize=10", 
                0 /* no KNN */, 0.5 /* range search radius 0.5 */ , 1.0, 1.0, 0.0, 0.0, 2.4, 3),  

  // *************** MVP-tree tests ******************** //
  // knn
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "mvptree:maxPathLen=4,bucketSize=10", 
                1 /* KNN-1 */, 0 /* no range search */ , 1.0, 1.0, 0.0, 0.0, 120, 140),  
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "mvptree:maxPathLen=4,bucketSize=10", 
                10 /* KNN-10 */, 0 /* no range search */ , 1.0, 1.0, 0.0, 0.0, 40, 50),  
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "mvptree:maxPathLen=4,bucketSize=10,maxLeavesToVisit=10", 
                1 /* KNN-1 */, 0 /* no range search */ , 0.82, 0.9, 0.2, 3, 230, 250),  
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "mvptree:maxPathLen=4,bucketSize=10,maxLeavesToVisit=20", 
                10 /* KNN-10 */, 0 /* no range search */ , 0.75, 0.82, 0.2, 2.0, 85, 100),  

  // range
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "mvptree:maxPathLen=4,bucketSize=10", 
                0 /* no KNN */, 0.1 /* range search radius 0.1 */ , 1.0, 1.0, 0.0, 0.0, 40, 55),  
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "mvptree:maxPathLen=4,bucketSize=10", 
                0 /* no KNN */, 0.5 /* range search radius 0.5*/ , 1.0, 1.0, 0.0, 0.0, 3, 4),  

  // *************** GH-tree tests ******************** //
  // knn
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "ghtree:bucketSize=10", 
                1 /* KNN-1 */, 0 /* no range search */ , 1.0, 1.0, 0.0, 0.0, 28, 35),  
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "ghtree:bucketSize=10", 
                10 /* KNN-10 */, 0 /* no range search */ , 1.0, 1.0, 0.0, 0.0, 8, 10.2),  
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "ghtree:bucketSize=10,maxLeavesToVisit=10", 
                1 /* KNN-1 */, 0 /* no range search */ , 0.8, 0.87, 0.2, 1.5, 95, 115),  
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "ghtree:bucketSize=10,maxLeavesToVisit=20", 
                10 /* KNN-10 */, 0 /* no range search */ , 0.75, 0.82, 0.1, 1.0, 52, 62),  
  // range
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "ghtree:bucketSize=10", 
                0 /* no KNN */, 0.1 /* range search radius 0.1 */ , 1.0, 1.0, 0.0, 0.0, 10, 16),  
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "ghtree:bucketSize=10", 
                0 /* no KNN */, 0.5 /* range search radius 0.5*/ , 1.0, 1.0, 0.0, 0.0, 1, 1.2),  

  // *************** SA-tree tests ******************** //
  // knn
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "satree:bucketSize=10", 
                1 /* KNN-1 */, 0 /* no range search */ , 1.0, 1.0, 0.0, 0.0, 25, 33),  
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "satree:bucketSize=10", 
                10 /* KNN-10 */, 0 /* no range search */ , 1.0, 1.0, 0.0, 0.0, 10, 25),  
  // range
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "satree:bucketSize=10", 
                0 /* no KNN */, 0.1 /* range search radius 0.1 */ , 1.0, 1.0, 0.0, 0.0, 14, 18),  
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "satree:bucketSize=10", 
                0 /* no KNN */, 0.5 /* range search radius 0.5*/ , 1.0, 1.0, 0.0, 0.0, 2.8, 3.4),  

  // *************** List of clusters tests ******************** //
  // knn
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "list_clusters:strategy=random,useBucketSize=1,bucketSize=10,maxLeavesToVisit=2147483647", 
                1 /* KNN-1 */, 0 /* no range search */ , 1.0, 1.0, 0.0, 0.0, 9.5, 11.5),  
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "list_clusters:strategy=random,useBucketSize=1,bucketSize=10,maxLeavesToVisit=2147483647", 
                10 /* KNN-10 */, 0 /* no range search */ , 1.0, 1.0, 0.0, 0.0, 7.5, 8.5),  
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "list_clusters:strategy=random,useBucketSize=1,bucketSize=10,maxLeavesToVisit=10", 
                1 /* KNN-1 */, 0 /* no range search */ , 0.78, 0.9, 0.2, 1.5, 9.5, 11.5),  
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "list_clusters:strategy=random,useBucketSize=1,bucketSize=10,maxLeavesToVisit=20", 
                10 /* KNN-10 */, 0 /* no range search */ , 0.85, 0.97, 0.05, 0.7, 8.5, 10.5),  
  // range
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "list_clusters:strategy=random,useBucketSize=1,bucketSize=10,maxLeavesToVisit=2147483647", 
                0 /* no KNN */, 0.1 /* range search radius 0.1 */ , 1.0, 1.0, 0.0, 0.0, 8, 10),  
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "list_clusters:strategy=random,useBucketSize=1,bucketSize=10,maxLeavesToVisit=2147483647", 
                0 /* no KNN */, 0.5 /* range search radius 0.5*/ , 1.0, 1.0, 0.0, 0.0, 2.4, 3.4),  

  // *************** bbtree tests ******************** //
  // knn
  /* 
   * TODO @leo 
   *      bbtree seems to be a bit wacky (missing a tiny fraction of answers), 
   *      need to debug it in the future.
   *      Therefore, we expect a slightly imperfect recall sometimes.
   */
  MethodTestCase(DIST_TYPE_FLOAT, "kldivgenfast", "final8_10K.txt", "bbtree:bucketSize=10,maxLeavesToVisit=2147483647", 
                1 /* KNN-1 */, 0 /* no range search */ , 0.999, 1.0, 0.0, 0.0, 9.5, 11.5),  
  MethodTestCase(DIST_TYPE_FLOAT, "kldivgenfast", "final8_10K.txt", "bbtree:bucketSize=10,maxLeavesToVisit=2147483647", 
                10 /* KNN-10 */, 0 /* no range search */ , 0.999, 1.0, 0.0, 0.0, 5.5, 8),  
  MethodTestCase(DIST_TYPE_FLOAT, "kldivgenfast", "final8_10K.txt", "bbtree:bucketSize=10,maxLeavesToVisit=10", 
                1 /* KNN-1 */, 0 /* no range search */ , 0.75, 0.85, 0.3, 1.5, 48, 52),  
  MethodTestCase(DIST_TYPE_FLOAT, "kldivgenfast", "final8_10K.txt", "bbtree:bucketSize=10,maxLeavesToVisit=20", 
                10 /* KNN-10 */, 0 /* no range search */ , 0.7, 0.78, 0.3, 1.6, 28, 37),  
  // range
  MethodTestCase(DIST_TYPE_FLOAT, "kldivgenfast", "final8_10K.txt", "bbtree:bucketSize=10,maxLeavesToVisit=2147483647", 
                0 /* no KNN */, 0.1 /* range search radius 0.1 */ , 0.999, 1.0, 0.0, 0.0, 4.5, 6.5),  
  MethodTestCase(DIST_TYPE_FLOAT, "kldivgenfast", "final8_10K.txt", "bbtree:bucketSize=10,maxLeavesToVisit=2147483647", 
                0 /* no KNN */, 0.5 /* range search radius 0.5*/ , 0.999, 1.0, 0.0, 0.0, 1.2, 2.4),  

#ifndef _MSC_VER
  // no LSH for Windows
  // *************** multi-probe LSH tests ******************** //
  // knn
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "lsh_multiprobe:desiredRecall=0.5,tuneK=1,T=5,L=25,H=16535",
                1 /* KNN-1 */, 0 /* no range search */ , 0.45, 0.6, 55, 75, 90, 130),  
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "lsh_multiprobe:desiredRecall=0.5,tuneK=10,T=5,L=25,H=16535",
                10 /* KNN-10 */, 0 /* no range search */ , 0.45, 0.6, 10, 40, 80, 120),  
  // *************** Guassian LSH tests ******************** //
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "lsh_gaussian:W=2,L=5,M=40,H=16535",

                1 /* KNN-1 */, 0 /* no range search */ , 0.85, 0.95, 0.1, 40, 90, 130),  
  MethodTestCase(DIST_TYPE_FLOAT, "l2", "final8_10K.txt", "lsh_gaussian:W=2,L=5,M=40,H=16535",

                10 /* KNN-10 */, 0 /* no range search */ , 0.68, 0.82, 0.1, 50, 80, 120),  
  // *************** Cauchy LSH tests ******************** //
  MethodTestCase(DIST_TYPE_FLOAT, "l1", "final8_10K.txt", "lsh_cauchy:W=2,L=5,M=10,H=16535",
                1 /* KNN-1 */, 0 /* no range search */ , 0.7, 0.9, 0.1, 50, 90, 130),  
  MethodTestCase(DIST_TYPE_FLOAT, "l1", "final8_10K.txt", "lsh_cauchy:W=2,L=5,M=10,H=16535",
                10 /* KNN-10 */, 0 /* no range search */ , 0.5, 0.8, 0.1, 50, 80, 120),  
  // *************** Thresholding LSH tests ******************** //
  MethodTestCase(DIST_TYPE_FLOAT, "l1", "final8_10K.txt", "lsh_threshold:L=5,M=60,H=16535",
                1 /* KNN-1 */, 0 /* no range search */ , 0.8, 0.99, 0.1, 50, 40, 70),  
  MethodTestCase(DIST_TYPE_FLOAT, "l1", "final8_10K.txt", "lsh_threshold:L=5,M=60,H=16535",
                10 /* KNN-10 */, 0 /* no range search */ , 0.65, 0.85, 0.1, 50, 40, 70),  
#endif
#endif
};


int main(int ac, char* av[]) {
  // This should be the first function called before
  string LogFile;
  if (ac == 2) LogFile = av[1];

  initLibrary(LogFile.empty() ? LIB_LOGSTDERR:LIB_LOGFILE, LogFile.c_str());

  WallClockTimer timer;
  timer.reset();

  set<string>           setDistType;
  set<string>           setSpaceType;
  set<string>           setDataSet;
  set<unsigned>         setKNN;
  set<float>            setRange;

  for (const auto& testCase: vTestCaseDesc) {
    setDistType.insert(testCase.mDistType);
    setSpaceType.insert(testCase.mSpaceType);
    setDataSet.insert(testCase.mDataSet);
    if (testCase.mKNN > 0)
      setKNN.insert(testCase.mKNN);
    if (testCase.mRange > 0)
      setRange.insert(testCase.mRange);
  };  

  size_t nTest = 0;
  size_t nFail = 0;
  /* 
   * 1. Let's iterate over all combinations of data sets,
   * distance, and space types. 
   * 2. For each combination, we select test cases 
   * with exactly same data set, distance and space type. 
   * 3. Create an array of arguments in the same format
   *    as used by the main benchmarking utility.
   * 4. Use a standard function to parse these arguments.
   */
  for (string dataSet  : setDataSet)
  for (string distType : setDistType)
  for (string spaceType: setSpaceType) {
    vector<string>    vArgvInit;


    vArgvInit.push_back("--distType"); vArgvInit.push_back(distType);
    vArgvInit.push_back("--spaceType"); vArgvInit.push_back(spaceType);
    vArgvInit.push_back("--dataFile"); vArgvInit.push_back(sampleDataPrefix + dataSet);

    vArgvInit.push_back("--testSetQty"); vArgvInit.push_back(TEST_SET_QTY);
    vArgvInit.push_back("--maxNumQuery"); vArgvInit.push_back(MAX_NUM_QUERY);

    
    for (unsigned K: setKNN) {
      vector<string>          vArgv = vArgvInit;
      vector<MethodTestCase>  vTestCases; 

      vArgv.push_back("--knn");
      stringstream cmn;
      cmn << K;
      vArgv.push_back(cmn.str());

      // Select appropriate test cases
      for (const auto& testCase: vTestCaseDesc) {
        if (testCase.mDataSet == dataSet &&
            testCase.mDistType == distType && 
            testCase.mSpaceType == spaceType &&
            testCase.mKNN  == K) {
          vArgv.push_back("--method");
          vArgv.push_back(testCase.mMethodDesc);
          vTestCases.push_back(MethodTestCase(testCase));
        }
      }

      if (!vTestCases.empty())  { // Not all combinations of spaces, data sets, and search types are non-empty
        for (size_t k = 1; k <= MAX_THREAD_QTY; ++k) {
          vArgv.push_back("--threadTestQty");
          stringstream cmn;
          cmn << k;
          vArgv.push_back(cmn.str());

          nTest += vTestCases.size();
          nFail += RunOneTest(vTestCases, vArgv);

          vArgv.erase(vArgv.begin() + vArgv.size() - 1);
          vArgv.erase(vArgv.begin() + vArgv.size() - 1);
        }
      }
    }

    for (float R: setRange) {
      vector<string>          vArgv = vArgvInit;
      vector<MethodTestCase>  vTestCases; 

      vArgv.push_back("--range");
      stringstream cmn;
      cmn << R;
      vArgv.push_back(cmn.str());

      // Select appropriate test cases
      for (const auto& testCase: vTestCaseDesc) {
        if (testCase.mDataSet == dataSet &&
            testCase.mDistType == distType && 
            testCase.mSpaceType == spaceType &&
            testCase.mRange  == R) {
          vArgv.push_back("--method");
          vArgv.push_back(testCase.mMethodDesc);
          vTestCases.push_back(MethodTestCase(testCase));
        }
      }

      if (!vTestCases.empty())  { // Not all combinations of spaces, data sets, and search types are non-empty
        for (size_t k = 1; k <= MAX_THREAD_QTY; ++k) {
          vArgv.push_back("--threadTestQty");
          stringstream cmn;
          cmn << k;
          vArgv.push_back(cmn.str());

          nTest += vTestCases.size();
          nFail += RunOneTest(vTestCases, vArgv);

          vArgv.erase(vArgv.begin() + vArgv.size() - 1);
          vArgv.erase(vArgv.begin() + vArgv.size() - 1);
        }
      }

    }

  }


  timer.split();

  LOG(LIB_INFO) << "Time elapsed = " << timer.elapsed() / 1e6;
  LOG(LIB_INFO) << "Finished at " << LibGetCurrentTime();

  cerr << endl << "==================================================" << endl;
  cerr << (nFail ? "FAILURE" : "SUCCESS") << endl;
  cerr << "Carried out: " << nTest << "  tests. Failed: " << nFail << " tests" << endl;

  return nFail ? 1:0;
}
