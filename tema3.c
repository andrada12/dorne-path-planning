#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct graf{
	int n;
	int **a; 
}graf;


typedef struct client{
	int nr;
	int tip;
}client;

graf *init(int n){
	graf *g=calloc(1,sizeof(graf));
	g->n=n;
	int i;
	g->a = calloc(n, sizeof(int*));
	for(i=0;i<n;i++)
		g->a[i]=calloc(n,sizeof(int));
	return g;
}

void add_edge(graf *g, int i, int j, int c){
	
	g->a[i][j]=c;
}

void del_edge(graf *g, int i, int j){
	g->a[i][j]=0;
}

void afisare(graf *g){
	int i, j;
	for(i=0;i<g->n;i++){
		for(j=0;j<g->n;j++)
			printf("%d ", g->a[i][j]);
		printf("\n");
	}
}

graf *hop(int n,graf *g){
	int i,j;
	graf *h=init(n);
	h->n=n;
	for(i=0;i<n;i++)
    	for(j=0;j<n;j++)
    	{
    		if(i==j || g->a[i][j]==10000)
    			h->a[i][j]=-1;
    		else
    			h->a[i][j]=i;
    	}
	return h;
}

void floyd(graf *g, graf *next_hop){
	int i,j,k;
	
	for(k=0;k<g->n;k++)
		for(i=0;i<g->n;i++)
			for(j=0;j<g->n;j++)
				if(g->a[i][j]>g->a[i][k]+g->a[k][j]){
					g->a[i][j]=g->a[i][k]+g->a[k][j];
					
					next_hop->a[i][j]=next_hop->a[k][j];
					
				}

}

graf *construire_mat_costuri(graf *mat_adiacenta){
	int i,j,n=mat_adiacenta->n;
	graf *mat_costuri=init(n);
	for(i=0;i<n;i++){
		for(j=0;j<n;j++)
			if(mat_adiacenta->a[i][j]==0)
				mat_costuri->a[i][j]=10000;
			else
				mat_costuri->a[i][j]=mat_adiacenta->a[i][j];
		mat_costuri->a[i][i]=0;
	}
	return mat_costuri;
}

void printare_drum_dus_intors(graf *mat_costuri, graf *next_hop, int aprovizionare, int destinatie,FILE *f2){
	int v[100],k=0,aux,w[100],i;
	//se creeaza drumul de la aprovizionare la destinatie
	i=destinatie;
	v[k]=destinatie; k++;
	while(next_hop->a[aprovizionare][i]!=-1){
		v[k]=next_hop->a[aprovizionare][i];
		k++;
		i=next_hop->a[aprovizionare][i];
	}
	//se printeaza drumul 
	for(i=k-2;i>=0;i--)
		fprintf(f2," %d", v[i]);
	//se creeaza drumul de la destinatie inapoi la aprovizionare
	aux=aprovizionare;aprovizionare=destinatie;destinatie=aux;
	k=0;
	i=destinatie;
	w[k]=destinatie; k++;
	while(next_hop->a[aprovizionare][i]!=-1){
		w[k]=next_hop->a[aprovizionare][i];
		k++;
		i=next_hop->a[aprovizionare][i];
	}
	//se printeaa drumul creat
	for(i=k-2;i>=0;i--)
		fprintf(f2," %d",w[i] );
}


//aceasta functie e folosita si pentru drumul de la aprovizionare la ferma si de la ferma la aprovizionare
//in functie de ordinea parametrilor primiti (am un vector de k-1 elemente, ultimul element nu se printeaza,
//pentru a nu se repeta atunci cand functie e apelata pentru drumul ferma->noua sursa de aprovizionare)
void printare_drum_dus(graf *mat_costuri, graf *next_hop, int aprovizionare, int destinatie,FILE *f2){
	int v[100],k=0,i;
	//se creeaza drumul de la aprovizionare la destinatie
	i=destinatie;
	v[k]=destinatie; k++;
	while(next_hop->a[aprovizionare][i]!=-1){
		v[k]=next_hop->a[aprovizionare][i];
		k++;
		i=next_hop->a[aprovizionare][i];
	}

	for(i=k-2;i>=0;i--)
		fprintf(f2," %d", v[i]);

}

//functia afla cea mai apropiata ferma de un client,luand in calcul drumul de la clientul curent, pana la ferma
//cea mai apropiata si pana la urmatorul client

int minim(int aprovizionare[], int k, graf *mat_costuri, int destinatie1, int destinatie2){
	int i,indice=0;
	int min=mat_costuri->a[destinatie1][aprovizionare[0]]+mat_costuri->a[aprovizionare[0]][destinatie2];
	for(i=1;i<k;i++){
		
		if(min>mat_costuri->a[destinatie1][aprovizionare[i]]+mat_costuri->a[aprovizionare[i]][destinatie2]){
			min=mat_costuri->a[destinatie1][aprovizionare[i]]+mat_costuri->a[aprovizionare[i]][destinatie2];
			indice=i;
		}
	}

	return indice;
	
}

//functia afla drumul pana la cea mai apropiata ferma in care se intoarce drona, dupa ce pleaca de la ultimul client
int minim2(int aprovizionare[], int k, graf *mat_costuri, int destinatie){
	int i,indice=0;
	int min=mat_costuri->a[destinatie][aprovizionare[0]];
	for(i=1;i<k;i++){
		
		if(min>mat_costuri->a[destinatie][aprovizionare[i]]){
			min=mat_costuri->a[destinatie][aprovizionare[i]];
			indice=i;
		}
	}

	return indice;
    
}

//functia calculeaza costul total a drumului si printeaza drumul dus-intos de la ferma, la client si inapoi la ferma
void task1(graf *mat_costuri, graf *next_hop, int tip_puncte_interes[], int vector_destinatii[],int l,FILE *f2){
	int i,j,n=mat_costuri->n,aprovizionare;
	int cost=0;
		for(i=0;i<n;i++)
			if(tip_puncte_interes[i]==1)
				aprovizionare=i;

	fprintf(f2,"%d", aprovizionare);
	for(j=0;j<l;j++){	
		cost=cost+mat_costuri->a[aprovizionare][vector_destinatii[j]]+mat_costuri->a[vector_destinatii[j]][aprovizionare];	
		printare_drum_dus_intors(mat_costuri,next_hop,aprovizionare,vector_destinatii[j],f2);
	
	}
	fprintf(f2,"\n");
	fprintf(f2,"%d\n",cost );

}

//se calculeaza costul de la ferma de start, pana la primul client si se printeaza; se calculeaza apoi drumul pana 
//la cea mai apropiata ferma, tinandu-se cont atat de locatia curenta (ultimul client vizitat), cat si de urmatorul client;
//se printeaza acest drum si se adauga costul lui la costul total
//se repeta algoritmul pentru toti clientii;
void task2(graf *mat_costuri, graf *next_hop, int tip_puncte_interes[], int vector_destinatii[],int l, int ferma_start, FILE *f2){
	int i,j,n=mat_costuri->n,aprovizionare[100];
	int cost=0,k=0,var_ferma;
	for(i=0;i<n;i++)
		if(tip_puncte_interes[i]==1){
			aprovizionare[k]=i;
			k++;
		}
	fprintf(f2, "%d",ferma_start);
	cost=cost+mat_costuri->a[ferma_start][vector_destinatii[0]];	
	printare_drum_dus(mat_costuri,next_hop,ferma_start,vector_destinatii[0],f2);
	var_ferma=aprovizionare[minim(aprovizionare,k,mat_costuri,vector_destinatii[0],vector_destinatii[1])];
	
	printare_drum_dus(mat_costuri,next_hop,vector_destinatii[0],var_ferma,f2);
	cost=cost+mat_costuri->a[vector_destinatii[0]][var_ferma];
	for(j=1;j<l-1;j++){
		printare_drum_dus(mat_costuri,next_hop,var_ferma,vector_destinatii[j],f2);
		cost=cost+mat_costuri->a[var_ferma][vector_destinatii[j]];
		var_ferma=aprovizionare[minim(aprovizionare,k,mat_costuri,vector_destinatii[j],vector_destinatii[j+1])];
		
		printare_drum_dus(mat_costuri,next_hop,vector_destinatii[j],var_ferma,f2);
		cost=cost+mat_costuri->a[vector_destinatii[j]][var_ferma];

	}
		printare_drum_dus(mat_costuri,next_hop,var_ferma,vector_destinatii[l-1],f2); 
		cost=cost+mat_costuri->a[var_ferma][vector_destinatii[l-1]];
		var_ferma=aprovizionare[minim2(aprovizionare,k,mat_costuri,vector_destinatii[l-1])];
		
		printare_drum_dus(mat_costuri,next_hop,vector_destinatii[l-1],var_ferma,f2); //aici 2
		cost=cost+mat_costuri->a[vector_destinatii[l-1]][var_ferma];

	fprintf(f2,"\n");
	fprintf(f2,"%d\n",cost );


}

//functia retine intr-un vector,drumul parcurs de la un punct la alatul; se  adauga in vectorul drum_total, care poate
//sa contina elemente anterioare, noul drum dintre cele doua puncte
void retinere_drum(int drum_total[], graf *next_hop, int plecare, int sosire, int *ind){
	int i=sosire, aux[100],k=0,j=*ind;
	aux[k]=sosire; k++;
	while(next_hop->a[plecare][i]!=-1){
		aux[k]=next_hop->a[plecare][i];
		k++;
		i=next_hop->a[plecare][i];
	}
	for(i=k-2;i>=0;i--){
		drum_total[j]=aux[i];
		j=j+1;
	}
	*ind=j;
}

//functia calculeaza drumul total de la ferma de start la ceilalti clienti si la fermele intermediare prin care trece drona;
//in vectorul aprovizionare, se retin fermele care sunt de acelasi tip cu urmtaorul client de vizitat; in var m, se retine
//cea mai apropiata dintre aceste ferme; drumul dintre punctul curent si cea mai apropiata ferma, se retine in vectorul drum_total;
//se adauga costul dintre cele doua puncte la costul total al drumului; se repeta algoritmul pentru toti clientii prin for
int calcul(int *drum_total, int *ind2, graf *mat_costuri, graf *next_hop, int tip_puncte_interes[], client *comenzi, int nr_comenzi,int ferma_start, FILE *f2){
	int i,j,n=mat_costuri->n,*aprovizionare;
	int cost=0,k=0,var_ferma,m;
	int tip_ferma=tip_puncte_interes[ferma_start];
	drum_total[0]=ferma_start;
	int ind=1;
	
	if(tip_ferma!=comenzi[0].tip){
		k=0;
		aprovizionare=malloc(30*sizeof(int));
		for(i=0;i<n;i++)
			if(tip_puncte_interes[i]==comenzi[0].tip){
				aprovizionare[k]=i; 
				k++;
			}
		
		m=aprovizionare[minim2(aprovizionare,k,mat_costuri,ferma_start)];
		retinere_drum(drum_total,next_hop,ferma_start,m,&ind);
		cost=cost+mat_costuri->a[ferma_start][m];
		var_ferma=m;
	}
	else
		var_ferma=ferma_start;
	for(i=0;i<nr_comenzi;i++){
		retinere_drum(drum_total,next_hop,var_ferma,comenzi[i].nr,&ind);
		cost=cost+mat_costuri->a[var_ferma][comenzi[i].nr];

		tip_ferma=comenzi[i+1].tip;
		
			//for(i=0;i<k;i++)
			//	aprovizionare[i]=-1;
			//free(aprovizionare);
			aprovizionare=malloc(30*sizeof(int));

		
		if(i!=nr_comenzi-1){
			k=0;
			for(j=0;j<n;j++)
				if(tip_puncte_interes[j]==tip_ferma){
					aprovizionare[k]=j;
					k++;
			}
		}
		else{
			k=0;
		
		for(j=0;j<n;j++)
		if(tip_puncte_interes[j]!=0){
			aprovizionare[k]=j;
			k++;
		}
		}
			m=aprovizionare[minim2(aprovizionare,k,mat_costuri,comenzi[i].nr)];
			retinere_drum(drum_total,next_hop,comenzi[i].nr,m,&ind);
			cost=cost+mat_costuri->a[comenzi[i].nr][m];
			var_ferma=m;
		
		
	}
	//if(aprovizionare!=NULL)
		//free(aprovizionare);

	*ind2=ind;
	return cost;
	
}

int corect(int *sol, int n)
{
	int i, j;
	for(i = 0;i < n;i++)
		for(j = i+1;j <= n;j++)
			if(sol[i] == sol[j] && i != j)
				return 0;
	return 1;
}


//prin backtracking, generez toate combinatiile posibile in care pot fi procesate comenzile, fiind retinut costul cel mai mic 

void bkt(int *sol, int n, int k, int *cost, graf *mat_costuri, graf *next_hop, int tip_puncte_interes[],  client comenzi[], int l, int ferma_start,FILE *f2)
{
	int i;
	for(i = 0;i < n;i++)
	{
		sol[k] = comenzi[i].nr;
		if(corect(sol, k))
		{
			if(k == n-1)
			{				
				int aux[200], cost_aux,j,p; 
				int ind2=0;
				client aux2[100];
				for(j=0;j<l;j++){
					aux2[j].nr=sol[j];
					for(p=0;p<l;p++)
						if(comenzi[p].nr==aux2[j].nr)
							aux2[j].tip=comenzi[p].tip;
				}
				cost_aux=calcul(aux,&ind2,mat_costuri,next_hop,tip_puncte_interes, aux2, l,ferma_start,f2);
				if(cost_aux<*cost)
					*cost=cost_aux;

			}
			else
				bkt(sol, n, k+1,cost,mat_costuri,next_hop,tip_puncte_interes,comenzi,l,ferma_start,f2);
			
		}
	}
}

//dupa ce este aflat cel mai mic cost posibil, se mai efectueaza o data backtracking, in care se gaseste drumul cu cel mai mic 
//cost si se printeaza
void bkt2(int *sol, int n, int k, int cost, graf *mat_costuri, graf *next_hop, int tip_puncte_interes[],  client comenzi[], int l, int ferma_start,FILE *f2, int *afi)
{
	int i;
	for(i = 0;i < n;i++)
	{
		sol[k] = comenzi[i].nr;
		if(corect(sol, k))
		{
			if(k == n-1)
			{
				int *aux=calloc(200,sizeof(int)), cost_aux,j,p; 
				int ind2=0;
				client aux2[100];
				for(j=0;j<l;j++){
					aux2[j].nr=sol[j];
					for(p=0;p<l;p++)
						if(comenzi[p].nr==aux2[j].nr)
							aux2[j].tip=comenzi[p].tip;
				}
				cost_aux=calcul(aux,&ind2,mat_costuri,next_hop,tip_puncte_interes, aux2, l,ferma_start,f2);
				if(cost==cost_aux && *afi==0){
					fprintf(f2, "%d", aux[0]);
					for(i=1;i<ind2;i++)
						fprintf(f2," %d", aux[i] );
						
					fprintf(f2, "\n");
					*afi=1;
				}

			}
			else
			{
				bkt2(sol, n, k+1,cost,mat_costuri,next_hop,tip_puncte_interes,comenzi,l,ferma_start,f2,afi);
			}
		}
	}
}

void numere_muchii(char s[], int *a, int *b, int *c){
	int i=0;
			while(s[i]!=' '){
				*a=*a*10+(s[i]-48);
				i++;
			}
			i++;
			while(s[i]!=' '){
				*b=*b*10+(s[i]-48); 
				i++;
			}
			i++;
			while(i<strlen(s)-1){
				*c=*c*10+(s[i]-48);
				i++;
			}

}

void numere_comenzi(char s[],int *l, int *p){
	int io=0;
	while(s[io]!=' '){
					*l=*l*10+(s[io]-48); 
					io++;
				}
				io++;
				while(io<strlen(s)-1){
					*p=*p*10+(s[io]-48);
					io++;
				}
}

int main(int argc, char const *argv[])
{
	int nr_task, nr_puncte_interes,i,k=0,l=0,ferma_start;
	int *tip_puncte_interes;
	int culoare_aeriene=100000,nr_comenzi=100000,aprovizionare=0,destinatie=0,vector_destinatii[100],vector_tip_aprovizionare[100];
	int contor=1;
	char s[100];
	graf *g=NULL, *mat_costuri=NULL, *next_hop=NULL;
	FILE *f=fopen(argv[1],"r");
	FILE *f2=fopen(argv[2],"w");
	if(f==NULL){ 
	printf("Nu se poate deschide la citire fişierul!\n");

		exit(1); 
	}	
	while(fgets(s,100,f)){
		if(contor==1)
			nr_task=atoi(s);
		if (contor==2){
			nr_puncte_interes=atoi(s);
			g=init(nr_puncte_interes);
		}
		if(contor==3){
			tip_puncte_interes=calloc(nr_puncte_interes,sizeof(int *));
			for(i=0;i<strlen(s);i=i+2){
				tip_puncte_interes[k]=s[i]-48;
				k++;
			}
		}
		if(contor==4)
			culoare_aeriene=atoi(s);
		
		if(contor>4 && strlen(s)>=6){
			int a=0,b=0,c=0;
			numere_muchii(s,&a,&b,&c);
			add_edge(g,a,b,c);
		}
		if (contor>4 && strlen(s)<6){
			if(strlen(s)==2 || strlen(s)==3){
				nr_comenzi=atoi(s);
				mat_costuri=construire_mat_costuri(g);
				next_hop=hop(g->n,mat_costuri);
				floyd(mat_costuri,next_hop);
			}
			else{
				aprovizionare=0; destinatie=0;
				numere_comenzi(s,&aprovizionare,&destinatie);

				vector_tip_aprovizionare[l]=aprovizionare;					
				vector_destinatii[l]=destinatie;
				l++;
			}
		 
		}
		if(contor>6+culoare_aeriene && strlen(s)<4 )
			{ferma_start=atoi(s);
		}
		contor++;
	}

	if(nr_task==1)
		task1(mat_costuri,next_hop,tip_puncte_interes,vector_destinatii,l,f2);
	if(nr_task==2)
	task2(mat_costuri, next_hop, tip_puncte_interes, vector_destinatii,l, ferma_start, f2);
	if(nr_task==3){
		client comenzi[100];
		for(i=0;i<l;i++){
			comenzi[i].nr=vector_destinatii[i];
			comenzi[i].tip=vector_tip_aprovizionare[i];

		}
		
	
		int  sol[100],cost=100000,sol2[100];
		int ind2=0,afi=0;
		

		bkt(sol, l, 0, &cost, mat_costuri, next_hop, tip_puncte_interes, comenzi, l, ferma_start,f2);
		bkt2(sol2, l, 0,cost, mat_costuri, next_hop, tip_puncte_interes, comenzi, l, ferma_start,f2,&afi);
		
		fprintf(f2,"%d\n", cost );	

	}
	//free(tip_puncte_interes); 
	//free(g);
	//free(mat_costuri);
	//free(next_hop);

	fclose(f);
	fclose(f2);
	return 0;
}
