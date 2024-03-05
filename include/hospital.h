#ifndef HOSPITAL_H
#define HOSPITAL_H


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

void listaProcessos();

//Valida o id
int validaId(Deposito** depo, int id);

//Leitura dos dados de produtos
void lerProduto(Deposito **depo);

//Leitura dos dados de cadastro
void lerCadastro(No **cadastro);

//Quebra a string, separando as informações unidas com o ';'
void quebrarString(char *str, Deposito** depo);

//Carrega o deposito da struct hospital com os dados do arquivo de deposito
void carregaVet(Deposito* depo);

void cadastrar(Deposito *depo);

//Função de enfilerar
void insert(No** fila, No** preferencial);

//Função de desenfilerar
No* delete(No **fila);

//Inicializa a fila
void iniciaFila(No** fila);

void iniciaDeposito(Deposito* depo);

void iniciaArquivo();

void iniciaProcessos(Processo *processos);

void atualizaEstoque(Deposito* depo, Processo p);

void printaCadastro(No* cadastro);

void imprimeEstoque(Deposito depo);

void imprimeFila(No *cadastro);

void painel();

#endif