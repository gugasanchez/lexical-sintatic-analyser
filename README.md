# Analisador Léxico-Sintático para a Linguagem PL/0

Este projeto consiste em um analisador léxico-sintático para a linguagem PL/0, uma linguagem de programação simples usada frequentemente para ensino de compiladores. O analisador léxico converte o código-fonte em uma sequência de tokens, enquanto o analisador sintático verifica se a sequência de tokens segue a gramática da linguagem PL/0.

## Estrutura do Projeto

O projeto está organizado da seguinte forma:

- **code/**: Contém o código-fonte do analisador léxico-sintático.

  - `main.c`: Arquivo principal com a implementação do analisador léxico-sintático.
  - `main`: Executável gerado após a compilação.

- **docs/**: Contém documentação relacionada ao projeto.

  - `Gramática de PL0.pdf`: Descrição da gramática da linguagem PL/0.
  - `Relatório 1.pdf`: Primeiro relatório do projeto.
  - `Trabalho 2 - especificação.pdf`: Especificação do segundo trabalho.

- **tests/**: Contém arquivos de teste.

  - `input.txt`: Arquivo de entrada com o código-fonte a ser analisado.
  - `output.txt`: Arquivo de saída com os tokens gerados pelo analisador léxico.
  - `error.txt`: Arquivo de saída com os erros encontrados durante a análise.

- **Makefile**: Script para compilar e executar o projeto.

## Instruções para Compilação e Execução

### Compilação

Para compilar o projeto, utilize o comando `make` na raiz do diretório do projeto. Isso irá compilar o arquivo `main.c` e gerar o executável `main` no diretório `code`.

```sh
make
```

Após a compilação, você verá a seguinte mensagem indicando que a compilação foi concluída com sucesso:

```
Compilação concluída com sucesso.
Para executar, utilize: make run input.txt output.txt error.txt
O arquivo input.txt deve estar no mesmo diretório que o Makefile.
```

### Execução

Para executar o analisador léxico-sintático, utilize o comando `make run` seguido dos nomes dos arquivos de entrada, saída e erro.

```sh
make run INPUT=input.txt OUTPUT=output.txt ERROR=error.txt
```

Certifique-se de que o arquivo `input.txt` esteja no mesmo diretório que o `Makefile`. O analisador irá processar o arquivo de entrada, gerando tokens no arquivo de saída e registrando quaisquer erros no arquivo de erro.

## Arquivos de Teste

O diretório `tests` contém arquivos de teste adicionais que podem ser usados para validar o funcionamento do analisador. Para testar com esses arquivos, basta alterar os parâmetros do comando `make run` conforme necessário.

### Exemplo de Teste

Para testar o analisador com os arquivos `teste1.txt`, `teste1out.txt` e `teste1error.txt`, utilize:

```sh
make run INPUT=tests/teste1.txt OUTPUT=tests/teste1out.txt ERROR=tests/teste1error.txt
```

Isso permitirá verificar se o analisador está funcionando corretamente com diferentes conjuntos de dados de entrada.

## Conclusão

Este projeto fornece uma implementação básica de um analisador léxico-sintático para a linguagem PL/0, útil para fins educacionais e como base para projetos de compiladores mais avançados. A estrutura modular do código e a utilização do Makefile facilitam a compilação e execução, permitindo a fácil validação com diferentes arquivos de teste.
