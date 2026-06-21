#include <iostream>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <fstream>

using namespace std;

typedef struct ListaDupla{
    char letra;
    int quantidade;
    struct ListaDupla *pProx,*pAnt;
    struct ListaSimples *listaSimples;
    }ListaDupla;


typedef struct ListaSimples{
    char palavra[50];
    char descricao[200];
    struct ListaSimples *pProx;
    }ListaSimples;


ListaDupla listaInicio_dupla, *pAuxD, *pSecD;
ListaSimples listaInicio_simples, *pAuxS, *pSecS;
ListaSimples* verificar(char letra);
void inserir_Palavra(ListaSimples *lista, char palavra[50], char descricao[200]);
void carregarArquivo();
void salvarArquivo();

void gotoxy(int y, int x){
    printf("\033[%d;%dH", x, y);
}
void limparTela(){
    printf("\033[2J");
    printf("\033[H");
} //modulo util

void maiusculo(char palavra[50]){

    for(int i = 0; palavra[i] != '\0'; i++){
        palavra[i] = toupper(palavra[i]);
    }
} //modulo util (passar a palavra para maisculo, para gerar sensitive case)

ListaSimples* existeProfissao(char palavra[50]){

    char letra = toupper(palavra[0]);

    ListaSimples *lista = verificar(letra);


    ListaSimples *aux = lista;

    while(aux != NULL){

        if(strcmp(aux->palavra,palavra) == 0){
            return aux;
        }

        aux = aux->pProx;
    }

    return NULL;
} //modulo util (verifica se ja existe profissao na lista, evita duplicidade)

void carregarArquivo(){

    ifstream arquivo("profissoes.txt"); //abro meu arquivo para leitura

    if(!arquivo){
        return;
    }

    char linha[300]; // armazeno cada linha do arquivo

    while(arquivo.getline(linha, 300)){ // leio arquivo linha por linha até o fim

        char *palavraArquivo = strtok(linha, ";"); // lê a profissão até o ;
        char *descricaoArquivo = strtok(NULL, ";"); // lê a descrição depois do ; até o outro ;

        if(palavraArquivo != NULL && descricaoArquivo != NULL){

            char palavra[50];
            char descricao[200]; //vetores que receberao as informacoes

            strcpy(palavra, palavraArquivo);
            strcpy(descricao, descricaoArquivo); // copio do arquivo para a memória

            maiusculo(palavra);

            char letra = toupper(palavra[0]);

            ListaSimples *lista = verificar(letra); //verifico se ja existe um nó para essa letra

            if(lista == NULL){

                // cria nova letra na lista dupla

                pAuxD = &listaInicio_dupla; //começo do inicio

                while(pAuxD->pProx != NULL){
                    pAuxD = pAuxD->pProx; //percorro até o ultimo nó da lista dupla
                }

                pSecD = new ListaDupla; //declaro meu novo nó na lista dupla

                pSecD->letra = letra; //adiciono a letra do meu nó
                pSecD->quantidade = 1; //agora o meu nó passará a ter uma palavra

                pSecD->pProx = NULL;
                pSecD->pAnt = pAuxD;

                pSecD->listaSimples = new ListaSimples; //declaro minha lista simples desse nó
                pSecD->listaSimples->pProx = NULL; // inicio vazia

                pAuxD->pProx = pSecD; // ligo o novo nó na lista dupla

                inserir_Palavra(pSecD->listaSimples, palavra, descricao); //insiro a palavra bem como a descricao a partir da funcao
            }
            else{

                // letra já existe

                inserir_Palavra(lista, palavra, descricao);

                ListaDupla *auxD = listaInicio_dupla.pProx;

                while(auxD != NULL){

                    if(auxD->letra == letra){
                        auxD->quantidade++;
                        break;
                    }

                    auxD = auxD->pProx;
                }
            }
        }
    }

    arquivo.close();
}

void salvarArquivo(){

    ofstream arquivo("profissoes.txt");

    if(!arquivo){
        cout << "Erro ao abrir o arquivo para salvar!\n";
        system("pause");
        return;
    }

    ListaDupla *auxD = listaInicio_dupla.pProx;

    while(auxD != NULL){

        ListaSimples *auxS = auxD->listaSimples->pProx;

        while(auxS != NULL){
            arquivo << auxS->palavra << ";" << auxS->descricao << endl;
            auxS = auxS->pProx;
        }

        auxD = auxD->pProx;
    }

    arquivo.close();

    cout << "Arquivo salvo com sucesso!\n";
    system("pause");
}

void inserir_Palavra(ListaSimples *lista, char palavra[50], char descricao[200]){
    ListaSimples *aux = lista;
    while(aux->pProx != NULL){
        aux = aux->pProx;
    }
    ListaSimples *novo = new ListaSimples;
    strcpy(novo->palavra, palavra);
    strcpy(novo->descricao, descricao);
    novo->pProx = NULL;
    aux->pProx = novo;
}

ListaSimples* verificar(char letra){
    ListaDupla *aux = listaInicio_dupla.pProx;

    while(aux != NULL){
        if(letra == aux->letra){
            return aux->listaSimples;
        }
        aux = aux->pProx;
    }
    return NULL;
}

void inserir_Letra(){
    char palavra[50];
    char descricao[200];

    limparTela();

    cout << "Digite a profissao: ";
    cin.ignore();
    cin.getline(palavra,50);

    cout << "Digite a descricao: ";
    cin.getline(descricao, 200);

    maiusculo(palavra);

    pAuxD = &listaInicio_dupla;
    char letra = toupper(palavra[0]);

	//se existir a profissao, avisar
	if(existeProfissao(palavra)){
        limparTela();
        cout << "Essa profissao ja esta cadastrada!\n";
        system("pause");
        return;
    }

    if(pAuxD->pProx == NULL){ //se a lista estiver vazia, cria o primeiro no
        pSecD = new ListaDupla;
        pSecD->letra = letra;
        pSecD->quantidade = 0;

        pSecD->pProx = NULL;
        pSecD->listaSimples = new ListaSimples;
        pSecD->listaSimples->pProx = NULL;
        pSecD->pAnt = pAuxD;

        pAuxD->pProx = pSecD;
        inserir_Palavra(pSecD->listaSimples,palavra,descricao);
        pSecD->quantidade++;

    }else{
        if(verificar(letra) == NULL){ //se a letra nao existir, ele add no final

            while(pAuxD->pProx != NULL){
                pAuxD = pAuxD->pProx;
            }
            pSecD = new ListaDupla;
            pSecD->letra = letra;
            pSecD->quantidade = 0;

            pSecD->pProx = NULL;
            pSecD->listaSimples = new ListaSimples;
            pSecD->listaSimples->pProx = NULL;
            pSecD->pAnt = pAuxD;
            pAuxD->pProx = pSecD;

            inserir_Palavra(pSecD->listaSimples,palavra, descricao);
            pSecD->quantidade++;
        }else{                              //se a letra exitir, add no nÃ³ existente
            inserir_Palavra(verificar(letra), palavra, descricao);

			ListaDupla *auxD = listaInicio_dupla.pProx;

			while(auxD != NULL){
    			if(auxD->letra == letra){
        			auxD->quantidade++;
        			break;
    			}

    		auxD = auxD->pProx;
			}
        }
    }
    limparTela();
    cout << "Profissao inserida!\n";
    system("pause");
}

void listar(){
    limparTela();
    pAuxD = listaInicio_dupla.pProx;
    int i = 0;

    gotoxy(15,5);
	cout << "LISTA DE PROFISSOES" << endl;

    if(pAuxD == NULL){
            gotoxy(15,7);
            cout << "LISTA VAZIA..." << endl;
    }

    while(pAuxD != NULL){
        gotoxy(10,7 + i);
        cout <<  pAuxD->letra  << " (" << pAuxD->quantidade << ")" << endl;

        if(pAuxD->listaSimples == NULL){
            gotoxy(15,7);
            cout << "LISTA VAZIA" << endl;
            cin.get();
        }
        else{
            pAuxS = pAuxD->listaSimples->pProx;
            while(pAuxS != NULL){
                gotoxy(10,8 + i);
                cout <<  pAuxS->palavra << " - " << pAuxS->descricao << endl;
                pAuxS = pAuxS->pProx;
                i++;
            }
        }
        pAuxD = pAuxD->pProx;
        i = i + 2;
    }
    cin.get();
    cin.ignore();
}
void editar(){
    cin.get();
    char palavra[50];
    cout << "Digite a profissao: ";
    cin.getline(palavra, 50);
    maiusculo(palavra);
    pAuxS = existeProfissao(palavra);
    if(pAuxS == NULL){
        cout << "Essa palavra nao exite no dicionario " << endl;

        cin.get();
        return;
    }
    cout << "Digite a profissao: ";
    cin.getline(palavra, 50);
    maiusculo(palavra);
    cout << palavra << endl;

    if(palavra[0] != pAuxS->palavra[0]){
        cout << "essa palavra nao faz parte dessa letra";
        cin.get();

    }else{
        strcpy(pAuxS->palavra, palavra);
        cout << "Edicao concluida ";
    }

}
void remover(){

    cin.get();

    char palavra[50];

    cout << "Digite a profissao para excluir: ";
    cin.getline(palavra, 50);

    maiusculo(palavra);

    ListaSimples *lista = verificar(palavra[0]);

    if(lista == NULL){
        cout << "Essa palavra nao existe no dicionario, ou esta incorreta..." << endl;
        cin.get();
        return;
    }

    pAuxS = lista->pProx;
    ListaSimples *aux = NULL;

    while(pAuxS != NULL){

        if(strcmp(palavra, pAuxS->palavra) == 0){
            break;
        }

        aux = pAuxS;
        pAuxS = pAuxS->pProx;
    }

    if(pAuxS == NULL){
        cout << "Essa palavra nao existe no dicionario, ou esta incorreta..." << endl;
        cin.get();
        return;
    }

    if(aux == NULL){
        lista->pProx = pAuxS->pProx;
    }
    else{
        aux->pProx = pAuxS->pProx;
    }

    delete pAuxS;

    pAuxD = listaInicio_dupla.pProx;

    while(pAuxD != NULL){

        if(pAuxD->letra == palavra[0]){

            pAuxD->quantidade--;

            if(pAuxD->quantidade == 0){

                pAuxD->pAnt->pProx = pAuxD->pProx;

                if(pAuxD->pProx != NULL){
                    pAuxD->pProx->pAnt = pAuxD->pAnt;
                }

                delete pAuxD->listaSimples;
                delete pAuxD;
            }

            break;
        }

        pAuxD = pAuxD->pProx;
    }

    cout << "Profissao removida com sucesso!" << endl;
    cin.get();
}

void pesquisar_Palavra(){
    char palavra[50];

    limparTela();

    cout << "Digite a profissao que deseja pesquisar: ";
    cin.ignore();
    cin.getline(palavra, 50);
	limparTela();

    maiusculo(palavra);
    char letra = toupper(palavra[0]);

    //procura a lista simples correspondente a essa letra
    ListaSimples *lista = verificar(letra);

    //se nao existir lista para essa letra, a palavra nao existe
    if(lista == NULL){
        cout << "Profissao nao encontrada!\n";
        system("pause");
        return;
    }

    //primeiro no valido da lista simples
    ListaSimples *aux = lista->pProx;

    //percorre a lista simples procurando a palavra
    while(aux != NULL){


        if(strcmp(aux->palavra, palavra) == 0){
            cout << "\nProfissao encontrada!\n";
            cout << "Nome: " << aux->palavra << endl;
            cout << "Descricao: " << aux->descricao << endl;

            system("pause");
            return;
        }

        aux = aux->pProx;
    }

    //se percorreu tudo e nao encontrou
    cout << "Profissao nao encontrada!\n";
    system("pause");
}

//funçoes que possibilitarao a ordenacao de dicionario
void ordenarLetras(){

    bool trocou; // variavel que controla se houve troca durante a passagem

    do{

        trocou = false; // começo com nenhuma troca feita

        ListaDupla *atual = listaInicio_dupla.pProx; // começo no primeiro nó válido

        while(atual != NULL && atual->pProx != NULL){ //enquanto existir nó atual e próximo nó para comparar

            // comparo a letra atual com a próxima letra
            if(atual->letra > atual->pProx->letra){

                // variáveis temporárias para realizar a troca
                char letraTemp = atual->letra;
                int qtdTemp = atual->quantidade;
                ListaSimples *listaTemp = atual->listaSimples;

                // copio os dados do próximo nó para o atual
                atual->letra = atual->pProx->letra;
                atual->quantidade = atual->pProx->quantidade;
                atual->listaSimples = atual->pProx->listaSimples;

                // copio os dados temporários para o próximo nó
                atual->pProx->letra = letraTemp;
                atual->pProx->quantidade = qtdTemp;
                atual->pProx->listaSimples = listaTemp;

                trocou = true; // informo que houve troca nesta passagem
            }

            atual = atual->pProx; // avanço para o próximo nó
        }

    }while(trocou); // continuo enquanto ainda existirem trocas
}

void ordenarProfissoes(){

    ListaDupla *letraAtual = listaInicio_dupla.pProx; //começo no primeiro no da lista dupla 

    while(letraAtual != NULL){ //enquanto houver nodos da lista dupla 

        bool trocou; //variavel que contra a troca 

        do{

            trocou = false; //começo com nenhuma troca 

            ListaSimples *atual = letraAtual->listaSimples->pProx; //começo no primeiro nó da lista simples contida na lista dupla 

            while(atual != NULL && atual->pProx != NULL){ //enquanto houverem palavras para comparar 

                if(strcmp(atual->palavra, atual->pProx->palavra) > 0){ //se for maior que 0, a string atual é maior que a proxima 

                    char palavraTemp[50];
                    char descricaoTemp[200];

                    strcpy(palavraTemp, atual->palavra);
                    strcpy(descricaoTemp, atual->descricao);

                    strcpy(atual->palavra, atual->pProx->palavra);
                    strcpy(atual->descricao, atual->pProx->descricao);

                    strcpy(atual->pProx->palavra, palavraTemp);
                    strcpy(atual->pProx->descricao, descricaoTemp); //passo a atual para tem, a proxima para atual e a temp para a proxima 

                    trocou = true; //indico que houve troca 
                }

                atual = atual->pProx; //passo para a proxima palavra 
            }

        }while(trocou); //enquanto houverem trocas 

        letraAtual = letraAtual->pProx; //passo para o proximo nó da lista dupla 
    }
}

int main(){

int opcao;
listaInicio_dupla.pProx = NULL;
listaInicio_dupla.pAnt = NULL;
listaInicio_dupla.listaSimples = NULL;
listaInicio_dupla.quantidade = 0;

carregarArquivo();

do {
	limparTela();
	gotoxy(15,5);
	cout << "MENU DICIONARIO DE PROFISSOES" << endl;
    gotoxy(10,7);
    cout << "1 - Inserir profissao" << endl;
    gotoxy(10,8);
    cout << "2 - Listar profissoes" << endl;
    gotoxy(10,9);
    cout << "3 - Pesquisar profissao por palavra" << endl;
    gotoxy(10,10);
    cout << "4 - Remover profissao" << endl;
    gotoxy(10,11);
    cout << "5 - Editar profissao" << endl;
    gotoxy(10,12);
    cout << "6 - Salvar arquivo" << endl;
    gotoxy(10,13);
    cout << "7 - Ordenar dicionario";
    gotoxy(10,14);
	cout << "0 - Sair";
    gotoxy(10,15);
    cout << "Opcao: ";
    gotoxy(17,15);
    cin >> opcao;
    switch(opcao){
        case 1: inserir_Letra();     break;
        case 2: listar();            break;
        case 3: pesquisar_Palavra(); break;
        case 4: remover();           break;
        case 5: editar();            break;
        case 6: salvarArquivo();     break;
		case 7: ordenarLetras(); ordenarProfissoes(); cout << "Dicionario ordenado!"; system("pause"); break;
        case 0: salvarArquivo(); return 0;           
        default: cout << "Informe uma opcao valida!"; system("pause"); break;
    }


} while(opcao != 0);

}

