/* MUSAT-BURCEA Adrian - 312CB */

#include "tlg.h"

int InsLG(TLG* aL, void* ae) /*inserare la inceputul listei*/
{
	TLG aux = malloc(sizeof(TCelulaG));
	if(!aux)
	    return 0;

	aux->info = ae;
	aux->urm = *aL;
	*aL = aux;

	return 1;
}

void Distruge(TLG* aL, TF free_elem) /* distruge lista */
{
	while(*aL != NULL)
    {
        TLG aux = *aL;     /* adresa celulei eliminate */
        if (!aux)
            return;

        free_elem(aux->info);  /* elib.spatiul ocupat de element*/
        *aL = aux->urm;    /* deconecteaza celula din lista */
        free(aux);   /* elibereaza spatiul ocupat de celula */
    }
}

size_t LungimeLG(TLG* a)      /* numarul de elemente din lista */
{
	size_t lg = 0;
	TLG p = *a;

     /* parcurge lista, numarand celulele */
	for (; p != NULL; p = p->urm)
        lg++;

	return lg;
}

/*functie afisare elemente in fisier*/
void Afisare(TLG* aL, TFF afiEL, FILE *g)
{
	if(!*aL) 
	{
        fprintf(g, "[].\n");
        return;
    }

	fprintf(g, "[");
	for(; *aL; aL = &(*aL)->urm) 
	{
		afiEL((*aL)->info, g);
		if((*aL)->urm)
			fprintf(g, ", ");
    }
	fprintf(g, "].\n");
}

/*functie pt eliminarea ultimului element din lista*/
void ElimUlt(TLG *aL, TF free_elem)
{
	if(*aL == NULL)
		return;
	TLG ant = *aL, p = *aL;

	//gasire element dinaintea ultimului
	while(p->urm != NULL)
	{
		ant = p;
		p = p->urm;
	}
	free_elem(p->info);
	free(p);
	ant->urm = NULL;
	
}

/*functie gasire si prelucrare element*/
int Prel(TLG *aL, char *nume, void *info, TFElem2 prel_elem, TFCmp f)
{
	if(!*aL)
		return 0;
	
	for(; *aL; aL = &(*aL)->urm)
	{
		if(f((*aL)->info, (void *)nume) == 0) //am gasit elementul dupa nume
		{
			return prel_elem((*aL)->info, info); //prelucrare element cu info
		}
	}
	return 0;
}

/*functie de inserare ordonata*/
int InsOrd(TLG *aL, void *ae, TFCmpF f)
{
	TLG p;
	TLG aux = malloc(sizeof(TCelulaG));
	if(!aux)
	    return 0;

	aux->info = ae;
	aux->urm = NULL;
	int poz = 1;
	if(*aL == NULL) //inserarea cand lista e vida
	{
		aux->urm = *aL;
		*aL = aux;
		return 1;
	}
	if(f(aux->info, (*aL)->info) <= 0) //inserarea inainte de primul element
	{
		aux->urm = *aL;
		*aL = aux;
		return 1;
	}
	p = *aL;
	//gasire pozitie unde trebuie inserat elementul
	while(p->urm != NULL && f(aux->info, p->urm->info) > 0)
	{
		p = p->urm;
		poz++;
	}
	aux->urm = p->urm;
	p->urm = aux;
	return poz + 1;
}

/*functie de inserare la o anumita pozitie 
cu actualizarea pozitiei elementelor de dupa cel inserat*/
int InsPoz(TLG *aL, void *ae, TFCmp f, TFElem m)
{
	TLG p;
	TLG aux = malloc(sizeof(TCelulaG));
	if(!aux)
	    return 0;

	aux->info = ae;
	aux->urm = NULL;
	int poz = 1;
	if(*aL == NULL) //inserarea cand lista e vida
	{
		aux->urm = *aL;
		*aL = aux;
		return 1;
	}
	if(f(aux->info, (*aL)->info) <= 0) //inserarea inainte de primul element
	{
		aux->urm = *aL;
		*aL = aux;
		p = aux->urm;
		while(p != NULL) //actualizzare pozitii pt elementele urmatoare
		{
			m(p->info);
			p = p->urm;
		}
		return 1;
	}
	p = *aL;
	//gasire pozitie unde trebuie inserat elementul
	while(p->urm != NULL && f(aux->info, p->urm->info) > 0)
	{
		p = p->urm;
		poz++;
	}
	aux->urm = p->urm;
	p->urm = aux;
	p = p->urm->urm;
	while(p != NULL) //actualizzare pozitii pt elementele urmatoare
	{
		m(p->info);
		p = p->urm;
	}
	return poz + 1;

}

/*functie de extragere element cu returnarea lui*/
void *Extrage(TLG* aL, char *nume, TFCmp f)
{
	TLG p = *aL, ant, *aux;
	void *info;
	//cazul eliminarii primului element
	if(p != NULL && f(p->info, (void *)nume) == 0)
	{
		*aL = p->urm;
		aux = &p;

		info = (*aux)->info;
		free(*aux);
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
	free(*aux);
	return info;
}

/*functie de extragere element cu returnarea lui si 
actualizare pozitie elemente urmatoare*/
void *ExtragePoz(TLG* aL, char *nume, TFCmp f, TFElem m)
{
	TLG p = *aL, ant, *aux, pa;
	void *info;
	//cazul eliminarii primului element
	if(p != NULL && f(p->info, (void *)nume) == 0)
	{
		*aL = p->urm;
		aux = &p;

		pa = p->urm;
		while(pa != NULL) //actualizzare pozitii pt elementele urmatoare
		{
			m(pa->info);
			pa = pa->urm;
		}

		info = (*aux)->info;
		free(*aux);
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

	pa = p->urm;
	while(pa != NULL) //actualizzare pozitii pt elementele urmatoare
	{
		m(pa->info);
		pa = pa->urm;
	}

	info = (*aux)->info;
	free(*aux);
	return info;
}

