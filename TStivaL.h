/* MUSAT-BURCEA Adrian - 312CB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#ifndef _STIVA_LISTA_
#define _STIVA_LISTA_

typedef struct celst
{ 
    struct celst *urm; /* adr urmatoarei celule */
    void* info; /* adresa informatie */
}TCelSt, *ACelSt;

typedef struct stiva
{ 
    size_t dime; /* dim.element */
    ACelSt vf; /* adresa celulei din varf */
}TStiva, *ASt;

typedef void (*TFS)(void *); //tip functie eliberare element
typedef int (*TFElemS)(void *, void *); //tip functie prelucrare element cu info
typedef void (*TFSF)(void *, FILE *); //tip functie afisare in fisier

#define VF(a) (((ASt)(a))->vf)
#define DIMES(a) (((ASt)(a))->dime)
#define VIDAS(a) (VF(a) == NULL)

void* InitS(size_t d); //initializare stiva
int Push(void* s, void* ae); //inserare in stiva
int Pop(void* s, void* ae); //extragere din stiva
void ResetS(void* s, TFS f); //resetare stiva
void DistrS(void** s, TFS f); //distrugere stiva
void Rastoarna(void *d, void *s); //rastoarna stiva sursa peste stiva dest
void Suprapune(void *d, void *s); //suprapune stiva sursa la stiva dest
void AfiS(void *s, TFSF afiel, FILE *g); //afisare stiva in fisier
int PrelS(void *s, char *nume, void *info, TFElemS prel_elem, TFElemS f);
/*^functie prelucrare element din stiva prin parcurgerea listei*/
void *FindS(void *s, char *nume, TFElemS f); //gasire element in stiva

#endif
