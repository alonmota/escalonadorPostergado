##########################   SOBRE   #################################################
CIC/UnB - 30/05/2018
Sistemas Operacionais
Prof. Dra Alba Cristina Melo
Escalonador Postergado

Autor: 
    Alon Mota - 130005002
Coautor:
    Rodrigo de Sousa Saldanha - 110139143

Este software tem como objetivo gerenciar a execução de processos de forma postergada,
com politica de Round-Robin com quantum de 5 segundos;
##########################################################################################




########################   EXECUÇÃO   ######################################################
para compilar utilize
    make build

para rodar suba primeiro o escalonador
    ./escalonador &    -> controla toda a execução

em seguida podem ser chamados quaisquer dos programas abaixo
    -executa_postergado <hh:mm> <copias> <pri> <arquivo>     -> Coloca o programa na fila de espera
    -remove_postergado <idJob>      -> Remove um programa da fila de espera
    -lista_postergado               -> Lista os programas em espera
    -shutdown_postergado            -> Encerra o escalonador
##########################################################################################


########################   COMPILADOR   ###################################################
Thread model: posix
gcc version 4.8.4 (Ubuntu 4.8.4-2ubuntu1~14.04.4)
##########################################################################################


##################   DETALHES DA IMPLEMENTAÇÃO  ##########################################
Escalonador:
    O escalonador consiste de um programa que invoca um programa filho com um fork,
    enquanto o pai fica em um loop recebendo os programas para executar e controlando a execução
    o filho fica em um loop recebendo e controlando programas para a fila de espera.

    Para comunicação com ele e os outros processos foram utilizadas filas de mensagens, de 3 tipos,
    o primeiro para receber mensagens de adição de processos nas filas de espera, o segundo para 
    receber mensagens de remoção e o terceiro para comunicação entre os processos do escalonador
    e controlar programas que devem entrar em execução.

    Para controlar o temporizador foram utilizados os mecanismos de alarm, e signal e spleep. Que trabalhando juntos permitem o controle do tempo de execuçao dos processos.

    Foram declaradas algumas variaveis globais para auxiliar as rotinas de tratamento de sinais,
    e tambem foram declarados semaforos para controlar o acesso a essas variaveis.

    Todas as impressoes realizadas durante a execução sao feitas pelo escalonador.

    E feito um tratamento de zumbies com waitpid enquanto o programa esta em execuçao

Executa Postergado:
    No executa postergado foram feitas alguns tratamentos dos inputs para melhor aloca-los nas
    estruturas de dados utilizadas, e enviado uma mensagen para o escalonador.

Remove Postergado:
    Logica semelante a executa postergado, so q o tipo de mensagem muda, e o escalonador trata 
    removendo os processos enviados.

Lista Postegado:
    mesmo que remove postergado so que envia um id -1, portanto e feita apenas uma listagem
    da lista de espera

Shutdown postegado:
    E feita uma chamada de systema para encerrar os processos 'escalonador', nao e feito o 
    tratamento dos programas em execuçao.

    Não e terminada a fila de mensagens;

    Nenhum tratamento de semaforos ou avisos de processos ainda executando
##########################################################################################


####################   OUTRAS INFORMAÇÕES   ##############################################
O programa teste foi usado nos testes do escalonador, basicamente contem um sleep e um printf

O 'make build' do makefile compila todos os arquivos cpp contidos no diretorio q se encontra 
para um executavel de mesmo nome

O 'make clear' do makefile remove todos os arquivos executaveis do diretorio em que se encontra

Todas as estruturas de dados criadas estao no arquivo dataTypes.h

Para gerencia das listas foi usado o pacote list da biblioteca do c++
##########################################################################################



########################### Copyright ####################################################
MIT License

Copyright (c) [2018] [AlonMota]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
##########################################################################################
