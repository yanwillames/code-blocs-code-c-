#include <iostream>
#include <limits>
using namespace std;

// ---------- Struct do paciente (nó da lista) ----------
struct Paciente {
    char nome[100];
    int idade;
    char cpf[15];
    int prioridade; // 1 = urgencia (prioridade) | 0 = normal
    Paciente* prox;
};

// ---------- Funcao auxiliar para copiar texto sem usar <cstring> ----------
void copiarTexto(char destino[], char origem[], int tamanho) {
    int i = 0;
    while (origem[i] != '\0' && i < tamanho - 1) {
        destino[i] = origem[i];
        i++;
    }
    destino[i] = '\0';
}

// ---------- Funcoes auxiliares para leitura segura ----------
void limparBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int lerInteiro(const char* mensagem) {
    int valor;
    while (true) {
        cout << mensagem;
        cin >> valor;

        if (cin.fail()) {
            cout << "Entrada invalida. Digite um numero inteiro.\n";
            limparBuffer();
        } else {
            limparBuffer();
            return valor;
        }
    }
}

void lerTexto(const char* mensagem, char destino[], int tamanho) {
    while (true) {
        cout << mensagem;
        cin.getline(destino, tamanho);

        if (destino[0] != '\0') {
            return;
        }

        cout << "Campo obrigatorio.\n";
    }
}

// ---------- TAD: Insercao ----------
// Pacientes com prioridade entram sempre antes dos normais,
// mas respeitando a ordem de chegada dentro de cada grupo.
void inserir(Paciente* &inicio, char nome[], int idade, char cpf[], int prioridade) {
    Paciente* novo = new Paciente;
    copiarTexto(novo->nome, nome, 100);
    novo->idade = idade;
    copiarTexto(novo->cpf, cpf, 15);
    novo->prioridade = prioridade;
    novo->prox = NULL;

    // Lista vazia
    if (inicio == NULL) {
        inicio = novo;
        return;
    }

    // Paciente prioritario: entra no fim do bloco de prioritarios
    if (prioridade == 1) {
        if (inicio->prioridade == 0) {
            // nao existe nenhum prioritario ainda, vai para o inicio
            novo->prox = inicio;
            inicio = novo;
            return;
        }
        Paciente* atual = inicio;
        while (atual->prox != NULL && atual->prox->prioridade == 1) {
            atual = atual->prox;
        }
        novo->prox = atual->prox;
        atual->prox = novo;
        return;
    }

    // Paciente normal: sempre vai para o final da lista
    Paciente* atual = inicio;
    while (atual->prox != NULL) {
        atual = atual->prox;
    }
    atual->prox = novo;
}

// ---------- TAD: Remocao (atendimento) ----------
// Remove sempre o primeiro paciente da fila (proximo a ser atendido)
bool remover(Paciente* &inicio) {
    if (inicio == NULL) {
        cout << "\nNao ha pacientes na fila.\n";
        return false;
    }

    Paciente* temp = inicio;
    cout << "\nPaciente atendido: " << temp->nome
         << " | CPF: " << temp->cpf << "\n";

    inicio = inicio->prox;
    delete temp;
    return true;
}

// ---------- Exibe a fila completa ----------
void exibirFila(Paciente* inicio) {
    if (inicio == NULL) {
        cout << "\nFila vazia.\n";
        return;
    }

    cout << "\n----- FILA DE ATENDIMENTO -----\n";
    Paciente* atual = inicio;
    int pos = 1;
    while (atual != NULL) {
        cout << pos << "] " << atual->nome
             << " | Idade: " << atual->idade
             << " | CPF: " << atual->cpf
             << " | " << (atual->prioridade == 1 ? "PRIORIDADE" : "Normal")
             << "\n";
        atual = atual->prox;
        pos++;
    }
    cout << "--------------------------------\n";
}

// ---------- Menu principal ----------
int main() {
    Paciente* inicio = NULL;
    int opcao;

    do {
        cout << "\n===== SISTEMA DE AGENDAMENTO - CLINICA =====\n";
        cout << "1 - Agendar paciente\n";
        cout << "2 - Atender proximo paciente\n";
        cout << "3 - Listar fila\n";
        cout << "0 - Sair\n";
        opcao = lerInteiro("Escolha: ");

        if (opcao == 1) {
            char nome[100];
            char cpf[15];
            int idade, prioridade;

            cout << "\n";
            lerTexto("Nome: ", nome, 100);
            idade = lerInteiro("Idade: ");
            lerTexto("CPF: ", cpf, 15);
            prioridade = lerInteiro("Prioridade de urgencia? (1-Sim / 0-Nao): ");

            inserir(inicio, nome, idade, cpf, prioridade);
            cout << "\nPaciente agendado com sucesso!\n";

        } else if (opcao == 2) {
            remover(inicio);

        } else if (opcao == 3) {
            exibirFila(inicio);

        } else if (opcao != 0) {
            cout << "\nOpcao invalida!\n";
        }

    } while (opcao != 0);

    // Libera a memoria restante ao sair
    while (inicio != NULL) {
        Paciente* temp = inicio;
        inicio = inicio->prox;
        delete temp;
    }

    cout << "\nSistema encerrado.\n";
    return 0;
}


