# indoorlocation

##Introdução

Este projeto tem como objetivo localizar objetos dentro de um edifício (fora do alcance do GPS) e mostrar graficamente a sua localização num mapa que representa o edifício ou espaço onde se encontra o objeto.

Decidimos usar uma técnica (WPS) que usa a força do sinal(RSSI) entre o Raspberry e os beacons para estimar uma posição. Cada beacon vai ter uma entrada na base de dados onde guarda sucessivamente o valor SSI que está a obter em cada pacote recebido pelo Raspberry. O Android acede à base de dados para obter os ultimos valores de RSSI guardados, calcula a distância de cada beacon para o Raspberry e estima a sua posição. 

