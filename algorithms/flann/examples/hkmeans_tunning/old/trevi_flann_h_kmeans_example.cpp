
#include <flann/flann.hpp>
#include <flann/io/hdf5.h>


#include <fstream>
#include <stdio.h>


using namespace flann;
using namespace std;
using std::endl;
using std::ofstream;

float compute_precision(const flann::Matrix<int>& match, const flann::Matrix<int>& indices)
{
	int count = 0;

	assert(match.rows == indices.rows);
	size_t nn = std::min(match.cols, indices.cols);

	for (size_t i=0; i<match.rows; ++i) {
		for (size_t j=0;j<nn;++j) {
			for (size_t k=0;k<nn;++k) {
				if (match[i][j]==indices[i][k]) {
					count ++;
				}
			}
		}
	}

	return float(count)/(nn*match.rows);
}

float compute_recall_at(const size_t nn, const flann::Matrix<int>& match, const flann::Matrix<int>& indices)
{
	int count = 0;

	assert(match.rows == indices.rows);
	//size_t nn = match.cols; //std::min(match.cols, indices.cols);

	for (size_t i=0; i<match.rows; ++i) 
	{
		for (size_t j=0;j<nn;++j) 
		{
			if (match[i][j]==indices[i][0]) 
			{
				count ++;
				break;
			}
		}
	}

	return float(count)/(match.rows);
}


int main(int argc, char** argv)
{
    int nn = 100;

    Matrix<float> dataset;
    Matrix<float> query;
    Matrix<int> groundtruth;

    load_from_file(dataset, "/home/liwen/Downloads/Dataset/Trevi/data/dataset.hdf5","dataset");
    load_from_file(query, "/home/liwen/Downloads/Dataset/Trevi/data/dataset.hdf5","query");
    load_from_file(groundtruth,"/home/liwen/Downloads/Dataset/Trevi/data/dataset.hdf5","groundtruth");
    
    int branches[5]={10, 20,32,64,128};
	for(int b=0;b<5;b++)
	{
    ofstream out;
	char result_path[100];
	sprintf(result_path,"/home/liwen/Downloads/Dataset/Trevi/exp_fig/result/Flann/flann_HKMeans_%dbranches.txt",branches[b]);
    out.open(result_path);

    //build index
    Index<L2<float> > index(dataset, flann::KMeansIndexParams(branches[b],10,FLANN_CENTERS_RANDOM ,0.4));
    clock_t start_time_=clock();
    index.buildIndex();
    double index_time = double(clock()-start_time_)/CLOCKS_PER_SEC;
    out<<"#Algorithm = H_KMeans  Centroids_count ="<<branches[b]<<" iterations=10 cb_index = 0.4"<<endl;
    out<< index_time <<" #index_time "<<endl;
            
	char index_path[100];
	sprintf(index_path, "hkmeans/trevi_HKMeans_indices_%dbranches",branches[b]);
                                                                              
    index.save(index_path); 

    // do a knn search, using 128 checks
    int checks[15]={100,200,300,500,700,1000,1500,2000,3000,5000,7000,10000,15000,20000,30000}; 
    for(int c=0;c<15;c++)
    {
    std::cout<<c<<endl; 
    Matrix<int> indices(new int[query.rows*nn], query.rows, nn);
    Matrix<float> dists(new float[query.rows*nn], query.rows, nn);

    start_time_=clock();
    index.knnSearch(query, indices, dists, nn, flann::SearchParams(checks[c]));
    double search_time = double(clock()-start_time_)/CLOCKS_PER_SEC;

    //compute the recall 
    //std::ofstream out("result.txt");
    //double recall[5]={0.0,0.0,0.0,0.0,0.0};
    float precision = compute_precision(indices,groundtruth);

    //recall[0]= compute_recall_at(1,indices,groundtruth);
    //recall[1]= compute_recall_at(10,indices,groundtruth);
    //recall[2]= compute_recall_at(100,indices,groundtruth);
    //recall[3]= compute_recall_at(1000,indices,groundtruth);
    //recall[4]= compute_recall_at(10000,indices,groundtruth);
	out.setf(ios::fixed);
	out<< precision <<" "<< search_time/query.rows <<" #N_"<< checks[c] <<" "<<endl;
    //out<<"#checks = "<< checks[c] <<endl;
    //std::cout<<"#result_count = "<< indices.rows <<" cols :"<< indices.cols << endl;
    //out<<"#precision = "<< precision <<endl; 
    //out<<"#recall@1 = "<< recall[0] <<endl; 
    //out<<"#recall@10 = "<< recall[1] <<endl; 
    //out<<"#recall@100 = "<< recall[2] <<endl; 
    //out<<"#recall@1000 = "<< recall[3] <<endl; 
    //out<<"#recall@10000 = "<< recall[4] <<endl;
    //out<< "\n\n" <<endl;
    
    delete[] indices.ptr();
    delete[] dists.ptr();
    }
    }

    delete[] dataset.ptr();
    delete[] query.ptr();
    
  
    
    return 0;
}


