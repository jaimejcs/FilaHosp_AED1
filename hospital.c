#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hospital.h"

void listaProcessos(){
    printf("\nExame de sangue (1)\n");
    printf("Raio-x (2)\n");
    printf("Tomografia (3)\n");
    printf("Primeiro socorros (4)\n");
    printf("Internacao (5)\n\n");
}


//Valida o id
int validaId(Deposito** depo, int id){
    for(int i = 0; i < (*depo)->totalItens; i++){
        if((*depo)->id[i] == id) return 1;
    }

    return 0;
}


//Leitura dos dados de produtos
void lerProduto(Deposito **depo){
    int id;
    //Nome do produto
    printf("Digite o nome do produto: ");
    scanf("%[^\n]", (*depo)->produto[(*depo)->totalItens]);
    //Código de identificação do produto
    do{
        printf("Digite o ID: ");
        scanf("%d%*c", &id);      //Verifica se o ID é válido, gerando um loop até que seja válido
    }while(validaId(depo, id));
    (*depo)->id[(*depo)->totalItens] = id;
    //Quantidade do produto
    printf("Digite a quantidade: ");
    scanf("%d%*c", &(*depo)->qtd[(*depo)->totalItens]);
}


//Leitura dos dados de cadastro
void lerCadastro(No **cadastro){
    int aux;
    //Nome do paciente
    printf("Digite o nome: ");
    scanf("%[^\n]", (*cadastro)->cliente.nome);
    //Peso do paciente
    printf("Digite o peso: ");
    scanf("%f%*c", &(*cadastro)->cliente.peso);
    //Idade do paciente
    printf("Digite a idade: ");
    scanf("%d%*c", &(*cadastro)->cliente.idade);
    //Código do procedimento a ser realizado pelo paciente
    do{
        printf("Digite o processo (-1 para ver a lista dos processos): ");
        scanf("%d%*c", &aux);
        if(aux == -1)
            listaProcessos();
    }while(aux < 1 || aux > 5);
    (*cadastro)->cliente.processo = aux;
}


//Quebra a string, separando as informações unidas com o ';'
void quebrarString(char *str, Deposito** depo){
    sscanf(str, "%d;%[^;];%d", &((*depo)->id[(*depo)->totalItens]), ((*depo)->produto[(*depo)->totalItens]), &((*depo)->qtd[(*depo)->totalItens]));
}

//Carrega o deposito da struct hospital com os dados do arquivo de deposito
void carregaVet(Deposito* depo){
    FILE *file = fopen("estoque.txt", "r");
    char buffer[lenstr];

    if(file){
        while(fgets(buffer, lenstr, file) != NULL){
            // Remove o caractere de nova linha da entrada, se presente
            size_t len = strlen(buffer);
            
            if (len > 0 && buffer[len - 1] == '\n') {
                buffer[len - 1] = '\0';
            }

            // Chama a função para quebrar a string
            quebrarString(buffer, &depo);
            depo->totalItens++;
        }
        
    }else{
        printf("\n-- Problema ao abrir arquivo --\n");
    }

    fclose(file);
}


void cadastrar(Deposito *depo){
    FILE* file = fopen("estoque.txt", "a");

    if(file){
        lerProduto(&depo); // Entrada dos dados do produto a ser cadastrado;
        fprintf(file, "%d;%s;%d\n", depo->id[depo->totalItens], depo->produto[depo->totalItens], depo->qtd[depo->totalItens]);

        depo->totalItens++; // Atualiza a quantidade de produtos;
    }else{
        printf("\n-- Problema ao abrir arquivo --\n");
    }

    fclose(file);
}


//Função de enfilerar
void insert(No** fila, No** preferencial){
    No *novo = malloc(sizeof(No)), *aux;

    if(novo){   //Verifica se a alocação funcionou
        
        //Entrada dos dados do paciente
        lerCadastro(&novo);

        //Ponteiro nulo da última posição da fila
        novo->prox = NULL;

        if(novo->cliente.idade >= 60){
            if(*preferencial == NULL){
                *preferencial = novo;    
            }else{
                aux = *preferencial;
                while(aux->prox){
                    aux = aux->prox;
                }
                aux->prox = novo;
            }
        }else if(novo->cliente.idade < 60 && novo->cliente.idade >= 1){
            if(*fila == NULL){
                *fila = novo;    
            }else{
                aux = *fila;
                while(aux->prox){
                    aux = aux->prox;
                }
                aux->prox = novo;
            }
        }
    }else{
        printf("\n---Erro ao alocar memoria---\n");
    }
}

//Função de desenfilerar
No* delete(No **fila){
    No* aux = NULL;

    if(*fila){ // Verifica se a fila existe
        aux = *fila;
        *fila = aux->prox;
    }else{
        printf("\n---Fila vazia---\n");
    }

    return aux;
}

//Inicializa a fila
void iniciaFila(No** fila){
    *fila = NULL;
}

void iniciaDeposito(Deposito* depo){
    int i;
    //Percorre o vetor qtd, inicializando-o
    for(i = 0; i < tam; i++){
        depo->qtd[i] = 0; 
        depo->id[i] = 0;
    }
    //Inicializa com nenhum item cadastrado
    depo->totalItens = 0;
}

void iniciaArquivo(){  //Cria o arquivo caso não exista
    FILE* arq = fopen("estoque.txt", "a+");
    fclose(arq);
}

void iniciaProcessos(Processo *processos){  //Há uma quantidade definida e fixa de processos, mas pode-se implementar uma gestão
    for(int i = 0; i < proc; i++){          //de adicionar ou retirar processo. Por falta de tempo, não fizemos
        processos[i].id = i+1;
    }
    //Processo id 1
    processos[0].produto[0] = 1;
    processos[0].quantidade[0] = 1;
    processos[0].produto[1] = 4;
    processos[0].quantidade[1] = 1;
    processos[0].produto[2] = 5;
    processos[0].quantidade[2] = 1;
    processos[0].totalItens = 3; 

    //Processo id 2
    processos[1].produto[0] = 3;
    processos[1].quantidade[0] = 1;
    processos[1].totalItens = 1;

    //Processo id 3
    processos[2].produto[0] = 3;
    processos[2].quantidade[0] = 1;
    processos[2].totalItens = 1;

    //Processo id 4
    processos[3].produto[0] = 2;
    processos[3].quantidade[0] = 2;
    processos[3].produto[1] = 6;
    processos[3].quantidade[1] = 1;
    processos[3].produto[2] = 1;
    processos[3].quantidade[2] = 1;
    processos[3].produto[3] = 5;
    processos[3].quantidade[3] = 1;
    processos[3].totalItens = 4;

    //Processo id 5
    processos[4].produto[0] = 7;
    processos[4].quantidade[0] = 1;
    processos[4].produto[1] = 1;
    processos[4].quantidade[1] = 1;
    processos[4].produto[2] = 8;
    processos[4].quantidade[2] = 1;
    processos[4].totalItens = 3;
}

void atualizaEstoque(Deposito* depo, Processo p){
    for(int i = 0; i < p.totalItens; i++){
        for(int j = 0; j < depo->totalItens; j++){
            if(p.produto[i] == depo->id[j]){
                depo->qtd[j] -= p.quantidade[i];
                if(depo->qtd[j] < 50){
                    printf("Estoque de %s baixo!\n", depo->produto[j]);
                }
            }
        }
    }

    FILE* file = fopen("estoque.txt", "w");

    if(file){
        for(int i = 0; i < depo->totalItens; i++){
            fprintf(file, "%d;%s;%d\n", depo->id[i], depo->produto[i], depo->qtd[i]);
        }
    }else{
        printf("\n--Erro ao atualizar o arquivo--\n");
    }

    fclose(file);
}

void printaCadastro(No* cadastro){
    printf("Nome: %s\n", cadastro->cliente.nome);
    printf("Peso: %.2f\n", cadastro->cliente.peso);
    printf("Idade: %d\n", cadastro->cliente.idade);
    printf("Processo: %d\n", cadastro->cliente.processo);
}

void imprimeEstoque(Deposito depo){
    for(int i = 0; i < depo.totalItens; i++){
        printf("Produto: %s\n", depo.produto[i]);
        printf("Codigo: %d\n", depo.id[i]);
        printf("Quantidade: %d\n", depo.qtd[i]);
        printf("\n");
    }
    printf("-- Fim estoque --\n");
}

void imprimeFila(No *cadastro){
    No* aux = cadastro; //Variável auxiliar para percorrer a fila

    //Percorre a fila printando todos os cadastros
    if(aux){
        do{
            printaCadastro(aux);
            aux = aux->prox;
        }while(aux);
    }else{
        printf("\n---Fila vazia---\n");
    }
}

void painel(){
    printf("\n\n      Sistema gestor 1.0\n");
    printf("-----------------------------\n");
    printf("1- Adicionar paciente a fila\n");
    printf("2- Retirar da fila\n");
    printf("3- Ver as filas\n");
    printf("4- Cadastrar novo produto\n");
    printf("5- Ver estoque\n");
    printf("6- Sair do sistema\n");
}