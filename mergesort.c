#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <time.h>

// DECLARA TODAS AS VARIAVEIS GLOBAIS
//**********************************************************************************         
int i;//Contador universal
int aux_vetor=2; //Auxiliar para o vetor    
int cont_n=2; //Contador para saber quantos inteiros tem o arquivo de saída.
int *mem; //Memoria alocada dinamicamente.
int aloc=5000; //Variavel para aumentar de 1000 em 1000 a memoria alocada.
int n_thread;


// MARCADOR DE TEMPO
//**********************************************************************************
#define GET_MS(ini, fim)  ((fim.tv_sec * 1000000 + fim.tv_usec) \
			- (ini.tv_sec * 1000000 + ini.tv_usec))
            struct timeval inicio, fim;



// STRUCT QUE RECEBE VALORES DE INICIO E FIM DO VETOR
//**********************************************************************************
typedef struct{  
        int *i_vetor; 
        int f_vetor; 
    }thread_arg, *ptr_thread_arg; 


// GERAÇÃO DO ARQOUT
//**********************************************************************************
void criar_arqOut(int x, char* y[]){  

	int n;    
    
	FILE *arqOut;        
	arqOut=fopen(y[x-1],"w");

	for(int i=2; i<x-1;i++){//Lendo arqIN
       
        	FILE *arqIn; 
        	arqIn=fopen(y[i],"r"); 
      
        	while (fscanf(arqIn,"%d", &n) != EOF) { 

        		fprintf(arqOut,"%d ",n);

        	}

        	fclose(arqIn);

	}

	fclose(arqOut); 

}


// GRAVA ARQUIVO NO VETOR
//**********************************************************************************
void gravar_vetor(int x, char* y[]){   
    
	FILE *saida; 
	saida=fopen(y[x-1],"r");

	mem =(int *) calloc(5000,sizeof(int));

	while(fscanf(saida,"%d\n", &mem[cont_n]) != EOF){

        	cont_n++; 
        	if(cont_n==aloc){
         
         		aloc=cont_n+5000; 
         		mem=realloc(mem,aloc*sizeof(int));

         	}         
    
	}

	fclose(saida);


}


// COLOCA OS NÚMEROS EM ORDEM CRESCENTE
//**********************************************************************************
void *crescente (void *arg){ 

    	ptr_thread_arg targ = (ptr_thread_arg)arg; 
    	int a, b;
    	for (i=aux_vetor; i<=targ->f_vetor;i++){ 

    		b = targ->i_vetor[i];
    		a=i-1;
    		targ->i_vetor[1] = b; 

    		while (b < targ->i_vetor[a]){

        		targ->i_vetor[a+1] = targ->i_vetor[a];
        		a--;

    		}

    		targ->i_vetor[a+1] = b;

    	}  
}


// GRAVA NÚMEROS NO ARQUIVO DE SAIDA
//**********************************************************************************
void gravar(int x, char* y[]){

    	FILE *saida; 
    	saida=fopen(y[x-1],"w");

    	for(int i=2;i<cont_n;i++){ 
      
      		fprintf(saida,"%d ",mem[i]);
    	} 

    	fclose(saida);
}


// GERENCIADOR DE THREADS
//**********************************************************************************
void executa_thread(){

	clock_t timer;//Cria uma variavel
	timer = clock();//Atribui a função clock() a variavel timer
 
    	pthread_t thread[n_thread]; 
    	thread_arg arguments[n_thread];
 
    	for(int i=0; i<n_thread;i++){ 
       
        	arguments[i].i_vetor=mem; 
        	int a=i+1;
        	arguments[i].f_vetor=(((cont_n-1)/n_thread)*a)+ (cont_n-1)%n_thread; 
        	pthread_create(&thread[i],NULL, crescente,&(arguments[i]));
        	pthread_join(thread[i],NULL); 
        	aux_vetor=arguments[i].f_vetor; 
        
    	}

   	timer=clock()-timer;

    	printf("TEMPO TOTAL: %f segundo(s) \n\n", ((float)timer)/CLOCKS_PER_SEC);
	
}


// FUNÇÃO PRINCIPAL
//**********************************************************************************
int main(int x,char* y[]){
  
	n_thread = atoi (y[1]);
    
	if(n_thread!=2 && n_thread!=4 && n_thread!=8 && n_thread!=16){ 
	        printf("NÃO FOI POSSIVEL RODAR O PROGRAMA COM %d THREADS\n", n_thread);
	        return 0; //Encerra execução
	}

	criar_arqOut(x, y);
	gravar_vetor(x,y);
	executa_thread();
	gravar(x,y);
   
        return 0;


}
