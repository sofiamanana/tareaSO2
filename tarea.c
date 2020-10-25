#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <time.h>

struct cuadrado { 
    int efecto;  
};
void crearTablero(struct cuadrado * tablero){
    for(int i=0; i<29; i++){
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
    
}
/*
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
}*/

int tirarDado(){
    int num = rand()%6+1;
    return num;
}

void signo1(struct cuadrado *tablero, int nuevapos){

}

void signo2(struct cuadrado *tablero, int nuevapos){

}

void moverJugador(struct cuadrado *tablero, int *pos, int id){
    int dado = tirarDado();
    int pos_jugador = pos[id];
    int nueva_pos = pos_jugador+dado;
    if(nueva_pos>=28){
        //gano
    }
    else if(tablero[nueva_pos].efecto==1){
        //1 signo de pregunta-->funcion
    }
    else if(tablero[nueva_pos].efecto==2){
        //2 signos de preguntas-->funcion
    }
    else if(tablero[nueva_pos].efecto==4){
        pos[5]=0;
    }
    else if(tablero[nueva_pos].efecto==0){
        pos[id]=nueva_pos;
        printf("Termino turno\n");
    }
}

int main(){
    FILE *fp, *fp2;
    fp = fopen("/tmp/comp","w+");
    fp2 = fopen("/tmp/comp","w+");
    int mem=0,mem2=0;
    key_t clave;
    key_t clave2;
    clave = ftok("/tmp/comp",123);
    clave2 = ftok("/tmp/comp",234);
    int id = 0;
    int *pos;
    struct cuadrado * tablero = NULL;
    mem = shmget(clave,sizeof(tablero)+1,0777 | IPC_CREAT);
    mem2 = shmget(clave2,sizeof(pos)+1,0777 | IPC_CREAT);
    tablero = (struct cuadrado *) shmat(mem,NULL,0); 
    pos = (int *) shmat(mem2,NULL,0);
    int ab[2],bc[2],cd[2],da[2];
    char leer[1];
    
    crearTablero(tablero);
    //printTablero(tablero);
    for(int i=0; i<4; i++){
        pos[i]=0;
    }
    pos[5]=100;

    //ARREGLAR Y PONER EXCEPCIONES
    int a = pipe(ab);
    if(a==-1){
        printf("fallo pipe");
    }
    pipe(bc);
    pipe(cd);
    pipe(da);

    int pid;
    for (int i = 0; i<3;i++){   
        pid = fork();
        if(pid == 0){
            id = i+1;
            break;
        }
    }

    if(id==0){
        printf("soy el proceso padre\n");
        close(da[1]);
        close(ab[0]);
    }
    else if(id==1){
        printf("soy el proceso 1\n");
        close(ab[1]);
        close(bc[0]);
    }
    else if(id==2){
        printf("soy el proceso 2\n");
        close(bc[1]);
        close(cd[0]);
    }
    else if(id==3){
        printf("soy el proceso 3\n");
        close(cd[1]);
        close(da[0]);
    }

    return 0;
}