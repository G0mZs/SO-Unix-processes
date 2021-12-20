#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "estruturas.h"

#include <string.h>
#include <getopt.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

int pergunta=1, pontos = 0;
jogadores jogador;

void dicas(int resposta, int certa, int tentativa);
void dicasfinal(int resposta, int certa, int tentativa, int iteracao);

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
        certa,
        tentativa,
        resposta;
        

    
    do
    {

        printf("Vai começar o Quiz!!\n\n");
        tentativa = 0;
        iteracao = 0;
        pergunta = 1;
        do
        {
            do  //pergunta 1
            {
                certa=1;
                tentativa++;
                iteracao++;
                printf("Tentativa %d de resposta a pergunta %d: ", tentativa, pergunta);
                printf("Quem foi o Primeiro Rei de Portugal?\n 1:Dom Afonso Henriques\n 2:Tino de Rãs\n 3:Maria Leal\n 4:Bernardo Gomes\n");
                fflush(stdout);
                scanf("%d", &resposta);
                
                dicas(resposta,certa,tentativa);

            } while(resposta!= certa);

        tentativa=0;
        pergunta++;
            
            do  //pergunta 2
            {
                certa=1;
                tentativa++;
                iteracao++;
                printf("Tentativa %d de resposta a pergunta %d: ", tentativa, pergunta);
                printf("Quantos fusos horários tem a Russia?\n 1: 11\n 2: 24\n 3: 2\n 4: 7\n");
                fflush(stdout);
                scanf("%d", &resposta);
                dicas(resposta,certa,tentativa);

            } while(resposta!= certa);
        tentativa=0;  
        pergunta++;

            do  //pergunta 3
            {
                certa=4;
                tentativa++;
                iteracao++;
                printf("Tentativa %d de resposta a pergunta %d: ", tentativa, pergunta);
                printf("Qual o lugar mais profundo do mundo?\n 1: Mponeng \n 2:Lago Baikal\n 3:Mar Vermelho\n 4:Fossa das Marianas\n");
                fflush(stdout);
                scanf("%d", &resposta);
                dicas(resposta,certa,tentativa);

            } while(resposta!= certa);
        tentativa=0;
        pergunta++;  

            do  //pergunta 4
            {
                certa=3;
                tentativa++;
                iteracao++;
                printf("Tentativa %d de resposta a pergunta %d: ", tentativa, pergunta);
                printf("Quando é que abriu o metropolitano de Londres?\n 1: 1780\n 2: 2002\n 3: 1863\n 4: 1994\n");
                fflush(stdout);
                scanf("%d", &resposta);
                dicas(resposta,certa,tentativa);

            } while(resposta!= certa);
        tentativa=1;
        pergunta++;    

            do  // pergunta 5
            {
                certa=2;
                tentativa++;
                iteracao++;
                printf("Tentativa %d de resposta a pergunta %d: ", tentativa, pergunta);
                printf("De que cidade são os Beatles?\n 1: Aveiro\n 2: Liverpool\n 3: Madrid\n 4: Londres\n");
                fflush(stdout);
                scanf("%d", &resposta);
                dicasfinal(resposta,certa,tentativa,iteracao);

            } while(resposta!= certa);
        pergunta++;       


        }
        while( pergunta <= 5);
        jogador.pontuacao = jogador.pontuacao + pontuacao(tentativa);
       
    }
    while(continuar == 1); // completar aqui com a duracao de campeonato ou seja while(durCampeonato < x)

      
}

void dicas(int resposta, int certa, int tentativa)
{
    if(resposta == certa)
        printf("Parabens! A resposta certa era %d e acertou em %d tentativas!\n\n", certa, tentativa);           
    else
        printf("Infelizmente a resposta não é %d\n\n", resposta);   
}

void dicasfinal(int resposta, int certa, int tentativa, int iteracao)
{
    if(resposta != certa)
            printf("Infelizmente a resposta não é %d\n\n", resposta);
        else
            printf("Parabens! A resposta certa era %d e acertou em %d tentativas!\n\n", certa, tentativa);
            printf("O Quiz foi compleatado em %d tentativas!\n\n", iteracao);
}
int pontuacao(int iteracao)
{
    if(iteracao >= 14)
    {
        int pontuacao = 1;
        return pontuacao;
    }
    if(iteracao >= 10 && iteracao < 14)
    {
        int pontuacao = 2;
        return pontuacao;
    }
    if(iteracao >= 7 && iteracao < 9)
    {
        int pontuacao = 3;
        return pontuacao;
    }
    if(iteracao ==6)
    {
        int pontuacao = 8;
        return pontuacao;
    }
    if(iteracao == 5)
    {
        int pontuacao = 10;
        return pontuacao;
    }
    else;
}