/* MUSAT-BURCEA Adrian - 312CB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#ifndef _COADA_LISTA_
#define _COADA_LISTA_

typedef struct cel   /*dimensiune variabila */
{
    struct cel * urm;
    void* info;
}TCel, *ACel;

typedef struct coada
{  
    size_t dime; /*dim element */
    ACel ic, sc; /*adr prima, ultima celula */
}TCoada, *AQ;

typedef void (*TFQ)(void *); //tip functie eliberare
typedef int (*TFElemQ)(void *, void *); //tip functie prelucrare element cu info
typedef void (*TFQF)(void *, FILE *); //tip functie afisare in fisier

#define IC(a) (((AQ)(a))->ic)
#define SC(a) (((AQ)(a))->sc)
#define DIMEQ(a) (((AQ)(a))->dime)
#define VIDAQ(a) (IC(a) == NULL && SC(a) == NULL)

void *InitQ(size_t d); //initializare coada
int InsQ(void* c, void* ae); //inserare in coada
int ExtrQ(void* c, void* ae); //extragere din coada
void ResetQ(void *c, TFQ f); //resetare coada
void DistrQ(void **c, TFQ f); //distrugere coada
void ConcatQ(void *d, void *s); //concatenare coada sursa la coada dest
void AfiQ(void *c, TFQF afiel, FILE *g); //afisare coada in fisier
int PrelQ(void *c, char *nume, void *info, TFElemQ prel_elem, TFElemQ f);
/*^functie prelucrare element din coada prin parcurgerea listei*/
void *FindQ(void *c, char *nume, TFElemQ f); //gasire element in coada

#endif
