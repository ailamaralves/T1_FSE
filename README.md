# Trabalho 1 - Fundamentos de Sistemas Embarcados

## Sobre

Este trabalho tem como objetivo desenvolver o software que efetua o ontrole de temperatura do forno utilizando dois atuadores para este controle: um resistor de potência de 15 Watts utilizado para aumentar temperatura e; uma ventoinha que puxa o ar externo (temperatura ambiente) para reduzir a temperatura do sistema.

Mais especificações em: [Trabalho 1 da disciplina de Fundamentos de Sistemas Embarcados (2021/2)](ttps://gitlab.com/fse_fga/trabalhos-2021_2/trabalho-1-2021-2)

## Intruções
Para rodar o software, após clonar o repositório basta executar na raiz do diretório:

```
$ make
```

Em seguida:

```
$ ./bin/bin
```

Caso queira finalizar:

```
$ Ctrl + c
```

## Programa

A execução do programa pode ser feito de dois modos: 
- Pelo terminal escolhendo a temperatura referencial;
- Através de um Potenciômetro externo.