/* MUSAT-BURCEA Adrian - 312CB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tlg.h"
#include "TCoadaL.h"
#include "TStivaL.h"

#define LENGTH 32

typedef struct serial
{
    size_t id;
    char *nume;
    float rating;
    size_t nr_sez; //nr de sezoane
    TCoada *sezoane; //coada de sezoane cu elemente de tip sezon
    size_t durata_totala; //durata totala a serialului
    size_t durata_vizionata; //durata vizionata de catre watch
}*TSerial, TS;

typedef struct sezon
{
    size_t nr_ep; // nr episoade din sezon
    TCoada *episoade; //coada de episoade cu elem de tip durata episod
    size_t durata_sezon; //durata intregului sezon
}*TSezon, TZ;

/*functie comparatie dupa nume si rating (pt inserarea ordonata in
primele 3 categorii)*/
float compSerial(void *a, void *b)
{
    TSerial sa = (TSerial)a;
    TSerial sb = (TSerial)b;
    if(sa->rating - sb->rating == 0)
        return strcmp(sa->nume, sb->nume);
    else
        return sb->rating - sa->rating;
}

//functie comparatie dupa pozitia in top10 (id reprezinta pozitia in acest caz)
int compPoz(void *a, void *b)
{
    TSerial sa = (TSerial)a;
    TSerial sb = (TSerial)b;
    return sa->id - sb->id;
}

//functie comparatie dupa nume (pt identificarea unui serial)
int compNume(void *a, void *b)
{
    TSerial sa = (TSerial)a;
    char *nume = (char *)b;
    return strcmp(sa->nume, nume);
}

/*functie pt incrementarea pozitiei unui elemt in top dupa inserare*/
int IncPoz(void *el) 
{
    TSerial s = (TSerial)el;
    s->id++;
    return 1;
}

/*functie pt decrementarea pozitiei unui elem in top dupa eliminare*/
int DecPoz(void *el) 
{
    TSerial s = (TSerial)el;
    s->id--;
    return 1;
}

//functie afisare serial (nume, rating)
void AfiSerial(void *el, FILE *g)
{
    TSerial s = (TSerial)el;
    fprintf(g, "(%s, %.1f)", s->nume, s->rating);
}

//functie adaugare sezon la serial
int addsezon(void *a, void *b)
{
    TSerial s = (TSerial)a;
    TSezon sez = (TSezon)b;
    s->durata_totala += sez->durata_sezon; 
    return InsQ((void *)s->sezoane, (void *)sez);

}

//functie distrugere sezon
void DistrugeSezon(void *el)
{
    TSezon sez = (TSezon)el;
    DistrQ((void **)&(sez->episoade), free);
    free(sez);
}

//functie distrugere serial
void DistrSerial(void *el)
{
    TSerial s = (TSerial)el;
    free(s->nume);
    DistrQ((void **)&(s->sezoane), DistrugeSezon);
    free(s);
}


int main(int argc, char *argv[])
{
    char *input_file = argv[1];
    char *output_file = argv[2];
    size_t n = 2;
    int i, j, poz_later = 0, nrtop = 0;
    
    char *line = malloc(n * sizeof(char));
    if(line == NULL)
        return 1;
    char *operation;

    //initializare categorii
    TLG cat1 = NULL, cat2 = NULL, cat3 = NULL;
    TLG top10 = NULL;
    TCoada *watch_later = InitQ(sizeof(TS));
    TStiva *watching = InitS(sizeof(TS));
    TStiva *history = InitS(sizeof(TS));
    
    FILE *f = fopen(input_file, "rt");
    FILE *g = fopen(output_file, "wt");

    //citim fiecare linie din fisier
    while(getline(&line, &n, f) != -1)
    {
        if(strcmp(line, "\n") == 0) //verificare linie goala
            continue;
        
        operation = strtok(line, " "); //citire operatie
        
        //operatia add
        if(strcmp(operation, "add") == 0)
        {
            int poz, duratatotala = 0;
            int id = atoi(strtok(NULL, " "));
            char *nume = strtok(NULL, " ");
            float rating = atof(strtok(NULL, " "));
            int nrsez = atoi(strtok(NULL, " "));
            TSerial s = (TSerial)malloc(sizeof(TS));
            if(s == NULL)
            {
                free(line);
                return 1;
            }
            s->id = id;
            s->nume = NULL;
            s->nume = strdup(nume);
            s->rating = rating;
            s->nr_sez = nrsez;
            s->sezoane = InitQ(sizeof(TZ)); //initializare coada sezoane
            for(i = 0; i < nrsez; i++)
            {
                int nrep = atoi(strtok(NULL, " "));
                int duratasezon = 0;
                TSezon sez = (TSezon)malloc(sizeof(TZ));
                if(sez == NULL)
                {
                    free(line);
                    DistrSerial((void *)s);
                    return 1;
                }
                sez->nr_ep = nrep;
                sez->episoade = InitQ(sizeof(int)); //initializare coada ep
                for(j = 0; j < nrep; j++)
                {
                    int durata = atoi(strtok(NULL, " "));
                    duratasezon += durata; //calculare durata sezon
                    InsQ((void *)sez->episoade, (void *)&durata);

                }
                sez->durata_sezon = duratasezon;
                duratatotala += duratasezon; //calculare durata totala
                InsQ((void *)s->sezoane, (void *)sez);
                free(sez);
            }
            s->durata_totala = duratatotala;
            s->durata_vizionata = 0;
            //adaugare serial in categoria corespunzatoare
            if(id == 1)
                poz = InsOrd(&cat1, s, compSerial);
            if(id == 2)
                poz = InsOrd(&cat2, s, compSerial);
            if(id == 3)
                poz = InsOrd(&cat3, s, compSerial);
            if(poz)
                fprintf(g, "Serialul %s a fost adaugat la pozitia %d.\n", nume, poz);
            
            continue;
        }
        
        //operatia add_top
        if(strcmp(operation, "add_top") == 0)
        {
            int rez, duratatotala = 0;
            int poz = atoi(strtok(NULL, " "));
            char *nume = strtok(NULL, " ");
            float rating = atof(strtok(NULL, " "));
            int nrsez = atoi(strtok(NULL, " "));
            TSerial s = (TSerial)malloc(sizeof(TS));
            if(s == NULL)
            {
                free(line);
                return 1;
            }
            s->id = poz; //folosire camp id pt retinerea pozitiei in top
            s->nume = NULL;
            s->nume = strdup(nume);
            s->rating = rating;
            s->nr_sez = nrsez;
            s->sezoane = InitQ(sizeof(TZ));
            for(i = 0; i < nrsez; i++)
            {
                int nrep = atoi(strtok(NULL, " "));
                int duratasezon = 0;
                TSezon sez = (TSezon)malloc(sizeof(TZ));
                if(sez == NULL)
                {
                    free(line);
                    DistrSerial((void *)s);
                    return 1;
                }
                sez->nr_ep = nrep;
                sez->episoade = InitQ(sizeof(int));
                for(j = 0; j < nrep; j++)
                {
                    int durata = atoi(strtok(NULL, " "));
                    duratasezon += durata; //calculare durata sezon
                    InsQ((void *)sez->episoade, (void *)&durata);
                }
                sez->durata_sezon = duratasezon;
                duratatotala += duratasezon; //calculare durata totala
                InsQ((void *)s->sezoane, (void *)sez);
                free(sez);
            }
            s->durata_totala = duratatotala;
            s->durata_vizionata = 0;
            rez = InsPoz(&top10, s, compPoz, IncPoz);
            if(rez)
            {
                if(nrtop == 10) //daca sunt deja 10 seriale in top10 
                    ElimUlt(&top10, DistrSerial); //eliminam pe ultimul
                if(nrtop < 10)
                    nrtop++;
                fprintf(g, "Categoria top10: ");
                Afisare(&top10, AfiSerial, g);
                continue;
            }
            
        }

        //operatia add_sez
        if(strcmp(operation, "add_sez") == 0)
        {
            char *nume = strtok(NULL, " ");
            int duratasezon = 0;
            int nrep = atoi(strtok(NULL, " "));
            TSezon sez = (TSezon)malloc(sizeof(TZ));
            if(sez == NULL)
            {
                free(line);
                return 1;
            }
            sez->nr_ep = nrep;
            sez->episoade = InitQ(sizeof(int));
            for(j = 0; j < nrep; j++)
            {
                int durata = atoi(strtok(NULL, " "));
                duratasezon += durata; //calculare durata sezon
                InsQ((void *)sez->episoade, (void *)&durata);
            }
            sez->durata_sezon = duratasezon;
            int rez;
            //cautare serial in categoria 1
            rez = Prel(&cat1, nume, (void *)sez, addsezon, compNume);
            if(rez)
            {
                fprintf(g, "Serialul %s are un sezon nou.\n", nume);
                free(sez);
                continue;
            }
            //cautare serial in categoria 2
            rez = Prel(&cat2, nume, (void *)sez, addsezon, compNume);
            if(rez)
            {
                fprintf(g, "Serialul %s are un sezon nou.\n", nume);
                free(sez);
                continue;
            }
            //cautare serial in categoria 3
            rez = Prel(&cat3, nume, (void *)sez, addsezon, compNume);
            if(rez)
            {
                fprintf(g, "Serialul %s are un sezon nou.\n", nume);
                free(sez);
                continue;
            }
            //cautare serial in categoria top10
            rez = Prel(&top10, nume, (void *)sez, addsezon, compNume);
            if(rez)
            {
                fprintf(g, "Serialul %s are un sezon nou.\n", nume);
                free(sez);
                continue;
            }
            //catare serial in watch later
            rez = PrelQ((void *)watch_later, nume, (void *)sez, addsezon, compNume);
            if(rez)
            {
                fprintf(g, "Serialul %s are un sezon nou.\n", nume);
                free(sez);
                continue;
            }
            //cautare serial in currently watching
            rez = PrelS((void *)watching, nume, (void *)sez, addsezon, compNume);
            if(rez)
            {
                fprintf(g, "Serialul %s are un sezon nou.\n", nume);
                free(sez);
                continue;
            }
        }

        //operatia later
        if(strcmp(operation, "later") == 0)
        {
            char *nume = strtok(NULL, " ");
            nume[strlen(nume)-1] = '\0';
            TSerial s;
            int rez;
            //cautare serial in prima categorie
            s = (TSerial)Extrage(&cat1, nume, compNume);
            if(s != NULL)
            {
                rez = InsQ((void *)watch_later, (void *)s);
                if(rez)
                {
                    free(s);
                    poz_later++;
                    fprintf(g, "Serialul %s se afla in coada de asteptare pe pozitia %d.\n", nume, poz_later);
                    continue;
                }
            }
            //cautare serial in categoria 2
            s = (TSerial)Extrage(&cat2, nume, compNume);
            if(s != NULL)
            {
                rez = InsQ((void *)watch_later, (void *)s);
                if(rez)
                {
                    free(s);
                    poz_later++;
                    fprintf(g, "Serialul %s se afla in coada de asteptare pe pozitia %d.\n", nume, poz_later);
                    continue;
                }
            }
            //cautare serial in categoria 3
            s = (TSerial)Extrage(&cat3, nume, compNume);
            if(s != NULL)
            {
                rez = InsQ((void *)watch_later, (void *)s);
                if(rez)
                {
                    free(s);
                    poz_later++;
                    fprintf(g, "Serialul %s se afla in coada de asteptare pe pozitia %d.\n", nume, poz_later);
                    continue;
                }
            }
            //cautare serial in categoria top10
            s = (TSerial)ExtragePoz(&top10, nume, compNume, DecPoz);
            if(s != NULL)
            {
                nrtop--; //actualizare nr elem din top10
                rez = InsQ((void *)watch_later, (void *)s);
                if(rez)
                {
                    free(s);
                    poz_later++;
                    fprintf(g, "Serialul %s se afla in coada de asteptare pe pozitia %d.\n", nume, poz_later);
                    continue;
                }
            }
        }

        //operatia watch
        if(strcmp(operation, "watch") == 0)
        {
            char *nume = strtok(NULL, " ");
            int durata = atoi(strtok(NULL, " "));
            TSerial s;
            int rez;
            //cautare serial in prima categorie
            s = (TSerial)Extrage(&cat1, nume, compNume);
            if(s != NULL)
            {
                s->durata_vizionata += durata;
                if(s->durata_vizionata < s->durata_totala)
                {
                    rez = Push((void *)watching, (void *)s);
                }
                else
                {
                    rez = Push((void *)history, (void *)s);
                    if(rez)
                    {
                        fprintf(g, "Serialul %s a fost vizionat integral.\n", nume);
                    }
                }
                free(s);
                continue;
            }
            //cautare serial in categoria 2
            s = (TSerial)Extrage(&cat2, nume, compNume);
            if(s != NULL)
            {
                s->durata_vizionata += durata;
                if(s->durata_vizionata < s->durata_totala)
                {
                    rez = Push((void *)watching, (void *)s);
                }
                else
                {
                    rez = Push((void *)history, (void *)s);
                    if(rez)
                    {
                        fprintf(g, "Serialul %s a fost vizionat integral.\n", nume);
                    }
                }
                free(s);
                continue;
            }
            //cautare serial in categoria 3
            s = (TSerial)Extrage(&cat3, nume, compNume);
            if(s != NULL)
            {
                s->durata_vizionata += durata; 
                if(s->durata_vizionata < s->durata_totala)
                {
                    rez = Push((void *)watching, (void *)s);
                }
                else
                {
                    rez = Push((void *)history, (void *)s);
                    if(rez)
                    {
                        fprintf(g, "Serialul %s a fost vizionat integral.\n", nume);
                    }
                }
                free(s);
                continue;
            }
            //cautare serial in categoria top10
            s = (TSerial)ExtragePoz(&top10, nume, compNume, DecPoz);
            if(s != NULL)
            {
                nrtop--; //actualizare nr elem din top10
                s->durata_vizionata += durata;
                if(s->durata_vizionata < s->durata_totala)
                {
                    rez = Push((void *)watching, (void *)s);
                }
                else
                {
                    rez = Push((void *)history, (void *)s);
                    if(rez)
                    {
                        fprintf(g, "Serialul %s a fost vizionat integral.\n", nume);
                    }
                }
                free(s);
                continue;
            }
            //cautare serial in watch later
            s = (TSerial)FindQ((void *)watch_later, nume, compNume);
            if(s != NULL)
            {
                poz_later--; //actualizare nr elem din watch later
                s->durata_vizionata += durata;
                if(s->durata_vizionata < s->durata_totala)
                {
                    rez = Push((void *)watching, (void *)s);
                }
                else
                {
                    rez = Push((void *)history, (void *)s);
                    if(rez)
                    {
                        fprintf(g, "Serialul %s a fost vizionat integral.\n", nume);
                    }
                }
                free(s);
                continue;
            }
            //cautare serial in currently watching
            s = (TSerial)FindS((void *)watching, nume, compNume);
            if(s != NULL)
            {
                s->durata_vizionata += durata;
                if(s->durata_vizionata < s->durata_totala)
                {
                    rez = Push((void *)watching, (void *)s);
                }
                else
                {
                    rez = Push((void *)history, (void *)s);
                    if(rez)
                    {
                        fprintf(g, "Serialul %s a fost vizionat integral.\n", nume);
                    }
                }
                free(s);
                continue;
            }

        }

        //operatia show
        if(strcmp(operation, "show") == 0)
        {
            char *x = strtok(NULL, " ");
            if(strcmp(x, "1\n") == 0)
            {
                fprintf(g, "Categoria 1: ");
                Afisare(&cat1, AfiSerial, g);
                continue;
            }
            if(strcmp(x, "2\n") == 0)
            {
                fprintf(g, "Categoria 2: ");
                Afisare(&cat2, AfiSerial, g);
                continue;
            }
            if(strcmp(x, "3\n") == 0)
            {
                fprintf(g, "Categoria 3: ");
                Afisare(&cat3, AfiSerial, g);
                continue;
            }
            if(strcmp(x, "top10\n") == 0)
            {
                fprintf(g, "Categoria top10: ");
                Afisare(&top10, AfiSerial, g);
                continue;
            }
            if(strcmp(x, "later\n") == 0)
            {
                fprintf(g, "Categoria later: ");
                AfiQ((void *)watch_later, AfiSerial, g);
                continue;
            }
            if(strcmp(x, "watching\n") == 0)
            {
                fprintf(g, "Categoria watching: ");
                AfiS((void *)watching, AfiSerial, g);
                continue;
            }
            if(strcmp(x, "history\n") == 0)
            {
                fprintf(g, "Categoria history: ");
                AfiS((void *)history, AfiSerial, g);
                continue;
            }
        }



        
    }

    //eliberare memorie
    free(line);
    Distruge(&cat1, DistrSerial);
    Distruge(&cat2, DistrSerial);
    Distruge(&cat3, DistrSerial);
    Distruge(&top10, DistrSerial);
    DistrQ((void **)&watch_later, DistrSerial);
    DistrS((void **)&watching, DistrSerial);
    DistrS((void **)&history, DistrSerial);

    fclose(f);
    fclose(g);
    return 0;
}
