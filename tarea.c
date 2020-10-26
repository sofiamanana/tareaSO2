#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>

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

int signo1(struct cuadrado *tablero, int *pos, int id){
    int num = rand()%5+1;
    if(num==1){
        //retrocede 1 cuadricula(no sobrepasar el inicio)
        printf("Retrocedes una casilla\n");
        int n_pos = pos[id]-1;
        if(n_pos<0){
            pos[id]=0;
        }else{
            pos[id]=n_pos;
        }
        return 1;
    }
    else if(num==2){
        //todos los jugadores (excepto el del turno) retroceden una cuadricula(no sobrepasar el inicio)
        printf("Todos retroceden una casilla\n");
        for(int i=0; i<4; i++){
            if(id==i){
                continue;
            }
            else{
                int n_pos = pos[i]-1;
                if(n_pos<0){
                    pos[i]=0;
                }else{
                    pos[i]=n_pos;
                }
            }
        }
        return 1;
    }
    else if(num==3){
        //el jugador avanza una cuadricula
        printf("Avanzaste una cuadricula\n");
        int n_pos = pos[id]+1;
        if(n_pos>=28){
            //gano
        }else{
            pos[id]=n_pos;
        }
        return 1;
    }
    else if(num==4){
        //el siguiente jugador pierde el turno
        printf("El siguiente jugador pierde el turno\n");
        return 2;
    }
    else if(num==5){
        //cambia el sentido de los turnos
        return 1;
    }

}

int signo2(struct cuadrado *tablero, int *pos, int id){
    int num = rand()%10+1;
    if(num==1 || num==2 || num==3){
        printf("Todos retroceden dos cuadriculas\n");
        for(int i=0; i<4; i++){
            int n_pos = pos[i]-2;
                if(n_pos<0){
                    pos[i]=0;
                }else{
                    pos[i]=n_pos;
                }
        }
        return 1;
    }
    else if(num==4 || num==5){
        printf("Todos avanzan hasta la cuadricula blanca mas cercana\n");
        for(int i=0; i<4; i++){
            int flag=0;
            int avanzar=1;
            while(flag==0 && (tablero[pos[i]+avanzar].efecto!=4)){
                if(tablero[pos[i]+avanzar].efecto==0){
                    flag=1;
                }
                else{
                    avanzar++;
                }
            }
        }
        return 1;
    }
    else if(num==6 || num==7){
        printf("El jugador cambia de lugar con el jugador que va en el ultimo lugar\n");
        int flag=0;
        int aux;
        int min=pos[id];
        int lugar=id;
        for(int i=0; i<4; i++){
            if(i==id){
                continue;
            }
            else{
                if(pos[id]>pos[i]){
                    flag=1;
                }
                else if(min>=pos[i]){
                    min=pos[i];
                    lugar=i;
                }
            }
        }
        if(flag==0){
            return 1;
        }
        aux=pos[id];
        pos[id]=min;
        pos[lugar]=aux;
        return 1;
    }
    else if(num==8 || num==9){
        printf("El jugador cambia de lugar con el jugador que va en primer lugar\n");
        int flag=0;
        int aux;
        int max=pos[id];
        int lugar=id;
        for(int i=0; i<4; i++){
            if(i==id){
                continue;
            }
            else{
                if(pos[id]<pos[i]){
                    flag=1;
                }
                else if(max<=pos[i]){
                    max=pos[i];
                    lugar=i;
                }
            }
        }
        if(flag==0){
            return 1;
        }
        aux=pos[id];
        pos[id]=max;
        pos[lugar]=aux;
        return 1;
    }
    else if(num==10){
        printf("Cambia de sentido el tablero\n");
        int i;
        for (i = 0; i<28;i++){
            if(tablero[i].efecto==1){
                tablero[i].efecto=2;
            }
            else if(tablero[i].efecto==2){
                tablero[i].efecto=1;
            }
        }
        for(i=0; i<4; i++){
            pos[i] = 28 - pos[i];
        }
        return 1;
    }
}

int moverJugador(struct cuadrado *tablero, int *pos, int id){
    int dado = tirarDado();
    printf("Dado: %d\n",dado);
    int pos_jugador = pos[id];
    int nueva_pos = pos_jugador+dado;
    pos[id]=nueva_pos;
    int enter;
    if(nueva_pos>=28){
        //gano
        pos[id]=nueva_pos;
        pos[5]=0;
        printf("Ganoo...Termino turno, presiones 0 para continuar\n");
        scanf("%d",&enter);
        return 0;
    }
    else if(tablero[nueva_pos].efecto==1){
        //1 signo de pregunta-->funcion
        pos[id]=nueva_pos;
        int ret = signo1(tablero,pos,id);
        printf("?...Termino turno, presiones 0 para continuar\n");
        printf("ret: %d\n",ret);
        scanf("%d",&enter);
        return ret;
    }
    else if(tablero[nueva_pos].efecto==2){
        //2 signos de preguntas-->funcion
        pos[id]=nueva_pos;
        int ret = signo2(tablero, pos, id);
        printf("??...Termino turno, presiones 0 para continuar\n");
        printf("ret: %d\n",ret);
        scanf("%d",&enter);
        return ret;
    }
    else if(tablero[nueva_pos].efecto==4){
        pos[5]=0;
        pos[id]=nueva_pos;
        printf("Ganooo...Termino turno, presiones 0 para continuar\n");
        scanf("%d",&enter);
        return 0;
    }
    else if(tablero[nueva_pos].efecto==0){
        pos[id]=nueva_pos;
        printf("Celda blanca...Termino turno, presiones 0 para continuar\n");
        scanf("%d",&enter);
        return 1;
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
    pos[5]=100; //flag de salida
    pos[6]=0;   //flag de saltar turno

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

    if(id==0){  //jugador 1
        close(da[1]);
        close(ab[0]);
    }
    else if(id==1){  //jugador 2
        close(ab[1]);
        close(bc[0]);
    }
    else if(id==2){  //jugador 3
        close(bc[1]);
        close(cd[0]);
    }
    else if(id==3){  //jugador 4
        close(cd[1]);
        close(da[0]);
    }

    int k=0;

    while(pos[5] == 100){
        if(id==0){  //JUGADOR 1
            if(k==0){
                printf("Turno del jugador 1\n");
                int opc = moverJugador(tablero,pos,id);
                k=1;
                if(opc==1){
                    write(ab[1],"1",1);    
                }
                else if(opc==2){ //se salta un turno
                    write(ab[1],"2",1);
                }
                else if(opc==0){
                    break;
                }
            }
            else{
                read(da[0],leer,1);
                if(strcmp(leer,"1\n")!=0){
                    printf("Turno del jugador 1\n");
                    int opc = moverJugador(tablero,pos,id);
                    if(opc==1){
                        write(ab[1],"1",1);    
                    }
                    else if(opc==2){ //se salta un turno
                        write(ab[1],"2",1);
                    }
                    else if(opc==0){
                        break;
                    }
                }
                else if(strcmp(leer,"2\n")!=0){
                    write(ab[1],"1",1);
                }
            }
        }
        else if(id==1){ //JUGADOR 2
            read(ab[0],leer,1);
            if(strcmp(leer,"1\n")!=0){
                printf("Turno del jugador 2\n");
                int opc = moverJugador(tablero,pos,id);
                if(opc==1){
                    write(bc[1],"1",1);    
                }
                else if(opc==2){ //se salta un turno
                    write(bc[1],"2",1);
                }
                else if(opc==0){
                    break;
                }
                
            }
            else if(strcmp(leer,"2\n")!=0){
                write(bc[1],"1",1);
            }
        }
        else if(id==2){ //JUGADOR 3
            read(bc[0],leer,1);
            if(strcmp(leer,"1\n")!=0){
                printf("Turno del jugador 3\n");
                int opc = moverJugador(tablero,pos,id);
                if(opc==1){
                    write(cd[1],"1",1);    
                }
                else if(opc==2){ //se salta un turno
                    write(cd[1],"2",1);
                }
                else if(opc==0){
                    break;
                }
                
            }
            else if(strcmp(leer,"2\n")!=0){
                write(cd[1],"1",1);
            }
        }
        else if(id==3){ //JUGADOR 4
            read(cd[0],leer,1);
            if(strcmp(leer,"1\n")!=0){
                printf("Turno del jugador 4\n");
                int opc = moverJugador(tablero,pos,id);
                if(opc==1){
                    write(da[1],"1",1);    
                }
                else if(opc==2){ //se salta un turno
                    write(da[1],"2",1);
                }
                else if(opc==0){
                    break;
                }
                
            }
            else if(strcmp(leer,"2\n")!=0){
                write(da[1],"1",1);
            }
        }
    }
    printf("GANASTE WENA WN!!!\n");
    return 0;
}