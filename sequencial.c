#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h> 
#include <sys/stat.h>

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

    //display(inicio);
    
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

int main(int argc, char *argv[]){
    double time_spent = 0.0;
    char ch;
    char *arq_entrada = "";
    while ((ch = getopt(argc, argv, "i:")) != EOF) {
        switch(ch) {
            //Coletando nome do arquivo de entrada.
            case 'i':
                arq_entrada = optarg;
                break;
            default:
                fprintf(stderr, "Opção inválida: '%s'\n", optarg);
                return 1;
        }
    }

    //Abertura do arquivo de entrada.
    FILE *f_in;
    if (!(f_in = fopen(arq_entrada, "r"))) {
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
    int qtd_divisores[linhas];
    rewind(f_in);
    //Leitura do arquivo de entrada.
    for (int i = 0; i < linhas; i++) {
        fscanf(f_in, "%d", &nums[i]);
    }

    clock_t begin = clock();
    //Calcula o número de divisores.
    for (int i = 0; i < linhas; i++){
        qtd_divisores[i] = qtd_Divisores_dinamico(nums[i]);
        printf("%d - %d\n", nums[i], qtd_divisores[i]);
    }
    clock_t end = clock();

    //Salvando resultado em um arquivo de saída.
    for (int i=0; i<linhas; i++){    
        fprintf(f_out, "%i\n", qtd_divisores[i]);
    }
        
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Tempo de execução da função qtd_Divisores_dinamica: %f segundos", time_spent);

    fclose(f_in);
    fclose(f_out);

    return 0;
}