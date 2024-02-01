#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//define o tamanho dos vetores 
#define tam 500
//define o tamanho das strings
#define lenstr 50


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
    char produto[tam][lenstr];  // O primeiro índice de produto é ligado ao vetor quantidade, assim a string produto[0] tem sua
    int quantidade[tam];        // quantidade especificada em quantidade[0] 
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


//Leitura dos dados de produtos
void lerProduto(Deposito **depo){
    //Nome do produto
    printf("Digite o nome do produto: ");
    scanf("%[^\n]", (*depo)->produto[(*depo)->totalItens]);
    //Código de identificação do produto
    printf("Digite o ID: ");
    scanf("%d%*c", &(*depo)->id[(*depo)->totalItens]);
    //Quantidade do produto
    printf("Digite a quantidade: ");
    scanf("%d%*c", &(*depo)->qtd[(*depo)->totalItens]);
}


//Leitura dos dados de cadastro
void lerCadastro(No **cadastro){
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
    printf("Digite o processo: ");
    scanf("%d%*c", &(*cadastro)->cliente.processo);
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
    Deposito depo;  //estrutura que define a abstração do deposito do hospital
    int cmd;    //r é o ponteiro de nó usada apenas para mostrar o item da fila retirado e liberar memória
    char buffer[lenstr];

    //Funções inicializadoras
    iniciaFila(&fila);
    iniciaFila(&preferencial);
    iniciaDeposito(&depo);
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
                    printf("Cadastro removido:\n");
                    printaCadastro(r);

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