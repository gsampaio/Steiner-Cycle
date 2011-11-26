# Heurística
Para resolver o problema utilizei a meta heurística Grasp utilizando processamento paralelo para abrir varias instancias ao mesmo tempo, então a base do algoritmo foi:

Enquanto temos tempo:
1. Criamos uma solução randomica de forma construtiva.
2. Realizamos uma busca local sobre esta solução tentando melhora-la
3. Comparamos com a melhor solução que já encontramos

## Solução randomica construtiva 

Criamos uma solução randomica de três maneiras que descrevo abaixo. A primeira usa apenas os nós terminais e é chamada de **randomSolution()** enquanto a segunda utiliza de um fator aleatório para adicionar vértices não terminais que se chama **randomSolutionWithNoise()**. Já a terceira solução consiste em montar uma solução de forma gulosa chamada de **greedRandomSolution()**.

**randomSolution()**: Criamos uma solução aleatória pegando todos os vértices terminais do problema e arranjando eles de forma aleatória. Como sempre temos um grafo completo, sempre teremos uma solução valida para este caso. 

**randomSolutionWithNoise()**: Esta implementação é bem parecida com a implementação do **randomSolution()** porém ele só muda com o fator de adicionar vértices não terminais na solução com intuito de aumentar a diversidade da solução. Sendo assim utilizo um fator randomico para verificar se um numero é menor que um limitante (no caso 10%) para adicionar o vértice não terminal a solução. 

**greedRandomSolution()**: Por ultimo esta implementação pega um nó terminal de forma aleatória e analisa todos as arestas vizinhas pegando a de menor custo. Para balancear a escolha  pegamos dividimos o valor do peso das arestas que levam a nós terminais por 2 para aumentar a chances delas serem escolhidas convergindo mais rápido para a solução. 

## Busca local

Na fase de busca local tentamos melhorar o resultado da solução tentando convergir para mínimos locais. Para isso utilizamos o seguinte principio gulosos.

Para reduzir o valor máximo da solução partimos do principio de que podemos remover uma aresta de peso alto da solução e achar um caminho mínimo entre estes dois vértices que tenha valor menor que a resta que liga a eles. A imagem abaixo ilustra bem o  principio guloso adotado neste algoritmo.


A figura acima mostra um exemplo onde podemos melhorar o valor da solução removendo a aresta de maior peso e procurando um caminho de peso menor para substitui-lo. Assim podemos remover a aresta 1->2 e substituir pelo caminho 1->3->2 reduzindo o valor de 200 para 20. 

*descrição da image: A aresta 0-> 1 possivelmente faça parte da solução inicial porém trocando esta aresta pelo caminho 0->3->1 terminamos uma solução de custo menor*

Executamos esta interação ate que tenhamos passado por um grande numero de aresta ou limitado por um inteiro para não perder muito tempo.

No caso do grafo ser do tipo TSP, onde todos os vértices são terminais a busca local foi substituída pela heurística 2-OPT para garantir que haverá valores melhores com o passar do tempo e não apenas uma variação aleatória. 

## Execução

Para executar o programa subimos o numero de instancias igual a quantidade de core's disponíveis. No caso se temos um computador com um processador de 4 núcleos utilizamos quatro instancias do Grasp rodando simultaneamente. Duas utilizando **randomSolution()** uma usando **randomSolutionWithNoise()** e outra usando **greedRandomSolution()**.  Para tirar proveito de todo o recurso da maquina.

# Programa

O código consiste basicamente na implementação da classe Grasp contida no arquivo **GRASP.cpp**. Neste arquivo se encontram todos os métodos para gerar as soluções aleatórias e fazer a busca local. 

Eu criei uma outra classe chamada GreedyHeuristic (contida no arquivo GreedHeuristic.cpp) que se baseia em pegar um vértice terminal de forma aleatória e procurar o caminho mínimo para todos os outros vértices terminais (ocultando os vértices da solução e os outros vértices terminais), adiciona o menor caminho a solução fazendo isso até fechar o ciclo. Porém sua execução foi retirada do trabalho por levar tempo proporcional a O(V^4) levando tempo não praticável para as instancias de 1000 vértices. 

O código fonte do programa pode ser encontrado na integra no repositório do github[1] utilizado. Ao baixai-lo você encontrara um arquivo arquivo run.sh que se encarrega de compilar e executar para todas as instancias dada problema. 

# Resultados
Devido a pouca familiaridade com a linguagem C++ e diversos a implementação não consegue rodar executar todos os casos de testes. Eventualmente algumas instancias não conseguem executar até o fim resultando em segmentation faults não resolvidos. Parte destas instancias foi testada com o valgrind rodando, o que curiosamente resultava numa execução mais demorada (devido fatores de debug/memcheck) porém sem erro. 

Os resultados abaixo foram rodados por até no máximo 20 minutos numa maquina Core 2 Duo de 2.2GHZ e 8gb de RAM com duas instancias da classe Grasp rodando simultaneamente.
 

Abaixo eu listo o resultado de algumas das instancias:

AC_1000.scp 1486 10min
AC_200.scp  290 10 min
IC_1000.scp 500 10min
tsp_kroA200.scp 296341 10min



# Conclusão

Embora não tenha acesso aos valores exatos para obter uma melhor comparação podemos perceber claramente que o algoritmo funciona muito mal quando executa instancias do tsp mesmo com a implementação do 2opt, com isso levo a acreditar que o opt esta implementado de maneira ineficiente, tanto por levar mais tempo por executar tanto por apresentar uma implementação possivelmente 

Em contra partida o algoritmo tende a funcionar melhor em grafos onde existe um bom balanceamento entre vértices terminais e não terminais, uma vez que ele tenta sempre melhorar as piores arestas/caminhos. 

# Referencia Bibliográfica 

