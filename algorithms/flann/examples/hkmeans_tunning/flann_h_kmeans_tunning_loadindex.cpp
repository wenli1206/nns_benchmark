
#include <flann/flann.hpp>
#include <flann/io/hdf5.h>
#include <getopt.h>
#include <sys/time.h>
#include <fstream>
#include <stdio.h>


using namespace flann;
using namespace std;
using std::endl;
using std::ofstream;


float get_distance(float* point, float* query, int dim)
{
	float dist=0.0;
	for(int i=0;i<dim;i++)
	{
		float diff = (*point++ - *query++);
		dist += diff*diff;
	}
	return dist;
}

float compute_recall(int k,const flann::Matrix<int>& results, const flann::Matrix<int>& gnds)
{
	int count = 0;
	for (int i=0; i<results.rows; ++i) {
		int re_n=0;
		int gs_n=0;
		while(re_n<k && gs_n<k)
		{
			if(results[i][re_n] == gnds[i][gs_n])
			{
				count++;
				re_n++;
				gs_n++;
			}
			else
				gs_n++;
		}
	}
	return float(count)/(k*results.rows);
}

float compute_relative_distance_error(int k, const flann::Matrix<float>& dataset, const flann::Matrix<float>& query, const flann::Matrix<float>& dists, const flann::Matrix<int>& gnds) 
{
	float avg = 0.0;
    for (int i = 0; i <dists.rows; ++i) //dists.rows
	{
		float dist = 0.0;
        for(int j=0; j<k; j++)
		{
			float d_qr = dists[i][j];   //query.cols
			float d_qg = get_distance(dataset[gnds[i][j]],query[i],query.cols);
            float d=(d_qr - d_qg)/d_qg;
			if(d<=4)
				dist += d;
			else
				dist += 4; 
		}
		avg += dist;
    } 
	return avg/k/dists.rows;     
}

float compute_number_closer (int k,const flann::Matrix<int>& results, const flann::Matrix<int>& gnds)
{        
	float sum = 0;
    for (int i = 0; i < results.rows; ++i) {
		float rate = 0.0;
		for (int gs_n=0;gs_n < k ;gs_n++)
		{
			for(int re_n=0; re_n < k; re_n++)
			{
				if(gnds[i][gs_n]==results[i][re_n])
				{
					rate += (float)(gs_n+1)/(re_n+1);
					break;
				}
			}
		}
        sum += rate/k;
    }
    return sum / results.rows;
}

float compute_mean_reciprocal_rank (int k,const flann::Matrix<int>& results, const flann::Matrix<int>& gnds)
{
	float sum = 0;
    for (int i = 0; i < results.rows; ++i) {
		float rate = 0.0;
		for(int re_n=0; re_n < k; re_n++)
		{
			if(gnds[i][0]==results[i][re_n])
			{
				rate += 1.0/(re_n+1);
				break;
			}
		}
        sum += rate;
    }
    return sum / results.rows;
}

float compute_mean_average_precision (int k,const flann::Matrix<int>& results, const flann::Matrix<int>& gnds)
{
     float sum = 0;
     for (int i = 0; i < results.rows; ++i) {
			float rate = 0.0;
			unsigned re_n = 0;
			int count = 0;
			for(unsigned gs_n=0; gs_n < k; gs_n++)
			{
				if(gnds[i][gs_n]==results[i][re_n])
				{
					count++;
					rate += 1.0*(count)/(gs_n+1);
					re_n ++;
				}
			}
            sum += rate/k;
        }
        return sum / results.rows;
    }

float compute_discounted_culmulative_gain(int k,const flann::Matrix<int>& results, const flann::Matrix<int>& gnds)
{
    float sum = 0;
    for (unsigned i = 0; i < results.rows; ++i) {
        
		float rate = 0.0;
		unsigned re_n = 0;
		for(unsigned gs_n=0; gs_n <k; gs_n++)
		{
			if(gnds[i][gs_n]==results[i][re_n])
			{
				rate += 1.0/log2(re_n+2);
				re_n ++;
			}
		}
        sum += rate;
    }
    return sum / results.rows;
}

float diff_timeval(timeval t1, timeval t2) {
  return (float) (t1.tv_sec - t2.tv_sec) + (t1.tv_usec - t2.tv_usec) * 1e-6;
}

int main(int argc, char** argv)
{

	const struct option longopts[] ={
    {"help",                        no_argument,       0, 'h'},
    {"file_path",                   required_argument, 0, 'f'},
    {"data_name",                   required_argument, 0, 'd'},
    {"branches",                    required_argument, 0, 'b'},
    {"nn",                          required_argument, 0, 'k'},
  };
  	int id;
  	int iarg = 0;
	char file_path[20];
	char data_name[10];
	int branches;
	int nn;

	while (iarg != -1) {
    iarg = getopt_long(argc, argv, "f:d:k:b:h",longopts, &id);

    switch (iarg) {
      case 'f':
        if (optarg) {
			strcpy(file_path, optarg);
        }
        break;
      case 'd':
        if (optarg) {
			strcpy(data_name, optarg);
        }
        break;
      case 'k':
        if (optarg) {
			nn = atoi(optarg);
        }
        break;
      case 'b':
        if (optarg) {
          branches = atoi(optarg);
        }
        break;
    }
	}

    Matrix<float> dataset;
    Matrix<float> query;
    Matrix<int> groundtruth;

	char data_path[100];
	sprintf(data_path,"/home/liwen/Downloads/Dataset/%s/data/dataset.hdf5",file_path);
    load_from_file(dataset, data_path,"dataset");
    load_from_file(query, data_path,"query");
    load_from_file(groundtruth,data_path,"groundtruth");
    
    ofstream out;
	char result_path[100];
	sprintf(result_path,"/home/liwen/Downloads/Dataset/%s/exp_fig/result/HKMeans/k=%d/flann_HKMeans_%dbranches.txt",file_path,nn,branches);
    out.open(result_path);

	char index_path[100];
	sprintf(index_path, "/home/liwen/Documents/LSH/flann-1.8.4-src/build/bin/hkmeans/%s_HKMeans_indices_%dbranches_%dk",data_name,branches,10);
	Index<L2<float> > index(dataset,flann::SavedIndexParams(index_path)) ; 

    // do a knn search, using 128 chec
    int checks[15]={100,200,300,500,700,1000,1500,2000,3000,5000,7000,10000,15000,20000,30000}; 
    for(int c=0;c<15;c++)
    {
    	//std::cout<<c<<endl; 
    	Matrix<int> indices(new int[query.rows*nn], query.rows, nn);
    	Matrix<float> dists(new float[query.rows*nn], query.rows, nn);

    	//double start_time_=clock();
		timeval start;
        gettimeofday(&start, NULL);

    	index.knnSearch(query, indices, dists, nn, flann::SearchParams(checks[c]));
		timeval end;
        gettimeofday(&end, NULL);
        double search_time = diff_timeval(end, start);

    	//double search_time = double(clock()-start_time_)/CLOCKS_PER_SEC;

    	//compute the recall
    
		float recall = compute_recall(nn,indices,groundtruth);
		float rde = compute_relative_distance_error(nn,dataset,query,dists,groundtruth);
		//float mrr = compute_mean_reciprocal_rank(nn,indices,groundtruth);
		//float nc = compute_number_closer(nn,indices,groundtruth);
		//float map = compute_mean_average_precision(nn,indices,groundtruth);
		//float dcg = compute_discounted_culmulative_gain(nn,indices,groundtruth);
    
		out.setf(ios::fixed);
		//out << recall << " " << rde << " " << mrr << " " << nc << " " << map << " " << dcg<< " "<< search_time/query.rows <<" #N_"<< checks[c] <<" "<<endl;
		out << recall << " " << rde << " "<< search_time/query.rows <<" #N_"<< checks[c] <<" "<<endl;

    	delete[] indices.ptr();
    	delete[] dists.ptr();
    }

    delete[] dataset.ptr();
    delete[] query.ptr();

    return 0;
}


