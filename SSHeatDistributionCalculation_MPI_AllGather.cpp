/*
Description:
Lab6 - OpenMPI program to determine the steady state heat distribution in a thin metal plate using synchronous iteration in a distributed system
*/


#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include<fstream>
#include <unistd.h> //getopt
#include <stdlib.h>  //atoi
#include<iomanip> //, precision
#include<cmath> //round
#include <chrono>
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
    int numInteriorPoints = 0;
    int numIterations = 0;
    int opt;

    while ((opt = getopt(argc, argv, "n:I:")) != -1)     // take inputs from command line arguments
    {
        switch (opt)
        {
        case 'n':
            numInteriorPoints = atoi(optarg);
            break;
        case 'I':
            numIterations = atoi(optarg);
            break;
        default:
            printf("ERROR: Usage: [-n numInteriorPoints -I numIterations");
            return 1; //exit program
        }
    }

    if (numInteriorPoints <= 0 || numIterations <= 0)
    {
        printf("ERROR: numInteriorPoints and numIterations cannot be <= 0\n");
        return 1;
    }

    int width = numInteriorPoints + 2;
    int rank, size, r, c, itr;
    int index, up, down, left, right;
    
    MPI_Init(&argc, &argv); // initialize MPI execution environment
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // get rank/task ID   
    MPI_Comm_size(MPI_COMM_WORLD, &size); // get total number of ranks/tasks in communicator
    
        
    // measure starttime ----------------------------------------------------------------------------------------------------------------------------------------------------

    double starttime, endtime;
    if(rank == 0)
    {
        starttime = MPI_Wtime();
    }
	
	// allocate oversized arrays so we can send the same amount of data to each processor
	int newWidth = size*(int)std::ceil((double)width/(double)size);
	int sendCount = newWidth*newWidth/size;
	int recvCount = sendCount;
	double* hPrevious = new double[newWidth*newWidth];
	double* hNext = new double[newWidth*newWidth];
	
	// determine num of rows that each processor gets
	int blockSize = newWidth/size;
	
	 //cout<<"before initialize" <<endl;
	 
    // initialize hPrevious and hNext with temperatures at boundaries as given in the problem statement
    
    for (c = 0; c< width; c++) //top 
    {
        if (c <= round(width * 0.3) || c >= round(width * 0.7))
        {
            hPrevious[0 * width + c] = 20.0;
            hNext[0 * width + c]= 20.0;
        }
        else
        {
            hPrevious[0 * width + c] = 100.0;
            hNext[0 * width + c]= 100.0;
        }
    }

    for (c = 0; c < width; c++) //bottom
    {
        hPrevious[((width - 1) * newWidth) + c] = 20.0;
        hNext[((width - 1) * newWidth) + c] = 20.0;
    }

    for (r = 0; r < width; r++) //left
    {
        hPrevious[((r)*newWidth) + 0] = 20.0;
        hNext[((r)*newWidth) + 0] = 20.0;
    }

    for (r = 1; r < width; r++) //right
    {
        hPrevious[(r*newWidth) + (width - 1)] = 20.0;
        hNext[(r*newWidth) + (width - 1)] = 20.0;
    }

    for (r = 1; r < width - 1; r++)   // initialise temp at interior points to zero
    {
        for (c = 1; c < width - 1; c++)
        {
            hPrevious[r * newWidth + c] = 0;
            hNext[r * newWidth + c] = 0;
        }
    }
    
    //cout<<"after initialize" <<endl;

     
    int startHere = rank*blockSize;
    
    for (itr = 0; itr<numIterations; itr++)
    {
        for(r = startHere; r<(startHere + blockSize); r++)
        {
            if(r==0 || r >= width-1)
                continue;
                
            for(c=0; c<width;c++)
            {
                if(c==0 || c>=width-1) //skip the first and last rows
                continue;
               
                index = r*newWidth + c; // (row number*length of row) + column number
                left = r*newWidth + (c-1);
                right = r*newWidth + (c+1);
                up = (r-1)*newWidth + c;
                down = (r+1)*newWidth + c;
                
                hNext[index] = 0.25*(hPrevious[up] + hPrevious[down] + hPrevious[left] + hPrevious[right]);
                
            }
        }
        
        //cout<<"after calc before gather, task id: " << rank << endl;
        
        MPI_Allgather(&hNext[startHere*newWidth],sendCount,MPI_DOUBLE,hPrevious, recvCount, MPI_DOUBLE, MPI_COMM_WORLD);
        
    }
    
    // measure endtime --------------------------------------------------------------------------------------------------------------------------------------------------

    if (rank == 0) // write out result
    {
        endtime = MPI_Wtime(); //in seconds
        printf("Thin plate calculation took:  %3.1f milliseconds\n",(endtime-starttime)*1000);
        
        std::ofstream myOutFile;
        myOutFile.open("finalTemperatures.csv"); // output to .csv file
        
        for (int r = 0; r < width; r++)
        {
            for (int c = 0; c < width; c++)
            {
                myOutFile << std::fixed << std::setprecision(5) << hPrevious[r *newWidth + c];
                if (c<width-1)
                myOutFile << ",";
            }
            myOutFile << "\n";
        }
        myOutFile.close();
    }
    
    MPI_Finalize(); // terminate MPI execution environment
    return 0;

}
