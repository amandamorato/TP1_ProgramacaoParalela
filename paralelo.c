#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MESTRE 0
#define STD_TAG 0

typedef struct contagem_divisores{
    int valor_divisivel;
    int expoente;
    struct contagem_divisores *prox;
} contagem_divisores;

contagem_divisores* create (int valor) {
    contagem_divisores *i = malloc(sizeof(contagem_divisores));
    i->valor_divisivel = valor;
    i->expoente = 1;
    i->prox = NULL;
    return i;
}

void release(contagem_divisores *inicio) {
    contagem_divisores *i = inicio;
    contagem_divisores *prox = NULL;
    for (; i != NULL; i = prox) {
        prox = i->prox;
        free(i);
    }
}
int qtd_Divisores_dinamico(int num){
    int count = 1, divisor = 2, aux = 1;
    contagem_divisores *inicio = NULL;
    contagem_divisores *i = NULL;
    contagem_divisores *prox = NULL;
    int verificaprimo = num;

    while (num > 1) {
        if ((num % divisor) == 0) {
            num = num/divisor;
            if (aux == 1) {
                prox = create(divisor);
                if (inicio == NULL) {
                    inicio = prox;
                }
                if (i != NULL) {
                    i->prox = prox;
                }
                i = prox;
                aux = 0;
            }
            else { 
                i->expoente++;
                aux = 0;
            }                       
        }
        else {
            divisor++;
            aux = 1;
        }
    }
    
    for (contagem_divisores *j = inicio; j != NULL; j = j->prox) {
        count = count * (j->expoente + 1);
    }
    release(inicio);
    return count;
}

int qtd_Divisores(int num){
    int count = 1;
    for (int i = 1; i <= (num/2); i++) {
        if ((num % i) == 0) {
            count++;
        }
    }
    return count;
}

//-----------------------------------------------

int main(int argc, char *argv[]) {
    double t0, t1; //marcador de tempo
    int process_rank, size_of_cluster;
    
    MPI_Init(&argc, &argv); // função que inicializa o MPI
    MPI_Comm_size(MPI_COMM_WORLD, &size_of_cluster); //numero de processos (quantidade total)
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank); // numero do processo atual
    MPI_Status status;

    int num_elementos_global = 1440;
    int num_elementos_local = 1440/size_of_cluster;
    int nums_global[num_elementos_global];
    int nums_local[num_elementos_local];

    if (process_rank == MESTRE){
        //Abertura do arquivo de entrada.
        FILE *f_in;
        if (!(f_in = fopen("entrada.txt", "r"))) {
            fprintf(stderr, "Não foi possível abrir o arquivo de entrada.\n");
            return 2;
        }

        //Leitura do arquivo de entrada.
        for (int i = 0; i < num_elementos_global; i++) {
            fscanf(f_in, "%d", &nums_global[i]);
        }
        
    }
    t0 = MPI_Wtime();
    MPI_Scatter(nums_global, num_elementos_local, MPI_INT, &nums_local, num_elementos_local, MPI_INT, MESTRE, MPI_COMM_WORLD);

    int divisores_local[num_elementos_local];
    for (int i = 0; i < num_elementos_local; i++){
        divisores_local[i] = qtd_Divisores(nums_local[i]);
    }

    MPI_Gather(&divisores_local, num_elementos_local, MPI_INT, nums_global, num_elementos_local, MPI_INT, MESTRE, MPI_COMM_WORLD);    
    t1 = MPI_Wtime();
    //Salvando resultados no arquivo
    if (process_rank == MESTRE) {

        //Abertura do arquivo de saída.
        FILE *f_out;
        if (!(f_out = fopen("saida.txt", "w"))) {
            fprintf(stderr, "Não foi possível criar/abrir o arquivo de entrada.\n");
            return 3;
        }
        //Salvando os resultados no arquivo de saída
        for (int i=0; i<num_elementos_global; i++){    
            fprintf(f_out, "%d\n", nums_global[i]);
        }  
    }
    
    MPI_Finalize();
    printf("\nTime %f\n", t1 - t0);
    return 0;
}