#include <stdio.h>
//define o tamanho dos vetores 
#define tam 500
//define o tamanho das strings
#define lenstr 50


//Estrutura que representa o cadastro de um paciente
typedef struct paciente{
    float peso;
    int idade, codigoProcesso;
    char nome[lenstr];
}Paciente;

//Estrutura de nós para a fila encadeada
typedef struct no{
    //Struct paciente em cada nó representa uma posição na fila
    struct paciente cliente;
    //Ponteiro nó apontando para o próximo na fila
    struct no *prox;
}No;


//Estrutura que representa os produtos médicos no deposito
typedef struct deposito{
    //vetor com a quantidade de produtos 
    int qtd[tam];
    //cada posição do vetor de produto possui a sua quantificação no vetor qtd na mesma posição de vetor
    char produto[tam][lenstr];
}Deposito;


typedef struct hospital{
    //Representa o deposito do hospital
    struct deposito estoque;
    //Ponteiro nó para a fila de pacientes
    struct no* fila;
}Hospital;

/*
//Função de enfilerar
void insert(No** fila){
    No *novo = malloc(sizeof(No)), *aux;

    if(novo){
        
        //Entrada dos dados do paciente
        printf("Digite o nome: ");
        scanf("%[^\n]", novo->cliente->nome)
        printf("Digite o peso: ");
        scanf("%f%*c", &novo->cliente->peso);
        printf("Digite a idade: ");
        scanf("%d%*c", &novo->cliente->idade);
        printf("Digite o processo: ")
        printf("%d%*c", &novo->cliente->processo);
        novo->prox = NULL;
        
        if(*fila == NULL){
            *fila = novo;    
        }else{
            aux = *fila;
            while(aux->prox){
                aux = aux->prox;
            }
            aux->prox = novo;
        }
    }else{
        printf("\nErro ao alocar memoria\n");
    }
}

//Função de desenfilerar
No* delete(No **fila){
    No* aux = NULL;

    if(*fila){
        aux = *fila;
        *fila = aux->prox;
    }else{
        printf("Fila vazia\n");
    }

    return aux;
}
*/

void iniciaFila(No** fila){
    *fila = NULL;
}



int main(){
    return 0;
}