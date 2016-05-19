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

enter src directory and run cmake .. and then make

##Build and Search



Documentation
-------------

Check FLANN web page [here](http://www.cs.ubc.ca/~mariusm/flann).

Documentation on how to use the library can be found in the doc/manual.pdf file included in the release archives.

More information and experimental results can be found in the following paper:

  * Marius Muja and David G. Lowe, "Fast Approximate Nearest Neighbors with Automatic Algorithm Configuration", in International Conference on Computer Vision Theory and Applications (VISAPP'09), 2009 [(PDF)](http://people.cs.ubc.ca/~mariusm/uploads/FLANN/flann_visapp09.pdf) [(BibTex)](http://people.cs.ubc.ca/~mariusm/index.php/FLANN/BibTex)


Getting FLANN
-------------

The latest version of FLANN can be downloaded from here:

 *  Version 1.8.4 (15 January 2013)
    [flann-1.8.4-src.zip](http://people.cs.ubc.ca/~mariusm/uploads/FLANN/flann-1.8.4-src.zip) (Source code)  
    [User manual](http://people.cs.ubc.ca/~mariusm/uploads/FLANN/flann_manual-1.8.4.pdf)  
    [Changelog](https://github.com/mariusmuja/flann/blob/master/ChangeLog)  

If you want to try out the latest changes or contribute to FLANN, then it's recommended that you checkout the git source repository: `git clone git://github.com/mariusmuja/flann.git`

If you just want to browse the repository, you can do so by going [here](https://github.com/mariusmuja/flann).


Conditions of use
-----------------

FLANN is distributed under the terms of the [BSD License](https://github.com/mariusmuja/flann/blob/master/COPYING).

Bug reporting
-------------

Please report bugs or feature requests using [github's issue tracker](http://github.com/mariusmuja/flann/issues).
