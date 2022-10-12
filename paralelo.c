#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MESTRE 0
#define ROOT 0
#define STD_TAG 0

int main(int argc, char *argv[]) {


    MPI_Init(int* argc, char *argv[]);

    int process_rank, size_of_cluster, linhas = 0;

    MPI_Comm_group(MIP_COMM_WORLD, &world_group); //group?

    MPI_Comm_Size(MIP_COMM_WORLD, &size_of_cluster);

    MPI_Comm_Rank(MIP_COMM_WORLD, &process_rank);

    MPI_Status status;

    if (process_rank == ROOT){
        //Abertura do arquivo de entrada.
        FILE *f_in;
        if (!(f_in = fopen("entrada.txt", "r"))) {
            fprintf(stderr, "Não foi possível abrir o arquivo de entrada.\n");
            return 2;
        }

        //Abertura do arquivo de saída.
        FILE *f_out;
        if (!(f_out = fopen("saida.txt", "w"))) {
            fprintf(stderr, "Não foi possível criar/abrir o arquivo de entrada.\n");
            return 3;
        }

        //Contagem de linhas do arquivo.
        int num;
        while(fscanf(f_in, "%i", &num) != EOF) {
            linhas++;      
        }

        int nums[linhas];
        rewind(f_in);
        //Leitura do arquivo de entrada.
        for (int i = 0; i < linhas; i++) {
            fscanf(f_in, "%d", &nums[i]);
        }

        /*
        //enviar número de linhas para calcular numElements ?
        for(i = 1; i < size_of_cluster; i++){
            MPI_Send(&linhas, 1, MPI_INT, i, STD_TAG, MPI_COMM_WORLD);    
        }
        */
    }

    /*
    if (process_rank != MESTRE) {
        //receber número de linhas para calcular numElements ?
        MPI_Recv(&linhas, 1, MPI_INT, ROOT, STD_TAG, MPI_COMM_WORLD, &status);
    }
    */

    MPI_Bcast(&linhas, 1, MPI_INT, ROOT, MPI_COMM_WORLD);

    int numElements = linhas/size_of_cluster;

    //starting timing
    MPI_Barrier(MPI_COMM_WORLD);
    total_time -= MPI_Wtime();

    int *recvbuff = malloc(sizeof(int) * numElements);

    //scatterv and gatherv -> numElements%size_of_cluster != 0

    MPI_Scatter(&nums, numElements, MPI_INT, recvbuff, int recvcount, MPI_INT, MESTRE, MPI_COMM_WORLD); // recvcount == numElements

    //int qtd_divisores[numElements];
    int *qtd_divisores = malloc(sizeof(int) * numElements);

    if (process_rank != MESTRE){
        for (int i = 0; i < numElements; i++){
            qtd_divisores[i] = qtd_Divisores_dinamico(recvbuff[i]);
            printf("%d - %d\n", nums[i], qtd_divisores[i]);
        }    
    }

    int *div_processados = NULL;

    if (process_rank == MESTRE){
        div_processados = malloc(sizeof(int) * numElements * size_of_cluster);
    }

    MPI_Gather(&qtd_divisores, numElements, MPI_INT, div_processados, numElements, MPI_INT, MESTRE, MPI_COMM_WORLD);

    total_time += MPI_Wtime();

    if (process_rank == MESTRE) {
        for (int i=0; i<linhas; i++){    
            fprintf(f_out, "%i\n", div_processados[i]);
        }  
    }


    MPI_Finalize();
    return 0;
}