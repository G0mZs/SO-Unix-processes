#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "estruturas.h"
#include <string.h>
#include <getopt.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

int tipo=0, num1=0, num2=0 , pontos = 0;
jogadores jogador;

int aleatorio();
int aleatorio2();
int aleatorio3();
void dicas(int number, int chave, int tentativa);

void trataSigEnd(int i)
{
    exit(jogador.pontuacao);
}


int main()
{
    srand( (unsigned)time(NULL) );
    pontos=0;
    signal(SIGUSR1,trataSigEnd);
    int continuar=1,
        iteracao=0,
        chave=0,
        tentativa,
        number;
        

    do
    {
        
        printf("Escolhendo uma equação para advinhar\n");
        num1 = aleatorio();
        num2 = aleatorio2();
        tipo = aleatorio3();

        printf("Comecou! Tente adivinhar o numero!\n\n");
        tentativa = 0;

            if(tipo==1){
                do
                {
                    tentativa++;
                    chave=num1+num2;
                    printf("Tentativa %d: ", tentativa);
                    printf("Qual a solução da equação %d + %d: ",num1,num2);
                    fflush(stdout);
                    scanf("%d", &number);
                    dicas(number,chave,tentativa);
                
                }while (number != chave);
            }

            if(tipo==2){
                do
                {
                    tentativa++;
                    chave=num1-num2;
                    printf("Tentativa %d: ", tentativa);
                    printf("Qual a solução da equação %d - %d: ",num1,num2);
                    fflush(stdout);
                    scanf("%d", &number);
                    dicas(number,chave,tentativa);
                }while (number != chave);
            }

            if(tipo==3){
                do
                {
                    tentativa++;
                    chave=num1*num2;
                    printf("Tentativa %d: ", tentativa);
                    printf("Qual a solução da equação %d * %d: ",num1,num2);
                    fflush(stdout);
                    scanf("%d", &number);
                    dicas(number,chave,tentativa);
                } while (number != chave);
            }

            if(tipo==4){
                do
                {
                    tentativa++;
                    chave=num1/num2;
                    printf("Tentativa %d: ", tentativa);
                    printf("Qual a solução da equação %d / %d: ",num1,num2);
                    fflush(stdout);
                    scanf("%d", &number);
                    dicas(number,chave,tentativa);
                } while (number != chave);
            }
        jogador.pontuacao = jogador.pontuacao + pontuacao(tentativa);
       
    }
    while(continuar == 1); // completar aqui com a duracao de campeonato ou seja while(durCampeonato < x)

      
}

int aleatorio()
{
    

    return (1 + rand()%1000);
}

int aleatorio2()
{
    

    return (1 + rand()%1000);
}

int aleatorio3()
{
  

    return (1 + rand()%4);
}

void dicas(int number, int chave, int tentativa)
{
    if(number == chave)
                printf("Parabens! Voce acertou o numero em %d tentativas!\n\n", tentativa);
                
        else
                 printf("Infelizmente a resultado da equação não é: %d\n", number);
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