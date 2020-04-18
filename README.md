# compiladores-trabalho1
*trabalhinho de compi* 

Com base na gramática da linguagem P-- (Repositório do Tidia) enriquecida com
o comando “for”, desenvolver o analisador léxico para esta
linguagem.
 Vocês deverão produzir:
  1. os autômatos projetados (podem usar a ferramenta JFlap ou outra que
preferirem para desenhar os autômatos);
  2. o código-fonte correspondente aos autômatos projetados

Ao fim, deverá ser entregue um arquivo compactado com:<br> 
          (i) os códigos-fonte de seu analisador léxico; <br>
          (ii) um relatório (de até 10 páginas), que contenha:<br>
                 - a identificação dos membros do grupo<br>
                 - um breve relato das decisões de projeto<br>
                 - os autômatos projetados<br>
                 - instruções para compilar/rodar seu código-fonte (que sistema usar, versão, parâmetros esperados, etc.) e pelo menos um exemplo de execução.


Espera-se que seu analisador léxico aceite um arquivo txt com o programa escrito em P-- e
produza um outro arquivo txt com a saída, com um par cadeia-token por linha (indicando os
erros léxicos, se houver), conforme ilustração abaixo.

<img width="1000px" 
src="https://uploaddeimagens.com.br/images/002/599/003/full/Captura_de_tela_de_2020-04-18_16-49-53.png?1587239407">

Sugere-se que seja implementada uma função principal cujo único propósito seja chamar o
procedimento do analisador léxico tantas vezes quanto necessário para processar o programa
inteiro do usuário. Assim, a cada chamada, o analisador léxico devolve um par cadeia-token,
que é impresso pela função principal. O propósito dessa função principal é simular o 
funcionamento do analisador sintático (que, no próximo trabalho prático, será incorporado ao
seu programa).

Será necessário tomar várias decisões de projeto, por exemplo, como implementar os
autômatos, como controlar os símbolos já lidos (reveja no material de aula a estratégia de
“retroceder” nos autômatos e de usar um símbolo “lookahead”), como tratar erros, quais são
os tokens associados às cadeias (sugere-se usar strings que indiquem claramente as classes das
cadeias), etc. Nesse ponto, sugere-se que se use a experiência adquirida em sala de aula,
empregue seu bom senso e faça o exercício de se colocar no lugar do possível usuário de seu
sistema, respondendo a perguntas como “esse sistema está simples e fácil de usar?” e “a saída
dele é legível?”.
