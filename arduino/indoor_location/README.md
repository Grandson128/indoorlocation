## Esta pasta é composta pelo:

  * structures.h 	      	**# este ficheiro tem funções para parse de buffers que são captados pelo nosso módulo WiFi, este parse é feito para estruturas C (i.e. ClientInfo) que estão declaradas no ficheiro.**
  * functions.h		       **# este ficheiro tem funções auxiliares**
  * indoor_location.ino	**# tem os includes, variáveis globais (credenciais AP, domínio NTP, dados de autenticação para Firebase). Este ficheiro, tem uma callback que faz o handle dos pacotes recolhidos pelo nosso modulo Wifi, um setup que sincroniza o timer da board, um loop que faz set do chanel entre 1 e 15 e uma função para construção e envio de payload em formato JSON para o Firebase.**
