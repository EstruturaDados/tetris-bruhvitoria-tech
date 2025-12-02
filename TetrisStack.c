#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ---------------------------
// Definições de estruturas
// ---------------------------

typedef struct {
    char nome;   // 'I', 'O', 'T', 'L'
    int id;      // ID único
} Peca;

// ---------------------------
// Configurações da fila e pilha
// ---------------------------
#define TAM_FILA 5
#define TAM_PILHA 3

Peca fila[TAM_FILA];
Peca pilha[TAM_PILHA];
int frente = 0, tras = 0, qtdFila = 0;
int topo = -1; // Pilha

int contadorID = 0; // gera IDs únicos

// ---------------------------
// Função gerar peça aleatória
// ---------------------------
char gerarTipoPeca() {
    char tipos[4] = {'I', 'O', 'T', 'L'};
    return tipos[rand() % 4];
}

Peca gerarPeca() {
    Peca p;
    p.nome = gerarTipoPeca();
    p.id = contadorID++;
    return p;
}

// ---------------------------
// Operações da FILA CIRCULAR
// ---------------------------

int filaCheia() {
    return qtdFila == TAM_FILA;
}

int filaVazia() {
    return qtdFila == 0;
}

void enqueue(Peca p) {
    if (filaCheia()) return;
    fila[tras] = p;
    tras = (tras + 1) % TAM_FILA;
    qtdFila++;
}

Peca dequeue() {
    Peca removido = {'-', -1};
    if (filaVazia()) return removido;

    removido = fila[frente];
    frente = (frente + 1) % TAM_FILA;
    qtdFila--;
    return removido;
}

Peca frenteFila() {
    return fila[frente];
}

// ---------------------------
// Operações da PILHA
// ---------------------------

int pilhaVazia() {
    return topo == -1;
}

int pilhaCheia() {
    return topo == TAM_PILHA - 1;
}

void push(Peca p) {
    if (pilhaCheia()) return;
    pilha[++topo] = p;
}

Peca pop() {
    if (pilhaVazia()) {
        Peca vazio = {'-', -1};
        return vazio;
    }
    return pilha[topo--];
}

Peca topoPilha() {
    if (pilhaVazia()) {
        Peca vazio = {'-', -1};
        return vazio;
    }
    return pilha[topo];
}

// ---------------------------
// Exibir estado atual
// ---------------------------
void mostrarEstado() {
    printf("\n===== Estado Atual =====\n");

    printf("Fila de peças: ");
    int idx = frente;
    for (int i = 0; i < qtdFila; i++) {
        printf("[%c %d] ", fila[idx].nome, fila[idx].id);
        idx = (idx + 1) % TAM_FILA;
    }

    printf("\nPilha de reserva (Topo -> Base): ");
    for (int i = topo; i >= 0; i--) {
        printf("[%c %d] ", pilha[i].nome, pilha[i].id);
    }

    printf("\n========================\n");
}

// ---------------------------
// Troca simples entre fila e pilha
// ---------------------------
void trocarFrenteComTopo() {
    if (filaVazia() || pilhaVazia()) {
        printf("\nNão é possível trocar: fila ou pilha vazia.\n");
        return;
    }

    Peca temp = fila[frente];
    fila[frente] = pilha[topo];
    pilha[topo] = temp;

    printf("\nTroca realizada entre frente da fila e topo da pilha.\n");
}

// ---------------------------
// Troca múltipla (3 peças)
// ---------------------------
void trocaMultiplas() {
    if (qtdFila < 3 || topo + 1 < 3) {
        printf("\nERRO: são necessárias pelo menos 3 peças na fila e na pilha.\n");
        return;
    }

    int idxFila = frente;

    for (int i = 0; i < 3; i++) {
        Peca temp = fila[idxFila];
        fila[idxFila] = pilha[topo - i];
        pilha[topo - i] = temp;
        idxFila = (idxFila + 1) % TAM_FILA;
    }

    printf("\nTroca múltipla realizada com sucesso!\n");
}

// ---------------------------
// Programa principal
// ---------------------------
int main() {
    srand(time(NULL));

    // Preenche a fila no início
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(gerarPeca());
    }

    int opcao;

    do {
        mostrarEstado();

        printf("\nAções disponíveis:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para a pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila com topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com as 3 peças da pilha\n");
        printf("0 - Sair\n");

        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {

            case 1: { // Jogar
                if (filaVazia()) printf("Fila vazia, impossível jogar.\n");
                else {
                    Peca jogada = dequeue();
                    printf("Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
                    enqueue(gerarPeca()); // reposição automática
                }
                break;
            }

            case 2: { // Reservar
                if (pilhaCheia()) printf("Pilha cheia, impossível reservar.\n");
                else if (filaVazia()) printf("Fila vazia!\n");
                else {
                    Peca reservada = dequeue();
                    push(reservada);
                    printf("Peça [%c %d] enviada para pilha.\n", reservada.nome, reservada.id);
                    enqueue(gerarPeca()); // reposição automática
                }
                break;
            }

            case 3: { // Usar peça da pilha
                if (pilhaVazia()) printf("Pilha vazia!\n");
                else {
                    Peca usada = pop();
                    printf("Peça usada: [%c %d]\n", usada.nome, usada.id);
                }
                break;
            }

            case 4:
                trocarFrenteComTopo();
                break;

            case 5:
                trocaMultiplas();
                break;

            case 0:
                printf("Encerrando programa...\n");
                break;

            default:
                printf("Opção inválida!\n");
        }

    } while (opcao != 0);

    return 0;
}
