#include <ilcplex/ilocplex.h>
#include <iostream>
#include <vector>

ILOSTLBEGIN

/*
    Dado um conjunto de n itens e um inteiro W que representa a capacidade da
    mochila. Cada item i possui um valor vi e um peso wi.
    Determinar o subconjuntos de itens que maximizam o somatório dos valores
    respeitando a capacidade de peso da mochila.
*/

int main() {
    IloEnv env;
    IloModel model(env);

    // dados do problema (mochila)
    int n = 10;
    int W = 15;
    std::vector<double> v = {10, 40, 30, 50, 35, 40, 30, 45, 25, 20};
    std::vector<double> w = {1, 3, 4, 5, 2, 3, 2, 4, 1, 2};

    // variáveis de decisão
    IloBoolVarArray x(env, n);

    // peso total
    IloExpr peso_total(env);
    for (int i = 0; i < n; ++i) {
        peso_total += w[i] * x[i];
    }
    model.add(peso_total <= W);
    peso_total.end();

    // função objetivo (maximizar valor)
    IloExpr valor_total(env);
    for (int i = 0; i < n; ++i) {
        valor_total += v[i] * x[i];
    }
    model.add(IloMaximize(env, valor_total));
    valor_total.end();

    // Resolver o modelo
    IloCplex cplex(model);
    if (!cplex.solve()) {
        std::cerr << "Sem solucao\n";
        return 1;
    }

    std::cout << "Resposta: " << cplex.getObjValue() << std::endl;

    env.end();
    return 0;
}