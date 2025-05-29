//keszitette: Nemeth Akos
//#include"debugmalloc.h"
#include<stdlib.h>  //listakhoz es tombokhoz kell
#include<stdio.h>
#include<string.h>  //string masolasahoz es osszehasonlitashoz kell
#include<time.h>    //A random mukodesehez kell

typedef struct animelista{  //Elso lista strukturaja
    char aname[30];     //anime neve
    int rating;     //anime ertekelese
    int epcount;    //anime reszeinek szama
    struct animelista *next;       //lista kovetkezo eleme
}animelista;

typedef struct studiolista{     //Masodik lista strukturaja
    char studioname[30];        //studio neve
    char anev[30];              //Ez az egyezesi pont a kent listaban (anime neve)
    struct studiolista *next;
}studiolista;

animelista *pop_front(animelista *head) //A listat az elejerol torli
{
    if(head!=NULL)      //ha fej nem NULL akkor elvegzi a muveletet
        {
            animelista *p=head;     //Ha lista nem a NULL-ba mutat, akkor letrehoz p mutatot, a headet egyel alrebb tolja es a p-t felszabaditja
            head=head->next;
            free(p);
        }
        return head;
}

void listafelszabadito(animelista *head)    //Elso lista torlo
{
    while(head!=NULL)       //ameddig nem ures a lista addig megy
        {head=pop_front(head);}     //meghivja a torlot
}

studiolista *pop_front2(studiolista *head)  //A lista elso elemet torli
{
    if(head!=NULL)      //NULL eseten nem vegzi el a muveletet
        {
            studiolista *p=head;    //Ha lista nem a NULL-ba mutat, akkor letrehoz p mutatót, a headet eggyel alrebb tolja es a p-t felszabaditja
            head=head->next;
            free(p);
        }
        return head;
}

void listafelszabadito2(studiolista *head)  //Masodik lista torlo
{
    while(head!=NULL)       //ameddig ures nem lesz a lista addig megy
        {head=pop_front2(head);}    //torles meghivasa
}

void feltolt(int *p, int size)  //A letrehozott tombot es azt feltolti 0-kal
{
    for(int i=0;i<size;i++)
        {p[i]=0;}
}

int ellenorzo(int hossz, int *p)    //A tombot atnezi es 0-at keres, mert az azt jelenti hogy a listaban ott azt az elemet meg nem vizsgaltuk
{                                   //1= az elem mar vizsgalt
    for(int i=0;i<hossz;i++)
        {
            if(p[i]==0)
                {return 0;}
        }
    return 1;       //ha nem talal akkor vege van
}

void ajanlas(int ossz, animelista *head)
{
    srand(time(0));                 //csinal egy random szamot es annyiszor lep elore a listaban amennyi a random szam, majd az eredmenyt kiirja
    int randnum=(rand()%(ossz-1));
    for(int i=0; i<randnum; i++)
        {
            head=head->next;        //addig lepkedunk amig oda nem erunk
        }
    printf("\nRandom anime neve: %s\nErtekelese: %d/10\nReszek szama: %d\n",head->aname,head->rating,head->epcount);        //kiiras
}

void darab(int ossz, studiolista *head2)
{
    int *p=(int *)malloc(sizeof(int)*ossz);     //Ez a tomb egy a lista "masa", ahol 1-es van azt az elemet mar vizsgaltuk
    feltolt(p,ossz);        // feltoltjuk 0-val, mert meg nem vizsgaltunk semmit
    studiolista *all=head2;     //Ezzel lepkedunk a listaban
    char ell[30];               //Ezzel a stringgel kereshetjuk az azonos nevu studiokat (egy fajta ellenorzo elem)
    int osszanime=0,veg=0,i=0;  //Osszanime=animek darabszama, Veg==1 a ciklus befejezese ezert kezdetben 0
    while(veg!=1)
        {
        while(p[i]!=0)  //Ezzel keresunk a tombben egy olyan elemet amely meg nem volt vizsgalva
            {
                all=all->next;
                i++;
            }
        if(p[i]==0)     //A valasztott elemet kimasoljuk es reseteljuk a valtozokat
            {
                strcpy(ell,all->studioname);
                i=0;
                all=head2;
            }
        while(all!=NULL)        //A lista bejerasa es az eredmenyek szamolasa itt tortenik
            {
                if(strcmp(ell,all->studioname)==0)  //Ha talal egy uj elemet akkor +1 es a tomben a megfelelo helyre egy 1-et tesz
                    {
                        osszanime++;
                        p[i]=1;
                    }
                all=all->next;
                i++;
            }
        printf("\nStudio neve: %s\nAnimek darabszama: %d db\n",ell,osszanime);  //Eredmeny kiirasa
        osszanime=0;            //Resetek
        all=head2;
        i=0;
        if(ellenorzo(ossz,p)==1)    //Vegeztunk-e mar? Lista atnezese 0-ert
            {veg=1;}        //vege
        }
    free(p);
}

void atlag(int ossz, animelista *head, studiolista *head2)
{
    int *p=(int *)malloc(sizeof(int)*ossz);     //Ez a tomb egy a lista "masa". Ahol 1-es van azt az elemet mar vizsgaltuk
    feltolt(p,ossz);        //Feltoltjuk nullaval
    studiolista *all=head2;     //Ezzel jarjuk be az egyik listat
    animelista *allanime=head;      //Ezzel jarjuk be a masik listat
    char ell[30],ell2[30];          //Ezzekkel keressuk meg egy studio animeinek az ertekeleset
    int osszanimertekeles=0,veg=0,i=0,ertekeles,osszanime=0;    //osszanimertekeles az eredmeny/atlag, veg a ciklus befejezesehez kell, ertekeles az ertekeles masolasahoz kell, osszanime az animek darabszama az atlaghoz
    while(veg!=1)   
        {
        while(p[i]!=0)      //0-as (nem vizsgalt elem) keresese itt tortenik
            {
                all=all->next;
                i++;
            }
        if(p[i]==0)
            {
                strcpy(ell,all->studioname);    //studio kivalasztasa es masolasa itt tortenik, reset
                i=0;
                all=head2;
            }
        while(all!=NULL)    //a studio animei megkeresese itt tortenik
            {
                if(strcmp(ell,all->studioname)==0)  //ha egyezest talal akkor belep ide
                    {
                        osszanime++;    //darabszam +1
                        p[i]=1;         //tomben a helyere 1 rakunk
                        strcpy(ell2,all->anev);     //lemasoljuk az anime cimet
                        while(allanime!=NULL)       //megkeressuk az animet a masik listaban
                        {
                            if(strcmp(ell2,allanime->aname)==0) //a megtalalt anime ertekeleset kimasoljuk
                                {
                                    ertekeles=allanime->rating;
                                    osszanimertekeles+=ertekeles;
                                }
                            allanime=allanime->next;
                        }
                        allanime=head;      //lista reset
                    }
                all=all->next;
                i++;
            }
        double atlag;
        atlag=(double)osszanimertekeles/osszanime;  //Eredmeny szamitas
        printf("\nStudio neve: %s\nAnime ertekeleseinek atlaga: %.2lf/10\n",ell,atlag);     //eredmeny kiirasa
        osszanimertekeles=0;    //Resetek
        osszanime=0;
        all=head2;
        allanime=head;
        i=0;
        if(ellenorzo(ossz,p)==1)    //Vege van?
            {veg=1;}        //vege
        }
    free(p);
}

void darab2(int ossz, animelista *head, studiolista *head2)
{
    int *p=(int *)malloc(sizeof(int)*ossz);     //Ez a tomb egy a lista "masa". Ahol 1-es van azt az elemet mar vizsgaltuk
    feltolt(p,ossz);        //0-kal feltoljuk a tombot
    studiolista *all=head2;     //Listak bejarashoz kellenek
    animelista *allanime=head;
    char ell[30],ell2[30];      //Listaban kereseshez kellenek, ezekre masoljuk a stringeket
    int osszanimeresz=0,veg=0,i=0,eps;  //osszanimeresz egy studional, eps=reszek
    while(veg!=1)
        {
        while(p[i]!=0)  //0 keresese tortenik itt
            {
                all=all->next;
                i++;
            }
        if(p[i]==0)
            {
                strcpy(ell,all->studioname);    //valasztott studio kimasolasa, reset
                i=0;
                all=head2;
            }
        while(all!=NULL)    //lista bejarasa, szukseges adatadok gyujtese itt tortenik
            {
                if(strcmp(ell,all->studioname)==0) //ha egyezest talal akkor tomben egy 1-t elhelyez
                    {
                        p[i]=1;
                        strcpy(ell2,all->anev);     //anime cim masolasa
                        while(allanime!=NULL)   //Megkeressuk a masiklistaban is az animet
                        {
                            if(strcmp(ell2,allanime->aname)==0) //megtalalt anime reszeinek szamat kimasoljuk es hozzaadjuk a vegso eredmenyhez
                                {
                                    eps=allanime->epcount;
                                    osszanimeresz+=eps;
                                }
                            allanime=allanime->next;        //lep
                        }
                        allanime=head;
                    }
                all=all->next;
                i++;
            }
        printf("\nStudio neve: %s\nAnime reszek darabszama: %d db\n",ell,osszanimeresz);    //eredmeny kiirasa
        osszanimeresz=0;        //Resetek
        all=head2;
        allanime=head;
        i=0;
        if(ellenorzo(ossz,p)==1)    //Is this the END?
            {veg=1;}
        }
    free(p);
}

void kiiras(int ossz, animelista *head, studiolista *head2)
{
    int veg=0;  //a ketto ciklus befejezesehez kellenek
    int veg2=1;
    studiolista *letra=head2;   //lista bejarasahoz kell
    while(veg!=1)
        {
            if(head==NULL)      //ha NULL-ra mutat a head akkor vege van
                {
                    veg=1;
                    veg2=0;
                }
            while(veg2!=0)  
                {
                    veg2=strcmp(head->aname,head2->anev); //megkersei a hasonlokat es kiirja azokat
                    if(veg2==0)
                        {
                            printf("\nAnime neve: %s\nStudio neve: %s\nErtekelese: %d/10\nReszekszama: %d db\n",head->aname, head2->studioname, head->rating, head->epcount);   //Results
                        }
                    head2=head2->next;
                }
            veg2=1; //resets
            head2=letra;
            if(head->next!=NULL)    //END?
                {head=head->next;}  //kovetkezo elemre lepes
            else
                {veg=1;}    //The End
        }
}

void dashboard(int vala, animelista *head, studiolista *head2, int hossz)
{
switch (vala)       //ide kerul a kivalasztott muvelet sorszama es kuld el a megfelelo muvelethez
    {
    case 1: ajanlas(hossz,head); break; //random anime ajanlas
    case 2: darab(hossz,head2); break;  //studio animeinek darabszama
    case 3: darab2(hossz,head,head2); break;    //studiok altal gyartott reszek darabszama
    case 4: atlag(hossz,head,head2); break;     //studiok atag ertekelese
    case 5: kiiras(hossz,head,head2); break;    //ossze fesult listak kiirasa
    }
}

int kerir()     
{
    char ans;  //valasz valtozo      //itt lehet megtudni, hogy a program mire kepes
    printf("\nValassz a kovetkezo muveletek kozul egyet:\n1: Random anime ajanlas\n2: Studiok altal gyartott animek szama\n3: Studiok altal gyartott osszes reszek szama\n4: Studiok ertekelese\n5: Osszes kiirasa\n6: Kilepes\n");
    scanf("%c",&ans);   //'\n' kiveszi a scanfbol
    if(ans=='\n' || ans==' ')
        {scanf("%c",&ans);} //itt kell megadni a valasztott muvelet sorszamat
    if(ans>'0' && ans<'7')      //valasz konvertalasa
        {return ans-'0';}
    else
        {return kerir();}   //ha nem jo az eredmeny ujra megkerdezi
}

int bevegzo()
{
    char ans;   //valasz valtozo     //Itt lehet donteni, hogy akarunk e meg tobb muveletet elvegezni
    printf("\nSzeretnel meg tobbet megnezni?\n1: Igen\n2: nem\n");
    scanf("%c",&ans);   //'\n' kiveszi a scanfbol
    if(ans=='\n' || ans==' ')
        {scanf("%c",&ans);}  //eine antwort geben   
    if(ans=='2' || ans=='1')    //valasz konvertalasa
        {
            if(ans=='2')
                {ans='6';}
            return ans-'0';
        }
    else
        {return bevegzo();}     //ha nem jo az eredmeny akkor ujra megkerdezi
}

animelista *ujelema(animelista *head, char *strg, int sz1, int sz2 )
{
    animelista *p=(animelista*)malloc(sizeof(animelista));          //(elso)lista elem letre hozasa itt tortenik
    strcpy(p->aname, strg);     //anime nevet itt masoljuk at
    p->rating=sz1;  //ertekeles
    p->epcount=sz2; //reszek szama
    p->next=head;   //lista eleme a headben tarolt adatra mutat
    head=p;     //head az uj elemre mutat
    return head;
}

studiolista *ujelems(studiolista *head,char *strg, char *strg2)
{
    studiolista *p=(studiolista*)malloc(sizeof(studiolista));       //(masodik)lista elem letre hozasa itt tortenik
    strcpy(p->studioname, strg);    //studio neve bemasolasa
    strcpy(p->anev, strg2); //anime neve bemasolasa
    p->next=head;   //lista nextje a headben tarolt adatra mutat
    head=p;     //head az ujelemre mutat
    return head;
}

int main()
{
    FILE *animef,*studiof;
    char nev1[20],nev2[20];             //fajlok neveinek string
    printf("\nFajlok neve: ");        //Itt lehet megadni a fajlok nevet. Eloszor az animeset utana studiosat
    scanf("%s %s", nev1, nev2);

    animef=fopen(nev1,"rt");        //megnyitjuk a fajlt
        if(animef==NULL)        //hiba kiszurese
            {
                printf("\nHiba: Nem megfelelo a fajlnev\n");   
                return -1;
            }
    
    studiof=fopen(nev2,"rt");      //megnitjuk a fajlt
        if(studiof==NULL)       //hiba kiszurese
            {
                printf("\nHiba: Nem megfelelo a fajlnev\n");
                return -1;
            }

    int max=0;  //listak hossza

    char olvasc1[30];   //anime neve
    int olvasi1;    //ertekeles
    int olvasi2;    //reszek szama
    animelista *list=NULL;  // lista "letrehozasa"
    while(fscanf(animef, "%s %d %d\n", olvasc1, &olvasi1, &olvasi2)==3)     //beolvasas
    {
        list=ujelema(list,olvasc1,olvasi1,olvasi2);
        max++;
    }

    int max2=0;     //lista hossza
    char solvasc1[30];  //studio neve
    char solvasc2[30];  //anime neve
    studiolista *lista=NULL;    //lista kezdese
    while(fscanf(studiof, "%s %s\n", solvasc1, solvasc2)==2)        //beolvasas
    {
        lista=ujelems(lista, solvasc1, solvasc2);
        max2++;
    }
    
    
    if(list==NULL || lista==NULL)   //hiba: lista nem letezik
        {
            printf("\nHiba: Az egyik lista ures\n");
            listafelszabadito(list);        //listak felszabaditasa
            listafelszabadito2(lista);
            fclose(animef);    //fajlok bezarasa
            fclose(studiof);
            return -1;
        }

    if(max!=max2)       //Listak hossz vizsgalata
        {
            printf("\nHiba: A ket lista hossza nem egyezik\n");
            listafelszabadito(list);        //listak felszabaditasa
            listafelszabadito2(lista);
            fclose(animef);    //fajlok bezarasa
            fclose(studiof);
            return -1;
        }

    int val=0;      //valaszod
    while(val!=6)   //Ebben a ciklusban fut a "Fo" program
    {
        val=kerir();    //valaszod megadasa
        if(val>=1 && val<=5)
            {
                dashboard(val,list,lista,max);  //valaszod tovabbitasa
                val=bevegzo();  //akarsz tobbet?
            }
    }
    listafelszabadito(list);        //list felszabaditasa
    listafelszabadito2(lista);      //lista felszabaditasa
    int status=fclose(animef);    //fajl bezarasa
    if(status!=0)       //hiba szures
        {
            printf("\nHiba, a fajl1 nem zart le rendesen\n");
            return -1;
        }
    status=fclose(studiof);     //fajl bezarasa
    if(status!=0)       //hiba szures
    {
        printf("\nHiba, a fajl2 nem zart le rendesen\n");
        return -1;
    }
    printf("\nHave a nice day!\n");     //  :)
    return 0;
}