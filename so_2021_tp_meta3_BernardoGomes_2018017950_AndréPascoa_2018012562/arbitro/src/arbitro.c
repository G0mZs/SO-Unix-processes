#include <stdio.h>
#include <stdlib.h>
#include "arbitro.h"
#include "estruturas.h"
#include <string.h>
#include <getopt.h>
#include <pthread.h>
#include <ctype.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/syscall.h>

int childpid;
informacoes informacao;
jogadores players[MAXPLAYERS];
jogadores extra;
jogo games[TOTALJOGOS];
int inicia = 1;
int acabajogo = 0;

void preenche_array_jogos(jogo *game)//Preenche o array de jogos utilizado posteriormente
{
  strcpy(game[0].identificacao,"G_001");
  strcpy(game[0].nome, "Numero da Sorte");
  strcpy(game[0].informacao, "Adivinhar um numero entre 1 e 100");

  strcpy(game[1].identificacao,"G_002");
  strcpy(game[1].nome, "Jogo da Matematica");
  strcpy(game[1].informacao, "Adivinhar uma equacao aritmetica");

  strcpy(game[2].identificacao,"G_003");
  strcpy(game[2].nome, "Quiz");
  strcpy(game[2].informacao, "Responder a um breve quiz");

  strcpy(game[3].identificacao,"G_004");
  strcpy(game[3].nome, "Jogo das Traducoes!");
  strcpy(game[3].informacao, "Traduzir as palavras de ingles para portugues");
}

void mostra_Jogos(jogo *game)
{
  for (int i = 0; i < TOTALJOGOS; i++)
  {
    printf("Nome do Jogo: %s    Descricao: %s\n", game[i].nome, game[i].informacao);//MOstra os jogos
  }
}

int inserir_utilizador(char nome[], int pid, jogadores *jogador)//Inserir um utilizador no array
{
  int nomeaux[20];
  int i, j, st, x;

  for (i = 0; i < informacao.maxplayers; i++)
  {
    if ((jogador + i)->ativo == -1)
    {
      break;
    }
  }

  if (i == informacao.maxplayers)
  {
    printf("\nNao existem vagas para mais jogadores!!\n");
    fflush(stdout);
    return 0;
  }
  else
  {

    for (j = 0, st = 1, x = 0; j < informacao.maxplayers; j++)//Para os jogadores nao terem nomes iguais, se for introduzido um nome igual então o segundo nome fica com um 1 e assim por adiante,joao1 joao2etc
    {
      if ((jogador + j)->ativo == 1)

        if (strcmp((jogador + j)->nome, nome) == 0)
        {
          if (x)
            nome[strlen(nome) - 1] = '\0';
          sprintf(nomeaux, "%s%d", nome, st++);
          strcpy(nome, nomeaux);
          x = 1;
          j = -1;
        }
    }

    strcpy((jogador + i)->nome, nome);//Dados predifinidos dos jogadores do array
    (jogador + i)->pid = pid;
    (jogador + i)->ativo = 1;
    (jogador + i)->pontuacao = 0;
    (jogador + i)->tentativa = 0;
    (jogador + i)->aguarda = 1;
    (jogador + i)->comunicaJogo = 1;

    printf("\nO Jogador %s com o PID %d iniciou sessao com sucesso!!\n\n", nome, pid);
    return i;
  }
}

void verify_args(informacoes *informacao)
{

  if (informacao->duraCampeonato == 0)
  {
    informacao->duraCampeonato = DURCAMP;
  }
  if (informacao->tempoEspera == 0)
  {
    informacao->tempoEspera = TEMPOESPERA;
  }
}

void verify_env_var(informacoes *informacao) //Variaveis de ambiente gamedir e maxplayers
{

  if (getenv("MAXPLAYERS") != NULL)
  {
    int maxplayers = atoi(getenv("MAXPLAYERS"));
    if (maxplayers > MAXPLAYERS)
    {
      informacao->maxplayers = MAXPLAYERS;
    }
    else
    {
      informacao->maxplayers = atoi(getenv("MAXPLAYERS"));
    }
  }
  else
  {
    informacao->maxplayers = MAXPLAYERS;
  }

  if (getenv("GAMEDIR") != NULL)
  {
    informacao->gamedir = getenv("GAMEDIR");
  }
  else
  {
    informacao->gamedir = GAMEDIR;
  }
}

void mostrarJogadores(jogadores *j)
{
  int i;

  for (i = 0; i < MAXPLAYERS; i++)
  {
    if (j[i].ativo == 1)
    {
      printf("Nome:%s PID:%d \n", j[i].nome, j[i].pid); //Mostrar jogadores
    }
  }
}

int kickJogador(char nome[], jogadores *jogador) // Obter o pid de um jogador
{
  int i, armazenaID;

  for (i = 0; i < MAXPLAYERS; i++)
  {
    if (strcmp(jogador[i].nome, nome) == 0)
    {
      armazenaID = jogador[i].pid;
      jogador[i].ativo = -1;
    }
  }

  return armazenaID;
}

int conta_utilizadores(jogadores *jogador) //Numero de utilizadores no array
{
  int conta = 0;
  for (int i = 0; i < MAXPLAYERS; i++)
  {
    if (jogador[i].ativo == 1)
    {
      conta++;
    }
  }

  return conta;
}

int jogo_aleatorio() // Random entre 0 e 3
{
  int numero;

  srand(time(NULL));
  numero = (rand() %4);

  return numero;
}

void *iniciagames(void *a)//Thread responsavel pela criacao de um processo child que vai executar um novo programa que neste caso vai ser o jogo e redirecionar a stdin e stdout para os dup2
{

  int PID = *((int *) a);
  int p_in[2],p_stdout[2];
  char lpipe[40];
  int fd3,fd4;
  pid_t processo;
  jogadores enviacli,recebe,recebecli;
  char inicio[200]; 
  char foo[200];
  char identificacao[20];
  char jogo[100];
  int exitstatus;
  


      pipe(p_in);  
      pipe(p_stdout);


      processo = fork();

      if(processo < 0)
      {
        perror("fork");
        return 1;
      }
      if(processo == 0)
      {
       

        if(dup2(p_in[0],STDIN_FILENO) == -1)
        {
          exit(errno);
        }
    
      
        if(dup2(p_stdout[1],STDOUT_FILENO) == -1)
        {
          exit(errno);
        }

        
        close(p_in[0]);
        close(p_in[1]);
        close(p_stdout[0]);
        close(p_stdout[1]);

       
        for(int i = 0; i < MAXPLAYERS; i++)
        {
          if(players[i].pid == PID)
          {
            strcpy(identificacao,players[i].game.identificacao);
            
          }
        }
          strcpy(jogo,GAMEDIR);
          strcat(jogo,identificacao);

          char *args[] = {identificacao,NULL}; ///home/bernardo/Desktop/so_2021_tp_meta1_BernardoGomes_2018017950_AndréPascoa_2018012562/out
        if(execv(jogo,args) == -1)//output escrito no p_out
        {
          perror("Erro no jogo!!");
        }

        
      
      }
      if(processo > 0)
      { 
        sleep(1);

        do    
        {
          
          
      
          int n = read(p_stdout[0],foo,sizeof(foo));//Le os dados do stdout 
          foo[n-1] = '\0';

           for(int i = 0; i < MAXPLAYERS; i++)
            {
              if(players[i].pid == PID)
              {
                players[i].game.id = processo;//Passa o child process id para o id do jogo na estrutura
                strcpy(enviacli.game.identificacao,players[i].game.identificacao);
              }
            }
    
            
            sprintf(lpipe,PIPE_CLI,PID);
            fd3 = open(lpipe,O_RDWR);
            enviacli.pid = PID;
            enviacli.acao = RESPOSTA;
            strcpy(enviacli.game.pergunta,foo);
          
            

            
            write(fd3,&enviacli,sizeof(enviacli));//Comunica ao cliente o stdout que por sua vez vai pedir o stdin
            sleep(1);
            
            fd4 = open(PIPE,O_RDWR);
            
            
            for(int i = 0; i < MAXPLAYERS; i++)
            {
              if(players[i].pid == PID)
              {
                
                do
                {
                  int status;

                  waitpid(processo,&status,WNOHANG);//Le o exitstatus enquanto nao recebe uma resposta do cliente(WNOHANG serve para poder executar o waitpid sem esttar a espera que o child acabe)
                  if(WIFEXITED(status))
                  {
                     exitstatus = WEXITSTATUS(status);
                   
                     players[i].pontuacao = exitstatus;//Passa o exitstatus do jogo(pontuacao para a estrutura dos players)
                 
                  }
                  sleep(1);

                }while (strcmp(players[i].msgResposta,"") == 0 || strcmp(players[i].msgResposta,"0") == 0);
               

                if(players[i].comunicaJogo == 0)//Se a comunicacao tiver sido cortada envia mensagem ao cliente do acontecido e fica num sleep infinito ate serem retomadas pelo arbitro
                {
                   enviacli.acao = SUSPENDE; 
                   write(fd3,&enviacli,sizeof(enviacli));

                    do
                  {

                    sleep(1);

                  }while (players[i].comunicaJogo == 0);
                }

                  write(p_in[1],players[i].msgResposta,strlen(players[i].msgResposta)); // Depois de receber a resposta do cliente vai escrever no pipe relativo ao stdin a sua resposta
                  write(p_in[1],"\n",1);//O \n serve para nao ter de usar o enter ao receber dados
                  strcpy(players[i].msgResposta,"");//Mete a resposta nula(nao é necessario fazer isto)

              }
            }

        }while(inicia == 0);

        close(p_in[0]);//Fecha os unnamed pipes relativos a stdin e stdout
        close(p_in[1]);
        close(p_stdout[0]);
        close(p_stdout[1]);
          
        
        
      }
      else
      {
        close(p_in[0]);
        close(p_in[1]);
        close(p_stdout[0]);
        close(p_stdout[1]);
      }
      close(fd3);
      
  
}

void *tempo() //Thread responsavel pela manutencao da duraçao do campeonato
{

  time_t hora;
  struct tm *loc_time;
  int timer = 0;
  int timerminutos = 0;
  int segundos = 0;
  int minutos = 0;
  int fd2;
  char pipe2[20];
  jogadores enviafinal;
  int maior = 0;

      hora = time(NULL);
      loc_time = localtime(&hora); // obtem a hora atual
      segundos = loc_time->tm_sec; //obtem os segundos no inicio do campeonato
      minutos = loc_time->tm_min;
       int x = floor((segundos + informacao.duraCampeonato )/60); //obtem os minutos arredondados ou seja , 90 segundos arredonda para 1 minuto enquanto que 120 arredonda para 2
       int aux = (x*60) + 1; //obtem os segundos dos minutos obtidos em cima
     
      if(segundos + informacao.duraCampeonato > 59) // se a soma dos dois for maior que 59 
      {
          timer = segundos + informacao.duraCampeonato - aux; // pega no tempo total e divide o aux ou seja se o campeonato começar no segundo 35 ele vai parar no minuto asseguir no segundo 34
          timerminutos = minutos + x; // soma aos minutos obtidos inicialmente os minutos(x) que vai ter de durar
      }
      else
      {
          timer = segundos + informacao.duraCampeonato; // o timer fica com o tempo previsto acabar
          timerminutos = minutos;
      }
      while(inicia == 0)
      {
          hora = time(NULL);
          loc_time = localtime(&hora);
          segundos = loc_time->tm_sec;
          

          
  
          if(loc_time->tm_sec == timer && loc_time->tm_min == timerminutos) //quando os minutos atuais e segundos forem iguais aos definidos pelos timers acima entao o campeonato acaba e manda um sinal sigusr1 aos clientes e aos jogos para terminarem
          {
        
              inicia = 1;
              
              for(int i = 0; i < MAXPLAYERS; i++)
              {
                if(players[i].ativo == 1)
                {
                  kill(players[i].game.id,SIGUSR1);//Comunica ao jogo que tem de acabar
                  sleep(1);
                  printf("\n\nDados do Jogo:\nNome: %s\nPontuacao do Jogo: %d\n\n",players[i].nome,players[i].pontuacao);
                  sleep(1);
                  
                }
                
              }

              printf("\n\nO Campeonato acabou !!Pressione enter para continuar!!\n\n");

              for(int i = 0;i < MAXPLAYERS; i++)
              {
                if(players[i].ativo == 1)
                {
                  kill(players[i].pid,SIGUSR1);//Comunica aos clientes o fim do campeonato e por sua vez mete os 2 players "inativos"
                  players[i].ativo = -1;
                  sleep(1); 
                  
                }
              }
          }
      }

  close(fd2);
  pthread_exit(0);
}


void *recebeu()//Thread auxiliar para a comunicacao entre cliente/arbitro atraves de cases 
{
  //pthread_mutex_lock(&lock);
  jogadores receive, send,envia;
  int fd, fd_send,fd_jogo;
  int guarda;
  char pipe[20],jpipe[40];


  for (int j = 0; j < MAXPLAYERS; j++)
  {
    players[j].ativo = -1;
  }

  mkfifo(PIPE, 0777);
  fd = open(PIPE, O_RDWR);

  do
  {
    read(fd, &receive, sizeof(receive));



    strcpy(extra.nome, receive.nome);
    extra.pid = receive.pid;

    

    switch (receive.acao)
    {
    case LOGIN:

      sprintf(pipe, PIPE_CLI, receive.pid);
      fd_send = open(pipe, O_RDWR);
      guarda = inserir_utilizador(receive.nome, receive.pid, players);
      send.acao = LOGGED;
      write(fd_send, &send, sizeof(send));
   
    
      break;
    case CLIENT_SHUTDOWN:
      for(int i = 0; i < MAXPLAYERS; i++)
      {
        if(players[i].pid == receive.pid)
        {
          players[i].ativo = -1;
        }
      }
       printf("O utilizador com o PID %d saiu do programa!\n",receive.pid);
      break;
    case QUIT:
      for (int o = 0; o < MAXPLAYERS; o++)
      {
        if (players[o].pid == receive.pid)
        {
          players[o].ativo = -1;
          printf("\nO player %s acabou de sair do campeonato!!\n\n",players[o].nome);
        }
      }
      break;
    case MANDARESPOSTA:
    
      for(int i = 0; i < MAXPLAYERS; i++)
      {
        if(players[i].pid == receive.pid)
        {
          strcpy(players[i].msgResposta,receive.msgResposta);
          
        }
      }
      break;
    }
  } while (1);
  close(fd_send);
  close(fd);
  pthread_exit(0);
}

char *remove_novalinha(char *comando)
{
  int length = strlen(comando);

  if(length > 1 && comando[length -1] == '\n')
  {
    comando[length - 1] = '\0';
  }
  return comando;
}

void DividirComando(char com[])//Funcao para retirar a primeira letra do comando introduzido
{
  char comando[20];
  int i = 1;
  while (com[i] != '\0')
  {
    comando[i - 1] = com[i];
    i++;
  }

  comando[i - 1] = '\0';
  i = 0;

  while (comando[i] != '\0')
  {
    com[i] = comando[i];
    i++;
  }

  com[i] = '\0';
}

void flush_in()
{
    int ch;
    do
    {
        ch = fgetc(stdin);
    } while (ch != EOF && ch != '\n');
}

int main(int argc, char **argv)
{
  system("cls || clear");
  preenche_array_jogos(games);
  int opt = 0;
  int fd_pipe, res,res1,res2;
  pthread_t thread,temporizador,jogos;
  informacao.duraCampeonato = 0;
  informacao.tempoEspera = 0;
  int nrandom = 0;

  while ((opt = getopt(argc, argv, "t:d:")) != -1) // dois pontos criam obrigatoriedade de ter argumento,onde se coloca os dados do tempo de espera e dur campeonato
  {

    switch (opt)
    {
    case 'd':
      informacao.duraCampeonato = atoi(optarg);

      break;
    case 't':
      informacao.tempoEspera = atoi(optarg);
      break;
    }
  }

  verify_args(&informacao);

  verify_env_var(&informacao);

  printf("Dados Iniciais do Campeonato:\n\n");
  printf("<------------------------------------------------------------------------------>\n\n");
  printf("Tempo de espera: %d\n", informacao.tempoEspera);
  printf("Duracao do campeonato: %d\n", informacao.duraCampeonato);
  printf("Numero maximo de players: %d\n", informacao.maxplayers);
  printf("Gamedir: %s\n\n", informacao.gamedir);
  printf("<------------------------------------------------------------------------------>\n");

  if (access(PIPE, F_OK) == 0)
  { // verifica se já existe o PIPE
    printf("Já se encontra um servidor em execução. A sair...\n");
    exit(0);
  }
  if (mkfifo(PIPE, S_IRWXU) < 0)
  { // cria o pipe
    printf("Erro ao criar pipe. A sair...\n");
    exit(0);
  }

  res = pthread_create(&thread, NULL, &recebeu, NULL);
  if (res != 0)
  {
    perror("ERRO! A criar a thread!!!\n");
    exit(1);
  }

  // vai abrir o pipe para leitura/escrita
  fd_pipe = open(PIPE, O_RDWR);
  if (fd_pipe == -1)
  {
    printf("Erro ao abrir pipe. A sair...\n");
    exit(0);
  }
  //system("clear"); // LIMPA A CONSOLA
  printf("\n[Servidor iniciado!]\n");

  // Thread para auxiliar leitura de dados do cliente
  /*if (pthread_mutex_init(&lock, NULL) != 0)
  {
    printf("Mutex init failed!\n");
    exit(1);
  }*/

  char comando[20], destino[20];
  unsigned int k, i, j;
  int fd,ch;
  char npipe[20];
  char game[40];
  char identificacao[20] = "G_000";
  char c;
  

  do
  {
       printf("\nA espera que dois jogadores iniciem sessão!!!\n");
    do
    {
    
      if (conta_utilizadores(players) == 0)
      {
        for (int a = 0; a < MAXPLAYERS; a++)
        {
          sprintf(npipe, PIPE_CLI, players[a].pid);
          fd = open(npipe, O_RDWR);
          players[a].acao = WAIT;
          players[a].aguarda = -1;
          write(fd, &players, sizeof(players));
        }
      }
      sleep(2);
      if (conta_utilizadores(players) == 1)
      {
        printf("\nA espera que um jogador inicie sessão!!!\n");
        for (int a = 0; a < MAXPLAYERS; a++)
        {
          sprintf(npipe, PIPE_CLI, players[a].pid);
          fd = open(npipe, O_RDWR);
          players[a].acao = WAIT;
          players[a].aguarda = -1;
          write(fd, &players, sizeof(players));
        }
        sleep(5);
      }

    } while (conta_utilizadores(players) < 2);
      
    if (conta_utilizadores(players) == 2)
    {
      for (int a = 0; a < MAXPLAYERS; a++)
      {
        sprintf(npipe, PIPE_CLI, players[a].pid);
        fd = open(npipe, O_RDWR);
        players[a].acao = WAIT;
        players[a].aguarda = -1;
        write(fd, &players, sizeof(players));
      }
      printf("\n\nAguardando um tempo para que entrem mais jogadores!!\n");
      sleep(informacao.tempoEspera);
    }

    system("clear");
    printf("\n\nIniciando  o campeonato !!\n\n"); 
    inicia = 0;


    for (int c = 0; c < MAXPLAYERS; c++)
    {
      if (players[c].ativo == 1)
      {
        sprintf(npipe, PIPE_CLI, players[c].pid);
        fd = open(npipe, O_RDWR);
        players[c].acao = WAIT;
        players[c].aguarda = 0;
        write(fd, &players[c], sizeof(players[c]));
      }
    }

    do
    {
      
      for (int o = 0; o < MAXPLAYERS; o++)
      {
        if (players[o].ativo == 1)
        {
          nrandom = jogo_aleatorio();
        
          
          strcpy(game,games[nrandom].identificacao);
         

          sprintf(npipe, PIPE_CLI, players[o].pid);
          fd = open(npipe, O_RDWR);
          strcpy(players[o].game.identificacao, game);
          players[o].acao = PLAY;

          write(fd, &players[o], sizeof(players[o]));
          sleep(2);
        }
        
      }

    res1 = pthread_create(&temporizador,NULL,tempo,NULL);
    if (res1 != 0)
    {
      perror("ERRO! A criar a thread!!!\n");
      exit(1);
    }

  for(int i = 0; i < MAXPLAYERS; i++)
  {
    if(players[i].ativo == 1)
    {
      int *pid = malloc(sizeof(*pid));
      *pid = players[i].pid;
      res2= pthread_create(&jogos,NULL,iniciagames,pid);
      if(res2 != 0)
      {
        perror("ERRO! A criar a thread!!!\n");
        exit(1);
      }
    }
  }

      while (strcmp(comando, "exit") != 0 && inicia == 0)
      {
       

        printf("\nComando: ");
        fgets(comando, 20, stdin);
        comando[strlen(comando) - 1] = '\0';

        for (i = 0; i < strlen(comando) && comando[i] != ' '; i++)
            ;

        for (j = 0, k = i + 1; k < strlen(comando); k++, j++)
        {
            destino[j] = comando[k];
        }
        comando[i] = '\0';
        destino[j] = '\0';
      

        if (strcmp(comando, "players") == 0)
        {
          printf("\n\nJogadores: \n\n");
          mostrarJogadores(players);
        }
        else if (strcmp(comando, "games") == 0)
        {
          printf("\nJogos Disponiveis: \n\n");
          mostra_Jogos(games);
        }
        else if (strcmp(comando, "exit") == 0)
        {
          for (int k = 0; k < MAXPLAYERS; k++)
          {
            if (players[k].ativo == 1)
            {
              sprintf(npipe, PIPE_CLI, players[k].pid);
              fd = open(npipe, O_RDWR);
              players[k].acao = SERVER_SHUTDOWN;
              write(fd, &players, sizeof(players));
              printf("O Jogador %s terminou com sucesso!\n", players[k].nome);
              sleep(1);
              
            }
          }
          printf("\nO Arbitro foi terminado!\n");
          exit(0);
        }
        else if(strcmp(comando,"end") == 0)
        {
            printf("\n\nO Campeonato acabou !!Pressione enter para continuar!!\n\n");
              inicia = 1;
              for(int i = 0; i < MAXPLAYERS; i++)
              {
                if(players[i].ativo == 1)
                {
                  players[i].ativo = -1;
                  kill(players[i].game.id,SIGUSR1);
                  sleep(1);
                  printf("\n\nDados do Jogo:\nNome: %s\nPontuacao do Jogo: %d\n\n",players[i].nome,players[i].pontuacao);
                  sleep(1); 
                  kill(players[i].pid,SIGUSR1);
                }
                else;
              }
        }
        else
        {
          if (comando[0] == 'k')
          {
            DividirComando(comando);

            jogadores temp;

            temp.pid = kickJogador(comando, players);

            for (int l = 0; l < MAXPLAYERS; l++)
            {
              if (players[l].pid == temp.pid)
              {
                players[l].ativo = -1;
                kill(players[l].game.id,SIGUSR1);//Mudar este kill para cima do write :)
                sleep(2);
                kill(players[l].pid,SIGUSR2);
                printf("O Jogador %s foi expulso com sucesso!\n", players[l].nome);
              }
            }
            
          }
          if(comando[0] == 's')
          {
            DividirComando(comando);

            for (int  i = 0; i < MAXPLAYERS; i++)
            {
              if(strcmp(players[i].nome,comando) == 0)
              {
                players[i].comunicaJogo = 0;
                
              }
            }
            
          }
          if(comando[0] == 'r')
          {
            DividirComando(comando);

            jogadores temp;

            for(int i = 0; i < MAXPLAYERS; i++)
            {
              if(strcmp(players[i].nome,comando) == 0)
              {
                players[i].comunicaJogo = 1;
                sprintf(npipe,PIPE_CLI,players[i].pid);
                fd = open(npipe,O_RDWR);
                temp.acao = RETOMA;
                write(fd,&temp,sizeof(temp));
              }
            }
          }
        }
      }
      
       
    } while (conta_utilizadores(players) >= 2 && inicia == 0);

    inicia = 1;
      
    
  } while (inicia == 1);

  close(fd_pipe);
  unlink(PIPE);
  return 0;
}