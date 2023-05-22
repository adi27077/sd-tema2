/* MUSAT-BURCEA Adrian - 312CB */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#ifndef _LISTA_GENERICA_
#define _LISTA_GENERICA_

typedef struct celulag
{
  void* info;           /* adresa informatie */
  struct celulag *urm;   /* adresa urmatoarei celule */
} TCelulaG, *TLG; /* tipurile Celula, Lista  */

typedef int (*TFElem)(void*);     /* functie prelucrare element */
typedef int (*TFElem2)(void *, void *); /*functie prelucrare element
                                        cu info suplimentar*/
typedef int (*TFCmp)(void *, void *); /* comparare doua elemente => int*/
typedef float (*TFCmpF)(void*, void*); /* comparare doua elemente => float*/
typedef void (*TF)(void*);     /* functie afisare/eliberare un element */
typedef void (*TFF)(void *, FILE *); /*functie afisare in fisier*/

/* functii lista generica */
int InsLG(TLG*, void*);  /*- inserare la inceput reusita sau nu (1/0) -*/
void Distruge(TLG* aL, TF fe); /* distruge lista */
size_t LungimeLG(TLG*);   /* numarul de elemente din lista */
/* afiseaza elementele din lista, folosind o functie de tip TFAfi */
void Afisare(TLG*, TFF afiEl, FILE *g);
int InsOrd(TLG *aL, void *ae, TFCmpF f); /*inserare ordonata, return pozitie*/
int InsPoz(TLG *aL, void *ae, TFCmp f, TFElem m); /*inserare la o anumita pozitie si
                                                    modificarea pozitiilor urmatoare*/
void ElimUlt(TLG *aL, TF free_elem); /*eliminare ultim element*/
int Prel(TLG *aL, char *nume, void *info, TFElem2 prel_elem, TFCmp f);
/*^functie pt prelucrarea unui element folosinf anumite informatii*/
void *Extrage(TLG* aL, char *nume, TFCmp f); /*functie eliminare element*/
void *ExtragePoz(TLG* aL, char *nume, TFCmp f, TFElem m);
/*functie eliminare element cu actualizarea pozitiei elementelor urmatoare*/


#endif
