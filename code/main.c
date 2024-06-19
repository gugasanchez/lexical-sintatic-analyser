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
    quebralinha,
    desconhecido,
    erro_lexico,
} tipo_token;

// Funções protótipos
const char *obterNomeTipoToken(tipo_token tipo);
tipo_token verificarPalavraReservada(char *palavra);
tipo_token obterToken(FILE *entrada, FILE *saida, FILE *saidaErro);
void obterProximoToken();
void erro(char *mensagem);
void casaToken(tipo_token esperado);
void programa();
void bloco();
void declaracao();
void constante();
void mais_const();
void variavel();
void mais_var();
void procedimento();
void comando();
void mais_cmd();
void expressao();
void operador_unario();
void termo();
void mais_termos();
void fator();
void mais_fatores();
void condicao();
void relacional();

// Variáveis globais
tipo_token tokenAtual;
FILE *arquivoEntrada, *arquivoSaida, *arquivoSaidaErro;
int linhaAtual = 1;

//////////////////////////////////////////////////////// ANÁLISE LÉXICA ////////////////////////////////////////////////////////

// Função que retorna o nome do tipo de token para impressão
const char *obterNomeTipoToken(tipo_token tipo)
{
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
    case virgula:
        return "simbolo_virgula";
    case quebralinha:
        return "quebra_linha";
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
tipo_token obterToken(FILE *entrada, FILE *saida, FILE *saidaErro)
{
    char caractereAtual, proximoCaractere;
    char token[COMPRIMENTO_MAX_TOKEN];
    int indiceToken = 0;
    tipo_token tipo;

    // Leitura do input até chegar no fim do arquivo
    while ((caractereAtual = fgetc(entrada)) != EOF)
    {
        // Rastrear a linha atual
        if (caractereAtual == '\n')
        {
            linhaAtual++;
        }

        // Identifica comentário
        if (caractereAtual == '{')
        {
            // Ignorar comentário
            while ((caractereAtual = fgetc(entrada)) != '}' && caractereAtual != EOF)
            {
                if (caractereAtual == '\n')
                {
                    linhaAtual++;
                }
            }
            if (caractereAtual == EOF)
            {
                fprintf(saida, "%s,fim de arquivo inesperado\n", token);
                fprintf(saidaErro, "Erro léxico na linha %d: fim de arquivo inesperado dentro de um comentário\n", linhaAtual);
                return erro_lexico;
            }
            continue;
        }

        // Ignora espaços
        if (isspace(caractereAtual))
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
            tipo = verificarPalavraReservada(token); // Verifica se a palavra é reservada
            fprintf(saida, "%s,%s\n", token, obterNomeTipoToken(tipo));
            return tipo;
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
            fprintf(saida, "%s,%s\n", token, obterNomeTipoToken(numero));
            return numero;
        }

        // Processa tokens que são símbolos (virgula, operadores, barra...)
        switch (caractereAtual)
        {
        case ';':
            fprintf(saida, "%c,%s\n", caractereAtual, obterNomeTipoToken(ponto_virgula));
            return ponto_virgula;

        case '.':
            fprintf(saida, "%c,%s\n", caractereAtual, obterNomeTipoToken(ponto));
            return ponto;

        case ':':
            if ((proximoCaractere = fgetc(entrada)) == '=')
            {
                fprintf(saida, ":=,%s\n", obterNomeTipoToken(atribuicao));
                return atribuicao;
            }
            else
            {
                ungetc(proximoCaractere, entrada);
                fprintf(saida, "%c,%s\n", caractereAtual, obterNomeTipoToken(desconhecido));
                return desconhecido;
            }

        case '+':
        case '-':
        case '*':
        case '/':
            // Grava token de operação matemática (mais, menos, vezes, divisão)
            tipo = (caractereAtual == '+' ? mais : (caractereAtual == '-' ? menos : (caractereAtual == '*' ? vezes : divisao)));
            fprintf(saida, "%c,%s\n", caractereAtual, obterNomeTipoToken(tipo));
            return tipo;

        case '=': // Igualdade (não é atribuição)
            fprintf(saida, "%c,%s\n", caractereAtual, obterNomeTipoToken(igual));
            return igual;

        case '<':
            // Verifica o próximo caractere para checar se é '<>', '<=' ou '<'
            proximoCaractere = fgetc(entrada);
            if (proximoCaractere == '>')
            {
                fprintf(saida, "<>,%s\n", obterNomeTipoToken(diferente));
                return diferente;
            }
            else if (proximoCaractere == '=')
            {
                fprintf(saida, "<=,%s\n", obterNomeTipoToken(menor_igual));
                return menor_igual;
            }
            else
            {
                ungetc(proximoCaractere, entrada);
                fprintf(saida, "%c,%s\n", caractereAtual, obterNomeTipoToken(menor));
                return menor;
            }

        case '>':
            // Verifica o próximo caractere para checar se é '<>', '>=' ou '>'
            proximoCaractere = fgetc(entrada);
            if (proximoCaractere == '=')
            {
                fprintf(saida, ">=,%s\n", obterNomeTipoToken(maior_igual));
                return maior_igual;
            }
            else
            {
                ungetc(proximoCaractere, entrada);
                fprintf(saida, "%c,%s\n", caractereAtual, obterNomeTipoToken(maior));
                return maior;
            }

        case ',':
            fprintf(saida, "%c,%s\n", caractereAtual, obterNomeTipoToken(virgula));
            return virgula;

        default:
            fprintf(saida, "%c,%s\n", caractereAtual, obterNomeTipoToken(erro_lexico));
            fprintf(saidaErro, "Erro léxico na linha %d: caractere '%c' inválido\n", linhaAtual, caractereAtual);
            return erro_lexico;
        }
    }
    return nulo; // Retorna nulo se atingir o final do arquivo
}

//////////////////////////////////////////////////////// ANÁLISE SINTÁTICA ////////////////////////////////////////////////////////

void obterProximoToken()
{
    tokenAtual = obterToken(arquivoEntrada, arquivoSaida, arquivoSaidaErro);
    printf("aqui: %s\n", obterNomeTipoToken(tokenAtual));
}

void erro(char *mensagem)
{
    fprintf(arquivoSaidaErro, "Erro sintático na linha %d: %s\n", linhaAtual - 1, mensagem);
    while (tokenAtual != ponto_virgula && tokenAtual != ponto && tokenAtual != nulo)
    {
        obterProximoToken();
    }
    if (tokenAtual == ponto_virgula)
    {
        obterProximoToken();
    }
}

void casaToken(tipo_token esperado)
{
    printf("---------------------> ESPERADO: %s\n", obterNomeTipoToken(esperado));
    if (tokenAtual == esperado)
    {
        printf("---------------------> ENCONTRADO: %s\n", obterNomeTipoToken(tokenAtual));
        obterProximoToken();
    }
    else
    {
        char msg[100];

        printf("---------------------> NÃO ESPERADO: %s\n", obterNomeTipoToken(tokenAtual));
        sprintf(msg, "Token '%s' esperado", obterNomeTipoToken(esperado));
        erro(msg);
    }
}

void programa()
{
    bloco();
    casaToken(ponto);
}

void bloco()
{
    declaracao();
    comando();
}

void declaracao()
{
    constante();
    variavel();
    procedimento();
}

void constante()
{
    if (tokenAtual == constsimbolo)
    {
        casaToken(constsimbolo);
        casaToken(identificador);
        casaToken(igual);
        casaToken(numero);
        mais_const();
        casaToken(ponto_virgula);
    }
}

void mais_const()
{
    while (tokenAtual == virgula)
    {
        casaToken(virgula);
        casaToken(identificador);
        casaToken(igual);
        casaToken(numero);
    }
}

void variavel()
{
    if (tokenAtual == varsimbolo)
    {
        casaToken(varsimbolo);
        casaToken(identificador);
        mais_var();
        casaToken(ponto_virgula);
    }
}

void mais_var()
{
    while (tokenAtual == virgula)
    {
        casaToken(virgula);
        casaToken(identificador);
    }
}

void procedimento()
{
    while (tokenAtual == procsimbolo)
    {
        casaToken(procsimbolo);
        casaToken(identificador);
        casaToken(ponto_virgula);
        bloco();
        casaToken(ponto_virgula);
    }
}

void comando()
{
    switch (tokenAtual)
    {
    case identificador:
        casaToken(identificador);
        casaToken(atribuicao);
        expressao();
        break;
    case chamadasimbolo:
        casaToken(chamadasimbolo);
        casaToken(identificador);
        break;
    case iniciosimbolo:
        casaToken(iniciosimbolo);
        comando();
        mais_cmd();
        casaToken(finsimbolo);
        break;
    case sesimbolo:
        casaToken(sesimbolo);
        condicao();
        casaToken(entaosimbolo);
        comando();
        break;
    case enquantosimbolo:
        casaToken(enquantosimbolo);
        condicao();
        casaToken(facasimbolo);
        comando();
        break;
    default:
        erro("Comando inválido");
        break;
    }
}

void mais_cmd()
{
    while (tokenAtual == ponto_virgula)
    {
        casaToken(ponto_virgula);
        comando();
    }
}

void expressao()
{
    operador_unario();
    termo();
    mais_termos();
}

void operador_unario()
{
    if (tokenAtual == mais || tokenAtual == menos)
    {
        casaToken(tokenAtual);
    }
}

void termo()
{
    fator();
    mais_fatores();
}

void mais_termos()
{
    while (tokenAtual == mais || tokenAtual == menos)
    {
        casaToken(tokenAtual);
        termo();
    }
}

void fator()
{
    switch (tokenAtual)
    {
    case identificador:
        casaToken(identificador);
        break;
    case numero:
        casaToken(numero);
        break;
    case parentese_esq:
        casaToken(parentese_esq);
        expressao();
        if (tokenAtual == parentese_dir)
        {
            casaToken(parentese_dir);
        }
        else
        {
            erro("Parêntese direito esperado");
        }
        break;
    case erro_lexico:
        // erro("Erro léxico");
        obterProximoToken(); // Move to the next token to attempt recovery
        break;

    default:
        erro("Fator esperado");
        obterProximoToken(); // Move to the next token to attempt recovery
    }
}

void mais_fatores()
{
    while (tokenAtual == vezes || tokenAtual == divisao)
    {
        casaToken(tokenAtual);
        fator();
    }
}

void condicao()
{
    if (tokenAtual == impar)
    {
        casaToken(impar);
        expressao();
    }
    else
    {
        expressao();
        relacional();
        expressao();
    }
}

void relacional()
{
    switch (tokenAtual)
    {
    case igual:
    case diferente:
    case menor:
    case menor_igual:
    case maior:
    case maior_igual:
        casaToken(tokenAtual);
        break;
    default:
        erro("Operador relacional esperado");
    }
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Uso: %s <arquivo de entrada> <arquivo de saída erro>\n", argv[0]);
        return 1;
    }

    arquivoEntrada = fopen(argv[1], "r");
    if (!arquivoEntrada)
    {
        perror("Erro ao abrir arquivo de entrada");
        return 1;
    }

    arquivoSaida = fopen(argv[2], "w");
    if (!arquivoSaida)
    {
        perror("Erro ao abrir arquivo de saída");
        fclose(arquivoEntrada);
        return 1;
    }

    arquivoSaidaErro = fopen(argv[3], "w");
    if (!arquivoSaidaErro)
    {
        perror("Erro ao abrir arquivo de saída de erro");
        fclose(arquivoEntrada);
        fclose(arquivoSaida);
        return 1;
    }

    obterProximoToken();
    programa();

    // printf("%s", obterNomeTipoToken(tokenAtual));

    if (tokenAtual != ponto)
    {
        erro("Ponto final esperado");
    }

    fclose(arquivoEntrada);
    fclose(arquivoSaida);
    fclose(arquivoSaidaErro);
    return 0;
}
