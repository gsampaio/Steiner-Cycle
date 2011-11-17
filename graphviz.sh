# graphviz.sh
# Progama que pega os arquivos de entrada para o problema de ciclo de steiner
# e gera uma imagem com os vertices, arestas, terminais e custos.
#
# Usage: ./graphviz.sh file.scp
#
# Atenção:
#	Não se esqueça de dar permissão de execução para o script: chmod u+x graphviz.sh
#	É necessario ter instalado o graphviz
#	É necessario ter o visualizador de imagens eog. (Já vem por padrão no ubuntu)
#
# Andre de Andrade Bicudo

#! /bin/bash



config_line='([[:digit:]]+)\ ([[:digit:]]+)\ ([[:digit:]]+)$'
edge='([[:digit:]]+)\ ([[:digit:]]+)\ ([[:digit:]]+\.[[:digit:]]+)$'
terminal='^[[:digit:]]+$'

graphVizFileName=$1".gv"
nodeInformation=''
nodeColorInformation=' '

while read line
do
	if [[ $line =~ $config_line ]]
	then
		echo "Configuration" 
		echo "Vertices: " ${BASH_REMATCH[1]} " | Edges: " ${BASH_REMATCH[2]}\
			" | Terminals: " ${BASH_REMATCH[3]}
	elif [[ $line =~ $edge ]]
	then
		echo "Edge:" ${BASH_REMATCH[1]} "--" ${BASH_REMATCH[2]} \
			" | Weight: " ${BASH_REMATCH[3]}
		
		nodeInformation=$nodeInformation'	'${BASH_REMATCH[1]}' -- '${BASH_REMATCH[2]}' [label='${BASH_REMATCH[3]}"];" 
	else
		echo "Terminal number:" $line
		nodeColorInformation=$nodeColorInformation$line" "
	fi
done < $1

echo "graph G { " > $graphVizFileName

# increase this value if the graph is dense
echo "	edge [len=10];" >> $graphVizFileName
echo "	{node [style = filled, color = skyblue] " $nodeColorInformation"}" >> $graphVizFileName
echo $nodeInformation "}" >> $graphVizFileName

neato -Tsvg $graphVizFileName -o $graphVizFileName'.svg'
open $graphVizFileName'.svg'
