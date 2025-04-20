#include <ilcplex/ilocplex.h>
#include <iostream>

ILOSTLBEGIN

/*
    Em um hospital, deseja-se planejar o horário de enfermeiras no turno da noite. A
    demanda de enfermeiras no turno da noite no dia i é um número inteiro di
    ,com i = 1... 7. Cada enfermeira trabalha 5 dias consecutivos e descansa por três dias
    seguintes. O objetivo consiste em minimizar o número de enfermeiras contratadas
*/

int main() {
    IloEnv env;
    IloModel model(env);

    // dados do problema
    int d[7] = {5, 7, 6, 8, 4, 7, 5};
    int a[7][8] = {
        {1, 0, 0, 0, 1, 1, 1, 1},
        {1, 1, 0, 0, 0, 1, 1, 1},
        {1, 1, 1, 0, 0, 0, 1, 1},
        {1, 1, 1, 1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 1, 1, 0}
    };

    // variáveis de decisão
    IloNumVarArray x(env, 8, 0, IloInfinity, ILOINT);

    // atender demanda diária
    for (int i = 0; i < 7; i++) {
        IloExpr cover(env);
        for (int s = 0; s < 8; s++) {
            cover += a[i][s] * x[s];
        }
        model.add(cover >= d[i]);
        cover.end();
    }

    // função objetivo (minimizar enfermeiras)
    IloExpr total_enfs(env);
    for (int s = 0; s < 8; s++) {
        total_enfs += x[s];
    }
    model.add(IloMinimize(env, total_enfs));
    total_enfs.end();

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