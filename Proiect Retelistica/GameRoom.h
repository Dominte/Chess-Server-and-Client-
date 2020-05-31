#define ROWS 8
#define COLLUMNS 8
#define DOT "…"
#define CELLS (MBSZ + 1)
#define MBSZ sizeof(DOT)
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define negruPion 1
#define albPion -1
#define negruTura 4
#define albTura -4
#define negruNebun 2
#define albNebun -2
#define negruCal 3
#define albCal -3
#define negruRegina 8
#define albRegina -8
#define negruRege 9
#define albRege -9
#define blank 0 

#define Alb -21
#define Negru 21






int albCheck=0; 

int negruCheck=0; 

int Enpassant; 

void showBoard(int boardGame1[][8])
{ 
     printf("%s","  A B C D E F G H \n");
     fflush(stdout);
    for(int j=0;j<=7;j++) 
       { printf("%d",j+1); 
         printf("%s"," ");

           for(int i=0;i<=7;i++)
           {
                switch(boardGame1[j][i])
                {
                    case 4: {printf("%s","♜ "); break;}
                    case 3: printf("%s","♞ "); break;
                    case 2: printf("%s","♝ "); break;
                    case 8: printf("%s","♛ "); break;
                    case 9: printf("%s","♚ "); break;
                    case 1: printf("%s","♟ "); break;

                    case -4: printf("%s","♖ "); break;
                    case -3: printf("%s","♘ "); break;
                    case -2: printf("%s","♗ "); break;
                    case -8: printf("%s","♕ "); break;
                    case -9: printf("%s","♔ "); break;
                    case -1: printf("%s","♙ "); break;
                    case 0: printf("%s","… "); break;
                }
           }
         printf("%d",j+1);      
         printf("\n");
       }
    printf("%s","  A B C D E F G H");
    printf("\n");
};

void showBoard2 (int boardGame1[][8])
{ 
     printf("%s","  A B C D E F G H \n");
     fflush(stdout);
    for(int j=7;j>=0;j--) 
       { printf("%d",j+1); 
         printf("%s"," ");

           for(int i=0;i<=7;i++)
           {
                switch(boardGame1[j][i])
                {
                    case 4: {printf("%s","♜ "); break;}
                    case 3: printf("%s","♞ "); break;
                    case 2: printf("%s","♝ "); break;
                    case 8: printf("%s","♛ "); break;
                    case 9: printf("%s","♚ "); break;
                    case 1: printf("%s","♟ "); break;

                    case -4: printf("%s","♖ "); break;
                    case -3: printf("%s","♘ "); break;
                    case -2: printf("%s","♗ "); break;
                    case -8: printf("%s","♕ "); break;
                    case -9: printf("%s","♔ "); break;
                    case -1: printf("%s","♙ "); break;
                    case 0: printf("%s","… "); break;
                }
           }
         printf("%d",j+1);      
         printf("\n");
       }
    printf("%s","  A B C D E F G H");
    printf("\n");
};


void initBoard(int boardGame[][COLLUMNS])
{
    int i,j;
    
   /*  boardGame[ROWS][COLLUMNS]={
        {4,3,2,8,9,2,3,4,},
        {1,1,1,1,1,1,1,1,},
        {0,0,0,0,0,0,0,0,},
        {0,0,0,0,0,0,0,0,},
        {0,0,0,0,0,0,0,0,},
        {0,0,0,0,0,0,0,0,},
    {-1,-1,-1,-1,-1,-1,-1,-1,},
    {-4,-3,-2,-8,-9,-2,-3,-4,},
    }; */

    boardGame[0][0]=4; boardGame[0][1]=3; boardGame[0][2]=2; boardGame[0][3]=8; boardGame[0][4]=9; boardGame[0][5]=2; boardGame[0][6]=3; boardGame[0][7]=4;
    for(j=0;j<8;j++) boardGame[1][j]=1;  

    for(i=2;i<6;i++) 
        for(j=0;j<8;j++)   
            boardGame[i][j]=0;
    
    for(j=0;j<8;j++) boardGame[6][j]=-1;
    boardGame[7][0]=-4; boardGame[7][1]=-3; boardGame[7][2]=-2; boardGame[7][3]=-8; boardGame[7][4]=-9; boardGame[7][5]=-2; boardGame[7][6]=-3; boardGame[7][7]=-4;
          
};

bool isAttacked(int x, int y,int Player,int boardGame[][8]);

bool legalMove(int x, int y, int xdest, int ydest, int Player, int boardGame1[][8])                                // x Linia , y coloana
{
    if (x < 0 || x > 7 || y < 0 || y > 7 || xdest < 0 || xdest > 7 || ydest < 0 || ydest > 7){
        return false; // inafara tablei
    }
    if ( boardGame1[x][y]==0){
        return false; // incercare sa muta spatiu goala 
    }
    if ( boardGame1[x][y]>0 && Player == Alb ){
        return false; // incercare sa mute piesa adversa
    }
    if ( boardGame1[x][y]<0 && Player == Negru ){
        return false; // incercare sa mute piesa adversa
    }       
    if(boardGame1[xdest][ydest]==negruRege && Player==Alb)
            return false;
    if(boardGame1[xdest][ydest]==albRege && Player==Negru)
            return false;
    
    int Piesa=boardGame1[x][y]; 
    int PozitieAtacata=boardGame1[xdest][ydest]; 
    
    //printf("%d",PozitieAtacata); 
   // printf("%d",Piesa); 
  //  showBoard(boardGame1);
    if(Player==Negru && PozitieAtacata>0){
        return false;}
    if(Player==Alb && PozitieAtacata<0){
        return false;}

    if (Piesa==negruPion)
    {
        if( (xdest-x)==1 && ydest==y && PozitieAtacata==0)
            return true; 
        
        if( (xdest-x)==2 && x==1 && ydest==y &&  PozitieAtacata==0 && boardGame1[x+1][y]==blank)
            return true; 

        if( (xdest-x)==1 && abs(y-ydest)==1 && PozitieAtacata < 0 )
            return true; 
        //Enpassant

        return false; 
    }
    
    if (Piesa==albPion)
    {
    
        if( (x-xdest)==1 && ydest==y && PozitieAtacata==0)
            return true; 
        
        if( (x-xdest)==2 && x==6 && ydest==y && PozitieAtacata==0 && boardGame1[x-1][y]==blank)
            return true; 

        if( (x-xdest)==1 && abs(y-ydest)==1 && PozitieAtacata > 0 )
            return true; 
        //Enpassant
       
        return false; 
    }

    if (Piesa==negruCal)
    {
       
        if( abs(x-xdest)==2 && abs(y-ydest)==1 && PozitieAtacata<=0)
            return true; 
        if( abs(x-xdest)==1 && abs(y-ydest)==2 && PozitieAtacata<=0)
            return true;

        return false;
    }

    if (Piesa==albCal)
    {
        if( abs(x-xdest)==2 && abs(y-ydest)==1 && PozitieAtacata>=0)
            return true; 
        if( abs(x-xdest)==1 && abs(y-ydest)==2 && PozitieAtacata>=0)
            return true;

        return false;
    }
 
    if (Piesa==negruTura)
    {
        int i;


        if(y!=ydest && x!=xdest)
            return false;

        if(x<xdest){
        for(i=x+1;i<xdest;i++)
            if(boardGame1[i][y]!=blank) //in jos
                return false;}
        
        if(x>xdest){
            for(i=x-1;i>xdest;i--)
            if(boardGame1[i][y]!=blank) //in sus
                return false;}

        if(y<ydest){
        for(i=y+1;i<ydest;i++)
            if(boardGame1[x][i]!=blank) // in dreapta 
                return false;}

        if(y>ydest){
        for(i=y-1;i>ydest;i--)
            if(boardGame1[x][i]!=blank) //in stanga
                return false;}

        if(PozitieAtacata > 0) // daca sunt negre
                return false;

        return true;
    }

    if (Piesa==albTura)
    {
        int i;
      
        if(y!=ydest && x!=xdest)
            return false;

        if(x<xdest){
        for(i=x+1;i<xdest;i++)
            if(boardGame1[i][y]!=blank)
                return false;}

        if(x>xdest){
        for(i=x-1;i>xdest;i--)
            if(boardGame1[i][y]!=blank)
                return false;}

        if(y<ydest){
        for(i=y+1;i<ydest;i++)
            if(boardGame1[x][i]!=blank)
                return false;}

        if(y>ydest){
        for(i=y-1;i>ydest;i--)
            if(boardGame1[x][i]!=blank)
                return false;}



        if(PozitieAtacata < 0) // daca sunt albe
                return false;

        return true;
    }

    if (Piesa==negruNebun)
    {
        if(abs(x-xdest)!=abs(y-ydest))
            return false;
        printf("Ajung aici neb");
        if(xdest < x && ydest > y) //dreapta sus 
        {
            int pasi=ydest-y-1;
            int pasiFacuti=0;
            while(pasi)
            {
                pasi--;
                pasiFacuti++;
                if(boardGame1[x - pasiFacuti][y + pasiFacuti]!=blank)
                    return false;
            }
        }
         
         if(xdest < x && ydest < y) //stanga sus 
        {
            int pasi=y-ydest-1;
            int pasiFacuti=0;
            while(pasi)
            {
                pasi--;
                pasiFacuti++;
                if(boardGame1[x - pasiFacuti][y - pasiFacuti]!=blank)
                    return false;
            }

        }


         if(xdest > x && ydest < y) //stanga jos
        {
            int pasi=xdest-x-1;
            int pasiFacuti=0;
            while(pasi)
            {
                pasi--;
                pasiFacuti++;
                if(boardGame1[x + pasiFacuti][y - pasiFacuti]!=blank)
                    return false;
            }

        }


         if(xdest > x && ydest > y) //dreapta jos
        {
            int pasi=ydest-y-1;
            int pasiFacuti=0;
            while(pasi)
            {
                pasi--;
                pasiFacuti++;
                if(boardGame1[x + pasiFacuti][y + pasiFacuti]!=blank)
                    return false;
            }
        }

        if(PozitieAtacata > 0) // ataca negru
            return false;

        return true;
    }

    if (Piesa==albNebun)
    {
        if(abs(x-xdest)!=abs(y-ydest))
            return false;

        if(xdest < x && ydest > y) //dreapta sus 
        {
            int pasi=ydest-y-1;
            int pasiFacuti=0;
            while(pasi)
            {
                pasi--;
                pasiFacuti++;
                if(boardGame1[x - pasiFacuti][y + pasiFacuti]!=blank)
                    return false;
            }
        }

         if(xdest < x && ydest < y) //stanga sus 
        {
            int pasi=y-ydest-1;
            int pasiFacuti=0;
            while(pasi)
            {
                pasi--;
                pasiFacuti++;
                if(boardGame1[x - pasiFacuti][y - pasiFacuti]!=blank)
                    return false;
            }

        }


         if(xdest > x && ydest < y) //stanga jos
        {
            int pasi=xdest-x-1;
            int pasiFacuti=0;
            while(pasi)
            {
                pasi--;
                pasiFacuti++;
                if(boardGame1[x + pasiFacuti][y - pasiFacuti]!=blank)
                    return false;
            }

        }


         if(xdest > x && ydest > y) //dreapta jos
        {
            int pasi=ydest-y-1;
            int pasiFacuti=0;
            while(pasi)
            {
                pasi--;
                pasiFacuti++;
                if(boardGame1[x + pasiFacuti][y + pasiFacuti]!=blank)
                    return false;
            }
        }

        if(PozitieAtacata < 0) // ataca alb
            return false;

        return true;
    }

    if(Piesa==negruRege)
    {
        if(abs(x-xdest)<2 && abs(y-ydest)<2 && PozitieAtacata <=0)
            return true;
            
        return false;
    }



    if(Piesa==albRege)
    {
        if(abs(x-xdest)<2 && abs(y-ydest)<2 && PozitieAtacata >= 0)
            return true;

        return false;
    }

    if(Piesa==negruRegina)
    {
        int i;


        if(y==ydest ||  x==xdest)
        {
        if(x<xdest){
        for(i=x+1;i<xdest;i++)
            if(boardGame1[i][y]!=blank) //in jos
                return false;}
        
        if(x>xdest){
            for(i=x-1;i>xdest;i--)
            if(boardGame1[i][y]!=blank) //in sus
                return false;}

        if(y<ydest){
        for(i=y+1;i<ydest;i++)
            if(boardGame1[x][i]!=blank) // in dreapta 
                return false;}

        if(y>ydest){
        for(i=y-1;i>ydest;i--)
            if(boardGame1[x][i]!=blank) //in stanga
                return false;}

        if(PozitieAtacata > 0) // daca sunt negre
                return false;

        return true;
        }
        else  if(abs(x-xdest)==abs(y-ydest))
        {

        if(xdest < x && ydest > y) //dreapta sus 
        {
            int pasi=ydest-y-1;
            int pasiFacuti=0;
            while(pasi)
            {
                pasi--;
                pasiFacuti++;
                if(boardGame1[x - pasiFacuti][y + pasiFacuti]!=blank)
                    return false;
            }
        }
         
         if(xdest < x && ydest < y) //stanga sus 
        {
            int pasi=y-ydest-1;
            int pasiFacuti=0;
            while(pasi)
            {
                pasi--;
                pasiFacuti++;
                if(boardGame1[x - pasiFacuti][y - pasiFacuti]!=blank)
                    return false;
            }

        }


         if(xdest > x && ydest < y) //stanga jos
        {
            int pasi=xdest-x-1;
            int pasiFacuti=0;
            while(pasi)
            {
                pasi--;
                pasiFacuti++;
                if(boardGame1[x + pasiFacuti][y - pasiFacuti]!=blank)
                    return false;
            }

        }


         if(xdest > x && ydest > y) //dreapta jos
        {
            int pasi=ydest-y-1;
            int pasiFacuti=0;
            while(pasi)
            {
                pasi--;
                pasiFacuti++;
                if(boardGame1[x + pasiFacuti][y + pasiFacuti]!=blank)
                    return false;
            }
        }

        if(PozitieAtacata > 0) // ataca negru
            return false;
        
        return true;
        }
        else return false;
    } 

    if(Piesa==albRegina)
    {
        int i;
        if(y==ydest ||  x==xdest){

        if(x<xdest){
        for(i=x+1;i<xdest;i++)
            if(boardGame1[i][y]!=blank) //in jos
                return false;}
        
        if(x>xdest){
            for(i=x-1;i>xdest;i--)
            if(boardGame1[i][y]!=blank) //in sus
                return false;}

        if(y<ydest){
        for(i=y+1;i<ydest;i++)
            if(boardGame1[x][i]!=blank) // in dreapta 
                return false;}

        if(y>ydest){
        for(i=y-1;i>ydest;i--)
            if(boardGame1[x][i]!=blank) //in stanga
                return false;}

        if(PozitieAtacata < 0) // daca sunt albe
                return false;

        return true;
        }
        else  if(abs(x-xdest)==abs(y-ydest))
        {
        if(xdest < x && ydest > y) //dreapta sus 
        {
            int pasi=ydest-y-1;
            int pasiFacuti=0;
            while(pasi)
            {
                pasi--;
                pasiFacuti++;
                if(boardGame1[x - pasiFacuti][y + pasiFacuti]!=blank)
                    return false;
            }
        }
         
         if(xdest < x && ydest < y) //stanga sus 
        {
            int pasi=y-ydest-1;
            int pasiFacuti=0;
            while(pasi)
            {
                pasi--;
                pasiFacuti++;
                if(boardGame1[x - pasiFacuti][y - pasiFacuti]!=blank)
                    return false;
            }

        }


         if(xdest > x && ydest < y) //stanga jos
        {
            int pasi=xdest-x-1;
            int pasiFacuti=0;
            while(pasi)
            {
                pasi--;
                pasiFacuti++;
                if(boardGame1[x + pasiFacuti][y - pasiFacuti]!=blank)
                    return false;
            }

        }


         if(xdest > x && ydest > y) //dreapta jos
        {
            int pasi=ydest-y-1;
            int pasiFacuti=0;
            while(pasi)
            {
                pasi--;
                pasiFacuti++;
                if(boardGame1[x + pasiFacuti][y + pasiFacuti]!=blank)
                    return false;
            }
        }

        if(PozitieAtacata < 0) // ataca alb
            return false;
        
        return true;
        }
        else return false;
    } 

}

int TranslateMoveY(char arg)
{
    if(arg=='A'||arg=='a')
        return 0;
    if(arg=='B'||arg=='b')
        return 1;
    if(arg=='C'||arg=='c')
        return 2;
    if(arg=='D'||arg=='d')
        return 3;
    if(arg=='E'||arg=='e')
        return 4;
    if(arg=='F'||arg=='f')
        return 5;
    if(arg=='G'||arg=='g')
        return 6;
    if(arg=='H'||arg=='h')
        return 7;
    
    return -1;
}

int TranslateMoveX(char arg)
{
    if(arg=='1')
        return 0;
    if(arg=='2')
        return 1;
    if(arg=='3')
        return 2;
    if(arg=='4')
        return 3;
    if(arg=='5')
        return 4;
    if(arg=='6')
        return 5;
    if(arg=='7')
        return 6;
    if(arg=='8')
        return 7;
    
    return -1;
}


bool isAttacked(int x, int y,int Player,int boardGame[][COLLUMNS])
{
    if(Player==Negru)
    {
        if(boardGame[x+1][y+1]==albPion||boardGame[x+1][y-1]==albPion) // atacat de pioni pe diagonala
            return true; 
        
        if(boardGame[x+1][y+2]==albCal||boardGame[x-1][y+2]==albCal||boardGame[x-2][y+1]==albCal||boardGame[x-2][y-1]==albCal||boardGame[x-1][y-2]==albCal||boardGame[x+1][y-2]==albCal||boardGame[x+2][y-1]==albCal||boardGame[x+2][y+1]==albCal)
            return true; //atacat de cal
        
        int j=1;
        int notRook=0; //ROOK~~~~~~~~~~~~~~~~

        while(notRook==0 && (x+j)<8) //||(x+j)>8 || (x-j)<0 || (y+j)>8 || (y-j)<0
        {
            
            if(boardGame[x+j][y]==albTura || boardGame[x+j][y]==albRegina) 
                return true; // atacat de tura de jos sau regina

            if(boardGame[x+j][y]!=albTura && boardGame[x+j][y]!=blank)
                if(boardGame[x+j][y]!=albRegina && boardGame[x+j][y]!=blank)
                    notRook++;

            j++;
        }

        j=1;
        notRook=0;

        while(notRook==0 && (x-j)>=0) //||(x+j)>8 || (x-j)<0 || (y+j)>8 || (y-j)<0
        {
            
            if(boardGame[x-j][y]==albTura || boardGame[x-j][y]==albRegina) 
                return true; // atacat de tura de sus
            if(boardGame[x-j][y]!=albTura && boardGame[x-j][y]!=blank)
                if(boardGame[x-j][y]!=albRegina && boardGame[x-j][y]!=blank)
                    notRook++;

            j++;
        }

        j=1;
        notRook=0;

        while(notRook==0 && (y-j)>=0) //||(x+j)>8 || (x-j)<0 || (y+j)>8 || (y-j)<0
        {
            
            if(boardGame[x][y-j]==albTura||boardGame[x][y-j]==albRegina) 
                return true; // atacat de tura din stanga
            
            if(boardGame[x][y-j]!=albTura && boardGame[x][y-j]!=blank)
                if(boardGame[x][y-j]!=albRegina && boardGame[x][y-j]!=blank)
                    notRook++;

            j++;
        }

        j=1;
        notRook=0;

        while(notRook==0 && (y+j)<8) //||(x+j)>8 || (x-j)<0 || (y+j)>8 || (y-j)<0
        {
            if(boardGame[x][y+j]==albTura||boardGame[x][y+j]==albRegina) 
                return true; // atacat de tura din dreapta
            if(boardGame[x][y+j]!=albTura && boardGame[x][y+j]!=blank)
                if(boardGame[x][y+j]!=albRegina && boardGame[x][y+j]!=blank)
                    notRook++;
            
            j++;

        } //ROOK

        j=1;
        int notBishop=0;

        while(notBishop==0 && (x+j)<8 && (y+j)<8) //BISHOP~~~~~~~~~~~~~~~~~~~
        {
            
            if(boardGame[x+j][y+j]==albNebun || boardGame[x+j][y+j]==albRegina)
                return true; // dreapta jos 
            if(boardGame[x+j][y+j]!=albNebun && boardGame[x+j][y+j]!=blank && boardGame[x+j][y+j]!=albRegina)
                      notBishop++;

            j++;
        }

        j=1;
        notBishop=0;

        while(notBishop==0 && (x-j)>=0 && (y+j)<8)
        {
            
            if(boardGame[x-j][y+j]==albNebun || boardGame[x-j][y+j]==albRegina)
                return true; // stanga jos 
            if(boardGame[x-j][y+j]!=albNebun && boardGame[x-j][y+j]!=blank && boardGame[x-j][y+j]!=albRegina)
                      notBishop++;

            j++;
        }

        j=1;
        notBishop=0;

        while(notBishop==0 && (x-j)>=0 && (y-j)>=0)
        {
            
            if(boardGame[x-j][y-j]==albNebun || boardGame[x-j][y-j]==albRegina)
                return true; // stanga sus
            if(boardGame[x-j][y-j]!=albNebun && boardGame[x-j][y-j]!=blank && boardGame[x-j][y-j]!=albRegina)
                      notBishop++;

            j++;
        }

        j=1;
        notBishop=0;

        while(notBishop==0 && (x+j)<8 && (y-j)>=0)
        {
            
            if(boardGame[x+j][y-j]==albNebun || boardGame[x+j][y-j]==albRegina)
                return true; // dreapta sus
            if(boardGame[x+j][y-j]!=albNebun && boardGame[x+j][y-j]!=blank && boardGame[x+j][y-j]!=albRegina)
                      notBishop++;

            j++;

        }//BISHOP

        //KING
        if(boardGame[x+1][y]==albRege) //jos
            return true;
        if(boardGame[x+1][y+1]==albRege) //jos dreapta
            return true;
        if(boardGame[x+1][y-1]==albRege) //jos stanga
            return true;

        if(boardGame[x][y+1]==albRege) // dreapta
            return true;
        if(boardGame[x][y-1]==albRege) // stanga
            return true;

        if(boardGame[x-1][y+1]==albRege) // sus dreapta
            return true;
        if(boardGame[x-1][y]==albRege) // sus 
            return true;
        if(boardGame[x-1][y-1]==albRege) // sus stanga
            return true;
        //KING
    
        
        return false;

    }

    if(Player==Alb)
    {
        if(boardGame[x-1][y+1]==negruPion||boardGame[x-1][y-1]==negruPion) // atacat de pioni pe diagonala
            return true;   

        if(boardGame[x+1][y+2]==negruCal ||boardGame[x-1][y+2]==negruCal ||boardGame[x-2][y+1]==negruCal||boardGame[x-2][y-1]==negruCal||boardGame[x-1][y-2]==negruCal||boardGame[x+1][y-2]==negruCal||boardGame[x+2][y-1]==negruCal||boardGame[x+2][y+1]==negruCal)
            return true; //atacat de cal

        int j=1;
        int notRook=0; //ROOK~~~~~~~~~~~~~~~~

        while(notRook==0 && (x+j)<8) //||(x+j)>8 || (x-j)<0 || (y+j)>8 || (y-j)<0
        {
            
            if(boardGame[x+j][y]==negruTura || boardGame[x+j][y]==negruRegina) 
                return true; // atacat de tura de jos sau regina

            if(boardGame[x+j][y]!=negruTura && boardGame[x+j][y]!=blank)
                if(boardGame[x+j][y]!=negruRegina && boardGame[x+j][y]!=blank)
                    notRook++;
            
            j++;
        }

        j=1;
        notRook=0;

        while(notRook==0 && (x-j)>=0) //||(x+j)>8 || (x-j)<0 || (y+j)>8 || (y-j)<0
        {
            
            if(boardGame[x-j][y]==negruTura || boardGame[x-j][y]==negruRegina) 
                return true; // atacat de tura de sus
            if(boardGame[x-j][y]!=negruTura && boardGame[x-j][y]!=blank)
                if(boardGame[x-j][y]!=negruRegina && boardGame[x-j][y]!=blank)
                    notRook++;

            j++;
        }

        j=1;
        notRook=0;

        while(notRook==0 && (y-j)>=0) //||(x+j)>8 || (x-j)<0 || (y+j)>8 || (y-j)<0
        {
            
            if(boardGame[x][y-j]==negruTura||boardGame[x][y-j]==negruRegina) 
                return true; // atacat de tura din stanga
            
            if(boardGame[x][y-j]!=negruTura && boardGame[x][y-j]!=blank)
                if(boardGame[x][y-j]!=negruRegina && boardGame[x][y-j]!=blank)
                    notRook++;

            j++;
        }

        j=1;
        notRook=0;

        while(notRook==0 && (y+j)<8) //||(x+j)>8 || (x-j)<0 || (y+j)>8 || (y-j)<0
        {
            
            if(boardGame[x][y+j]==negruTura||boardGame[x][y+j]==negruRegina) 
                return true; // atacat de tura din dreapta
            if(boardGame[x][y+j]!=negruTura && boardGame[x][y+j]!=blank)
                if(boardGame[x][y+j]!=negruRegina && boardGame[x][y+j]!=blank)
                    notRook++;

            j++;
        } //ROOK

       

        //BISHOP~~~~~~~~~~~~~~~~~~~

        j=1;
        int notBishop=0;

        while(notBishop==0 && (x+j)<8 && (y+j)<8) 
        {
            
            if(boardGame[x+j][y+j]==negruNebun || boardGame[x+j][y+j]==negruRegina)
                return true; // dreapta jos 
            if(boardGame[x+j][y+j]!=negruNebun && boardGame[x+j][y+j]!=blank && boardGame[x+j][y+j]!=negruRegina)
                      notBishop++;

            j++;
        }

        j=1;
        notBishop=0;

        while(notBishop==0 && (x-j)>=0 && (y+j)<8)
        {
            
            if(boardGame[x-j][y+j]==negruNebun || boardGame[x-j][y+j]==negruRegina)
                return true; // stanga jos 
            if(boardGame[x-j][y+j]!=negruNebun && boardGame[x-j][y+j]!=blank && boardGame[x-j][y+j]!=negruRegina)
                      notBishop++;

            j++;
        }

        j=1;
        notBishop=0;

        while(notBishop==0 && (x-j)>=0 && (y-j)>=0)
        {
            
            if(boardGame[x-j][y-j]==negruNebun || boardGame[x-j][y-j]==negruRegina)
                return true; // stanga sus
            if(boardGame[x-j][y-j]!=negruNebun && boardGame[x-j][y-j]!=blank && boardGame[x-j][y-j]!=negruRegina)
                      notBishop++;

            j++;
        }

        j=1;
        notBishop=0;

        while(notBishop==0 && (x+j)<8 && (y-j)>=0)
        {
            
            if(boardGame[x+j][y-j]==negruNebun || boardGame[x+j][y-j]==negruRegina)
                return true; // dreapta sus
            if(boardGame[x+j][y-j]!=negruNebun && boardGame[x+j][y-j]!=blank && boardGame[x+j][y-j]!=negruRegina)
                      notBishop++;

            j++;

        }//BISHOP

        //KING
        if(boardGame[x+1][y]==negruRege) //jos
            return true;
        if(boardGame[x+1][y+1]==negruRege) //jos dreapta
            return true;
        if(boardGame[x+1][y-1]==negruRege) //jos stanga
            return true;

        if(boardGame[x][y+1]==negruRege) // dreapta
            return true;
        if(boardGame[x][y-1]==negruRege) // stanga
            return true;

        if(boardGame[x-1][y+1]==negruRege) // sus dreapta
            return true;
        if(boardGame[x-1][y]==negruRege) // sus 
            return true;
        if(boardGame[x-1][y-1]==negruRege) // sus stanga
            return true;
        //KING
    
        
        return false;
    
    }
}

bool isCheckMate(int xRege, int yRege, int Player,int boardGame[][COLLUMNS])
{
    int Rege=boardGame[xRege][yRege];

    if(isAttacked(xRege+1,yRege+1,Player,boardGame)==false && legalMove(xRege,yRege,xRege+1,yRege+1,Player,boardGame)==true)
        return false;
    if(isAttacked(xRege+1,yRege,Player,boardGame)==false && legalMove(xRege,yRege,xRege+1,yRege,Player,boardGame)==true)
        return false;
    if(isAttacked(xRege+1,yRege-1,Player,boardGame)==false && legalMove(xRege,yRege,xRege+1,yRege-1,Player,boardGame))
        return false;

    if(isAttacked(xRege,yRege+1,Player,boardGame)==false && legalMove(xRege,yRege,xRege,yRege+1,Player,boardGame))
        return false;
    if(isAttacked(xRege,yRege-1,Player,boardGame)==false && legalMove(xRege,yRege,xRege,yRege-1,Player,boardGame))
        return false;

    if(isAttacked(xRege-1,yRege+1,Player,boardGame)==false && legalMove(xRege,yRege,xRege-1,yRege+1,Player,boardGame))
        return false;
    if(isAttacked(xRege-1,yRege,Player,boardGame)==false && legalMove(xRege,yRege,xRege-1,yRege,Player,boardGame))
        return false;
    if(isAttacked(xRege-1,yRege-1,Player,boardGame)==false && legalMove(xRege,yRege,xRege-1,yRege-1,Player,boardGame))
        return false;



    return true;

}

bool isTie(int boardGame[][COLLUMNS])
{
    int i,j;

    int pieseAlb=0;
    int pieseNegru=0;

    for(i=0;i<=7;i++)
        for(j=0;j<=7;j++)
        {
            if(boardGame[i][j]>0) // negru
            pieseNegru++;
            
            if(boardGame[i][j]<0) // alb
            pieseAlb++;

            if(pieseAlb>2||pieseNegru>2)
            return false;

            if(boardGame[i][j]!=albCal)
                if(boardGame[i][j]!=albCal)
                    if(pieseAlb==2)
                        return false;

            if(boardGame[i][j]!=negruCal)
                if(boardGame[i][j]!=negruCal)
                    if(pieseNegru==2)
                        return false;
        }

    return true;

}