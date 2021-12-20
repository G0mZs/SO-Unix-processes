#include "estruturas.h"

extern jogadores Jogadores;

int main(int argc, char **argv);
void verify_env_var(informacoes *informacao);
void verifyGamedir(char *gamedir, char *directory);
void verify_args(informacoes *informacao);
void mostrarJogadores(jogadores *j);
void *recebeu();
void *tempo();
int inserir_utilizador(char nome[],int pid,jogadores *jogador);
int kickJogador(char nome[],jogadores *jogador);
void DividirComando(char com[]);
int conta_utilizadores(jogadores *jogador);
void mostra_Jogos(jogo *game);
void preenche_array_jogos(jogo *game);
int conta_utilizadores(jogadores *jogador);
int jogo_aleatorio();
void flush_in();
char *remove_novalinha(char *comando);
void *iniciagames(void* a);