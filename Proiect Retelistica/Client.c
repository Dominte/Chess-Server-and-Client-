#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>
#include <fcntl.h>
#include <stdbool.h>
#include "GameRoom.h"
#define PORT 4359

struct A7{
    int x;
    int y;
    int xdest;
    int ydest;
    int trimisa;
};

struct rege{
    int x;
    int y;
};

char boardAspect[][sizeof("♜ ♞ ♝ ♛ ♚ ♝ ♞ ♜")] = {
    {"♜ ♞ ♝ ♛ ♚ ♝ ♞ ♜"},
    {"♟ ♟ ♟ ♟ ♟ ♟ ♟ ♟"},
    {"… … … … … … … …"},
    {"… … … … … … … …"},
    {"… … … … … … … …"},
    {"… … … … … … … …"},
    {"♙ ♙ ♙ ♙ ♙ ♙ ♙ ♙"},
    {"♖ ♘ ♗ ♕ ♔ ♗ ♘ ♖"}
    }; 



int main()
{
    struct sockaddr_in server;
    int socket_descriptor;
    int Logged=0;
    char name[256];
    int OK=0;
    int Number=0;
    char comandaJucator[256];
    char comanda[256];
    struct A7 MutareCoord;
    int MasaJoc=0;
    char Mutare[256];
    int Jucator=0; //culoare
    int Oponent=0;
    struct rege Rege;
    int onGoingGame=1;
    int boardGame2[ROWS][COLLUMNS]={
        {4,3,2,8,9,2,3,4,},
        {1,1,1,1,1,1,1,1,},
        {0,0,0,0,0,0,0,0,},
        {0,0,0,0,0,0,0,0,},
        {0,0,0,0,0,0,0,0,},
        {0,0,0,0,0,0,0,0,},
    {-1,-1,-1,-1,-1,-1,-1,-1,},
    {-4,-3,-2,-8,-9,-2,-3,-4,},
    };



    memset(&server,0,sizeof(struct sockaddr_in));
    server.sin_family=AF_INET; //formatul de conexiune
    server.sin_addr.s_addr=inet_addr("127.0.0.1");
    server.sin_port=htons(PORT);

    if(-1==(socket_descriptor=socket(AF_INET,SOCK_STREAM,0)))
    {
    perror("Error socket client");
    return 1;
    }
    /* Client init~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    if(-1==connect(socket_descriptor,(struct sockaddr*)&server,sizeof(struct sockaddr)))
    {
    perror("Error connect client");
    return 2;
    }

    
    printf("%s","Introduceti Username-ul \n");
    //read(0,name,256);
    
    while(1)
    {
        memset(name,0,sizeof(char)*256);
        scanf("%s",name);
        Oponent=0;
        Jucator=0;

        if(0 >= write(socket_descriptor,name,sizeof(char)*256))
        {
            perror("Error write in client");
            return 3;
        }

        if(0>=read(socket_descriptor,&OK,sizeof(int)))
        {
            perror("Error read in client");
            return 4;
        }

        if(OK==1)
        {
            printf("%s","Sunteti logged cu user-ul: "); printf("%s",name); printf("\n");
            Logged=1;
            while(Logged)
            { 
                restart:
                onGoingGame=0;
                printf("Scrieti comanda dorita \n");
                memset(&MasaJoc,0,sizeof(int));
                memset(comanda,0,sizeof(char)*256);
                memset(&Jucator,0,sizeof(int));
                scanf("%s",comanda);
                if (strcmp(comanda,"Quit")==0)
                {
                    printf("Logging out...\n");fflush(stdout);
                    if(0>=write(socket_descriptor,comanda,sizeof(char)*256))
                    return 0; 
                    return 0;
                }
                else if(strcmp(comanda,"InfoAboutTable")==0)
                {
                    int Status=0;
                    int ParametruGresit=1;
                    if(0>=write(socket_descriptor,comanda,sizeof(char)*256))
                    return 0;
                    while(ParametruGresit==1){
                    memset(&MasaJoc,0,sizeof(int));
                    printf("%s","Alegeti camera de la 1-30...\n"); 
                    (scanf("%d%1[\n]", &MasaJoc, (char [2]){ 0 }) < 2);
                    if(MasaJoc<1||MasaJoc>30)
                    {
                        printf("Nu exista aceasta masa\n");
                    }
                    else ParametruGresit=0;
                    }

                    if(0>=write(socket_descriptor,&MasaJoc,sizeof(int)))
                    return 0;

                    if(0>=read(socket_descriptor,&Status,sizeof(int)))
                    return 0;
                    if(Status==1)
                    {
                    printf("Masa "); 
                    printf("%d",MasaJoc); 
                    fflush(stdout); 
                    printf(" este libera...\n");
                    }
                    if(Status==2)
                    {
                    printf("Masa "); 
                    printf("%d",MasaJoc); 
                    fflush(stdout); 
                    printf(" are un singur jucator...\n");
                    }
                    if(Status==3)
                    {
                    printf("Masa "); 
                    printf("%d",MasaJoc); 
                    fflush(stdout); 
                    printf(" este plina...\n");
                    }
                }
                else if(strcmp(comanda,"FindRandomGame")==0) // raspunde serveru
                {
                    printf("%s","Finding random game...\n");
                    if(0>=write(socket_descriptor,comanda,sizeof(char)*256))
                    return 0; //scriu instructiunea1
                    memset(&MasaJoc,0,sizeof(int));
                 
                    if(0>=read(socket_descriptor,&MasaJoc,sizeof(int))) //citesc raspunsu1
                    {
                    perror("Error read in client");
                    return 4;
                    } 
                  
                   
                   
                    if(MasaJoc==-1)
                    {
                        printf("%s","Nu exista masa libera");
                    }
                    else 
                    {
                        printf("%s","Sunteti asignati la masa ");
                        printf("%d",MasaJoc); fflush(stdout);
                        printf("\n");
                    }

                   

                    if(0>=read(socket_descriptor,&Jucator,sizeof(int)))
                    return 0;

                    if(Jucator==1)
                    {   
                       system("clear");
                       printf("User: "); printf("%s",name); printf("\n");
                        printf("%s","Jucatorul 1, jucati cu piesele albe... \n"); fflush(stdout);
                        printf("%s","Asteptati adversar... \n"); fflush(stdout);
                        Jucator=Alb;
                        if(0>=read(socket_descriptor,&Oponent,sizeof(int)))
                            return 0;
                        printf("Am gasit jucator. Partida va incepe in curand"); printf("\n");

                       
                    }
                    else if(Jucator==2)
                    {
                      system("clear");
                      printf("User: "); printf("%s",name); printf("\n");
                        printf("%s","Jucatorul 2, jucati cu piesele negre \n");  fflush(stdout);
                        Jucator=Negru;
                       
                    }
                    
                     goto game;

                }
                else if(strcmp(comanda,"ShowTable")==0) 
                {   
                  system("clear");
                  printf("User: "); printf("%s",name); printf("\n");
                    initBoard(boardGame2);
                    showBoard(boardGame2);
                    printf("\n");
                    showBoard2(boardGame2); 
                }
                else if(strcmp(comanda,"FindGame")==0) //FIND GAME X
                {
                    int ParametruGresit=1;
                    if(0>=write(socket_descriptor,comanda,sizeof(char)*256))
                    return 0;
                    
                    printf("%s","Alegeti camera de la 1-30...\n"); 
                    (scanf("%d%1[\n]", &MasaJoc, (char [2]){ 0 }) < 2);
                   
                    

                  if(0>=write(socket_descriptor,&MasaJoc,sizeof(int)))
                   return 0;
                   
                    
                  if(0>=read(socket_descriptor,&Jucator,sizeof(int)))
                    return 0;

                    if(Jucator==1)
                    {
                        system("clear");
                        printf("%s","Ati ales masa ") ; printf("%d",MasaJoc); printf("\n");
                        printf("User: "); printf("%s",name); printf("\n");
                        printf("%s","Jucatorul 1, jucati cu piesele albe... \n"); fflush(stdout);
                        printf("%s","Asteptati adversar... \n"); fflush(stdout);
                        Jucator=Alb;
                      if(0>=read(socket_descriptor,&Oponent,sizeof(int)))
                        return 0;

                         printf("Am gasit jucator. Partida va incepe in curand"); printf("\n");
                    }
                    else if (Jucator==2)
                    {
                        system("clear");
                        printf("User: "); printf("%s",name); printf("\n");
                        printf("%s","Ati ales masa ") ; printf("%d",MasaJoc); printf("\n");
                        printf("%s","Jucatorul 2, jucati cu piesele negre \n");  fflush(stdout);
                        Jucator=Negru;
                    }
                    else
                    {
                        printf("%s", "Masa aleasa este deja ocupata sau nu exista\n"); fflush(stdout);
                        Jucator=0;
                    }
                    game:

                    if(Jucator==Alb){ //player 1
                    initBoard(boardGame2);
                    Rege.x=7;
                    Rege.y=4;
                    int trimis=0;
                    onGoingGame=1;
                    int kingMoved=0;

                    showBoard(boardGame2); 
                    while(onGoingGame)
                    {
                      
                        
                        
                        memset(Mutare,0,sizeof(char)*256);
                        memset(comandaJucator,0,sizeof(char)*256);

                        

                        printf("Scrieti mutarea sub formatul clasic \n"); fflush(stdout);
                        scanf("%s",comandaJucator);
                        if(strcmp(comandaJucator,"forfeit")==0)
                        {
                            printf("Ati cedat partida. Jucator 2 castiga \n\n");
                            if(0>=write(socket_descriptor,&comandaJucator,sizeof(char)*256))
                            return 0;
                            goto restart;
                        }
                        else if(comandaJucator[2]=='-'&& strlen(comandaJucator)==5)
                        {
                            int MutareValida=1;
                            MutareCoord.y=TranslateMoveY(comandaJucator[0]);
                            MutareCoord.x=TranslateMoveX(comandaJucator[1]);
                            MutareCoord.ydest=TranslateMoveY(comandaJucator[3]);
                            MutareCoord.xdest=TranslateMoveX(comandaJucator[4]);
                            MutareCoord.trimisa=0;
                            trimis=0;
                            

                            if(legalMove(MutareCoord.x,MutareCoord.y,MutareCoord.xdest,MutareCoord.ydest,Jucator,boardGame2)==true)
                            {  

                                 if(Rege.x==MutareCoord.x&&MutareCoord.y==Rege.y)
                                        {
                                            Rege.x=MutareCoord.xdest; Rege.y=MutareCoord.ydest; //update pozitie rege
                                            kingMoved=1;
                                        }

                                       
                                        boardGame2[MutareCoord.xdest][MutareCoord.ydest]=boardGame2[MutareCoord.x][MutareCoord.y];
                                        boardGame2[MutareCoord.x][MutareCoord.y]=0;

                                    if(isAttacked(Rege.x,Rege.y,Jucator,boardGame2)==false)
                                    {
                                      system("clear");
                                        printf("User: "); printf("%s",name); printf("\n");
                                        showBoard(boardGame2);
                                        strcpy(comandaJucator,"move");
                                        if(0>=write(socket_descriptor,&comandaJucator,sizeof(char)*256))
                                        return 0;

                                        if(0>=write(socket_descriptor,&MutareCoord.x,sizeof(int)))
                                        return 0;
                                        if(0>=write(socket_descriptor,&MutareCoord.y,sizeof(int)))
                                        return 0;
                                        if(0>=write(socket_descriptor,&MutareCoord.xdest,sizeof(int)))
                                        return 0;
                                        if(0>=write(socket_descriptor,&MutareCoord.ydest,sizeof(int)))
                                        return 0;
                                        
                                        trimis=1;
                                        printf("Miscare valida \n");
                                    }
                                    else
                                    {
                                        if(kingMoved==1)
                                        {
                                            Rege.x=MutareCoord.x;
                                            Rege.y=MutareCoord.y;
                                            kingMoved=0;
                                        }

                                        boardGame2[MutareCoord.x][MutareCoord.y]=boardGame2[MutareCoord.xdest][MutareCoord.ydest];
                                        boardGame2[MutareCoord.xdest][MutareCoord.ydest]=0;


                                    }
                                    
                            }
                            else
                            {
                                printf("Miscare nevalida \n");
                            }
                            
                        if(trimis==1)
                        {
                        printf("Asteptati miscarea adversarului... \n"); fflush(stdout);
                        if(0>=read(socket_descriptor,Mutare,sizeof(char)*256))
                        return 0;
                        if(strcmp(Mutare,"Tie")==0)
                        {
                            printf("Remiza");
                            goto restart;
                        }

                        if(strcmp(Mutare,"Win")==0)
                        {
                           
                            printf("Oponentul a cedat partida. Ati castigat! \n\n"); fflush(stdout);
                            goto restart;
                        }
                        if(strcmp(Mutare,"move")==0)
                        {
                        memset(&MutareCoord.x,0,sizeof(int));
                        memset(&MutareCoord.y,0,sizeof(int));
                        memset(&MutareCoord.xdest,0,sizeof(int));
                        memset(&MutareCoord.ydest,0,sizeof(int));
                        
                        if(0>= read(socket_descriptor,&MutareCoord.x,sizeof(int)))
                        return 0;
                        if(0>=read(socket_descriptor,&MutareCoord.y,sizeof(int)))
                        return 0;
                        if(0>=read(socket_descriptor,&MutareCoord.xdest,sizeof(int)))
                        return 0;
                        if(0>=read(socket_descriptor,&MutareCoord.ydest,sizeof(int)))
                        return 0;

                        }
                        boardGame2[MutareCoord.xdest][MutareCoord.ydest]=boardGame2[MutareCoord.x][MutareCoord.y];
                        boardGame2[MutareCoord.x][MutareCoord.y]=0;
                        system("clear");
                        printf("User: "); printf("%s",name); printf("\n");
                        showBoard(boardGame2);
                        if(isAttacked(Rege.x,Rege.y,Jucator,boardGame2)==true)
                            {
                                printf("Sunteti in sah! \n\n");fflush(stdout);
                            }
                        }
                        }
                    }
                    
                    }
                    else if(Jucator==Negru) //PLAYER 2 ~~~~~~~~~~~~~~~~~~~~~~~
                    {
                    initBoard(boardGame2);
                    Rege.x=0;
                    Rege.y=4;
                    int trimis=0;
                    onGoingGame=1;
                    int kingMoved=0;
                while(onGoingGame)
                {
                       
                        showBoard2(boardGame2); 
                        memset(Mutare,0,sizeof(char)*256);
                        memset(comandaJucator,0,sizeof(char)*256);

                        printf("\n");
                        printf("Asteptati miscarea adversarului... \n"); fflush(stdout);

                        if(0>=read(socket_descriptor,Mutare,sizeof(char)*256))
                            return 0;
                        
                        if(strcmp(Mutare,"Tie")==0)
                        {
                            printf("Remiza");
                            goto restart;
                        }


                        if(strcmp(Mutare,"Win")==0)
                        {
                           
                            printf("Oponentul a cedat partida. Ati castigat!  \n\n"); fflush(stdout);
                            
                            goto restart;
                        }
                        if(strcmp(Mutare,"move")==0)
                        {
                        
                        memset(&MutareCoord.x,0,sizeof(int));
                        memset(&MutareCoord.y,0,sizeof(int));
                        memset(&MutareCoord.xdest,0,sizeof(int));
                        memset(&MutareCoord.ydest,0,sizeof(int));

                        if(0>=read(socket_descriptor,&MutareCoord.x,sizeof(int)))
                        return 0;
                        if(0>=read(socket_descriptor,&MutareCoord.y,sizeof(int)))
                        return 0;
                        if(0>=read(socket_descriptor,&MutareCoord.xdest,sizeof(int)))
                        return 0;
                        if(0>=read(socket_descriptor,&MutareCoord.ydest,sizeof(int)))
                        return 0;

                        }
                        boardGame2[MutareCoord.xdest][MutareCoord.ydest]=boardGame2[MutareCoord.x][MutareCoord.y];
                        boardGame2[MutareCoord.x][MutareCoord.y]=0;
                        
                        system("clear");
                        printf("User: "); printf("%s",name); printf("\n");
                        if(isAttacked(Rege.x,Rege.y,Jucator,boardGame2)==true)
                            {
                                printf("Sunteti in sah!\n \n");fflush(stdout);
                            }
                        showBoard2(boardGame2);
                        trimis=0;

                    if(onGoingGame){
                        
                        miscareGresitaNegru: memset(comandaJucator,0,sizeof(char)*256);
                        trimis=0;
                        printf("\nScrieti mutarea sub formatul clasic \n"); fflush(stdout); // misca negru
                        scanf("%s",comandaJucator);
                        if(strcmp(comandaJucator,"forfeit")==0)
                        {
                            printf("Ati cedat partida. Jucator 1 castiga \n\n");
                            if(0>=write(socket_descriptor,&comandaJucator,sizeof(char)*256))
                            return 0;
                            goto restart;
                        }
                        else if(comandaJucator[2]=='-' && strlen(comandaJucator)==5)
                        {
                           
                            int MutareValida=1;
                            MutareCoord.y=TranslateMoveY(comandaJucator[0]);
                            MutareCoord.x=TranslateMoveX(comandaJucator[1]);
                            MutareCoord.ydest=TranslateMoveY(comandaJucator[3]);
                            MutareCoord.xdest=TranslateMoveX(comandaJucator[4]);
                            MutareCoord.trimisa=0;
                            trimis=0;

                            if(legalMove(MutareCoord.x,MutareCoord.y,MutareCoord.xdest,MutareCoord.ydest,Jucator,boardGame2)==true)
                            {  
                                if(Rege.x==MutareCoord.x&&MutareCoord.y==Rege.y)
                                        {
                                            Rege.x=MutareCoord.xdest; Rege.y=MutareCoord.ydest; //update pozitie rege
                                            kingMoved=1;
                                        }


                                boardGame2[MutareCoord.xdest][MutareCoord.ydest]=boardGame2[MutareCoord.x][MutareCoord.y];
                                boardGame2[MutareCoord.x][MutareCoord.y]=0;

                                if(isAttacked(Rege.x,Rege.y,Jucator,boardGame2)==false)
                                    {
                                        
                                       system("clear");
                                       printf("User: "); printf("%s",name); printf("\n");
                                        strcpy(comandaJucator,"move");
                                        if(0>=write(socket_descriptor,&comandaJucator,sizeof(char)*256))
                                        return 0;

                                        if(0>=write(socket_descriptor,&MutareCoord.x,sizeof(int)))
                                        return 0;
                                        if(0>=write(socket_descriptor,&MutareCoord.y,sizeof(int)))
                                        return 0;                   
                                        if(0>=write(socket_descriptor,&MutareCoord.xdest,sizeof(int)))
                                        return 0;
                                        if(0>=write(socket_descriptor,&MutareCoord.ydest,sizeof(int)))
                                        return 0;
                                        

                                        trimis=1;
                                        printf("\nMiscare valida \n");
                                    }
                                else
                                    {
                                        if(kingMoved==1)
                                        {
                                            Rege.x=MutareCoord.x;
                                            Rege.y=MutareCoord.y;
                                            kingMoved=0;
                                        }

                                        boardGame2[MutareCoord.x][MutareCoord.y]=boardGame2[MutareCoord.xdest][MutareCoord.ydest];
                                        boardGame2[MutareCoord.xdest][MutareCoord.ydest]=0;
                                    }

                            }
                            else
                            {
                                printf("Miscare nevalida \n");
                                goto miscareGresitaNegru;
                            }
                            
                            if(trimis==1)
                            {
                            printf("Miscarea se trimite la adversar...\n") ;
                            }

                        }
                        else
                        {
                            printf("Gresit \n");
                            goto miscareGresitaNegru;
                        }

                    }

                    }

                    }

                }
                else
                {
                    printf("%s","Comanda nevalida. \n");
                }

                
            } 


           

        }
        else
        {
            printf("%s","User nevalid, incercati din nou \n");
        }
       // printf("%d\n",OK);
    }
    
}