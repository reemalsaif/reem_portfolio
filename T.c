#include<stdio.h>
#include<mpi.h>
 #include<math.h>
#include<stdlib.h>
#include <time.h>

int main( int argc, char **argv){
int rank,size ; 
MPI_Init(&argc,&argv);
int sizear;
int send_data_tag =0 ; 
MPI_Status status ; 
int recv ; 
int pstart , pend,div ; 
double *array1 =NULL;
double total1 =0.0;
double *array2 =NULL;
int rowstosend ; 
MPI_Comm_rank(MPI_COMM_WORLD, &rank); // this store the id of each p
MPI_Comm_size(MPI_COMM_WORLD, &size); //here are the total p's 
 srand(time(NULL)+rank);
    //random numbers
    
    double tic = MPI_Wtime();
//DISTRIBUTE THE WORK !
if(rank == 0 ){

printf("enter the array size : \n ");
scanf("%d" , &sizear);

printf("the array size is %d \n",sizear);
 div = floor(sizear/size) ; 
 
}

MPI_Bcast(&sizear,1,MPI_INT,0,MPI_COMM_WORLD);

MPI_Bcast(&div,1,MPI_INT,0,MPI_COMM_WORLD);

 
 
 pstart = rank* div ; //here is the starting indx for each p 

 if( rank == size-1){//last process does the remaining elements 
 pend = sizear ; }else{pend = pstart + div ; }
  rowstosend = pend-pstart; 




 array1 = (double *)malloc(sizeof(double)*sizear);
 array2 = (double *)malloc(sizeof(double)*sizear);

if(rank ==0){


for(int i = 0 ; i < sizear ; i++)
{
 
  array1[i]= rand()%50 + 1;
   array2[i]= rand()%50 + 1;
}

 }

 MPI_Scatter(array1,rowstosend,MPI_DOUBLE,&array1[pstart],rowstosend,MPI_DOUBLE,0,MPI_COMM_WORLD);

MPI_Scatter(array2,rowstosend,MPI_DOUBLE,&array2[pstart],rowstosend,MPI_DOUBLE,0,MPI_COMM_WORLD);



 
 double sum1=0;


for(int i=pstart ;i<pend ; i++){
 
 

  //  printf("The process %d  executes i = %d\n", rank, i);
        sum1 += pow(array1[i] -array2[i], 2);


}







MPI_Reduce( &sum1,&total1,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);





if(rank ==0){
printf("final sum (not root) = %f \n", total1);


   double final_euclid  = sqrt(total1);

 
        printf("Final sum(square root) = %f\n", final_euclid);
           double toc = MPI_Wtime();
        printf("Elapsed time: %f seconds\n",(toc - tic) );
}
 



free(array1);
free(array2);




 

MPI_Finalize(); //ends mpi env 

return 0;

}

 



