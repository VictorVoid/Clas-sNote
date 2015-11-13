#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#define RUB 0
#define NEG 1
#define N_MAX 1027

typedef struct NO{
    char nome[100];
    int matricula;
    float nota1, nota2, nota3;
    char cor;
    struct NO *pai;
    struct NO *esquerda;
    struct NO *direita;
}NO;

typedef struct tipoArvore{
    NO *topo;
}tipoArvore;

tipoArvore arvore;

void criarArvore(tipoArvore *arvore){
    arvore->topo = NULL;
}

NO* noAvo(NO *n){
    if(n == NULL){
        return NULL;
    }

    if(n->pai == NULL){
        return NULL;
    }

    else{
        return n->pai->pai;
    }
}

NO* noIrmao(NO *n){
    if(n == NULL){
        return NULL;
    }

    if(n->pai == NULL){
        return NULL;
    }

    if(n == n->pai->esquerda){
        return n->pai->direita; /*Apenas verifica se ele tá no lado esquerdo, se for direito fica falsa. Retorna o irmao/tio*/
    }

    else{
        return n->pai->esquerda;
    }
}

NO* noTio(NO *n){
    if(n == NULL){
        return NULL;
    }

    return noIrmao(n->pai);/*passando o pai, a funcao irmao verifica o irmao dele */
}


NO* rotacaoEsquerda(NO* raiz, NO* n){
    NO* r = n->direita;/* 3 */

    if(n == NULL){
        return raiz;
    }

    if(raiz == n){
        raiz = r;/*Caso estive uma reta, a raiz fica no meio para ficar balanceada, que o segundo passo ^ */
    }else
        if(n == n->pai->esquerda){/*Verifica se é filho da esquerda*/
            n->pai->esquerda = r;/*Faz a rotacao e deixa uma reta*/
    }else{
        n->pai->direita = r;
    }

    n->direita = r->esquerda;/*Caso o no tenha filhos, eles vao todos para o lado direito do 2 (no),
                            ja que o 3 ficara no meio: filhos da dir de 3, fica no lado dir do 2 */

    if(r->esquerda != NULL){
        r->esquerda->pai = n;
    }

    r->esquerda = n;
    r->pai = n->pai;
    n->pai = r;

    return raiz;
}

void rotacaoEsquerdaRR(NO **x){

    NO *y;
    y = *x;
    *x = (*x)->direita;
    (*x)->pai = y->pai;
    y->direita = (*x)->esquerda;
    if(y->direita)    y->direita->pai = y;
    (*x)->esquerda = y;
    y->pai = *x;
}
NO* rotacaoDireita(NO* raiz, NO* n){
    NO* l = n->esquerda;

    if(n == NULL){
        return raiz;
    }

    if(raiz == n){
        raiz = l;
    }

    else if(n == n->pai->esquerda){
        n->pai->esquerda = l;
    }

    else{
        n->pai->direita = l;
    }

    n->esquerda = l->direita;

    if(l->direita != NULL){
        l->direita->pai = n;
    }

    l->direita = n;
    l->pai = n->pai;
    n->pai = l;

    return raiz;
}
void rotacaoDireitaRR(NO **x){
    NO *y;
    y = *x;
    *x = (*x)->esquerda;
    (*x)->pai = y->pai;
    y->esquerda = (*x)->direita;
    if(y->esquerda)   y->esquerda->pai = y;
    (*x)->direita = y;
    y->pai = *x;
}

int verificaCor(NO* n){
    if(n == NULL){
        return NEG;/*Caso vermelho 1 e preto 0*/
    }

    else{
        return n->cor;
    }
}

NO* balanceamentoCaso5(NO* raiz, NO* n){/*Sempre depois do caso4 ele vem pra ca, onde ajusta as cores.*/
    n->pai->cor = NEG;/*Ja que o no é vermelho, caso esteja vermelho o pai, já troca para preto.*/
    noAvo(n)->cor = RUB;/*Caso esse avo ainda nao seja a raiz, deixando vermelho para assim ficar V P V*/
    if(n == n->pai->esquerda && n->pai == noAvo(n)->esquerda){ /*Verificando de qual lado esta localizado para fazer a ultima rotacao. No desenho que fiz logo a cima, é o segundo passo.*/
        raiz = rotacaoDireita(raiz, noAvo(n));
    }

    else if(n == n->pai->direita && n->pai == noAvo(n)->direita){
        raiz = rotacaoEsquerda(raiz, noAvo(n));
    }

    return raiz;
}
NO* balanceamentoCaso4(NO* raiz, NO* n){
    if(n == n->pai->direita && n->pai == noAvo(n)->esquerda){/*se ele tiver no lado direito e o pai lado esquerdo*/
   /*   (4)             (4)              (3)
        /                /               /   \
     (2)        =>    (3)no    =>      (2)   (4)
        \             /
         (3)no      (2) */
        raiz = rotacaoEsquerda(raiz, n->pai);
        n = n->esquerda;
    }
    /* (4)           (4)                 (5)
         \              \               /   \
         (6)    =>      (5)    =>     (4)  (6)
        /                  \
     (5)no                 (6)                  */

    else if(n == n->pai->esquerda && n->pai == noAvo(n)->direita){
        raiz = rotacaoDireita(raiz, n->pai);
        n = n->direita;
    }

    return balanceamentoCaso5(raiz, n);
}

NO* balanceamentoCaso1(NO* raiz, NO* n);

NO* balanceamentoCaso3(NO* raiz, NO* n){
    if(verificaCor(noTio(n)) == RUB){/*tem pai vermelho e tio vermelho, troca as cores*/
        n->pai->cor = NEG; /*Trocando a cor do pai, já que quando insere com vermelho, o pai nao pode ser também.*/
        noTio(n)->cor = NEG; /*Trocando a cor do tio*/
        noAvo(n)->cor = RUB;/*Trocando a cor do avô, para assim ficar: V, P, V*/
        return balanceamentoCaso1(raiz, noAvo(n));/*Caso o noAvo ja seja a raiz, entao nao pode ficar vermelha como
                                                deixei, vai cair no caso1 para deixar a raiz preta e retornar raiz.*/
    }

    else{
        return balanceamentoCaso4(raiz, n);
    }
}

NO* balanceamentoCaso2(NO* raiz, NO* n){
    if(n->pai->cor == NEG){
        return raiz;
    }

    else{
        return balanceamentoCaso3(raiz, n);
    }
}

NO* balanceamentoCaso1(NO* raiz, NO* n){
    if(n->pai == NULL){
        n->cor = NEG;/*Raiz de uma arvore é sempre negra.*/
        return raiz;
    }

    else{
        return balanceamentoCaso2(raiz, n);
    }
}

NO* noNovo(int n1, int n2, int n3, char nome[], int matricula){
    NO* aux;

    aux = (NO*) malloc(sizeof(NO));
    aux->matricula = matricula;
    aux->nota1 = n1;
    aux->nota2 = n2;
    aux->nota3 = n3;
    strcpy(aux->nome,nome);
    aux->cor = RUB;
    aux->esquerda = NULL;
    aux->direita = NULL;
    aux->pai = NULL;
    return aux;
}

NO* inserirElemento(NO* novo_aluno, NO *raiz){
    NO* novo = noNovo(novo_aluno->nota1, novo_aluno->nota2, novo_aluno->nota3,novo_aluno->nome, novo_aluno->matricula);
    if(raiz == NULL){
        novo->cor = NEG;
        return novo;
    }

    NO* aux = raiz;

    while(1){
        if(novo_aluno->matricula == aux->matricula){
            aux = novo; /*Substituindo o novo no lugar do antigo*/
            return raiz;
        }
        else if(novo_aluno->matricula < aux->matricula){
            if(aux->esquerda == NULL){
                aux->esquerda = novo;
                novo->pai = aux;
                return balanceamentoCaso1(raiz, novo);
            }else{
                aux = aux->esquerda;
            }
        }

        else {
            if (aux->direita == NULL){
                aux->direita = novo;
                novo->pai = aux;
                return balanceamentoCaso1(raiz, novo);
            }

            else{
                aux = aux->direita;
            }
        }
    }
}

void inserirArvore(NO* novo_aluno, tipoArvore* arvore){
    arvore->topo = inserirElemento(novo_aluno, arvore->topo);
}

NO* buscarElemento(int numero, NO *raiz){
    if ( raiz == NULL || raiz->matricula == numero){
        return raiz;
    }
    if (numero < raiz->matricula){
        return buscarElemento(numero, raiz->esquerda);
    }
    if(numero > raiz->matricula){
        return buscarElemento(numero,raiz->direita);
    }
}

NO* buscarNaArvore(int numero, tipoArvore arvore){
    return buscarElemento(numero, arvore.topo);
}

NO* maiorDosMenores(NO *raiz){
    NO *aux;

    aux = raiz->esquerda;

    if(aux == NULL){
        return NULL;
    }
    else{
        while (aux->direita != NULL){
            aux = aux->direita;
        }

    return aux;
    }
}

NO* menorDosMaiores(NO *raiz){
    NO *aux;

    aux = raiz->direita;

    if(aux == NULL){
        return NULL;
    }
    else{
        while(aux->esquerda != NULL){
            aux = aux->esquerda;
        }
    return aux;
    }
}
NO* conserta(NO** raiz, NO* no){
    NO* aux;
    while((no != *raiz) && (verificaCor(no) == NEG)){
        if(no == no->pai->esquerda){
            aux = no->pai->direita;

            if(verificaCor(aux) == RUB){
                aux->cor = NEG;
                no->pai->cor = RUB;
                rotacaoEsquerdaRR(&no->pai);
                aux = no->pai->direita;
                continue;
            }

            if(verificaCor(aux->esquerda) == NEG && verificaCor(aux->direita) == NEG){
                aux->cor = RUB;
                no = no->pai;
                continue;
            }

            else if(verificaCor(aux->direita) == RUB){
                aux->esquerda->cor = NEG;
                aux->cor = RUB;
                rotacaoDireitaRR(&aux);
                aux = aux->pai->direita;
                continue;
            }
            aux->cor = no->pai->cor;
            no->pai->cor = NEG;
            aux->direita->cor = NEG;
            rotacaoEsquerdaRR(&no->pai);
            no = *raiz;
        }

        else{
            aux = no->pai->esquerda;

            if(verificaCor(aux) == RUB){
                aux->cor = NEG;
                no->pai->cor = RUB;
                rotacaoDireitaRR(&no->pai);
                aux = no->pai->esquerda;
                continue;
            }

            if(verificaCor(aux->esquerda) == NEG && verificaCor(aux->direita) == NEG){
                aux->cor = RUB;
                no = no->pai;
                continue;
            }

            else if(verificaCor(aux->esquerda) == RUB){
                aux->direita->cor = NEG;
                aux->cor = RUB;
                rotacaoEsquerdaRR(&aux);
                aux = aux->pai->esquerda;
                continue;
            }
            aux->cor = no->pai->cor;
            no->pai->cor = NEG;
            aux->esquerda->cor = NEG;
            rotacaoDireitaRR(&no->pai);
            no = *raiz;
        }
    }
    if(no != NULL){
        no->cor = NEG;
    }
    return *raiz;
}

NO* transplante(NO **raiz, NO* no,NO* noAux){/*depois que remover precisamos usar*/
    if(no->pai == NULL){
        *raiz = noAux;
    }

    else if(no == no->pai->esquerda){
        no->pai->esquerda = noAux;
    }

    else{
        no->pai->direita = noAux;
    }

    if(noAux == NULL){
        return *raiz;
    }
    else{
        noAux->pai = no->pai;
        return *raiz;
    }
}

NO* removerElemento(int numero, NO *raiz){
    NO *no;
    NO *aux;
    NO *M;

    int auxCor;

    no = buscarElemento(numero, raiz);

    if(raiz == NULL || no == NULL){
        return raiz;
    }

    auxCor = no->cor;

    if(no->esquerda == NULL){
        aux = no->direita;
        transplante(&raiz,no, no->direita);
    }

    else if(no->direita == NULL){
        aux = no->esquerda;
        transplante(&raiz, no, no->esquerda);
    }

    else{
        M = menorDosMaiores(no);
        auxCor = M->cor;
        aux = M->direita;

        if(M->pai == no){
            if(aux != NULL){
                aux->pai = M;
            }
        }else{
            transplante(&raiz, M, M->direita);
            M->direita = no->direita;
            M->direita->pai = M;
        }

        transplante(&raiz, no, M);
        M->esquerda = no->esquerda;
        M->esquerda->pai = M;
        M->cor = no->cor;
    }

    if(auxCor == NEG){
        if(aux!= NULL){
            conserta(&raiz, aux);
        }
    }
    free(no);
    return raiz;
}

void removerDaArvore(int numero, tipoArvore *arvore){
    arvore->topo = removerElemento(numero, arvore->topo);
}
void carregaArquivo(tipoArvore* arvore, FILE *arquivo){//passo por parametro a raiz e url
    FILE *arquivo2; //auxiliar para trocar o ; pelo espaço
    char txt, nome[50];
    float nt1, nt2, nt3;
    int matricula;
    NO *novo_aluno;
    // arquivo apenas ler o arquivo que quer carregar
    arquivo2= fopen ("auxiliar.dat","w"); //Cria um arquivo para ser temporário, onde vou trocar as ',' pelo '.' para os numeros
    while ((txt=fgetc(arquivo))!= EOF){ //caracter por caracter até encontrar o fim do arquivo.
        if (txt == ',') //faço a troca da virgula por ponto, pois o computador so entende float com ponto
        txt = '.';
        fprintf(arquivo2,"%c", txt);
    }
        fclose(arquivo2);//fechando para abrir em modo leitura
        arquivo2 = fopen("auxiliar.dat", "r");
        while (!feof(arquivo2)){//enquanto nao encontrar o fim do arquivo
            fscanf(arquivo2, "%d; %[^;]s", &matricula, nome); // o [^;] é para ler toda a string ate encontrar ';'
            fscanf(arquivo2, ";%f ;%f ;%f;", &nt1, &nt2, &nt3);
            //Enviando para ARVORE:
            if (!feof(arquivo2)){ //pra que na ultima vez ele nao faça de novo
                novo_aluno = noNovo(nt1, nt2, nt3, nome,matricula); //aloca os valores no novo aluno.
                inserirArvore(novo_aluno, arvore);
            }
        }
    fclose(arquivo);
    fclose(arquivo2);
    remove("auxiliar.dat");//Apago o arquivo temporário!
}

int maior(int numeroA, int numeroB){
    if(numeroA > numeroB){
        return numeroA;
    }
    else {
        return numeroB;
    }
}

int calculaAltura(NO *raiz){
    if(raiz == NULL){
        return 0;
    }
    else {
        return (maior(calculaAltura(raiz->esquerda),calculaAltura(raiz->direita)) + 1);
    }
}

void menuRemocaoArvore(){
    NO* aux;
    int opcao = -1;
    int elemento;
    scanf("%d", &opcao);
    switch(opcao){
        case 1:
            printf("\nEscolha a matricula para remover da arvore: ");
            scanf("%d", &elemento);
            aux = buscarNaArvore(elemento, arvore);
            if(aux){
                removerDaArvore(elemento, &arvore);
                printf("\nSucesso. O elemento foi removido.\n");
            } else printf("\nEste elemento nao esta na arvore.\n");
            break;
        default:
            printf("Opcao Invalida.\n\n");
    }
}
/****************************************************************************/
void remover(tipoArvore arvore, FILE* arquivo, FILE* excluidos){//arvore, entrada, saida
        char delentrada [200], vet[600], *texto, delsaida[200], *token = NULL;
        NO *aux;
        int valor;
        struct dirent *drt;
        fgets(vet,600,arquivo); //passando tudo para o array
        token = strtok(vet,";");//separando
        while(token){
            valor = atoi(token);//transforma em numero
           // printf("%d\n", valor);

            aux = buscarElemento(valor, arvore.topo);
            if (aux == NULL){
                //printf("Nao encontrado!");
                fprintf(excluidos,"%d Nao encontrado\n", valor);
            }else
                if (aux->matricula == valor){
                    //printf ("Existe alguem com esse numero :D\n");
                    fprintf(excluidos,"%d Excluido\n",valor);
                    removerDaArvore(valor, &arvore);
                }
            token = strtok(NULL, ";");
        }
            fprintf(excluidos, "\n\n\n");//deixando espaço para o proximo desenho
    fclose(excluidos);
}
/***********IMPRESSAO EM DESENHO*********/
void printnode(int c, int b, FILE *da1) {
    int i;
    for (i = 0; i < b; i++) fprintf(da1, "   ");
    fprintf(da1 , "%d\n" , c);
}
void mostrar(NO *x, int b, FILE *da1) {

    if (x == NULL)
    {
        printnode(-1, b, da1);
        return;
    }
    mostrar(x->direita, b+1, da1);
    printnode(x->matricula, b, da1);
	mostrar(x->esquerda, b+1, da1);
}
/****************salva final*****************/
void salvar(NO *no,FILE *arqsalvar){
	int num;
	char vet[50];
	if(no!=NULL){
		salvar(no->esquerda,arqsalvar);
		fprintf(arqsalvar,"%d;", no->matricula);
        fprintf(arqsalvar,"%s;", no->nome);
        fprintf(arqsalvar,"%.2f;", no->nota1);
        fprintf(arqsalvar,"%.2f;", no->nota2);
        fprintf(arqsalvar,"%.2f;", no->nota3);
        fprintf(arqsalvar,"\n");
		salvar(no->direita,arqsalvar);
    }
}
/*********************************MAIN************************************/
int main(int argc, char **argv){
    criarArvore(&arvore);/*passa o endereco pra que a funcao crie*/
    FILE *arquivo, *arquivoremove, *arquivoentrada, *desenho_arvore, *fim;
    DIR *dir;
    struct dirent *lsdir;
    dir= opendir(argv[1]);/*Url passada dos arquivos dos alunos.dat*/
    char caminho[300], caminhosaida[300], caminhoremove[300];
    /* Primeiro parâmetro: o de leitura */

    /***********************CARREGANDO ARQUIVOS DA PASTA****************************************/
    while ( ( lsdir = readdir(dir) ) != NULL )//atraves do readdir o lsdir tem struct com nomes como parametros..
    {
       if (strcmp(lsdir->d_name, ".") != 0 && strcmp(lsdir->d_name, "..")){ /*O lsdir traz pontos indesejaveis, entao para que nao use-o - uma condicao*/
            /*De um por um, com o lsdir->d_name*/
            strcpy(caminho,argv[1]);//salvando em uma variavel
            strcat(caminho, "/");
            strcat(caminho,lsdir->d_name);/*Concatenando*/
            //printf("%s\n",caminho);
            arquivo = fopen(caminho, "r");
            carregaArquivo(&arvore, arquivo);
        }
    }

        closedir(dir);
        /**************IMPRESSAO EM DESENHO INICIAL.RES*****************/
         strcpy(caminhosaida, argv[3]);
         strcat(caminhosaida,"/carga-inicial.res");
         desenho_arvore = fopen(caminhosaida, "w");
         mostrar(arvore.topo, 0, desenho_arvore);
         fclose(desenho_arvore);
        /*********************REMOVER**********************/
         strcpy(caminhoremove, argv[3]);//terceiro argumento..saída
         strcat(caminhoremove, "/");
         strcat(caminhoremove, "excluir.res");
         arquivoremove = fopen(caminhoremove, "w");
        /**********concatenando strings para a entrada ************/
        strcpy(caminho, argv[2]);
        strcat(caminho, "/");
        strcat(caminho, "excluir.in");
        arquivo = fopen(caminho, "r");
        remover(arvore, arquivo, arquivoremove);//removendo todos do excluir.in o argv2 é o parametro de remocao
        /*****************DESENHO APOS EXCLUSAO.RES (matriculas impressa em formato de arvore********************/
        strcpy(caminhosaida, argv[3]);
        strcat(caminhosaida, "/apos-exclusao.res");
        desenho_arvore = fopen(caminhosaida, "w");
        mostrar(arvore.topo, 0, desenho_arvore);
        fclose(desenho_arvore);
        /**************IMPRESSAO APOS EXCLUSAO***********/
        strcpy(caminhosaida, argv[3]);
        strcat(caminhosaida,"/final.res");
        fim = fopen(caminhosaida,"w");
        salvar(arvore.topo, fim);
        /**************************************************/
    return 0;
}
