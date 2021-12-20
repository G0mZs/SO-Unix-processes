#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "estruturas.h"
#include "G_001.h"
#include <string.h>
#include <getopt.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

int pontos = 0;
jogadores jogador;

int aleatorio();
void dicas(int number, int chave, int tentativa);

void trataSigEnd(int i)
{
    exit(jogador.pontuacao);
}


int main()
{
    pontos=0;
    signal(SIGUSR1,trataSigEnd);
    int continuar=1,
        iteracao=0,
        chave,
        tentativa,
        number;
        

    do
    {
        
        printf("Sorteando numero entre 1 e 100...\n");
        chave = aleatorio();

        printf("Comecou! Tente adivinhar o numero!\n\n");
        tentativa = 0;
        do
        {
            tentativa++;
            iteracao++;
            printf("Tentativa %d: ", tentativa);
            fflush(stdout);
            scanf("%d", &number);
            dicas(number,chave,tentativa);
        }
        while( number != chave);
        jogador.tentativa = tentativa;
        jogador.pontuacao = jogador.pontuacao + pontuacao(tentativa);
    
       
    }
    while(continuar == 1); // completar aqui com a duracao de campeonato ou seja while(durCampeonato < x)

      
}

int aleatorio()
{
    srand( (unsigned)time(NULL) );

    return (1 + rand()%100);
}

void dicas(int number, int chave, int tentativa)
{
    if(number > chave)
                printf("O numero sorteado e menor que %d\n\n", number);
    else
        if(number < chave)
            printf("O numero sorteado e maior que %d\n\n", number);
        else
             printf("Parabens! Voce acertou o numero em %d tentativas!\n\n", tentativa);
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
    else;
}