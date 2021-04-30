#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

#define NO_OF_POINTS 100

int main(int argc, char** argv)
{
    int rank, no_proc, n = NO_OF_POINTS;
    double h = 1.0 / n, k = 1.0 / n, c = 1;
    int tag = 0;

    MPI_Status stat;
    MPI_Init(&argc, &argv);
    
    // finding rank of process
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    // finding number of processors
    MPI_Comm_size(MPI_COMM_WORLD, &no_proc);

    int n_per_proc = n/no_proc;
    
    int num = n_per_proc + 1;
    if(rank == no_proc - 1) {
        num = n - n_per_proc * (no_proc - 1) + 1;
    }

    cout << "rank : " << rank << " n_per_proc : " << n_per_proc << endl;

    double *u = new double[num];
    double *u_old = new double[num];
    double *u_old2 = new double[num];

    for (int i = 0; i < num; i++) {
        u_old2[i] = pow(((double)((rank * n_per_proc + i) * h)), 2);
        u_old[i] = u_old2[i];
    }

    for (int t = 1; t <= n; t++)
    {
        for (int i = 1; i < n_per_proc; i++) {
           u[i] = 2 * u_old[i] - u_old2[i] + ((k*k*c*c) / (h*h)) * (u_old[i+1] - 2 * u_old[i] + u_old[i-1]);
        }

        memcpy(u_old2,u_old,num*sizeof(double));
        memcpy(&u_old[1],&u[1],(num-2)*sizeof(double));
       
        // Sending terminal values in segment 
        if(rank!=0)
            MPI_Send(&u_old[1], 1, MPI_DOUBLE, rank - 1, tag, MPI_COMM_WORLD);
        
        if(rank!=no_proc-1)
            MPI_Send(&u_old[num-2], 1, MPI_DOUBLE, rank + 1, tag, MPI_COMM_WORLD);
       
       // Receiving left terminal values or setting left Boundary Conditions 
        if(rank!=0)
            MPI_Recv(&u_old[0], 1, MPI_DOUBLE, rank - 1, tag, MPI_COMM_WORLD, &stat);
        else
            u_old[0] = pow(((double)(t*k)),2);
      
       // Receiving right terminal values or setting right Boundary Conditions 
        if(rank!=no_proc-1)
            MPI_Recv(&u_old[num-1], 1, MPI_DOUBLE, rank + 1, tag, MPI_COMM_WORLD, &stat);
        else
            u_old[num-1] = 1 + pow(((double)(t*k)),2);
        
    }

    cout << "rank final = " << rank << "\t";
    for(int i = 0; i <= 5; i++) {
        cout << "u__old[" << rank * n_per_proc + i * (num - 1) / 5 << "] = " << u_old[i*(num-1)/5] << "\t";
    }
    cout << endl;

    delete u_old2;
    delete u_old;
    delete u;

    MPI_Finalize();
}
