#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct contagem_divisores{
    int valor_divisivel;
    int expoente;
    struct contagem_divisores *prox;
} contagem_divisores;

int qtd_Divisores(int num){
    int count = 0;
    for (int i = 1; i < (num/2); i++) {
        if ((num % i) == 0) {
            count++;
        }
    }
    return count+1;
}

*contagem_divisores create(int valor){
    contagem_divisores *i = malloc(sizeof(contagem_divisores));
    i->valor_divisivel = valor_divisivel;
    i->expoente = 1;
    i->prox = NULL;
    return i;
}

int qtd_Divisores_melhor(int num){
    int count = 0, divisor = 2;
    contagem_divisores *inicio = NULL;
    contagem_divisores *i = NULL;
    contagem_divisores *prox = NULL;
    while (num > 1) {
        if ((num % divisor) == 0) {
            aux = 1;
            prox = create(divisor);
            if (inicio == NULL) {
                inicio = prox;
            }
            if (i != NULL) {
                i->prox = prox;
            }
            i = prox;
            /*if (i->valor_divisivel == NULL){
                i->valor_divisivel = divisor;
                i->expoente = 1;
                i->prox = NULL;
                *inicio = i;
            }
            else {
                i->expoente++;
            }*/
        }
        else {
            divisor++;
            prox = i->prox;
        }
    }
}

int main(int argc, char *argv[]){

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

    //Leitura do arquivo.
    int num, qtd_divisores;
    while(fscanf(f_in, "%i", &num) != EOF) {
        //Calcula o número de divisores.
        //qtd_divisores = qtd_Divisores(num);
        int result = qtd_Divisores_melhor(120);
        //Salva em um arquivo de saída.
        //fprintf(f_out, "%i\n", qtd_divisores);
        printf("%i\n", result);
    }

    fclose(f_in);
    fclose(f_out);

    return 0;
}