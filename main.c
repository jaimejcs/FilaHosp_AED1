#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//define o tamanho dos vetores 
#define tam 500
//define o tamanho das strings
#define lenstr 50
//define a quantidade fixa de processos
#define proc 5


//Estrutura que representa o cadastro de um paciente
typedef struct paciente{
    float peso;
    //A variável processo refere-se ao código do tipo de procedimento a ser realizado pelo paciente
    int idade, processo;
    char nome[lenstr];
}Paciente;

//Estrutura de nós para a fila encadeada
typedef struct no{
    //Struct paciente em cada nó representa uma posição na fila
    struct paciente cliente;
    //Ponteiro nó apontando para o próximo na fila
    struct no *prox;
}No;

//Estrutura que define o que cada processo, ou procedimento, gasta de produto
typedef struct processo{  
    int id;
    int produto[tam];  // O índice de produto é ligado ao vetor quantidade, assim id do produto[0] tem sua
    int quantidade[tam];        // quantidade especificada em quantidade[0]
    int totalItens; 
}Processo;

//Estrutura que representa os produtos médicos no deposito
typedef struct deposito{
    //vetor com a quantidade de produtos 
    int qtd[tam];
    //cada posição do vetor de produto possui a sua quantificação no vetor qtd na mesma posição de vetor
    char produto[tam][lenstr];
    int totalItens; //especifica a quantidade de itens cadastrados
    int id[tam]; // Codigo que identifica um produto
}Deposito;


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
void quebrarString(char *str, Deposito** depo) {
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

int main(){
    No *fila, *preferencial, *r;   //fila e preferencial são os ponteiros de nó que apontam para o topo das filas
    Processo processos[proc];
    Deposito depo;  //estrutura que define a abstração do deposito do hospital
    int cmd;    //r é o ponteiro de nó usada apenas para mostrar o item da fila retirado e liberar memória
    char buffer[lenstr];

    //Funções inicializadoras
    iniciaFila(&fila);
    iniciaFila(&preferencial);
    iniciaDeposito(&depo);
    iniciaProcessos(processos);
    iniciaArquivo();
    
    // Carrega o vetor depo com o conteúdo dos arquivos para melhor manipulação
    carregaVet(&depo);

    do{
        painel();
        printf("Opcao: ");

        scanf("%[^\n]", buffer);
        getchar(); //Consumir caractere de quebra de linha;
        cmd = strtoul(buffer, NULL, 0);
        
        while(cmd < 1 || cmd > 6){  // Evita entradas inadequadas
            printf("Valor invalido. Digite novamente: ");
            scanf("%[^\n]", buffer);
            getchar(); //Consumir caractere de quebra de linha;
            cmd = strtoul(buffer, NULL, 0);
        }

        switch(cmd){
            case 1:
                printf("Adicionando paciente a fila...\n");
                insert(&fila, &preferencial);
                break;

            case 2:
                printf("Fila preferencial (P) ou normal (N): ");
                char opc; // char para indicar de qual fila se trata

                scanf("%c%*c", &opc);  // %*c ignora o \n que segue o char, deixando o buffer limpo

                while(opc != 'N' && opc != 'P'){  // Evita entradas inválidas
                    printf("Opcao invalida. Digite novamente: ");
                    scanf("%c%*c", &opc);  // 
                }

                if(opc == 'N')
                    r = delete(&fila);
                
                if(opc == 'P')
                    r = delete(&preferencial);

                if(r){
                    printf("\nCadastro removido:\n");
                    printaCadastro(r);

                    //Atualiza estoque
                    atualizaEstoque(&depo, processos[(r->cliente.processo) - 1]);

                    //Libera memória alocada
                    free(r);
                }

                break;
            case 3:
                printf("\nImprimindo filas...\n");
                printf("\n- Normal:\n");
                imprimeFila(fila);
                printf("\n- Preferencial:\n");
                imprimeFila(preferencial);
                break;

            case 4:
                printf("\nCadastrando produto...\n");
                cadastrar(&depo);
                break;

            case 5:
                printf("\n-- Estoque atual --\n\n");
                imprimeEstoque(depo);
                break;

            case 6:
                printf("Saindo...\n");
                break;

            default:
            printf("Opcao invalida\n");
                break;
        }
    }while(cmd != 6);

    return 0;
}