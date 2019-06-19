#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//Neste trabalho será usado somente as primeiras 10.000 linhas
#define QTD 10000

//A princípio, um buffer de 170 caracteres parece suficiente para conter uma linha a ser processada, todavia, isto deverá ser verificado.
#define MAXCHAR 170


//struct para armazenar os dados das pessoas
typedef struct _pessoa 
{
    int  linha;
    char endereco[50];
    int  chave;
    char datanasc[10];
    char nome[40];
    char email[20];
    char celular[13];
} Pessoa;

//struct para a tabela hash (cada item da tabela hash é um Registro)
typedef struct _registro 
{
    Pessoa pessoa;
    char situacao;
    int proximo;
} Registro;

//struct para ler arquivo binario
typedef struct _tableFile
{
    FILE * fp;
    int header; //tamanho da tabela?
}TableFile;


//função para extrair dados do arquivo
Pessoa parseData(char linha[]){

    Pessoa r;
    char *pt;
    pt = strtok (linha, ",");
    r.linha = atoi(pt); // numero da linha
    pt = strtok (NULL, ",");
    strncpy(r.endereco, pt, 50); // endereco
    pt = strtok (NULL, ",");
    r.chave = atoi(pt); // id
    pt = strtok (NULL, ",");
    strncpy(r.datanasc, pt, 10); // data de nascimento
    pt = strtok (NULL, ",");
    strncpy(r.nome, pt, 40); // nome
    pt = strtok (NULL, ",");
    strncpy(r.email, pt, 20); // email
    pt = strtok (NULL, ",");
    strncpy(r.celular, pt, 13); // celular
    return r;
}

int criaHeader(){
    int header = QTD;
    return header;
}

void leHeader(TableFile * tf){
    rewind(tf->fp);
    fread(tf->header, sizeof(int), 1, tf->fp);
}

void gravaHeader(TableFile * tf){
    rewind(tf->fp);
    fwrite(tf->header, sizeof(int), 1, tf->fp);
}

void gravaTabela(TableFile * tf, Registro * dados){
    leHeader(tf);
    fseek(tf->fp, sizeof(int), SEEK_SET); //move ponteiro apartir do começo do arquivo (pula o header)
    int i = 0;
    while(i < tf->header){
        fwrite(&(dados[i].pessoa), sizeof(Registro), 1, tf->fp);
        printf("Gravado registro %d - %s", dados[i].pessoa.linha, dados[i].pessoa.nome);
    }
}

int main()
{
    //dados[], uma array de Registros
    Registro dados[QTD];

    //Abaixo segue um exemplo de trecho de código para abrir, ler "todas" as linhas, processa-las e fechar o arquivo:
    
    FILE *fp; // arquivo de texto
    char linha[MAXCHAR];
    char* filename = "dados-500000-win.csv";
    Pessoa r;
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Could not open file %s",filename);
        return;
    }
    fgets(linha, MAXCHAR, fp); // ignorando a 1a linha
    int i = 0;

    //ler linha por linha
    while ((i < QTD) && (fgets(linha, MAXCHAR, fp) != NULL)){
        dados[i].pessoa = parseData(linha);
        printf("%d - Address = %s\n",i , dados[i].pessoa.endereco);
        i++;
    }
    fclose(fp);

    //Abaixo segue um exemplo de abertura de um arquivo binário:
    
    TableFile * tf = malloc(sizeof(TableFile));

    char * header;

    char* arquivo = "tabelaHash1.dat";

    int result = access(arquivo, F_OK); // F_OK testa existencia.

    //Sugestão, quando o arquivo for criado, já crie uma tabela hash para o total de registros a serem armazenados.
    //Defina em seu layout um cabeçalho para te auxiliar a definir quantos registros há na tabela.

    if (result == 0){
    // Se o arquivo existe, abra e leia o header
        printf("result == 0\n");
        tf->fp = fopen(arquivo, "r+b");
        leHeader(tf);
    } else {
    // Se o arquivo nao existe, crie-o, defina um header e escreva no arquivo
        printf("result != 0\n");
        tf->fp = fopen(arquivo, "w+b");        
        tf->header = criaHeader();
        gravaHeader(tf);
    }

    //gravar tabela

    gravaTabela(tf,dados);

    return 0;    
}