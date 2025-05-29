- [x] Ajuste introdução e resumo ([...] grafo T de G [...])
- [x] Nova subseção 1.1 chamada "Resultados obtidos", explicando quais foram os resultados obtidos neste trabalho
    - [x] Antes dela, adicionar um breve texto explicando que algoritmos sequenciais e paralelos de força bruta já foram propostos (citando artigos IPL e CCPE) e que deixaram algumas questões em aberto (e.g. otimização de checagem de ciclos e implementação com lista de arestas). Além disso, mencionar o motivo do estudo de algoritmos paralelos (e.g. aferir abordagens heurísticas e obter soluções para o problema, dado que é NP-completo)

    IPL ("D:\Documents\UFF\TCC\Documentos\couto_cunha_juventude_santiago.pdf")
    CCPE (não sei com certeza se é "D:/Documents/UFF/TCC/Documentos/cunha_marciano_moraes_santiago_santos.pdf")
- [x] Nova subseção 1.2 chamada "Organização do trabalho", indicando o conteúdo de cada seção do trabalho:
    - Seção 2: preliminares de complexidade e do problema da t-admissibilidade
    - Seção 3: desenvolvimento teórico mostrando as abordagens sequenciais e paralelas desenvolvidas nesse trabalho
    - Seção 4: experimentos obtidos das implementações realizadas e comparações com outras abordagens existentes
    - Seção 5: conclusão do trabalho onde você vai apresentar trabalhos os futuros
- [x] Nova seção 2 chamada "Preliminares", contendo:
    - [x] Equivalência de definições presentes no paper do Cai e Corneil
    - [x] Definições de tratabilidade, intratabilidade, P, NP, NP-completo, NP-hard
    - [x] Explicitar que a complexidade de pior caso tem que ser polinomial (3o parágrafo da introdução)
- [x] Dizer para que valor de t o problema é difícil para grafos planares (7o parágrafo da introdução)
- [x] Expandir (e adicionar mais referências) ou remover penúltimo parágrafo da introdução
    - [x] Por enquanto, removido.
- [x] Remover criação de subseções para os exemplos
- [x] Mover a Figura 3 para mais perto de onde é citada
- [x] Na seção "Desenvolvimento Teórico", agora 3, adicionar um breve texto antes das subseções explicando o conteúdio de cada uma
    - Subseção 3.1: abordagem sequencial para geração de árvores-geradoras
    - Subseção 3.2: abordagem paralelizada para geração de árvores-geradoras
    - Subseção 3.3: detecção de ciclos utilizando conjuntos-disjuntos
- [x] Na seção 3.1 explicitar que o algoritmo foi desenvolvido no trabalho do IPL, e que o intuito é revê-lo pois a abordagem paralela é baseada nele também.
- ~~[ ] Mover subseção de Conjuntos Disjuntos, agora 3.3, para a seção de preliminares~~
    - Avaliar essa mudança uma vez que as demais forem feitas
- [x] Ao final da subseção de "Conjuntos Disjuntos", agora 3.3, definir
    - [x] A função inversa de Ackermann e complexidade amortizada (ver no livro do Jayme de Estruturas de Dados)

    ou

    - ~~[ ] Que a complexidade é logarítmica (ver material de ASA/APA)~~
- [ ] Na seção de Detecção de Ciclos, agora 4.1: 
    - [x] Adicionar especificações do computador utilizado
    - [ ] Explicitar o interesse em comparar com o algoritmo utilizado nos artigos anteriores (e citá-los)
        - *"O algoritmo de busca em profundidade é o que a gente já tinha do outro artigo, certo? Então cite esse artigo aqui e diga explicitamente que você quer comparar a abordagem existente com essa sua."*

    - [x] Rever nomenclatura e definições:
        - *n* para número de vértices
        - *m* para número de vértices
        - Definir densidade de arestas *d* com maior detalhe
        - Explicitar aumento do número de vértices e da densidade de arestas, ao invés de utilizar delta ([500, 1000, 1500, ..., 5000] e [0, 10, 20, ..., 100])
    - [x] Fazer uma análise mais rica dos resultados da Figura 4
    - [x] Adicionar Caption na Figura 4 (e.g. quantos foram os grafos, o que esses traços dizem)
- [x] Na seção de "Obtenção de Árvores Geradoras", agora 4.2:
    - [x] Explicitar quais os "algoritmos apresentados anteriormente"
    - [x] Descrever como foi feito o cálculo do speedup
    - [x] Explicitar o que diz o Teorema de Kirchhoff
- [ ] Na seção de "Conclusão", agora 5:
    - [ ] Falar de outras abordagens de paralelismo que poderiam ser exploradas em trabalhos futuros. Dentre elas:
        - Paralelismo com corte de arestas
        - Paralelismo com remoção de arestas de um ciclo (implementada pelo Carlos)
    - [ ] Falar de outros problemas para os quais essa geração de árvores pedia ser aplicada/adaptada. Dentre eles:
        - Árvore geradora com menor diâmetro
        - Árvore geradora com menor altura
        











1a linha do resumo:
é um subgrafo T de G tal que T
->
é um subgrafo gerador T de G tal que T

Esse mesmo comentário vale para a definição apresentada no início da introdução.

2o parágrafo da introdução:
A definição de t-admissibilidade apresentada aqui está correta, ela é equivalente a definição apresentada no resumo e no paper do Cai e Corneil eles provam isso. Para enriquecer o texto, meu pedido é que você ponha a equivalência dos problemas que existe lá no paper deles. É um resultado que mostra uma série de equivalências, não somente a que você apresentou no resumo e aqui nesse parágrafo.

3o parágrafo da introdução:
Aqui você define o que é um problema tratável. Você tem que falar da complexidade sim, mas seja explícito sobre a complexidade de pior caso ter que ser polinomial.
Em todo caso, uma melhor forma de organizar o conteúdo é que após a introdução você ponha uma seção de Preliminares onde você possa pôr definições mais formais do que você apresenta aqui sobre tratabilidade, intratabilidade, P, NP, NP-completo, NP-hard. 
Além disso, nessa mesma seção de preliminares você pode adicionar o teorema do Cai e Corneil sobre as equivalências do problema da t-admissibilidade.

7o parágrafo da introdução:
Aqui você fala que para grafos planares a gente sabe que é NP-completo, porém não diz para qual valor de t o problema é difícil. Se não me engano, o paper que prova isso considera um valor de t bem alto, mas tem que ver lá.

Penúltimo parágrafo da introdução:
Esse texto ficou um pouco solto em relação aos demais conteúdos apresentados. Caso você queira falar de alguma outra aplicação, fale mais sobre isso e ponha referências.

Último parágrafo da introdução:
Aqui você deve investir mais espaço no seu texto. Faça uma subseção que se chame: Resultados obtidos
Nela você explica um pouco mais sobre quais foram os resultados do seu TCC. 
Na verdade, antes disso, fale que algoritmos paralelos e sequenciais de força bruta já foram propostos antes (pode citar meus dois artigos anteriores, do IPL e do CCPE) e fale que eles deixaram algumas questões relevantes em aberto, como otimização de checagem de ciclos na construção das árvores e também na implementação de abordagens paralelas utilizando lista de arestas (era isso, não era?) como estruturas de dados de grafos.
Daí depois disso, nas suas contribuições você fala o que fez e fala também dos resultados experimentais obtidos comparados com os já conhecidos da literatura.

Você também deve mencionar o motivo de estudarmos os algoritmos paralelos. Muito vem pelo fato da gente aferir abordagens heurísticas ou mesmo para de fato ter soluções do problema, dado que é NP-completo.

Depois disso, você põe um parágrafo (ou subseção) que se chama Organização do trabalho.
Daí aí você diz que na seção 2 vai ter as preliminares de complexidade e no problema da t-admissibilidade; Na seção 3 vai ter o desenvolvimento teórico mostrando as abordagens sequenciais e paralelas desenvolvidas nesse trabalho; Na seção 4 vão ter os experimentos obtidos das implementações realizadas e comparações com outras abordagens existentes; Na seção 5 é a conclusão do trabalho onde você vai apresentar trabalhos os futuros.


Antes de entrar na seção 2.1 você pode apresentar o que vai ter nessa seção. Por exemplo, diga que na seção 2.1 você vai apresentar a abordagem sequencial existente para gerar as árvores, na seção 2.2 uma abordagem paralela, na seção 2.3 vai ser sobre conjuntos disjuntos, …

Seção 2.1
Esse algoritmo foi desenvolvido no meu trabalho do IPL, não foi? Neste caso, fale isso por explícito. Diga que esse algoritmo foi desenvolvido lá e aqui seu intuito é revê-lo pois a abordagem paralela se baseia nesse algoritmo também. 

Os exemplos apresentados não precisam ser em seções 2.1.1 e 2.2.2. Você pode abrir um enviromment \begin{example} \end{example} contendo os exemplos.
Você está usando latex, certo?
Neste caso tem que ver se example é definido no preâmbulo.
Se der esse, você então pode criar:
\newtheorem{example}{example}

Seção 2.3
Fiquei pensando se o melhor lugar dessa seção é aqui mesmo ou antes, eventualmente até mesmo nessa nova seção de preliminares que te pedi para fazer. Dado que é uma estruturas de dados que serve como uma ferramenta para a gente, talvez valha mais a pena apresentar antes, o que acha?

Último parágrafo da seção 2.3
Alpha é a função inversa de Ackermann?! Se sim, tem que definir isso. Além disso, você deve definir o que é uma complexidade amortizada (existe a definição no livro do Jayme de Estruturas de Dados).
Outra possibilidade, seria você mostrar que a complexidade é logarítmica (provo isso no curso de ASA/APA). Pode adicionar essa prova.

Seção 3.1
É dito N = 500, N \leq 5000
Está certo isso?
O que é não seria M \leq 5000.
Além disso, normalmente utilizamos n para quantidade de vértices, não N, e m para quantidade de arestas, não M.
Qual a definição de densidade de arestas d que você usa aqui? Ponha isso por explícito.
Qual a definição de Delta N e Delta d?
Faltam algumas informações importantes demais nessa parte.



Como aqui é uma seção sobre experimentos, diga quais as especificações do computador utilizado.

Falta uma análise dos resultados obtidos da Figura 4. Qual a consequência você tirou dos resultados? Falta escrever sobre isso. Atualmente é tido que sua abordagem é melhor, mas pode falar um pouco melhor sobre o quão melhor é.

Além disso, o caption da figura deve explicar o que tem na figura. Ter escrito somente Figura 4 sem texto não é legal. Diga por exemplo quantos foram os grafos, o que esses traços dizem.

Seção 3.2
1a frase
Quais algoritmos que foram apresentados anteriormente? Seja explícito dizendo Algoritmo x, Algoritmo y.

Fale um pouco mais sobre o como foi o speedup, como foi calculado, por exemplo.

Pág 12, 1o parágrafo:
O que diz o Teorema de Kirchhoff? Você tem que adicionar esse teorema para que tenha um texto auto-contido. Mesmo pondo a citação a ele, você deve ter essa explicação aqui.

A Figura 3 ficou muito longe do exemplo 3. Tente pôr mais próximo.

Na conclusão você pode falar de outras abordagens de paralelismo para trabalhos futuros. Você lembra que já conversamos sobre elas? Pensando por exemplo em cortes de arestas.
Fale também das possibilidades de utilizar abordagens para gerar árvores geradoras úteis para outros problemas relacionados. Fale desses outros problemas também (já conversamos um pouco, lembra? Tipo árvore geradora com menor diâmetro, ou árvore geradora com menor altura, tinham vários outros problemas possíveis que esse algoritmo poderia se adaptar, fale deles)