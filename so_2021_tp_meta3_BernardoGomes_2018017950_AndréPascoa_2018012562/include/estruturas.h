#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#define MAXPLAYERS 30
#define GAMEDIR "/home/bernardo/Desktop/so_2021_tp_meta3_BernardoGomes_2018017950_AndréPascoa_2018012562/out/" //Meter diretoria
#define DURCAMP 60            // 5 minutos
#define TEMPOESPERA 10          //30 SEGUNDOS
#define PIPE "../main_pipe"
#define PIPE_CLI "../pipe-%d"
#define PIPE_GM "../pipe_jogo-%d"

//--------------------------------------------------------

#define SERVER_SHUTDOWN 1
#define CLIENT_SHUTDOWN 2
#define LOGIN 3
#define LOGGED 4
#define TEMPOCAMPEONATO 5
#define PERGUNTA 6
#define NOT_LOGGED 7
#define QUIT 8
#define MYGAME 9
#define PLAY 10
#define KICK 11
#define WAIT 12
#define END 13
#define RESPOSTA 14
#define MANDARESPOSTA 15
#define SUSPENDE 16
#define RETOMA 17
#define LOGGEDFAILED 18
#define PONTUACAO 19
#define WINNER 20
#define TOTALJOGOS 4


typedef struct INFORMACOES{
    int maxplayers;
    char *gamedir;
    int duraCampeonato;
    int tempoEspera;
}informacoes;



typedef struct JOGO{
   informacoes inf;
   char identificacao[50];
   char nome[40];
   char informacao[100];
   char pergunta[100];
   char resposta[100];
   int id;
   char dicas[100];
}jogo;


typedef struct CAMPEONATO{
    jogo game;
    int tempo;
}campeonato;




typedef struct JOGADORES{
    jogo game;
    char nome[100];
    int pid;
    int pontuacao;
    int tentativa;
    char msgResposta[50];
    int tentaResposta;
    int acao;
    int ativo;
    int aguarda;
    int comunicaJogo;
}jogadores;



#endif