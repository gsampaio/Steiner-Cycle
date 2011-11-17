------------------------------------------------------------------------------
Inst�ncias para o Problema do Ciclo de Steiner
(C) Carlos Eduardo de Andrade, Outubro de 2011.
------------------------------------------------------------------------------

Este diret�rio cont�m algumas inst�ncias para o Problema do Ciclo de Steiner.
Grande parte delas foram extra�das da SteinerLib e TSPLib, com pequenas
modifica��es.

-----------------------
Diret�rios
-----------------------

-> simple: inst�ncias simples. Metade dos v�rtices s�o terminais e o subgrafo
    induzido por estes � completo. Al�m disso,   existe um ciclo alternante
    entre terminais e v�rtices de steiner. S�o divididas em:
    - IC: ciclos internos, usando apenas arestas entre terminais;
    - EC: ciclos externos, que n�o usam arestas entre terminais;
    - AC: ciclos alternantes, que usa uam aresta entre terminais seguida de
          de uma aresta entre terminal e v�rtice de steiner;
    - random: arestas de custos aleat�rios.

-> tsp: inst�ncias do problema do caxeiro viajante. Neste caso, o grafo �
    completo e todos os v�rtices s�o terminais;

-> small: inst�ncias de pequeno porte;

-> large: inst�ncias de grande porte.

-----------------------
Scripts
-----------------------

Os scripts contidos neste diret�rio s�o de carater experimental e est�o
incompletos, podendo apresentar bugs. Basicamente s�o scripts geradores e de
transforma��o de inst�ncias.

- steinlib2simple.py: toma uma inst�ncia da SteinerLib e a transforma no formato
    mais simples;

- tsplib2simple.py: toma uma inst�ncia da TSPLib e a transforma no formato
    mais simples. Implementamos apenas a dist�ncia euclidiana e a dist�ncia
    geod�sica.

- instance2dot.py: gera um arquivo no formato do Graphviz para que seja
    plotado. Recomendamos a utiliza��o dos programas "dot", "circo" e "neato"
    para gerar diferentes vizualiza��es do grafo;

- completa_grafo.py: dado uma inst�ncia transformada da SteinerLib
    ou outra qualquer, completa o grafo com arestas de peso MAX * num_vertices
    onde MAX � o peso m�ximo das arestas do grafo original.
