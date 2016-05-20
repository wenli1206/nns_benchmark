FLANN - Fast Library for Approximate Nearest Neighbors
======================================================

FLANN is a library for performing fast approximate nearest neighbor searches in high dimensional spaces. This is a modified version of Flann. The original version can be found form  [here](http://www.cs.ubc.ca/~mariusm/flann). FLANN is written in C++ and contains bindings for the following languages: C, MATLAB and Python. We only evaluate the C++ results.

Main differences:

We disabled the improved version of the distance computation and only use a single thread.

##Prerequisites:

- A modern compiler that supports C++11 and G++ 4.7
- Linux System 
- HDF5 should be installed. 
- [Sample data](https://github.com/DBWangGroupUNSW/nns_benchmark/tree/master/data) (e.g., audio) is downloaded, including data points, query points and ground truth results. Note that the input format is .hdf5 and all data files downloaded should be put to the knn_benchmark/data directory.

##Compile 

enter code/build directory and run 

cmake .. 

and then

make

##Build index

build_hkmeans.sh, build_kdtree.sh, build_auto.sh

Build Hierarchical KMeans tree, Randomized KDTree and the Flann index which is selected automatical.  

##Search 

run_hkmeans.sh, run_kdtree.sh, run_auto.sh 

Note that, in our experiment paper, we use N value (retrived number of the data points) to achieve the trade-off between search speed and search quality (recall). The search performance (time and recall) results are kept in the Flann/results directory.
