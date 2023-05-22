/* MUSAT-BURCEA Adrian - 312CB */

#include "TStivaL.h"

//initializare stiva
void *InitS(size_t d)
{
    ASt s;
    s = (ASt)malloc(sizeof(TStiva));
    if(!s) 
        return NULL;
    s->dime = d;
    s->vf = NULL;
    return (void*)s;
}

//introducere element in stiva
int Push(void* s, void* ae)
{
    ACelSt aux;
    aux = (ACelSt)malloc(sizeof(TCelSt));
    if(!aux) 
        return 0;
    aux->info = malloc(DIMES(s));
    if(!aux->info)
    {
        free(aux);
        return 0;
    }
    memcpy(aux->info, ae, DIMES(s));
    aux->urm = VF(s);
    VF(s) = aux;
    return 1;
}

//extragere element din varful stivei
int Pop(void* s, void* ae)
{
    ACelSt aux = VF(s); //varful stivei
    if(aux == NULL) 
        return 0;
    memcpy(ae, aux->info, DIMES(s));
    VF(s) = aux->urm;
    free(aux);
    return 1;
}

//resetare stiva
void ResetS(void* s, TFS f)
{
    ACelSt aux, p;
    //distrugere lista VF(s);
    aux = VF(s);
    while(aux)
    {
        p = aux;
        aux = aux->urm;
        f(p->info); //eliberare element
        free(p);
    }
    VF(s) = NULL;
}

//distrugere stiva
void DistrS(void** s, TFS f)
{
    if(*s == NULL)
        return;
    ResetS(*s, f);
    free(*s);
    *s = NULL;
}

//rastoarna stive
void Rastoarna(void *d, void *s)
{
    if(VIDAS(s))
        return;
    ACelSt p, u;
    p = VF(s);
    while(p)
    {
        u = p->urm;
        p->urm = VF(d);
        VF(d) = p;
        p = u;
    }
    VF(s) = NULL;
}

//suprapunere stive
void Suprapune(void *d, void *s)
{
    ACelSt u, p;
    if(VIDAS(s))
        return;
    if(VIDAS(d))
    {
        VF(d) = VF(s);
        VF(s) = NULL;
    }
    p = VF(s);
    while(p)
    {
        u = p;
        p = p->urm;
    }
    u->urm = VF(d);
    VF(d) = VF(s);
    VF(s) = NULL;
}

//afisare elemente stiva
void AfiS(void *s, TFSF afiel, FILE *g)
{
    if(s == NULL)
    {
        fprintf(g, "[].\n");
        return;
    }
    ACelSt p;
    p = VF(s);
    

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

//prelucrare elemente din stiva in functie de info
int PrelS(void *s, char *nume, void *info, TFElemS prel_elem, TFElemS f)
{
	if(!s)
		return 0;
    if(VIDAS(s))
        return 0;

    ACelSt p;
    p = VF(s);
	
	for(; p != NULL; p = p->urm)
	{
		if(f(p->info, (void *)nume) == 0) //am gasit elementul dupa nume
		{
			return prel_elem(p->info, info); //prelucrare element cu info
		}
	}
	return 0;
}

//gasire element din stiva prin parcurgerea listei
void *FindS(void *s, char *nume, TFElemS f)
{
    if(!s)
		return 0;
    if(VIDAS(s))
        return 0;
    ACelSt p, ant, *aux;
    p = VF(s);
    void *info;
    //cazul eliminarii primului element
	if(p != NULL && f(p->info, (void *)nume) == 0)
	{
		VF(s) = p->urm;
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
