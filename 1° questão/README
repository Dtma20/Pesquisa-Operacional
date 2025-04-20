# Lista de Pesquisa Operacional

Este arquivo contém a primeira questão da lista de Pesquisa Operacinal que condiz a implementação de um algoritmo heurístico para o problema de empacotamento de caixas (bin packing), desenvolvido por **Diogo Amorim** e **Davi Romão** como parte de uma lista avaliativa de Pesquisa Operacional. O código, escrito em C++11, utiliza a heurística Best-Fit Decreasing (BFD) combinada com Iterated Local Search (ILS) para minimizar o número de caixas.

## Descrição do Problema

O objetivo é alocar *n* itens de tamanhos variados em caixas de capacidade fixa, usando o menor número possível de caixas. Cada item deve ser colocado em exatamente uma caixa, sem exceder a capacidade.

## Detalhes da Implementação

### (a) Representação da Solução

- **Modelo**: A solução é representada como um vetor de caixas (`vector<Bin>`), onde cada `Bin` contém:
  - Um vetor de itens (`vector<Item>`), com cada item tendo um `id` e `size`.
  - Um inteiro `total` indicando a soma dos tamanhos dos itens na caixa.
- **Manipulação**: Itens são adicionados, removidos ou trocados entre caixas. Caixas vazias podem ser eliminadas, e novas caixas são criadas quando necessário.

### (b) Função de Avaliação

- **Custo**: O custo de uma solução é o número de caixas usadas (`bins.size()`).
- **Cálculo**: Após alocar todos os itens, o tamanho do vetor `bins` é retornado como o número de caixas necessárias.

### (c) Estratégia de Busca Local

- **Vizinhança**: A busca local explora trocas de itens entre caixas:
  - Para cada item em uma caixa `i`, tenta movê-lo para outra caixa `j` se a capacidade de `j` não for excedida.
  - Se a caixa `i` ficar vazia após a troca, ela é removida, reduzindo o número de caixas.
- **Implementação**: Usa **first improvement**:
  - Itera por todas as caixas e itens, aplicando a primeira troca que reduz o número de caixas (removendo uma caixa vazia).
  - A busca termina quando nenhuma troca melhora a solução.

### (d) Critério de Parada

- **Tempo Limite**: O algoritmo recebe um tempo limite em segundos via linha de comando (`argv[1]`).
- **Controle**: Usa `steady_clock` para verificar se o tempo decorrido excede o limite. A busca (ILS) para quando o prazo é atingido.