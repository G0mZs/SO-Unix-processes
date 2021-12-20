#include <stdio.h>
#include <stdlib.h>
#include "estruturas.h"
#include "cliente.h"
#include <string.h>
#include <getopt.h>
#include <pthread.h>
#include <ctype.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

jogadores tempinfo;
int logged = 0;
int comando = 0;
jogadores recebe;
int saida = 0;
int tam = 0;
int continua = 0;
int autoriza = 0;



void client_shutdown()
{
    char pipe[20];
    sprintf(pipe, PIPE_CLI, getpid());
    unlink(pipe);
    printf("\nPrograma Terminado\n");
    exit(0);
}

void server_shutdown()
{
    char pipe[20];
    sprintf(pipe, PIPE_CLI, getpid());
    unlink(pipe);
    exit(0);
}

void shutdown()
{
    int fd;
    jogadores manda;
    if (logged == 1)
    {
        manda.pid = getpid();
        manda.acao = CLIENT_SHUTDOWN;

        fd = open(PIPE, O_WRONLY, 0777);
        write(fd, &manda, sizeof(manda));
        client_shutdown();
    }
    else
    {
        client_shutdown();
    }
}


void *receber() // completar
{
    time_t hora;
    struct tm *loc_time;
    informacoes info;
    jogadores temp,envia;
    char pipe[20];
    int fd_pipe,fd_server;
    int timer = 0;
    int segundos = 0;
    pid_t processo;
    int resp;

    sprintf(pipe, PIPE_CLI, getpid());
    mkfifo(pipe, 0777);
    fd_pipe = open(pipe, O_RDWR, 0777);
    fd_server = open(PIPE,O_RDWR,0777);

    do
    {
        read(fd_pipe, &temp, sizeof(temp));

        switch (temp.acao)
        {
        case SERVER_SHUTDOWN:
            logged = 0;
            printf("\nPrograma Terminado!!\n");
            server_shutdown();
            break;
        case LOGGED:
            logged = 1;
            break;
        case LOGGEDFAILED:
            printf("\nO campeonato ja iniciou deverá aguardar ate que acabe !!\n");
            break;
            
        case PLAY:
            strcpy(tempinfo.game.identificacao,temp.game.identificacao); 
            break;

        case KICK:
            logged = 0;
            fflush(stdout);
            printf("\n\nVoce foi expulso do campeonato pelo arbitro!!Pressione enter para continuar...\n");

            break;
        case WAIT:
            tempinfo.aguarda = temp.aguarda;
            if(tempinfo.aguarda == 0)
            {
                system("clear");
                printf("\nO Campeonato começou !! Boa sorte :) \n\n");
                sleep(2);
            }
            break;
        case RESPOSTA:   
    
                system("clear");
                strcpy(tempinfo.game.identificacao,temp.game.identificacao);
                tempinfo.pid = temp.pid;
                strcpy(tempinfo.game.identificacao,temp.game.identificacao);
                tempinfo.game.id = temp.game.id;
                strcpy(tempinfo.game.pergunta,temp.game.pergunta);
                tempinfo.tentativa = temp.tentativa;
                tempinfo.comunicaJogo = temp.comunicaJogo;
                
                printf("%s",tempinfo.game.pergunta);
                fflush(stdout);
                autoriza = 1;
                
                do
                {
                    sleep(1);
                }
                while(autoriza == 1);

                tempinfo.acao = MANDARESPOSTA;
                write(fd_server,&tempinfo,sizeof(tempinfo));

            
        
            break;
        case SUSPENDE:

            printf("\nA comunicacao com o seu jogo foi suspendida pelo arbitro!! Aguarde...\n");
            break;
        case RETOMA:

            printf("\n\nComunicacao retomada!! Reencaminhando a resposta em 3 segundos...\n");
            sleep(3);
            break;
        case WINNER:
    
            break;
        case PONTUACAO:
            printf("\nOla\n");
            tempinfo.pontuacao = temp.pontuacao;
            
            break;
        }
    
    }while(1);

    close(fd_server);
    close(fd_pipe);
    unlink(pipe);
    pthread_exit(0);
}

void le_comandos()
{
    char comando[100], destino[100];
    unsigned int k, i, j;
    int fd, verifica = 0;
    jogadores send;
    int resp;
    int alfabeto=0,numero=0;

    fd = open(PIPE, O_RDWR);

    do
    { 
        if (logged != 1)
        {
            logged = 0;
            break;
        }
       
    
        printf("\nComando: ");
        fgets(comando, 100, stdin);
        comando[strlen(comando) - 1] = '\0';

        for (i = 0; i < strlen(comando) && comando[i] != ' '; i++)
            ;

        for (j = 0, k = i + 1; k < strlen(comando); k++, j++)
        {
            destino[j] = comando[k];
        }
        comando[i] = '\0';
        destino[j] = '\0';


    
        if (strcmp(comando, "#mygame") == 0 && tempinfo.aguarda == 0)
        {
            system("cls || clear");
            printf("Foi lhe atribuido o jogo %s",tempinfo.game.identificacao);
        }
        else if (strcmp(comando, "#quit") == 0 && tempinfo.aguarda == 0)
        {
            send.pid = getpid();
            send.acao = QUIT;
            write(fd, &send, sizeof(send));
            logged = 0;
            return;
        }
        else if (strcmp(comando, "#quit") == 0 && tempinfo.aguarda != 0)
        {
            printf("\nA espera que mais jogadores entrem!!Aguarde...\n\n");
        }
        else if (strcmp(comando, "#mygame") == 0 && tempinfo.aguarda != 0)
        {
            printf("\nA espera que mais jogadores entrem!!Aguarde...\n\n");
        }
        else if(strcmp(tempinfo.game.identificacao,"G_001") == 0 || strcmp(tempinfo.game.identificacao,"G_002") == 0 || strcmp(tempinfo.game.identificacao,"G_003") == 0)
        {


            for(int i = 0; comando[i] != '\0'; i++)
            {
               if(isalpha(comando[i]) != 0)
               {
                   alfabeto++;
        
               }
               else if(isdigit(comando[i]) != 0)
               {
                   numero++;
               } 
            }


            if(alfabeto == 0 && numero > 0)
            {    

                if(autoriza == 1)
                {
                    sleep(1);
                    strcpy(tempinfo.msgResposta,comando);
                    sleep(2);
                    autoriza = 0;
                }
               
            }
            else;
                
        }
        else if(strcmp(tempinfo.game.identificacao,"G_004") == 0)
        {
            for(int i = 0; comando[i] != '\0'; i++)
            {
               if(isalpha(comando[i]) != 0)
               {
                   alfabeto++;
        
               }
               else if(isdigit(comando[i]) != 0)
               {
                   numero++;
               } 
            }


            if(alfabeto > 0 && numero == 0)
            {    

                if(autoriza == 1)
                {
                    sleep(1);
                    strcpy(tempinfo.msgResposta,comando);
                    sleep(2);
                    autoriza = 0;
                }
               
            }
            else;
        }

        alfabeto = 0;
        numero = 0;
        memset(comando,0,sizeof(comando));

    } while (strcmp(comando, "#quit") != 0 || logged == 1);

}

void flush_in()
{
    int ch;
    do
    {
        ch = fgetc(stdin);

    } while (ch != EOF && ch != '\n');
}

void trataSig(int i)
{
    system("clear");
    printf("\n\nO Campeonato acabou !!Pressione enter para continuar...\n",tempinfo.pontuacao);
    logged = 0;
}

void trataSig2(int i)
{
    shutdown();
}

void trataSig3(int i)
{
    system("clear");
    printf("Foi expulso deste campeonato pelo arbitro!!Pressione enter para continuar...\n");
    logged = 0;
}

int main() // completar
{
    system("cls || clear");
    jogadores temp;
    char pipe[20], npipe[20];
    pthread_t tarefa;
    int res, fd;
    signal(SIGINT,trataSig2);
    signal(SIGUSR1,trataSig);
    signal(SIGUSR2,trataSig3);

    tempinfo.aguarda = 0;

    temp.pid = getpid();
    sprintf(npipe, PIPE_CLI, temp.pid);

    if (access(PIPE, F_OK) != 0)
    {
        printf("O servidor nao se encontra em execucao!\n");
        exit(0);
    }

    if (mkfifo(npipe, S_IRWXU) == -1)
    {
        printf("Erro a criar o pipe.A sair...\n");
        exit(0);
    }
    res = pthread_create(&tarefa, NULL, receber, NULL);
    if (res != 0)
    {
        perror("Erro a criar thread !!\n");
        unlink(pipe);
        exit(1);
    }

    fd = open(PIPE, O_RDWR);
    if (fd == -1)
    {
        printf("Erro ao abrir o pipe do servidor!!\n");
        exit(0);
    }

    do
    {
        printf("Insira o seu nome de jogador: ");
        fflush(stdout);
        scanf("%s", temp.nome);
        flush_in();
        temp.ativo = -1;
        temp.acao = LOGIN;
        write(fd, &temp, sizeof(temp));
        sleep(2);
        //continuar com a pthread..

        if (logged == 1)
        {
            le_comandos();
            sleep(1);
        }
       
    } while (logged == 0);

    close(fd);
    unlink(pipe);
    return 0;
}
