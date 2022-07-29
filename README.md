# pokemonGo_battle_system

Esse projeto consiste numa versão adaptada do sistema de batalha do jogo Pokémon GO com o intuito de aperfeiçoar as habilidades na linguagem C++.

Todo conteúdo presente nesse projeto foi apresentado como trabalho final na disciplina "Introdução a programação C/C++" em 2019.2, sem nenhum fim financeiro.

O código funciona de forma bem simples: 2 jogadores inserem seus nomes e escolhem 3 pokémon, além de seus levels e seus ataques (1 rápido e 1 carregado), logo a seguir é simulado uma batalha de 1v1 até que todos os pokémon de algum jogador sejam derrotados.

Os pokémon são representador por uma struct contendo seu nome, seus status (base vida, ataque, defesa e velocidade), 2 vectors que possuem os ataques rápidos e carregados que ele pode usar e seus tipos (fire,water e entre outros). Esses são os valores base de cada pokémon que são preenchidos por meio dos arquivos .csv do projeto, as outras variáveis serão preenchidas ao decorrer do funcionamento do código.

Informações mais detalhadas do sistema principal estão contidas no arquivo ***CompIC_Workclass.pdf***
