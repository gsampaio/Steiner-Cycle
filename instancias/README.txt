------------------------------------------------------------------------------
Instâncias para o Problema do Ciclo de Steiner
(C) Carlos Eduardo de Andrade, Outubro de 2011.
------------------------------------------------------------------------------

Este diretório contêm algumas instâncias para o Problema do Ciclo de Steiner.
Grande parte delas foram extraídas da SteinerLib e TSPLib, com pequenas
modificações.

-----------------------
Diretórios
-----------------------

-> simple: instâncias simples. Metade dos vértices são terminais e o subgrafo
    induzido por estes é completo. Além disso,   existe um ciclo alternante
    entre terminais e vértices de steiner. São divididas em:
    - IC: ciclos internos, usando apenas arestas entre terminais;
    - EC: ciclos externos, que não usam arestas entre terminais;
    - AC: ciclos alternantes, que usa uam aresta entre terminais seguida de
          de uma aresta entre terminal e vértice de steiner;
    - random: arestas de custos aleatórios.

-> tsp: instâncias do problema do caxeiro viajante. Neste caso, o grafo é
    completo e todos os vértices são terminais;

-> small: instâncias de pequeno porte;

-> large: instâncias de grande porte.

-----------------------
Scripts
-----------------------

Os scripts contidos neste diretório são de carater experimental e estão
incompletos, podendo apresentar bugs. Basicamente são scripts geradores e de
transformação de instâncias.

- steinlib2simple.py: toma uma instância da SteinerLib e a transforma no formato
    mais simples;

- tsplib2simple.py: toma uma instância da TSPLib e a transforma no formato
    mais simples. Implementamos apenas a distância euclidiana e a distância
    geodésica.

- instance2dot.py: gera um arquivo no formato do Graphviz para que seja
    plotado. Recomendamos a utilização dos programas "dot", "circo" e "neato"
    para gerar diferentes vizualizações do grafo;

- completa_grafo.py: dado uma instância transformada da SteinerLib
    ou outra qualquer, completa o grafo com arestas de peso MAX * num_vertices
    onde MAX é o peso máximo das arestas do grafo original.
