#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "estruturas.h"
#include "G_004.h"
#include <string.h>
#include <getopt.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

int pontos = 0;
jogadores jogador;

int aleatorio();
void dicasfinal(char* resposta, char* traducao, int tentativa, int iteracao);
void dicas(char* resposta, char* traducao, int iteracao);
void trataSigEnd(int i)
{
    exit(jogador.pontuacao);
}


int main()
{
    pontos=0;
    signal(SIGUSR1,trataSigEnd);
    int continuar = 1,
        iteracao = 0,
        tentativa = 0;
        char resposta[30];
        char traducao1[30] = "agua";
        char traducao2[30] = "manteiga";
        char traducao3[30] = "biblia";
        char traducao4[30] = "laranja";
    

    do
    {
        printf("Comecou! Tente traduzir as palavras em Inglês para Português!\n\n");
        tentativa = 0;
        iteracao = 0;
            do{
                tentativa++;
                iteracao++;
                printf("Tentativa %d : Qual a tradução da palavra WATER:", iteracao);               
                fflush(stdout);
                scanf("%s", resposta);
                dicas(resposta,traducao1,iteracao);
            }while(strcmp(resposta,traducao1) != 0);
        iteracao=0;

            do{
                tentativa++;
                iteracao++;
                printf("Tentativa %d : Qual a tradução da palavra BUTTER:", iteracao);               
                fflush(stdout);
                scanf("%s", resposta);
                dicas(resposta,traducao2,iteracao);
            }while(strcmp(resposta, traducao2) != 0);
        iteracao=0;

            do{
                tentativa++;
                iteracao++;
                printf("Tentativa %d : Qual a tradução da palavra BIBLE:", iteracao);               
                fflush(stdout);
                scanf("%s", resposta);
                dicas(resposta,traducao3,iteracao);
            }while(strcmp(resposta, traducao3) != 0);
        iteracao=0;

            do{
                tentativa++;
                iteracao++;
                printf("Tentativa %d : Qual a tradução da palavra ORANGE:", iteracao);               
                fflush(stdout);
                scanf("%s", resposta);
                dicas(resposta,traducao4,iteracao);
            }while(strcmp(resposta, traducao4) != 0);
            jogador.pontuacao = jogador.pontuacao + pontuacao(tentativa);

    }
    while(continuar == 1); 
      
}
void dicas(char* resposta, char* traducao, int iteracao)
{
   if(strcmp(resposta,traducao) == 0)
             printf("Parabens! A tradução era %s\n Voce acertou em %d tentativas!\n\n", resposta ,iteracao );                
        else
            printf("Infelizmente essa não é a tradução de \n\n");

}
void dicasfinal(char* resposta, char* traducao, int tentativa, int iteracao)
{
            if(strcmp(resposta,traducao) == 0)
            {
                printf("Parabens! A tradução era %s\n Voce acertou em %d tentativas!\n\n", resposta ,iteracao );   
                printf("Concluio o jogo em %d!\n\n",tentativa );
            }               
            else
            {
                printf("Infelizmente essa não é a tradução de \n\n");
            }

}

int pontuacao(int tentativa)
{
    if(tentativa >= 10)
    {
        int pontuacao = 1;
        return pontuacao;
    }
    if(tentativa >= 5 && tentativa < 10)
    {
        int pontuacao = 2;
        return pontuacao;
    }
    if(tentativa >= 3 && tentativa < 5)
    {
        int pontuacao = 3;
        return pontuacao;
    }
    if(tentativa == 2)
    {
        int pontuacao = 5;
        return pontuacao;
    }
    if(tentativa == 1)
    {
        int pontuacao = 10;
        return pontuacao;
    }
    else
    {
        int pontuacao = 0;
        return pontuacao;
    }
    
}