#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>

#define MESTRE 0
#define STD_TAG 0
#define DEPTH 6

typedef struct contagem_divisores{
    int valor_divisivel;
    int expoente;
    struct contagem_divisores *prox;
} contagem_divisores;

//Função que cria um nó sempre que aparece um novo primo divisor.
contagem_divisores* Create (int valor) {
    contagem_divisores *i = malloc(sizeof(contagem_divisores));
    i->valor_divisivel = valor;
    i->expoente = 1;
    i->prox = NULL;
    return i;
}

//Função que libera a memória alocada.
void Release(contagem_divisores *inicio) {
    contagem_divisores *i = inicio;
    contagem_divisores *prox = NULL;
    for (; i != NULL; i = prox) {
        prox = i->prox;
        free(i);
    }
}

//Função que realiza a fatoração do número e calcula o produto dos expoentes destes primos adicionados em 1.
int Calcula_Divisores_Dinamico(int num){
    int count = 1, divisor = 2, aux = 1;
    contagem_divisores *inicio = NULL;
    contagem_divisores *i = NULL;
    contagem_divisores *prox = NULL;
    int verificaprimo = num;

    while (num > 1) {
        if ((num % divisor) == 0) {
            num = num/divisor;
            if (aux == 1) {
                //Cria um nó para cada valor primo divisor.
                prox = Create(divisor);
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
    //Calcula o produto dos expoentes destes primos adicionados em 1.
    for (contagem_divisores *j = inicio; j != NULL; j = j->prox) {
        count = count * (j->expoente + 1);
    }
    //Libera a memória alocada.
    Release(inicio);
    return count;
}

//Função que calcula os divisores.
int Calcula_Divisores(int num, int process_rank){
    int count = 1;
    int tid;
    #pragma omp parallel for reduction(+: count) num_threads(6)
        for (int i = 1; i <= (num/2); i++) {
            if ((num % i) == 0) {
                //printf("Thread number: %d rank: %d\n", omp_get_thread_num(), process_rank);
                count = count + 1;
            }
        }
    return count;
}

/*
void nested(int depth)
{
    if (depth == DEPTH)
        return;

    #pragma omp parallel num_threads(2)
    {
        report_num_threads(depth);
        nested(depth+1);
    }
}
*/

int main(int argc, char *argv[]) {


    //omp_set_nested(1);
    //nested(1);

    omp_set_nested(1);
    omp_set_dynamic(0);

    double t0, t1; 
    int process_rank, size_of_cluster;
    
    MPI_Init(&argc, &argv); 
    MPI_Comm_size(MPI_COMM_WORLD, &size_of_cluster); 
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank); 
    MPI_Status status;

    int num_elementos_global;
    
    int *nums_global;
    int *nums_local;


    if (process_rank == MESTRE){
        //Abertura do arquivo de entrada.
        FILE *f_in;
        if (!(f_in = fopen("entrada.txt", "r"))) {
            fprintf(stderr, "Não foi possível abrir o arquivo de entrada.\n");
            return 2;
        }

        //Contagem de linhas do arquivo.
        int linhas = 0, num;
        while(fscanf(f_in, "%i", &num) != EOF) {
            linhas++;      
        }

        nums_global = malloc(sizeof(int) * linhas);
        rewind(f_in);
        //Leitura do arquivo de entrada.
        for (int i = 0; i < linhas; i++) {
            fscanf(f_in, "%d\n", &nums_global[i]);
        }
        num_elementos_global = linhas;
        t0 = MPI_Wtime();
    }

    //Enviando o tamanho do vetor de elementos para todos os processos.
    MPI_Bcast(&num_elementos_global, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    //Alocando os vetores de elementos locais.
    int num_elementos_local = (num_elementos_global/size_of_cluster);
    nums_local = malloc(sizeof(int) * num_elementos_local);

    //Enviando uma parte do vetor global para cada processo.
    MPI_Scatter(nums_global, num_elementos_local, MPI_INT, nums_local, num_elementos_local, MPI_INT, MESTRE, MPI_COMM_WORLD);

    //Calculando os divisores de cada elemento dos vetores locais paralelamente.
    int divisores_local[num_elementos_local];

    #pragma omp parallel for
    for (int i = 0; i < num_elementos_local; i++){
        divisores_local[i] = Calcula_Divisores(nums_local[i], process_rank);
    }
    

    
    //Recebendo os vetores locais de todos os processos.
    MPI_Gather(&divisores_local, num_elementos_local, MPI_INT, nums_global, num_elementos_local, MPI_INT, MESTRE, MPI_COMM_WORLD);    
    
    if (process_rank == MESTRE) {
        t1 = MPI_Wtime();

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
        printf("Tempo de execução: %f segundos\n", t1 - t0);
    }
    
    MPI_Finalize();
    
    return 0;
}
