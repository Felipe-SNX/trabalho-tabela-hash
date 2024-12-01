#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TABLE_SIZE 31

typedef struct Agenda Contato;
typedef struct Hash Hash;
typedef struct TabelaHash TabelaHash;

TabelaHash *inicializarHash();
int hashColisao(int chave);
int criaHash(char nome[]);
void adicionarContato(TabelaHash *tabela, Contato *pessoa, int rep);
void buscarContato(TabelaHash *tabela, char nome[], int rep);
void removerContato(TabelaHash *tabela, char nome[]);
char *formatarTelefone(char telefone[]);
void exibirContatos(TabelaHash *tabela);

// Estrutura dos dados da lista telefônica
typedef struct Agenda {
    char nome[50];
    char telefone[11];
}Contato;

// Estrutura para a chave e valor
typedef struct Hash {
    int chave;
    struct Agenda *contato;
}Hash;

// Estrutura da tabela hash
typedef struct TabelaHash {
    struct Hash *espacos[TABLE_SIZE]; // Vetor de ponteiros para "Hash"
}TabelaHash;

// Função de inicialização
TabelaHash *inicializarHash() {
    TabelaHash *tabela = (TabelaHash *)malloc(sizeof(TabelaHash));
    if (tabela == NULL) {
        fprintf(stderr, "Erro ao alocar memoria\n");
        exit(1);
    }
    for (int i = 0; i < TABLE_SIZE; i++) {
        tabela->espacos[i] = NULL; // Preencher os espaços como NULL
    }
    return tabela;
}

// Função para lidar com colisões usando endereçamento aberto
int hashColisao(int chave) {
    return chave % TABLE_SIZE;
}

// Função de espalhamento
int criaHash(char nome[]) {
    int tam = strlen(nome);
    unsigned int hash = 0;
    for (int i = 0; i < tam; i++) {
        hash += nome[i] * (i + 1);
    }
    return hash % TABLE_SIZE;
}

// Adicionar contato na tabela hash
void adicionarContato(TabelaHash *tabela, Contato *pessoa, int rep) {
    int id = criaHash(pessoa->nome);

    while (tabela->espacos[id] != NULL) {
        id = hashColisao(id + 1); // Resolver colisão
    }
    if(rep == 0){
        tabela->espacos[id] = (Hash *)malloc(sizeof(Hash));
        tabela->espacos[id]->chave = id;
        tabela->espacos[id]->contato = (Contato *)malloc(sizeof(Contato));
        strcpy(tabela->espacos[id]->contato->nome, pessoa->nome);
        strcpy(tabela->espacos[id]->contato->telefone, pessoa->telefone);
    }
}

// Buscar contato por nome
void buscarContato(TabelaHash *tabela, char nome[], int rep) {
    int id = criaHash(nome);

    if(rep == 0){
    while (tabela->espacos[id] != NULL) {
        if (strcmp(tabela->espacos[id]->contato->nome, nome) == 0) {
            char *telefone = formatarTelefone(tabela->espacos[id]->contato->telefone);
            printf("Telefone: %s\n", telefone);
            free(telefone);
            return;
        } else {
            id = hashColisao(id + 1);
        }
    }
        printf("Contato nao encontrado\n");
    }
}

// Remover contato
void removerContato(TabelaHash *tabela, char nome[]) {
    int id = criaHash(nome);

    while (tabela->espacos[id] != NULL) {
        if (strcmp(tabela->espacos[id]->contato->nome, nome) == 0) {
            free(tabela->espacos[id]->contato);
            free(tabela->espacos[id]);
            tabela->espacos[id] = NULL;
            printf("Contato removido com sucesso\n");
            return;
        } else {
            id = hashColisao(id + 1);
        }
    }
    printf("Contato nao encontrado para remocao\n");
}

// Formatar telefone
char *formatarTelefone(char telefone[]) {
    char *telefoneFormatado = (char *)malloc(15 * sizeof(char));
    if (telefoneFormatado == NULL) {
        fprintf(stderr, "Erro ao alocar memoria\n");
        exit(1);
    }
    sprintf(telefoneFormatado, "(%c%c) %c%c%c%c%c-%c%c%c%c",
            telefone[0], telefone[1],
            telefone[2], telefone[3], telefone[4], telefone[5], telefone[6],
            telefone[7], telefone[8], telefone[9], telefone[10]);
    return telefoneFormatado;
}

// Exibir todos os contatos
void exibirContatos(TabelaHash *tabela) {
    char *telefoneFormatado;
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (tabela->espacos[i] != NULL) {
            telefoneFormatado = formatarTelefone(tabela->espacos[i]->contato->telefone);
            printf("Nome: %s  Telefone: %s\n", tabela->espacos[i]->contato->nome, telefoneFormatado);
            free(telefoneFormatado);
        }
    }
}

int main() {
    TabelaHash *tabela = inicializarHash();
    Contato aux;
    char nome[50];
    int opcao;
    double tempoInsercao = 0.0, tempoBusca = 0.0;
    int rep = 0;

    do {
        printf("\nEscolha uma opcao:\n");
        printf("1 - Adicionar contato\n");
        printf("2 - Buscar contato por nome\n");
        printf("3 - Remover contato\n");
        printf("4 - Exibir todos os contatos\n");
        printf("0 - Sair\n");
        printf("Digite uma opcao: ");

        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                rep = 0;
                printf("Digite o nome do contato: ");
                scanf("%[^\n]", aux.nome);
                getchar();
                printf("Digite o numero do contato com DDD (apenas numeros): ");
                scanf("%s", aux.telefone);

                int repeticoesInsercao = 10000000;
                double tempo = clock();
                for (int i = 0; i < repeticoesInsercao; i++) {
                    adicionarContato(tabela, &aux, rep);
                    rep++;
                }
                tempo = (clock() - tempo)/repeticoesInsercao;
                printf("Tempo medio na insercao: %.6f ms\n", tempo);
                break;

            case 2:
                printf("Digite o nome do contato que deseja buscar: ");
                scanf("%[^\n]", nome);
                getchar();

                rep = 0;
                int repeticoesBusca = 10000000;
                double time = clock();
                for (int i = 0; i < repeticoesBusca; i++) {
                    buscarContato(tabela, nome, rep);
                    rep++;
                }
                time = (clock() - time)/repeticoesBusca;
                printf("Tempo medio na busca: %.6f ms\n", time);
                break;

            case 3:
                printf("Digite o nome do contato que deseja remover: ");
                scanf("%[^\n]", nome);
                getchar();
                removerContato(tabela, nome);
                break;

            case 4:
                exibirContatos(tabela);
                break;

            case 0:
                printf("Saindo...\n");
                break;

            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}

