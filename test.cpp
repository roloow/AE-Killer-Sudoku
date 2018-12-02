//algoritmo genetico en C++ optimizar F(x) 
//clase desarrollado el 24nov
//definicion de librerias
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>

//definicion de constantes
#define TAMANO_POB	13
#define LONG_CROM	343
#define PCRUCE	0.34
#define PMUTACION	0.33

//definicion de la estructura poblacion
struct poblacion{
double cantidad;
unsigned int cromosoma[LONG_CROM];
double capacidad;
double frec;
double frec_ac;
};

//declaracion de variables de tipo poblacion
struct poblacion pob[TAMANO_POB],best,bestgen;
struct poblacion nueva_pob[TAMANO_POB];

//declaracion de funciones a emplear en el codigo genetico
void initialize_poblacion();
double funcion(double cantidad);
double valor(int indiv,int pos_ini,int pos_fin,int lim_inf,int lim_sup);
void evaluar();
void mejor();
float aleatorio();
int alea_int(int lim_inf,int lim_sup);
void seleccion();
void cruce();
void crossover(int parent1,int parent2);
void mutacion();
void mutation(int pos);

//definicion de funcion principal
void main(){
int t;
double cap_ant;

t=0;
initialize_poblacion();
evaluar();
printf("%4s %7s	%6s\n",
"generacion","bg can", "bg cap");
printf(
"%4d %7.0f %7.1f \n",
t,best.cantidad,best.capacidad);
cap_ant=best.capacidad;
while(t<1000){
t++;
seleccion();
cruce();
mutacion();
evaluar();
if(best.capacidad!=cap_ant){
printf(	"%4d %7.0f %7.1f \n", t,best.cantidad,best.capacidad);
cap_ant=best.capacidad;
}
}
getch();
}

//inicializar de manera aleatoria la poblacion OK
void initialize_poblacion(){
int i,j;
srand( (unsigned)time( NULL ) );
for(i=0;i<TAMANO_POB;i++)
for(j=0;j<LONG_CROM;j++)
pob[i].cromosoma[j]=alea_int(0,1);
}

//funcion que genera un numero aleatorio de tipo flotante
float aleatorio(){
return (float) (rand()*10001/
(RAND_MAX-1))/10000;
}

// funcion que devuelve de manera aleatoria el valor de un gen
int alea_int(int lim_inf,int lim_sup){
return (int) (aleatorio()*
(lim_sup-lim_inf+1))+lim_inf;
}


//funcion que calcula F(x), F(x)/sum, F.acum
void evaluar(){
int i;
double suma=0;

for(i=0;i<TAMANO_POB;i++){
pob[i].cantidad=valor(i,0,5,-1,3);
pob[i].capacidad=funcion(pob[i].cantidad);
suma+=pob[i].capacidad;
}
pob[0].frec=pob[0].capacidad/suma;
pob[0].frec_ac=pob[0].frec;
for(i=1;i<TAMANO_POB;i++){
pob[i].frec=pob[i].capacidad/suma;
pob[i].frec_ac=pob[i-1].frec_ac +
pob[i].frec;
}
mejor();
}

//calcular el valor de la funcion a optimizar
double funcion(double cantidad){
double fx;
fx= 10+sin(10*3.14159*cantidad);
return fx;
}

//calcula el valor real X de un cromosoma 
double valor(int indiv,int pos_ini,int pos_fin,
int lim_inf,int lim_sup){

double factor,val;
int i;

factor=1;
val=0;
for(i=pos_fin;i>=pos_ini;i--){
val+=
(pob[indiv].cromosoma[i] * factor);
factor*=2;
}
val=lim_inf + (lim_sup-lim_inf) /(pow((double)2,pos_fin-pos_ini+1)-1) * val;
return val;
}

//guarda los mejores cromosomas
void mejor(){
int i;

i=0;
best=pob[i];
for(i=1;i<TAMANO_POB;i++)
if(best.capacidad<pob[i].capacidad)
best=pob[i];
if(bestgen.capacidad<best.capacidad)
bestgen=best;

}

//selecciona los mejores cromosomas
void seleccion(){
float aleat;
int i,j;

for(i=0;i<TAMANO_POB;i++){
aleat=aleatorio();
j=0;
while(aleat>pob[j].frec_ac &&
j<TAMANO_POB-1) j++;
nueva_pob[i]=pob[j];
}
for(i=0;i<TAMANO_POB;i++)
pob[i]=nueva_pob[i];
}
//identifica el punto de cruce
void cruce(){
int selec[TAMANO_POB],indice,i;
float aleat;

indice=-1;
for(i=0;i<TAMANO_POB;i++){
aleat=aleatorio();
if(aleat<PCRUCE){
indice++;
selec[indice]=i;
}
}
if(indice % 2) indice--;
for(i=0;i<indice/2;i++)
crossover(selec[i],selec[indice/2+i]);
}

//cruza los cromosomas
void crossover(int padre1,int padre2){
int punto_cruce,i;
unsigned int temp;

punto_cruce=alea_int(0,LONG_CROM-2);
for(i=punto_cruce+1;i<LONG_CROM;i++){
temp=pob[padre1].cromosoma[i];
pob[padre1].cromosoma[i]=
pob[padre2].cromosoma[i];
pob[padre2].cromosoma[i]=temp;
}
}

//compara las probabilidades de los genes con PMUTACION
void mutacion(){
int selec[TAMANO_POB],indice,i;
float aleat;

indice=-1;
for(i=0;i<TAMANO_POB;i++){
aleat=aleatorio();
if(aleat<PMUTACION){
indice++;
selec[indice]=i;
}
}
for(i=0;i<indice;i++) mutation(selec[i]);
}

//funcion que va mutar
void mutation(int pos){
int punto;

punto=alea_int(0,LONG_CROM-1);
pob[pos].cromosoma[punto]=
!pob[pos].cromosoma[punto];
