#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Comprimento máximo que um token pode ter
#define COMPRIMENTO_MAX_TOKEN 100

// Tipos de tokens possíveis
typedef enum
{
    nulo = 1,
    identificador,
    numero,
    mais,
    menos,
    vezes,
    divisao,
    impar,
    igual,
    diferente,
    menor,
    menor_igual,
    maior,
    maior_igual,
    parentese_esq,
    parentese_dir,
    virgula,
    ponto_virgula,
    ponto,
    atribuicao,
    iniciosimbolo,
    finsimbolo,
    sesimbolo,
    entaosimbolo,
    enquantosimbolo,
    facasimbolo,
    chamadasimbolo,
    constsimbolo,
    varsimbolo,
    procsimbolo,
    comentario,
    desconhecido,
    erro_lexico
} tipo_token;

// Função que retorna o nome do tipo de token para impressão
const char *obterNomeTipoToken(tipo_token tipo)
{
    // String correspondente a cada tipo de token
    switch (tipo)
    {
    case constsimbolo:
        return "CONST";
    case varsimbolo:
        return "VAR";
    case procsimbolo:
        return "PROCEDURE";
    case chamadasimbolo:
        return "CALL";
    case iniciosimbolo:
        return "BEGIN";
    case finsimbolo:
        return "END";
    case sesimbolo:
        return "IF";
    case entaosimbolo:
        return "THEN";
    case enquantosimbolo:
        return "WHILE";
    case facasimbolo:
        return "DO";
    case impar:
        return "ODD";
    case comentario:
        return "comentario";
    case identificador:
        return "ident";
    case numero:
        return "numero";
    case mais:
        return "simbolo_mais";
    case menos:
        return "simbolo_menos";
    case vezes:
        return "simbolo_multiplicacao";
    case divisao:
        return "simbolo_divisao";
    case igual:
        return "simbolo_igual";
    case diferente:
        return "simbolo_diferente";
    case menor:
        return "simbolo_menor";
    case menor_igual:
        return "simbolo_menor_igual";
    case maior:
        return "simbolo_maior";
    case maior_igual:
        return "simbolo_maior_igual";
    case ponto_virgula:
        return "simbolo_ponto_virgula";
    case ponto:
        return "simbolo_ponto";
    case atribuicao:
        return "simbolo_atribuicao";
    case erro_lexico:
        return "<ERRO_LEXICO>";
    default:
        return "<ERRO_LEXICO>";
    }
}

// Verifica se uma palavra é reservada e retorna seu tipo
tipo_token verificarPalavraReservada(char *palavra)
{
    if (strcmp(palavra, "CONST") == 0)
        return constsimbolo;
    if (strcmp(palavra, "VAR") == 0)
        return varsimbolo;
    if (strcmp(palavra, "PROCEDURE") == 0)
        return procsimbolo;
    if (strcmp(palavra, "CALL") == 0)
        return chamadasimbolo;
    if (strcmp(palavra, "BEGIN") == 0)
        return iniciosimbolo;
    if (strcmp(palavra, "END") == 0)
        return finsimbolo;
    if (strcmp(palavra, "IF") == 0)
        return sesimbolo;
    if (strcmp(palavra, "THEN") == 0)
        return entaosimbolo;
    if (strcmp(palavra, "WHILE") == 0)
        return enquantosimbolo;
    if (strcmp(palavra, "DO") == 0)
        return facasimbolo;
    if (strcmp(palavra, "ODD") == 0)
        return impar;
    return identificador; // "identificador" se não for uma palavra reservada
}

// Obtenção e processamento de tokens
void obterToken(FILE *entrada, FILE *saida)
{
    char caractereAtual, proximoCaractere;
    char token[COMPRIMENTO_MAX_TOKEN];
    int indiceToken = 0;
    tipo_token tipo;

    // Leitura do input até chegar no fim do arquivo
    while ((caractereAtual = fgetc(entrada)) != EOF)
    {
        // Identifica comentário
        if (caractereAtual == '{')
        {
            // Ignorar comentário
            while ((caractereAtual = fgetc(entrada)) != '}' && caractereAtual != EOF)
            {
                // Continuar lendo até encontrar o fim do comentário ou o fim do arquivo
            }
            if (caractereAtual == EOF)
            {
                fprintf(saida, "Erro: fim de arquivo inesperado dentro de um comentário\n");
                return;
            }
            continue;
        }

        // Ignora espaços
        if (isspace(caractereAtual))
        {
            continue;
        }

        // Ignora vírgulas
        if (caractereAtual == ',')
        {
            continue;
        }

        // Identificação e processamento de caracteres alfabéticos
        if (isalpha(caractereAtual))
        {
            token[indiceToken++] = caractereAtual;
            while (isalnum((caractereAtual = fgetc(entrada))))
            {
                token[indiceToken++] = caractereAtual;
            }
            ungetc(caractereAtual, entrada);
            token[indiceToken] = '\0';
            indiceToken = 0;
            tipo = verificarPalavraReservada(token); // Verifica se a palavra é reservada
            fprintf(saida, "%s,%s\n", token, obterNomeTipoToken(tipo));
            continue;
        }

        // Processa tokens numéricos
        if (isdigit(caractereAtual))
        {
            token[indiceToken++] = caractereAtual;
            while (isdigit((caractereAtual = fgetc(entrada))))
            {
                token[indiceToken++] = caractereAtual;
            }
            ungetc(caractereAtual, entrada);
            token[indiceToken] = '\0';
            indiceToken = 0;
            fprintf(saida, "%s,%s\n", token, obterNomeTipoToken(numero));
            continue;
        }

        // Processa tokens que são símbolos (virgula, operadores, barra...)
        switch (caractereAtual)
        {
        case ';':
            fprintf(saida, "%c,%s\n", caractereAtual, obterNomeTipoToken(ponto_virgula));
            break;

        case '.':
            fprintf(saida, "%c,%s\n", caractereAtual, obterNomeTipoToken(ponto));
            break;

        case ':':
            // Verifica o próximo caractere para determinar se forma o token de atribuição
            if ((proximoCaractere = fgetc(entrada)) == '=')
            {
                fprintf(saida, ":=,%s\n", obterNomeTipoToken(atribuicao));
            }
            else
            {
                ungetc(proximoCaractere, entrada);
                fprintf(saida, "%c,%s\n", caractereAtual, obterNomeTipoToken(desconhecido));
            }
            break;

        case '+':
        case '-':
        case '*':
        case '/':
            // Grava token de operação matemática (mais, menos, vezes, divisão)
            fprintf(saida, "%c,%s\n", caractereAtual, obterNomeTipoToken(caractereAtual == '+' ? mais : (caractereAtual == '-' ? menos : (caractereAtual == '*' ? vezes : divisao))));
            break;

        case '=': // Igualdade (não é atribuição)
            fprintf(saida, "%c,%s\n", caractereAtual, obterNomeTipoToken(igual));
            break;

        case '<':
            // Verifica o próximo caractere para checar se é '<>', '<=' ou '<'
            proximoCaractere = fgetc(entrada);
            if (proximoCaractere == '>')
            {
                fprintf(saida, "<>,%s\n", obterNomeTipoToken(diferente));
            }
            else if (proximoCaractere == '=')
            {
                fprintf(saida, "<=,%s\n", obterNomeTipoToken(menor_igual));
            }
            else
            {
                ungetc(proximoCaractere, entrada);
                fprintf(saida, "%c,%s\n", caractereAtual, obterNomeTipoToken(menor));
            }
            break;

        case '>':
            // Verifica o próximo caractere para checar se é '<>', '>=' ou '>'
            proximoCaractere = fgetc(entrada);
            if (proximoCaractere == '=')
            {
                fprintf(saida, ">=,%s\n", obterNomeTipoToken(maior_igual));
            }
            else
            {
                ungetc(proximoCaractere, entrada);
                fprintf(saida, "%c,%s\n", caractereAtual, obterNomeTipoToken(maior));
            }
            break;

        default:
            // Grava como erro léxico outros simbolos não identificados
            fprintf(saida, "%c,%s\n", caractereAtual, obterNomeTipoToken(erro_lexico));
        }
    }
}

int main(int argc, char *argv[])
{
    // Verifica se a entrada está de acordo com o padrão
    if (argc != 3)
    {
        fprintf(stderr, "Uso: %s <arquivo de entrada> <arquivo de saída>\n", argv[0]);
        return 1;
    }

    // Abre arquivos de entrada (modo read) e saída (modo write)
    FILE *arquivoEntrada = fopen(argv[1], "r");
    if (!arquivoEntrada)
    {
        perror("Erro ao abrir arquivo de entrada");
        return 1;
    }

    FILE *arquivoSaida = fopen(argv[2], "w");
    if (!arquivoSaida)
    {
        perror("Erro ao abrir arquivo de saída");
        fclose(arquivoEntrada);
        return 1;
    }

    obterToken(arquivoEntrada, arquivoSaida);

    // Fechamento dos arquivos
    fclose(arquivoEntrada);
    fclose(arquivoSaida);
    return 0;
}

// suprimir comentario
// colocar fim de arquivo inesperado