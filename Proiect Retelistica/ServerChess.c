#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h> //bind-uri, listen/accept etc
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>
#include <fcntl.h>
#include "GameRoom.h"
#include <stdbool.h>
#include <mysql/mysql.h>



#define PORT 4359
#define MAX_SIZE 30



struct MasaDeJoc{

    int client_descriptor1;
    int client_descriptor2;
    int xInitAnterior;
    int yInitAnterior;
    int xFinalAnterior;
    int yFinalAnterior;
    int Winner;
    int xInit;
    int yInit;
    int xFinal;
    int yFinal;
    //   int pipe12[2];
    //  int pipe21[2];
    //   int pipe12Verify;
    //  int pipe21Verify;
    pthread_mutex_t general_mutex;
    int killed;
    int BlackErr;
    int WhiteErr;
    int Tie;
};

struct A7{
    int x;
    int y;
    int xdest;
    int ydest;
};


struct MasaDeJoc Partide[MAX_SIZE+6];

void ResetPieces(int i)
{
    Partide[i].xInitAnterior=0;
    Partide[i].yInitAnterior=0;
    Partide[i].xFinalAnterior=0;
    Partide[i].yFinalAnterior=0;
}

void CuratareDescriptor(int i)
{
    Partide[i].client_descriptor1=0;
    Partide[i].client_descriptor2=0;
}

void CuratareMasa(int i)
{   
    
    Partide[i].client_descriptor1=0;
    Partide[i].client_descriptor2=0;
    Partide[i].xInit=0;
    Partide[i].xFinal=0;
    Partide[i].yFinal=0;
    Partide[i].yInit=0;
    Partide[i].Tie=0;

    Partide[i].Winner=0;
    Partide[i].killed=0;
    Partide[i].BlackErr=0;
    Partide[i].WhiteErr=0;

};

void InitMutex(int i)
{
    pthread_mutex_init(&Partide[i].general_mutex, NULL );
   // Partide[i].pipe12Verify=0;
 //   Partide[i].pipe21Verify=0;
};
struct DataTh
{
    int client_descriptor_unu; //cine si cui trimit date
    int idTh; 
   // char* userLogged_unu;
    
    /* int client_descriptor_doi; 
    char* userLogged_doi; */
};

int VectorFrecventaThreads[2*MAX_SIZE];
int VectorPocniti[MAX_SIZE];

int CautaMasaLibera()
{
    int i;
    for(i=1;i<=MAX_SIZE;i++)
        {
        if(Partide[i].client_descriptor1!=0 && Partide[i].client_descriptor2==0)
            {
            return i;
            }
        }

    for(i=1;i<=MAX_SIZE;i++)
        {
        if(Partide[i].client_descriptor1==0)
            {
            return i;
            }
        }

    return -1;

};
void executeThread(struct DataTh localData) 
{
    // start_routine()
    FILE *fname;
    FILE *fpass;
    char readName[256];
    memset(readName,0,sizeof(char)*256);
    int lengthName;
    fname=fopen("Name.txt","r");
    int existaOponent=0;
    int onGoingGame=0;
    char Mutare[256];

    if(fname==NULL)
    {
        perror("Error citire Name.txt");
        return ;
    }

    char name[256];
    memset(name,0,sizeof(char)*256);
    int Unu=1;
    int Doi=2;

   // write(localData.client_descriptor_unu,&Unu,sizeof(int));
  /*  write(localData.client_descriptor_doi,&Doi,sizeof(int)); */
    int OK=0;  
    while(OK==0)
    {      
        fseek(fname,0,SEEK_SET);

        if(0>=read(localData.client_descriptor_unu,&name,sizeof(char)*256))
         {
              perror("Error read in thread");
              return ;
         }

         while(!feof(fname)) 
        {
        memset(readName,0,sizeof(char)*256);
        fgets(readName,256,fname);
        lengthName=strlen(readName)-1; 
        readName[strlen(readName)-1] = 0;
       
         
            if(strcmp(readName,name)==0)
           {
            OK=1; 
            
            if(0>=write(localData.client_descriptor_unu,&OK,sizeof(int)))
            return;
             printf("%s", readName); fflush(stdout);
            printf("%s","User valid\n");
            goto jump1;
           }
        }
        if(0>=write(localData.client_descriptor_unu,&OK,sizeof(int)))
        return;
        
    }
    fclose(fname);
 //al doilea client
 /*


    
    jump1:
    memset(name,0,sizeof(char)*256);
    memset(readName,0,sizeof(char)*256);

    
    OK=0;        

    while(OK==0)
    {
        fseek(fname,0,SEEK_SET);
        if(-1==read(localData.client_descriptor_doi,name,sizeof(char)*256))
        {
            perror("Error read in thread");
            return;
        }

         while(!feof(fname)) 
         {
           memset(readName,0,sizeof(char)*256);
           fgets(readName,256,fname);
           readName[strlen(readName) -1 ] = 0;
           if(strcmp(readName,name)==0)
           {
           OK=1;
           write(localData.client_descriptor_doi,&OK,sizeof(int));
           printf("%s", "Am scris si in al doilea client \n");
           goto jump2;
           }
           
        }
     
        printf("%s", "Nu am scris si in al doilea client \n");
        write(localData.client_descriptor_doi,&OK,sizeof(int));
    }
    jump2:
*/
    jump1:

    OK=1;
    
    printf("%s", "Finalizat log in pe thread-ul ");
    printf("%d", localData.idTh);
    printf("\n"); fflush(stdout);
       Partide[1].xFinal=2;
    int freePartida=0;
    
    char instructiune[256];
 
    while(1){
        restartThread:
        onGoingGame=0;
        memset(instructiune,0,sizeof(char)*256);
        memset(&freePartida,0,sizeof(int));
        int Masa=0;
        int MasaInfo=0;
        memset(&existaOponent,0,sizeof(int));
        
       if(0>=read(localData.client_descriptor_unu,instructiune,sizeof(char)*256)) //citesc instructiunea1
         return ;

        printf("%s",instructiune);
        printf("\n"); fflush(stdout);
        
        if(strcmp(instructiune,"Quit")==0)
        {
            printf("Thread-ul cu id-ul "); printf("%d",localData.idTh); printf(" va fi terminat...\n"); fflush(stdout);
            return ;
        }
        else if(strcmp(instructiune,"InfoAboutTable")==0)
        {
            int PlayeriExistenti=1;

            printf("Info About Table "); printf("\n");
            if(0>=read(localData.client_descriptor_unu,&MasaInfo,sizeof(int)))
            return;

            if(Partide[MasaInfo].client_descriptor1==1)
                PlayeriExistenti++;
            if(Partide[MasaInfo].client_descriptor2==1)
                PlayeriExistenti++;
            
            if(0>=write(localData.client_descriptor_unu,&PlayeriExistenti,sizeof(int)))
            return;

            
        }
        else if(strcmp(instructiune,"FindRandomGame")==0) //FIND RANDOM GAME
        {   
    
            freePartida=0;
            existaOponent=0;
            
        //    printf("Inainte de functie ");
         //   printf("%d",Masa);
       //     printf("\n");
            Masa=CautaMasaLibera();

     /*       if(Partide[Masa].pipe12Verify==0)
            {
                pipe(Partide[Masa].pipe12);
            }

             if(Partide[Masa].pipe21Verify==0)
            {
                pipe(Partide[Masa].pipe21);
            } */

      //      printf("Inainte de functie "); fflush(stdout);
       //     printf("%d",Masa); 
       //     printf("\n"); fflush(stdout);
            if(Masa==-1)
            {
                printf("%s","Nu exista o masa libera \n");
            }
            else if(Partide[Masa].client_descriptor1==0)
            {
                Partide[Masa].client_descriptor1=localData.client_descriptor_unu; // primu player client descriptor 
                freePartida=1;
            }
            else 
            {
                Partide[Masa].client_descriptor2=localData.client_descriptor_unu;
                freePartida=2;
            }
           

            if(0>=write(localData.client_descriptor_unu,&Masa,sizeof(int)))
            {
            CuratareMasa(Masa);
            return; //scriu raspunsu 1
            }
        //    printf("Dupa Write Masa=");
        //    printf("%d",Masa); fflush(stdout);
        //    printf("\n");

            if(0>=write(localData.client_descriptor_unu,&freePartida,sizeof(int)))
            {
            CuratareMasa(Masa);
            return; //scriu raspunsu 1
            }
        /*  if(freePartida==1)
          {
            printf("Am ajuns aici12");fflush(stdout);
            read(Partide[Masa].pipe21[0],&existaOponent,sizeof(int));
         //   pthread_mutex_lock(&Partide[Masa].general_mutex);
            existaOponent=1;
            write(localData.client_descriptor_unu,&existaOponent,sizeof(int));
          }
          if(freePartida==2)
          {
              printf("Am ajuns aici2");
            existaOponent=1;
            write(Partide[Masa].pipe21[1],&existaOponent,sizeof(int)); // scriu ca exista oponent

          } */

            goto FindGame;
            
        } 
        else if(strcmp(instructiune,"FindGame")==0) //FIND GAME
        {
            if(0>= read(localData.client_descriptor_unu,&Masa,sizeof(int)))
            {
            return; //scriu raspunsu 1
            }

            freePartida=0;
            int existaOponent=0;
            printf("Am citit masa dorita \n");
            if(Masa<0 || Masa>MAX_SIZE)
            {
                freePartida=0;
                Masa=0;
                if(0>=write(localData.client_descriptor_unu,&freePartida,sizeof(int)))
            {
                CuratareMasa(Masa); 
                return;
            }


            }
            else if(Partide[Masa].client_descriptor1==1&&Partide[Masa].client_descriptor2==1)
            {
            freePartida=0;
            if(0>=write(localData.client_descriptor_unu,&freePartida,sizeof(int)))
            {
                return;
            }    

            }
            else if(Partide[Masa].client_descriptor1==0)
            {
                freePartida=1;
                CuratareMasa(Masa); 
                Partide[Masa].client_descriptor1=1;

                if(0>=write(localData.client_descriptor_unu,&freePartida,sizeof(int)))
            {
                CuratareMasa(Masa); 
                return;
            }

            }
            else if(Partide[Masa].client_descriptor2==0)
            {
                Partide[Masa].client_descriptor2=1;
                freePartida=2;

                if(0>=write(localData.client_descriptor_unu,&freePartida,sizeof(int)))
            {
                CuratareMasa(Masa); 
                return;
            }

            }  
            
            FindGame:

            if(freePartida==1){
            while(Partide[Masa].client_descriptor2==0)
            {
                usleep(100);
            }
            
            existaOponent=1;
            if(0>=write(localData.client_descriptor_unu,&existaOponent,sizeof(int)))
            {   
                pthread_mutex_lock(&Partide[Masa].general_mutex);
                CuratareMasa(Masa);
                Partide[Masa].Winner=Negru;
                pthread_mutex_unlock(&Partide[Masa].general_mutex);
                return;
            }
            }// incepe jocul~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            
            onGoingGame=1;
            Partide[Masa].Winner=0;
            int Inceput=1;
            if(freePartida==1)
            {    
            while(onGoingGame==1&&freePartida==1) // miscarile alb
            {
                pthread_mutex_lock(&Partide[Masa].general_mutex);
               
                memset(&Mutare,0,sizeof(char)*256);

                if(Inceput==1)
                    Inceput=0;
                else
                {   
                    if(Partide[Masa].xInitAnterior==Partide[Masa].xInit &&
                        Partide[Masa].yInitAnterior==Partide[Masa].yInit && 
                        Partide[Masa].xFinalAnterior==Partide[Masa].xFinal &&
                        Partide[Masa].yFinalAnterior==Partide[Masa].yFinal )
                       { Partide[Masa].Winner=Alb; }

                    if(Partide[Masa].Tie==1)
                    {
                        strcpy(Mutare, "Tie");
                        if(0>=write(localData.client_descriptor_unu,Mutare,sizeof(char)*256))
                        {
                         CuratareMasa(Masa);
                         pthread_mutex_unlock(&Partide[Masa].general_mutex);
                         return ;
                        }
                        onGoingGame=0;
                        CuratareMasa(Masa);
                        Partide[Masa].Tie=1;
                        pthread_mutex_unlock(&Partide[Masa].general_mutex);
                        goto restartThread;
                    }

                    if(Partide[Masa].Winner==Alb||Partide[Masa].BlackErr==1)
                    {

                         strcpy(Mutare, "Win");
                         if(0>=write(localData.client_descriptor_unu,Mutare,sizeof(char)*256))
                        {
                         CuratareMasa(Masa);
                         pthread_mutex_unlock(&Partide[Masa].general_mutex);
                         return ;
                        }
                        onGoingGame=0;
                        CuratareMasa(Masa);
                        pthread_mutex_unlock(&Partide[Masa].general_mutex);
                        goto restartThread;
                    }
                    else if(Partide[Masa].xInit==Partide[Masa].xFinal && Partide[Masa].yInit==Partide[Masa].yFinal)
                    {
                    strcpy(Mutare,"Win");
                    CuratareMasa(Masa);
                    if(0>=write(localData.client_descriptor_unu,Mutare,sizeof(char)*256))
                    {
                        CuratareMasa(Masa);
                        pthread_mutex_unlock(&Partide[Masa].general_mutex);
                        return;
                    }
                    pthread_mutex_unlock(&Partide[Masa].general_mutex);
                    goto restartThread;
                        }
                    
                       else
                       {
                        strcpy(Mutare, "move");
                        if(0>= write(localData.client_descriptor_unu,Mutare,sizeof(char)*256))
                        {
                        CuratareMasa(Masa);
                        Partide[Masa].Winner=Negru;
                        Partide[Masa].WhiteErr=1;
                        pthread_mutex_unlock(&Partide[Masa].general_mutex);
                      
                        return ;
                        }

                        Partide[Masa].xInitAnterior=Partide[Masa].xInit;
                        Partide[Masa].yInitAnterior=Partide[Masa].yInit;
                        Partide[Masa].xFinalAnterior=Partide[Masa].xFinal;
                        Partide[Masa].yFinalAnterior=Partide[Masa].yFinal;

                        if(0>=write(localData.client_descriptor_unu,&Partide[Masa].xInit,sizeof(int)))
                        {
                        CuratareMasa(Masa);
                        Partide[Masa].Winner=Negru;
                        Partide[Masa].WhiteErr=1;
                        pthread_mutex_unlock(&Partide[Masa].general_mutex);
                        return ;
                        }
                        if(0>=write(localData.client_descriptor_unu,&Partide[Masa].yInit,sizeof(int)))
                        {
                        CuratareMasa(Masa);
                        Partide[Masa].Winner=Negru;
                        Partide[Masa].WhiteErr=1;
                        pthread_mutex_unlock(&Partide[Masa].general_mutex);
                        
                        return ;
                        }
                        if(0>=write(localData.client_descriptor_unu,&Partide[Masa].xFinal,sizeof(int)))
                        {
                        CuratareMasa(Masa);
                        Partide[Masa].Winner=Negru;
                        Partide[Masa].WhiteErr=1;
                        pthread_mutex_unlock(&Partide[Masa].general_mutex);
                      
                        return ;
                        }
                        if(0>=write(localData.client_descriptor_unu,&Partide[Masa].yFinal,sizeof(int)))
                        {
                        CuratareMasa(Masa);
                        Partide[Masa].Winner=Negru;
                        Partide[Masa].WhiteErr=1;
                        pthread_mutex_unlock(&Partide[Masa].general_mutex);
                        
                        return ;
                        }
                        }

                   
               //     write(localData.client_descriptor_unu,Mutare,sizeof(char)*256);
                }

              
                //else 
              if(0>=read(localData.client_descriptor_unu,Mutare,sizeof(char)*256))
                {
                        CuratareMasa(Masa);
                        Partide[Masa].Winner=Negru;
                        Partide[Masa].WhiteErr=1;
                        pthread_mutex_unlock(&Partide[Masa].general_mutex);
                    
                        return ;
                }
            //    printf("%s",Mutare); fflush(stdout);
                
                
                if(strcmp(Mutare,"forfeit")==0)
                {
                    printf("Castiga negru \n"); fflush(stdout);
                    CuratareMasa(Masa);
                    Partide[Masa].Winner=Negru;
                    pthread_mutex_unlock(&Partide[Masa].general_mutex);
                    goto restartThread;
                }
                else if(strcmp(Mutare,"move")==0)
                {
                   int bufferMutari=0;
                   if(0>=read(localData.client_descriptor_unu,&bufferMutari,sizeof(int)))
                   {
                    CuratareMasa(Masa);
                    Partide[Masa].Winner=Negru;
                    Partide[Masa].WhiteErr=1;
                    pthread_mutex_unlock(&Partide[Masa].general_mutex);
                   
                    return ;
                   }
                   Partide[Masa].xInit=bufferMutari;

                   if(0>=read(localData.client_descriptor_unu,&bufferMutari,sizeof(int)))
                   {
                    CuratareMasa(Masa);
                    Partide[Masa].Winner=Negru;
                    Partide[Masa].WhiteErr=1;
                    pthread_mutex_unlock(&Partide[Masa].general_mutex);
             
                    return ;
                   }
                   Partide[Masa].yInit=bufferMutari;

                   if(0>=read(localData.client_descriptor_unu,&bufferMutari,sizeof(int)))
                   {
                    CuratareMasa(Masa);
                    Partide[Masa].Winner=Negru;
                    Partide[Masa].WhiteErr=1;
                    pthread_mutex_unlock(&Partide[Masa].general_mutex);
                    return ;
                   }
                   Partide[Masa].xFinal=bufferMutari;

                   if(0>=read(localData.client_descriptor_unu,&bufferMutari,sizeof(int)))
                   {
                    CuratareMasa(Masa);
                    Partide[Masa].Winner=Negru;
                    Partide[Masa].WhiteErr=1;
                    pthread_mutex_unlock(&Partide[Masa].general_mutex);
               
                    return ;
                   }
                    Partide[Masa].yFinal=bufferMutari;
                   
                  //  printf("%d",bufferMutari); 
                 // fflush(stdout);
                 //   Partide[Masa].yInit=Mutare[1]-'0';
             //       Partide[Masa].xFinal=Mutare[2]-'0';
               //     Partide[Masa].yFinal=Mutare[3]-'0';
                }
                
                pthread_mutex_unlock(&Partide[Masa].general_mutex);
                usleep(40);
            }  
            
            }

            if(freePartida==2)
            { 
               usleep(101);
               
            while(onGoingGame==1&&freePartida==2) // miscarile negru
            {
                usleep(40);
                pthread_mutex_lock(&Partide[Masa].general_mutex);
                
                memset(&Mutare,0,sizeof(char)*256);

                    if(Partide[Masa].xInitAnterior==Partide[Masa].xInit &&
                        Partide[Masa].yInitAnterior==Partide[Masa].yInit && 
                        Partide[Masa].xFinalAnterior==Partide[Masa].xFinal &&
                        Partide[Masa].yFinalAnterior==Partide[Masa].yFinal )
                       { Partide[Masa].Winner=Negru; }

                    if(Partide[Masa].Tie==1)
                    {
                        strcpy(Mutare, "Tie");
                        if(0>=write(localData.client_descriptor_unu,Mutare,sizeof(char)*256))
                        {
                         CuratareMasa(Masa);
                         pthread_mutex_unlock(&Partide[Masa].general_mutex);
                         return ;
                        }
                        onGoingGame=0;
                        CuratareMasa(Masa);
                        Partide[Masa].Tie=1;
                        pthread_mutex_unlock(&Partide[Masa].general_mutex);
                        goto restartThread;
                    }

                    if(Partide[Masa].Winner==Negru||Partide[Masa].WhiteErr==1)
                    {
                        strcpy(Mutare,"Win");
                        CuratareMasa(Masa);
                        if(0>=write(localData.client_descriptor_unu,Mutare,sizeof(char)*256))
                       {   
                           CuratareMasa(Masa);
                           pthread_mutex_unlock(&Partide[Masa].general_mutex);
                           return;
                       }
                        pthread_mutex_unlock(&Partide[Masa].general_mutex);
                        goto restartThread;
                }
                else if(Partide[Masa].xInit==Partide[Masa].xFinal && Partide[Masa].yInit==Partide[Masa].yFinal)
                {
                        strcpy(Mutare,"Win");
                        CuratareMasa(Masa);
                        if(0>=write(localData.client_descriptor_unu,Mutare,sizeof(char)*256))
                        {   
                           CuratareMasa(Masa);
                           pthread_mutex_unlock(&Partide[Masa].general_mutex);
                           return;
                       }
                        pthread_mutex_unlock(&Partide[Masa].general_mutex);
                        goto restartThread;
                }
                else
                {
                        strcpy(Mutare, "move");
                        if(0>=write(localData.client_descriptor_unu,Mutare,sizeof(char)*256))
                        {
                        CuratareMasa(Masa);
                        Partide[Masa].Winner=Alb;
                        Partide[Masa].BlackErr=1;
                        pthread_mutex_unlock(&Partide[Masa].general_mutex);
                        return ;
                        }

                        Partide[Masa].xInitAnterior=Partide[Masa].xInit;
                        Partide[Masa].yInitAnterior=Partide[Masa].yInit;
                        Partide[Masa].xFinalAnterior=Partide[Masa].xFinal;
                        Partide[Masa].yFinalAnterior=Partide[Masa].yFinal;

                        if(0>=write(localData.client_descriptor_unu,&Partide[Masa].xInit,sizeof(int)))
                        {
                        CuratareMasa(Masa);
                        Partide[Masa].Winner=Alb;
                        Partide[Masa].BlackErr=1;
                        pthread_mutex_unlock(&Partide[Masa].general_mutex);
                        return ;
                        }
                        if(0>=write(localData.client_descriptor_unu,&Partide[Masa].yInit,sizeof(int)))
                        {
                        CuratareMasa(Masa);
                        Partide[Masa].Winner=Alb;
                        Partide[Masa].BlackErr=1;
                        pthread_mutex_unlock(&Partide[Masa].general_mutex);
                        return ;
                        }
                        if(0>=write(localData.client_descriptor_unu,&Partide[Masa].xFinal,sizeof(int)))
                        {
                        CuratareMasa(Masa);
                        Partide[Masa].Winner=Alb;
                        Partide[Masa].BlackErr=1;
                        pthread_mutex_unlock(&Partide[Masa].general_mutex);
                        return ;
                        }
                        if(0>=write(localData.client_descriptor_unu,&Partide[Masa].yFinal,sizeof(int)))
                        {
                        CuratareMasa(Masa);
                        Partide[Masa].Winner=Alb;
                        Partide[Masa].BlackErr=1;
                        pthread_mutex_unlock(&Partide[Masa].general_mutex);
                        return ;
                        }

                }

           //     write(localData.client_descriptor_unu,Mutare,sizeof(char)*256);
                
                //Mutare

                if(0>=  read(localData.client_descriptor_unu,Mutare,sizeof(char)*256))
                {
                    CuratareMasa(Masa);
                    Partide[Masa].Winner=Alb;
                    Partide[Masa].BlackErr=1;
                    pthread_mutex_unlock(&Partide[Masa].general_mutex);
                    return ;
                }
                
                if(strcmp(Mutare,"forfeit")==0)
                {
                    printf("Castiga albul \n"); fflush(stdout);
                    CuratareMasa(Masa);
                    Partide[Masa].Winner=Alb;
                    pthread_mutex_unlock(&Partide[Masa].general_mutex);
                    goto restartThread;
                }
                else if(strcmp(Mutare,"move")==0)
                {
                    int bufferMutari=0;
                   if(0>=read(localData.client_descriptor_unu,&bufferMutari,sizeof(int)))
                   {
                    CuratareMasa(Masa);
                    Partide[Masa].Winner=Alb;
                    Partide[Masa].BlackErr=1;
                    pthread_mutex_unlock(&Partide[Masa].general_mutex);
                    return ;
                   }
                   Partide[Masa].xInit=bufferMutari;

                   if(0>=read(localData.client_descriptor_unu,&bufferMutari,sizeof(int)))
                   {
                    CuratareMasa(Masa);
                    Partide[Masa].Winner=Alb;
                    Partide[Masa].BlackErr=1;
                    pthread_mutex_unlock(&Partide[Masa].general_mutex);
                    return ;
                   }
                   Partide[Masa].yInit=bufferMutari;

                   if(0>=read(localData.client_descriptor_unu,&bufferMutari,sizeof(int)))
                   {
                    CuratareMasa(Masa);
                    Partide[Masa].Winner=Alb;
                    Partide[Masa].BlackErr=1;
                    pthread_mutex_unlock(&Partide[Masa].general_mutex);
                    return ;
                   }
                   Partide[Masa].xFinal=bufferMutari;

                   if(0>=read(localData.client_descriptor_unu,&bufferMutari,sizeof(int)))
                   {
                    CuratareMasa(Masa);
                    Partide[Masa].Winner=Alb;
                    Partide[Masa].BlackErr=1;
                    pthread_mutex_unlock(&Partide[Masa].general_mutex);
                    return ;
                   }
                   Partide[Masa].yFinal=bufferMutari;

                    
                }
                


                pthread_mutex_unlock(&Partide[Masa].general_mutex);
            }

                
            }
        }
    }

};

/*void executeThread2(struct DataTh localData)
{
    char instructiune[256];
    printf("ajunge in execute thread 2");
    while(1){
        memset(instructiune,0,sizeof(char)*256);
        printf("%s","eroare");
        read(localData.client_descriptor_unu,instructiune,sizeof(char)*256);
         
        printf("%s",instructiune);
         if(strcmp(instructiune,"FindRandomGame")==0)
        {
             Partide[Masa].client_descriptor2=localData.client_descriptor_unu;
            int Masa=0;
            Masa=CautaMasaLibera();
            printf("%d",Masa);
            if(Masa==-1)
            {
                printf("%s","Nu exista o masa libera \n");
            }
            else if(Partide[Masa].client_descriptor1==0)
            {
                Partide[Masa].client_descriptor1==localData.client_descriptor_unu; // primu player client descriptor 
            }
            else 
            {
                Partide[Masa].client_descriptor2==localData.client_descriptor_unu;
            }
            write(localData.client_descriptor_unu,&Masa,sizeof(int));
        }  
    }

} */

static void* threadInit(void* arg) //adresa fara tip de date
{ 
    struct DataTh localData=*((struct DataTh*) arg);//castam arg care nu are data in data th si apoi cerem valoare ei
     pthread_mutex_t mutexThread;
    pthread_mutex_init(&mutexThread, NULL );

    executeThread(localData);
   // executeThread2(localData);

    close(localData.client_descriptor_unu);
    // close(localData.client_descriptor_doi);
    pthread_mutex_lock(&mutexThread);
    VectorFrecventaThreads[localData.idTh]=0;
    pthread_mutex_unlock(&mutexThread);
    
    pthread_cancel(pthread_self());
    
};
//~~~~~~~~~~~~~~~~~~~~~~~Executia programului
int main()
{
    struct sockaddr_in server; 
    struct sockaddr_in client;
    memset(&server,0,sizeof(struct sockaddr_in)); //golim inf server
    memset(&client,0,sizeof(struct sockaddr_in));
    pthread_mutex_t mutexServer;
    pthread_mutex_init(&mutexServer, NULL );


    for(int k=0;k<MAX_SIZE;k++)
    {
        CuratareDescriptor(k);
        CuratareMasa(k);
        InitMutex(k);
        ResetPieces(k);
    }
    for(int k=0;k<2*MAX_SIZE;k++)
    {
        VectorFrecventaThreads[k]=0;
    }

    server.sin_family=AF_INET; 
    server.sin_addr.s_addr=htonl(INADDR_ANY); // in server primim orice adresa. din aia poti modifica ce si cum 
    server.sin_port=htons(PORT);
    int optionnumber=1; //echiv optval

    pthread_t threads[2*MAX_SIZE];
 //   pthread_t gameRooms[MAX_SIZE]; //Mesele in sine
    memset(threads,0,sizeof(pthread_t)*MAX_SIZE);

    int socket_descriptor;
    if(-1==(socket_descriptor=socket(AF_INET,SOCK_STREAM,0)))
    {
    perror("Error socket");
    return 1;
    }

    setsockopt(socket_descriptor, SOL_SOCKET, SO_REUSEADDR,&optionnumber,sizeof(int)); //serveru poate refolosi adresa

    if(-1==(bind(socket_descriptor,(struct sockaddr*)&server,sizeof(struct sockaddr)))) //trimitem adresa datei serverului
    {
    perror("Error bind");
    return 2;
    }

    if(-1==listen(socket_descriptor,10))//Permitem N conexiuni,tratam coada de asteptare
    {
        perror("Error listen");
        return 3;
    }
    /* Server init ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    int idClient_unu;
   // int idClient_doi;
    struct DataTh* dateThread;
    int contorTh=0;
 //   int lengthAddress=sizeof(client);
    int i=0; 
    int contor;
  //  for(i=0; i<50; i++) 
  while(1)
    {
        
        int lengthAddress=sizeof(client);
        
        for(contorTh=0;contorTh<2*MAX_SIZE;contorTh++)
        { pthread_mutex_lock(&mutexServer);
        if(VectorFrecventaThreads[contorTh]==0)
        { pthread_mutex_unlock(&mutexServer);

        if(-1==(idClient_unu=accept(socket_descriptor,(struct sockaddr*)&client, &lengthAddress)))
        {
            perror("Error la accept 1");
            continue; //nu omoram serveru 
        }
        printf("Thread-ul format "); printf("%d",contorTh);  fflush (stdout); printf("\n") ;
        dateThread=(struct DataTh*)malloc(sizeof(struct DataTh)); //C
        dateThread->client_descriptor_unu=idClient_unu;
        dateThread->idTh=contorTh;

        /* if(-1==(idClient_doi=accept(socket_descriptor,(struct sockaddr*) &client, &lengthAddress)))
        {
            perror("Error la accept 1");
            continue; // al doilea client
        }
        dateThread->client_descriptor_doi=idClient_doi;
         */;
        pthread_mutex_lock(&mutexServer);
        VectorFrecventaThreads[contorTh]=1;
        pthread_mutex_unlock(&mutexServer);
        pthread_create(&threads[contorTh],NULL,threadInit,dateThread); 
        
        }
        else
        {
            pthread_mutex_unlock(&mutexServer);
        }
        
        }
    }


    return 0;
    
    

    
}