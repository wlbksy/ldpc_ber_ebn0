#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <ctime>
#include "GSL_rng.h"
#include "GSL_randist.h"
#include "BP.h"


using namespace std;

int main(int argc, char * argv[])
{
	int numFrame;
	double minEb_N0, stepEb_N0, maxEb_N0;
	string MatrixFile, resultName;
	int numIter;
	
	if (argc!=8)
	{
		cout<<"**************************************************************************"<<endl;
		cout<<"usage : "<<argv[0]<<" numFrame minEb_N0 stepEb_N0 maxEb_N0 numIter MatrixFile resultName" <<endl;
		cout<<argv[0]<<" 1 0 1 0 10 h.txt 1.txt" <<endl;
		return 0;
	}
	else
	{
		numFrame=atof(argv[1]);
		minEb_N0=atof(argv[2]);
		stepEb_N0=atof(argv[3]);
		maxEb_N0=atof(argv[4]);
		numIter=atof(argv[5]);
		MatrixFile=argv[6];
		resultName=argv[7];
	}
	
	ifstream infn(MatrixFile.c_str());
	if (!infn) 
	{
		cout <<"can not open "<< MatrixFile << endl; 
		exit(-1); 
	} 
	int M,N,size;
	infn >>M>>N>>size;
	
    int K = N-M;

	int HI[size],HJ[size];
	for(int i=0;i<size;i++)
		infn >>HI[i];
	for(int i=0;i<size;i++)
		infn >>HJ[i];
	
	infn.close();
	
	double R=(double)K/N;
	
	gsl_rng_default_seed = ((unsigned long)(time(NULL))); 
	gsl_rng *r = gsl_rng_alloc (gsl_rng_mt19937);
	
	double Y[N],f1[N];
	int xhat[N];
	
	int Eb_N0range=(int)((maxEb_N0-minEb_N0)/stepEb_N0);  
	double ber[Eb_N0range];
	double bler[Eb_N0range];
	
	ofstream codefile;
	clock_t start_time, end_time;
	start_time=clock();
	
	for(int i=0;i<=Eb_N0range;i++)
	{
		double sigma2=pow(10,(-(minEb_N0+stepEb_N0*i)/10))/2/R;
		double sigma=sqrt(sigma2);
		int error_bits=0;
		int error_frame=0;

		#pragma omp parallel for
		for(int j=0;j<numFrame;j++)
		{
			cout<<"Eb/N0:"<<minEb_N0+stepEb_N0*i<<"\t"<<"counts:"<<numFrame<<" -> "<<j<<endl;
			
			for(int k=0;k<N;k++)
			{							
				Y[k]= -1 + gsl_ran_gaussian (r, sigma);//sigma * randn(1,N);		//AWGN Channel
				
				f1[k]=1./(1+exp(-2*Y[k]/sigma2));        //likelihoods
			}
            
			BP(M, N, size, f1, HI, HJ, numIter, xhat);
            
            int last = error_bits;
            
			for(int k=0;k<K;k++)
				error_bits+=xhat[k];
                
            if(error_bits!=last)
                error_frame+=1;
		}
		
		ber[i]=(double)error_bits/K/numFrame;
		bler[i]=(double)error_frame/numFrame;
		
		if(0==i)
			codefile.open(resultName.c_str(),ios::out);
		else
			codefile.open(resultName.c_str(),ios::app);
		
		codefile.unsetf( ios::scientific);
		codefile<<minEb_N0+stepEb_N0*i<<" ";
		codefile.setf( ios::scientific);
		codefile<<ber[i]<<" "<<bler[i]<<" ;";
		codefile.close();
	}
	
	end_time=clock();
	
	gsl_rng_free (r);
	
	cout<<"time : "<<(double)(end_time-start_time)/CLOCKS_PER_SEC<<"(s)"<<endl;
	return 0;
}
