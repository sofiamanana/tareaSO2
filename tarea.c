#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <time.h>

struct cuadrado { 
    int jugadores[4];
    int efecto;  
};
void crearTablero(struct cuadrado * tablero){
    for(int i=0; i<29; i++){
        tablero[i].jugadores[0] = 0;
        tablero[i].jugadores[1] = 0;
        tablero[i].jugadores[2] = 0;
        tablero[i].jugadores[3] = 0;
        if(i==2 || i==4 || i==6 || i==12 || i==14 || i==21 || i==23 || i==25 || i==27){
            tablero[i].efecto = 1;
        }
        else if(i==16 || i==22 || i==24 || i==26){
            tablero[i].efecto = 2;
        }
        else if(i==8 || i==10 || i==18 || i==20){
            tablero[i].efecto = 3;
        }
        else if(i==0 || i==28){
            tablero[i].efecto = 4;
        }
        else{
            tablero[i].efecto = 0;
        }   
    }
    tablero[0].jugadores[0] = 1;
    tablero[0].jugadores[1] = 1;
    tablero[0].jugadores[2] = 1;
    tablero[0].jugadores[3] = 1;
    
}

void printTablero(struct cuadrado * tablero){
    printf("------------------\n");
    for(int i = 0; i<29; i++){
        if(tablero[i].efecto == 1){
            if(tablero[i].jugadores[0] == i){
                printf("| 1");
            }
            else{
                printf("| ? ");
            }
            
        }
        else if(tablero[i].efecto == 2){
            if(tablero[i].jugadores[0] == i){
                printf("| 1");
            }else{
                printf("| ?? ");
            }            
        }
        else if(tablero[i].efecto == 3){
            if(tablero[i].jugadores[0] == i){
                printf("| 1");
            }else{
                printf("|  \n");
            }
            
        }
        else if(tablero[i].efecto == 4){
            if(tablero[i].jugadores[0] == i){
                printf("| 1");
            }else{
                printf("| inicio");
            }
            
        }
        else{
            if(tablero[i].jugadores[0] == i){
                printf("| 1");
            }else{
                printf("|  ");
            }            
        }
    }
}

int main(){
    FILE *fp;
    fp = fopen("/tmp/comp","w+");
    int mem=0;
    key_t clave;
    clave = ftok("/tmp/comp",123);
    int id = 0;
    struct cuadrado * tablero = NULL;
    mem = shmget(clave,sizeof(tablero)+1,0777 | IPC_CREAT);
    printf("%d\n",mem);
    tablero = (struct cuadrado *) shmat(mem,NULL,0); 
    crearTablero(tablero);
    //printTablero(tablero);
/*
     for (int i = 1; i<4;i++){       //crear los procesos hijos (3)
        if (fork() == 0){
            id = i;
            break;
        }
    }*/
    int p[2];
    char leer[6];
    pipe(p);
    if(fork() == 0){
        id = 1;
    }
    if(id==0){
        close(p[0]);
        write(p[1],"55",1);
    }
    else if (id==1){
        close(p[1]);
        read(p[0],leer,1);
        printf("%s\n",leer);
    }
    return 0;
}