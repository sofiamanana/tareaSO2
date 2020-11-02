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

/*
Nombre: crearTablero.
Parametros: puntero a struct cuadrado.
Retorno: funcion void.
Descripcion: define el entero efecto del struct y forma el tablero. El puntero tablero esta en memoria compartida.
*/

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
        else if(i==0){
            tablero[i].efecto = 4;
        }
        else if(i==28){
            tablero[i].efecto = 5;
        }
        else{
            tablero[i].efecto = 0;
        }   
    }
    
}

/*
Nombre: printTablero.
Parametros: puntero struct cuadrado y puntero a un arreglo de enteros (ambos en memoria compartida).
Retorno: funcion void.
Descripcion: imprime en pantalla el tablero guiandose por el entero efecto en cada struct.
*/

void printTablero(struct cuadrado * tablero,int *pos){
    printf("------------------\n");
    printf("|");
    for(int i = 0; i<29; i++){
        if(pos[0]==i){
            printf("1 ");
        }
        if(pos[1]==i){
            printf("2 ");
        }
        if(pos[2]==i){
            printf("3 ");
        }
        if(pos[3]==i){
            printf("4 ");
        }
        if(tablero[i].efecto==0){
            printf(" |");
        }else if(tablero[i].efecto==1){
            printf("?|");
        }
        else if(tablero[i].efecto==2){
            printf("??|");
        }
        else if(tablero[i].efecto==3){
            printf("->|\n|");
        }
        else if(tablero[i].efecto==4){
            printf("inicio|");
        }
        else if(tablero[i].efecto==5){
            printf("fin|");
        }
    }
    printf("\n");
}

/*
Nombre: tirarDado().
Parametros: ninguno.
Retorno: entero.
Descripcion: genera un numero aleatorio entre 1 y 6.
*/

int tirarDado(){
    srand(getpid());
    int num = 1+(int)(((5-0+1.0)*rand())/(INT_MAX+1.0));
    return num;
}

/*
Nombre: signo1.
Parametros: puntero struct cuadrado, puntero a un arreglo de enteros y un entero id (id del proceso que la llama).
Retorno: entero.
Descripcion: se definen las funciones que pueden aparecer cuando un jugador cae en una casilla con un signo de pregunta,
todas tienen la misma probabilidad de ocurrencia.
*/

int signo1(struct cuadrado *tablero, int *pos, int id){
    srand(getpid());
    int num = 0+(int)(((4-0+1.0)*rand())/(INT_MAX+1.0));
    if(num==0){
        //retrocede 1 cuadricula(no sobrepasar el inicio) READY
        printf("Retrocedes una casilla\n");
        int n_pos = pos[id]-1;
        if(n_pos<0){
            pos[id]=0;
        }else{
            pos[id]=n_pos;
        }
        return 1;
    }
    else if(num==1){
        //todos los jugadores (excepto el del turno) retroceden una cuadricula(no sobrepasar el inicio) READY
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
    else if(num==2){
        //el jugador avanza una cuadricula READY
        printf("Avanzaste una cuadricula\n");
        int n_pos = pos[id]+1;
        if(n_pos>=28){
            //gano
        }else{
            pos[id]=n_pos;
        }
        return 1;
    }
    else if(num==3){
        //el siguiente jugador pierde el turno  READY
        printf("El siguiente jugador pierde el turno\n");
        return 2;
    }
    else if(num==4){
        //cambia el sentido de los turnos  READY
        printf("Se cambia el sentido de los turnos\n");
        if(pos[6]==0){
            pos[6]=1;
        }else{
            pos[6]=0;
        }
        return 3;
    }
    return 1;
}

/*
Nombre: signo2
Parametros:puntero a struct cuadrado, puntero a arreglo de enteros y entero id (id del proceso que la llama).
Retorno: entero.
Descripcion: igual que signo1 solo que ahora son las funciones de cuando un jugador cae en una casilla con dos signos
de pregunta, cada una con su respectiva probabilidad de ocurrencia.
*/

int signo2(struct cuadrado *tablero, int *pos, int id){
    srand(getpid());
    int num = 0+(int)(((9-0+1.0)*rand())/(INT_MAX+1.0));
    if(num==0 || num==1 || num==1){
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
    else if(num==3 || num==4){
        printf("Todos avanzan hasta la cuadricula blanca mas cercana\n");
        for(int i=0; i<4; i++){
            int flag=0;
            int avanzar=1;
            while(flag==0 && (tablero[pos[i]+avanzar].efecto!=4)){
                if(tablero[pos[i]+avanzar].efecto==0){
                    flag=1;
                    pos[i]=pos[i]+avanzar;
                }
                else{
                    avanzar++;
                }
            }
        }
        return 1;
    }
    else if(num==5 || num==6){
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
                if(min>pos[i]){
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
    else if(num==7 || num==8){
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
                if(max<pos[i]){
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
    else if(num==9){
        printf("Se cambia de sentido el tablero\n");
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
    return 1;
}

/*
Nombre: moverJugador
Parametros: puntero a struct cuadrado, puntero a arreglo de enteros y entero id (id del proceso que la llama).
Retorno: entero.
Descripcion: funcion que reemplaza las nuevas posiciones de los jugadores una vez que avanzan y que llama a las respectivas
funciones de acuerdo con el efecto que este en la casilla que cae el jugador.
*/

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
        pos[7]=id;
        return 0;
    }
    else if(tablero[nueva_pos].efecto==1){
        //1 signo de pregunta-->funcion
        pos[id]=nueva_pos;
        printf("El jugador quedo en una celda ??\n");
        int ret = signo1(tablero,pos,id);
        printf("Termino turno, presiones 0 para continuar\n");
        scanf("%d",&enter);
        return ret;
    }
    else if(tablero[nueva_pos].efecto==2){
        //2 signos de preguntas-->funcion
        pos[id]=nueva_pos;
        printf("El jugador quedo en una celda ??\n");
        int ret = signo2(tablero, pos, id);
        printf("Termino turno, presiones 0 para continuar\n");
        scanf("%d",&enter);
        return ret;
    }
    else if(tablero[nueva_pos].efecto==4){
        //gano
        pos[5]=0;
        pos[7]=id;
        pos[id]=nueva_pos;
        return 0;
    }
    else if(tablero[nueva_pos].efecto==0 || tablero[nueva_pos].efecto==3){
        pos[id]=nueva_pos;
        printf("El jugador quedo en una celda blanca\n");
        printf("Termino turno, presiones 0 para continuar\n");
        scanf("%d",&enter);
        return 1;
    }
    return 1;
}

int main(){
    //Se define la memoria compartida a usar y las variables que se necesitaran despues.
    FILE *fp, *fp2;
    fp = fopen("/tmp/comp","w+");
    fp2 = fopen("/tmp/comp2","w+");
    int mem=0,mem2=0;
    key_t clave;
    key_t clave2;
    clave = ftok("/tmp/comp",123);
    clave2 = ftok("/tmp/comp2",234);
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
    //Se definen las primeras 4 posiciones del arreglo pos, guardara las posiciones de los jugadores en cada momento.
    for(int i=0; i<4; i++){
        pos[i]=0;
    }
    pos[5]=100; //flag de salida para cuando un jugador gane.
    pos[6]=0;   //flag de cambiar orden de turnos, si es 1 los turnos van 4-3-2-1, si es 0 los turnos va 1-2-3-4.
    pos[7]=0;   //id del jugador ganador.

    //Se definen las pipes entre los procesos.

    if(pipe(ab)==-1){
        printf("fallo pipe");
    }
    if(pipe(bc)==-1){
        printf("fallo pipe");
    }
    if(pipe(cd)==-1){
        printf("fallo pipe");
    }
    if(pipe(da)==-1){
        printf("fallo pipe");
    }

    //Se crean los procesos hijos con fork() y se define la variable id en cada una de ellos, sera la variable con la cual 
    //identificaremos a los procesos mas adelante.

    int pid;
    for (int i = 0; i<3;i++){   
        pid = fork();
        if(pid == 0){
            id = i+1;
            break;
        }
    }

    //Se cierran las pipes donde corresponde en cada proceso, con esto hacemos que las pipes sean unilaterales entre cada
    //proceso asi se forma un "circulo" de mensajes entre ellos, comenzando en el proceso 0 y terminando en el 3.

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

    //Cuando se cumpla la condicion del while, significara que un jugador gano el juego.

    while(pos[5] == 100){
        if(id==0){  //Jugador 1
            if(k==0){ //k nos avisa que es la primera ronda y que este no parte leyendo un mensaje de las pipes, una vez pasado
            //esto, el programa no vuelve a entrar.
                printTablero(tablero,pos);
                printf("Turno del jugador 1\n");
                int opc = moverJugador(tablero,pos,id);  //Llama a la funcion moverJugador, segun el valor que retorne es que hace despues.
                printTablero(tablero,pos);
                k=1;
                if(opc==1){ //Sigue al siguiente jugador
                    write(ab[1],"1",1);    
                }
                else if(opc==2){ //Se salta un turno
                    write(ab[1],"2",1);
                }
                else if(opc==3){ //Da vuelta los turnos
                        write(ab[1],"3",1);                    
                }
                else if(opc==0){
                    break;
                }
            }
            else{ //Entra aca la segunda vez que le toca al jugador 1 en adelante.
                read(da[0],leer,1);
                if(strcmp(leer,"1")==0 && pos[5]==100){
                    printf("Turno del jugador 1\n");
                    int opc = moverJugador(tablero,pos,id);
                    printTablero(tablero,pos);
                    if(opc==1){
                        if(pos[6]==1){ //Si es 1, se continua con los turnos en sentido contrario.
                            write(ab[1],"3",1);
                        }
                        else{
                            write(ab[1],"1",1);   
                        }  
                    }
                    else if(opc==2){ //Se salta un turno.
                        if(pos[6]==1){
                            write(ab[1],"3",1);
                        }
                        else{
                            write(ab[1],"2",1);   
                        }    
                    }
                    else if(opc==3){ //Da vuelta los turnos
                        if(pos[6]==0){
                            write(ab[1],"1",1);
                        }
                        else{
                            write(ab[1],"3",1);
                        }
                    }
                    else if(opc==0){
                        break;
                    }
                }
                else if(strcmp(leer,"3")==0 && pos[5]==100){
                    write(ab[1],"2",1);
                }
                else if(strcmp(leer,"2")==0 && pos[5]==100){
                    write(ab[1],"1",1);
                }
            }
        }
        else if(id==1){ //Jugador 2
            read(ab[0],leer,1);
            if(strcmp(leer,"1")==0 && pos[5]==100){
                printf("Turno del jugador 2\n");
                int opc = moverJugador(tablero,pos,id);
                printTablero(tablero,pos);
                if(opc==1){
                    if(pos[6]==1){
                        write(bc[1],"3",1);
                    }
                    else{
                        write(bc[1],"1",1);   
                    }   
                }
                else if(opc==2){ //se salta un turno
                    if(pos[6]==1){
                        write(bc[1],"3",1);
                    }
                    else{
                        write(bc[1],"2",1);   
                    }
                }
                else if(opc==3){
                    if(pos[6]==0){
                        write(bc[1],"1",1);
                    }
                    else{
                        write(bc[1],"3",1);
                    }
                }
                else if(opc==0){
                    break;
                }
                
            }
            else if(strcmp(leer,"3")==0 && pos[5]==100){
                write(bc[1],"2",1);
            }
            else if(strcmp(leer,"2")==0 && pos[5]==100){
                write(bc[1],"1",1);
            }
        }
        else if(id==2){ //JUGADOR 3
            read(bc[0],leer,1);
            if(strcmp(leer,"1")==0 && pos[5]==100){
                printf("Turno del jugador 3\n");
                int opc = moverJugador(tablero,pos,id);
                printTablero(tablero,pos);
                if(opc==1){
                    if(pos[6]==1){
                        write(cd[1],"3",1);
                    }
                    else{
                        write(cd[1],"1",1);   
                    }
                }
                else if(opc==2){ //se salta un turno
                    if(pos[6]==1){
                        write(cd[1],"3",1);
                    }
                    else{
                        write(cd[1],"2",1);   
                    }
                }
                else if(opc==3){
                    if(pos[6]==1){
                        write(cd[1],"3",1);
                    }
                    else{
                        write(cd[1],"1",1);
                    }
                }
                else if(opc==0){
                    break;
                }
                
            }
            else if(strcmp(leer,"3")==0 && pos[5]==100){
                write(cd[1],"2",1);
            }
            else if(strcmp(leer,"2")==0 && pos[5]==100){
                write(cd[1],"1",1);
            }
        }
        else if(id==3){ //JUGADOR 4
            read(cd[0],leer,1);
            if(strcmp(leer,"1")==0 && pos[5]==100){
                printf("Turno del jugador 4\n");
                int opc = moverJugador(tablero,pos,id);
                printTablero(tablero,pos);
                if(opc==1){ 
                    if(pos[6]==1){
                        write(da[1],"3",1);
                    }
                    else{
                        write(da[1],"1",1);   
                    }  
                }
                else if(opc==2){ //se salta un turno
                    if(pos[6]==1){
                        write(da[1],"3",1);
                    }
                    else{
                        write(da[1],"2",1);   
                    }
                }
                else if(opc==3){
                    if(pos[6]==0){
                        write(da[1],"1",1);
                    }
                    else{
                        write(da[1],"3",1);
                    }
                }
                else if(opc==0){
                    break;
                }
                
            }
            else if(strcmp(leer,"3")==0 && pos[5]==100){
                write(da[1],"2",1);
            }
            else if(strcmp(leer,"2")==0 && pos[5]==100){
                write(da[1],"1",1);
            }
        }
    }
    if(id==0){
        printf("Gano el jugador: %d!!! FELICIDADES!!!\n",id+1);
    }
    
    fclose(fp);
    fclose(fp2);
    return 0;
}