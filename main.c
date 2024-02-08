#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hospital.h"

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