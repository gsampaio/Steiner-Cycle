# Heurística
Para resolver o problema utilizei a meta heurística Grasp utilizando processamento paralelo para abrir varias instancias ao mesmo tempo, então a base do algoritmo foi:

Enquanto temos tempo:
1. Criamos uma solução randomica de forma construtiva.
2. Realizamos uma busca local sobre esta solução tentando melhora-la
3. Comparamos com a melhor solução que já encontramos

## Solução randomica construtiva 

Criamos uma solução randomica de duas maneiras que descrevo abaixo. A primeira usa apenas os nós terminais e é chamada de **randomSolution()** enquanto a segunda utiliza de um fator para adicionar vertices não terminais que se chama **randomSolutionWithNoise()**.

**randomSolution()**: Criamos uma solução aleatória pegando todos os vértices terminais do problema e arranjando eles de forma aleatória. Como sempre temos um grafo completo, sempre teremos uma solução valida para este caso. 

**randomSolutionWithNoise()**: Esta implementação é bem parecida com a implementação do **randomSolution()** porém ele só muda com o fator de adicionar vértices não terminais na solução com intuito de aumentar a diversidade da solução. Sendo assim utilizo um fator randomico para verificar se um numero é menor que um limitante (no caso 10%) para adicionar o vértice não terminal a solução. 
 
## Busca local

Na fase de busca local tentamos melhorar o resultado da solução tentando convergir para mínimos locais. Para isso utilizamos o seguinte principio gulosos.

Para reduzir o valor máximo da solução partimos do principio de que podemos remover uma aresta de peso alto da solução e achar um caminho mínimo entre estes dois vértices que tenha valor menor que a resta que liga a eles. A imagem abaixo ilustra bem o  principio guloso adotado neste algoritmo.

A figura acima mostra um exemplo onde podemos melhorar o valor da solução removendo a aresta de maior peso e procurando um caminho de peso menor para substitui-lo. Assim podemos remover a aresta 1->2 e substituir pelo caminho 1->3->2 reduzindo o valor de 200 para 20. 

*descrição da image: A aresta 0-> 1 possivelmente faça parte da solução inicial porém trocando esta aresta pelo caminho 0->3->1 terminamos uma solução de custo menor*

Executamos esta interação ate que tenhamos passado por um grande numero de aresta ou limitado por um inteiro para não perder muito tempo.

## Execução

Para executar o programa subimos o numero de instancias igual a quantidade de core's disponíveis. No caso se temos um computador com um processador de 2 núcleos utilizamos duas instancias do Grasp rodando simultaneamente. Uma utilizando **randomSolution()** e a outra usando **randomSolutionWithNoise()**.  Para tirar proveito de todo o recurso da maquina.

# Programa

O código consiste basicamente na implementação da classe Grasp contida no arquivo **GRASP.cpp**. Neste arquivo se encontram todos os métodos para gerar as soluções aleatórias e fazer a busca local. 

Para a comunicação com a classe **RA073177** foi criada um classe, que segue o design pattern Delegate, chamanda GraspDelegate. A classe RA073177 herda esta classe com o intuito de capturar novas soluções assim que elas chegam. 

A classe Grasp contem um GraspDelegate, e toda vez que ela encontra uma solução para o problema ela retorna para a classe RA073177 que verifica se a solução encontrada é melhor do que a anterior. Neste modelo a class Grasp só tem a responsabilidade de achar novas soluções enquanto fica a cargo da classe RA073177 em manter a melhor solução de todas as instancias do problema que estão rodando. 

O código fonte do programa pode ser encontrado na integra no repositório do github[1] utilizado. Ao baixar-lo você encontrara um arquivo   

# Resultados
Devido a pouca familiaridade com a linguagem C++ e diversos a implementação não consegue rodar executar todos os casos de testes. Eventualmente algumas instancias não conseguem executar até o fim resultando em segmentation faults não resolvidos. Parte destas instancias foi testada com o valgrind rodando, o que curiosamente resultava numa execução mais demorada (devido fatores de debug/memcheck) porém sem erro. 

Abaixo eu listo o resultado de algumas das instancias:

AC_1000.scp 1486 10min
AC_200.scp  290 10 min
IC_1000.scp 500 10min
tsp_kroA200.scp 296341 10min



# Conclusão

Embora não tenha acesso aos valores exatos para obter uma melhor comparação podemos perceber claramente que o algoritmo funciona muito mal quando executa instancias do tsp uma vez que ele remove as piores arestas e procura por caminhos alternativos. No caso do tsp ele sempre irá adicionar a mesma aresta novamente ao grafo, fazendo com que a solução não mude e assim criando apenas soluções aleatórias. 

Em contra partida o algoritmo tende a funcionar melhor em grafos onde existe um bom balanceamento entre vértices terminais e não terminais, uma vez que ele tenta sempre melhorar as piores arestas/caminhos.  

