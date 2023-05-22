/* MUSAT-BURCEA Adrian - 312CB */

#include "TCoadaL.h"

//initializare coada
void *InitQ(size_t d)
{
    AQ c;
    c = (AQ)malloc(sizeof(TCoada));
    if(!c) 
        return NULL;
    c->dime = d;
    c->ic = NULL;
    c->sc = NULL;
    return (void*)c;
}

//introducere in coada
int InsQ(void* c, void* ae)
{
    ACel aux;
    aux = malloc(sizeof(TCel));
    if(!aux)
        return 0;
    aux->info = malloc(DIMEQ(c));
    if(!aux->info)
    {
        free(aux);
        return 0;
    }
    memcpy(aux->info, ae, DIMEQ(c));
    aux->urm = NULL;
    if(VIDAQ(c))
    {
        IC(c) = aux;
        SC(c) = aux;
    }
    else
    {
        SC(c)->urm = aux;
        SC(c) = aux;
    }
    return 1;
}

//extragere din coada
int ExtrQ(void* c, void* ae)
{
    if(VIDAQ(c))
        return 0;
    ACel aux = IC(c);
    IC(c) = aux->urm;
    memcpy(ae, aux->info, DIMEQ(c));
    free(aux);
    if(IC(c) == NULL) 
        SC(c) = NULL;
    return 1;
}

//resetare coada
void ResetQ(void *c, TFQ f)
{
    //distrugere lista IC(c);
    ACel aux, p;
    aux = IC(c);
    while(aux)
    {
        p = aux;
        aux = aux->urm;
        f(p->info);
        free(p);
    }
    IC(c) = NULL;
    SC(c) = NULL;
}

//distrugere coada
void DistrQ(void **c, TFQ f)
{
    if(*c == NULL)
        return;
    ResetQ(*c, f);
    free(*c);
    *c = NULL;
}

//concatenare cozi
void ConcatQ(void *d, void *s)
{
    if(VIDAQ(s))
        return;
    if(VIDAQ(d))
    {
        IC(d) = IC(s);
        SC(d) = SC(s);
        s = NULL;
        return;
    }
    SC(d)->urm = IC(s);
    SC(d) = SC(s);
    IC(s) = NULL;
    SC(s) = NULL;
}

//afisare elemente din coada
void AfiQ(void *c, TFQF afiel, FILE *g)
{
    if(c == NULL)
    {
        fprintf(g, "[].\n");
        return;
    }
    ACel p;
    p = IC(c);
    

    fprintf(g, "[");
    while(p != NULL)
    {
        afiel(p->info, g);
        if(p->urm)
			fprintf(g, ", ");
        p = p->urm;
    }
	fprintf(g, "].\n");
}

//prelucrare elemente din coada in functie de info
int PrelQ(void *c, char *nume, void *info, TFElemQ prel_elem, TFElemQ f)
{
	if(!c)
		return 0;
    if(VIDAQ(c))
        return 0;

    ACel p;
    p = IC(c);
	
	for(; p != NULL; p = p->urm)
	{
		if(f(p->info, (void *)nume) == 0) //am gasit elementul dupa nume
		{
			return prel_elem(p->info, info); //prelucrare element cu info
		}
	}
	return 0;
}

//gasire element in coada prin parcurgerea listei
void *FindQ(void *c, char *nume, TFElemQ f)
{
    if(!c)
		return 0;
    if(VIDAQ(c))
        return 0;
    ACel p, ant, *aux;
    p = IC(c);
    void *info;
    //cazul eliminarii primului element
	if(p != NULL && f(p->info, (void *)nume) == 0)
	{
		IC(c) = p->urm;
		aux = &p;

		info = (*aux)->info;
		return info;
	}
	//gasire element de eliminat
	while(p != NULL && f(p->info, (void *)nume) != 0)
	{
		ant = p; //avem nevoie si de elementul anterior
		p = p->urm;
	}
	//nu s-a gasit elementul
	if(p == NULL)
	{
		return NULL;
	}
	//elementul a fost gasit
	ant->urm = p->urm;
	aux = &p;

	info = (*aux)->info;
	return info;
    
}