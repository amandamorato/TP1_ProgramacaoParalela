#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int process_Rank, size_Of_Cluster;
    MPI_Comm_group(MPI_COMM_WORLD, &world_group);
    
    MPI_Init(int* argc, char *argv[]);
    if (process_Rank == 0){
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
        int linhas = 0, num;
        while(fscanf(f_in, "%i", &num) != EOF) {
            linhas++;      
        }

        int nums[linhas];
        rewind(f_in);
        //Leitura do arquivo de entrada.
        for (int i = 0; i < linhas; i++) {
            fscanf(f_in, "%d", &nums[i]);
        }
    }
    MPI_Scatter(&nums, linhas/2, int, void *recvbuff, int recvcount, MPI_Datatype recvtype, 0, MPI_Comm comm)
    MPI_Finalize();
    return 0;
}