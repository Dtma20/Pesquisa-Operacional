#include <ilcplex/ilocplex.h>
#include <iostream>
#include <iomanip>
#include <string>

ILOSTLBEGIN

/* A empresa de tintas fabrica dois tipos de tinta: secagem rápida (SR) e secagem normal (SN). 
   Ambas utilizam os mesmos componentes, mas com proporções diferentes. 
   Os principais componentes são:
   - Componente de Secagem (SEC), responsável pela secagem da tinta.
   - Componente de Cor (COR), responsável pela coloração da tinta.

   A empresa tem duas opções de mistura para a produção das tintas:
   - Solução A (SolA) com 30% de SEC e 70% de COR, a R$ 1,5 o litro.
   - Solução B (SolB) com 60% de SEC e 40% de COR, a R$ 1,0 o litro.

   Além disso, a empresa pode comprar os componentes separados:
   - Componente de SEC a R$ 4,0 o litro.
   - Componente de COR a R$ 6,0 o litro.

   As tintas precisam atender às seguintes proporções mínimas de componentes:
   - Para a tinta de secagem rápida (SR), deve-se ter no mínimo 25% de SEC e no mínimo 50% de COR.
   - Para a tinta de secagem normal (SN), deve-se ter no mínimo 20% de SEC e no mínimo 50% de COR.

   O objetivo é determinar a quantidade de produtos (SolA, SolB, SEC, COR) a serem comprados 
   para produzir 1000 litros de SR e 250 litros de SN com o menor custo possível.
   Esse é um problema típico de programação linear, onde o objetivo é minimizar o custo total 
   respeitando as restrições de proporção dos componentes nas tintas. */

   int main() {
    IloEnv env;
    IloModel model(env);

    // dados do problema
    int V[2] = {1000, 250};
    double sec_min[2] = {0.25 * V[0], 0.20 * V[1]};
    double cor_min[2] = {0.50 * V[0], 0.50 * V[1]};
    double comp_sec[4] = {0.30, 0.60, 1.00, 0.00};
    double comp_cor[4] = {0.70, 0.40, 0.00, 1.00};
    double custo[4] = {1.50, 1.00, 4.00, 6.00};

    // variáveis de decisão
    IloArray<IloNumVarArray> x(env, 2);
    for (int p = 0; p < 2; ++p) {
        x[p] = IloNumVarArray(env, 4, 0, IloInfinity, ILOINT);
    }

    // volume total por produto
    for (int p = 0; p < 2; ++p) {
        IloExpr soma(env);
        for (int i = 0; i < 4; ++i) soma += x[p][i];
        model.add(soma == V[p]);
        soma.end();
    }

    // mínimo de secante
    for (int p = 0; p < 2; ++p) {
        IloExpr sec(env);
        for (int i = 0; i < 4; ++i) sec += comp_sec[i] * x[p][i];
        model.add(sec >= sec_min[p]);
        sec.end();
    }

    // mínimo de corante
    for (int p = 0; p < 2; ++p) {
        IloExpr cor(env);
        for (int i = 0; i < 4; ++i) cor += comp_cor[i] * x[p][i];
        model.add(cor >= cor_min[p]);
        cor.end();
    }

    // função objetivo (minimizar custo)
    IloExpr custo_tot(env);
    for (int p = 0; p < 2; ++p)
        for (int i = 0; i < 4; ++i)
            custo_tot += custo[i] * x[p][i];
    model.add(IloMinimize(env, custo_tot));
    custo_tot.end();

    // Resolver o modelo
    IloCplex cplex(model);
    cplex.setOut(env.getNullStream());
    if (!cplex.solve()) {
        std::cerr << "Sem solução\n";
        env.end();
        return 1;
    }

    // Resultados
    std::cout << std::fixed << std::setprecision(0);
    const char* nomes[4] = {"SolA","SolB","SEC","COR"};
    for (int p = 0; p < 2; ++p) {
        std::string prod = (p == 0 ? "SR" : "SN");
        std::cout << prod << ":\n";
        for (int i = 0; i < 4; ++i) {
            int v = (int)std::round(cplex.getValue(x[p][i]));
            std::cout << "  " << nomes[i] << " = " << v << " L\n";
        }
    }


    env.end();
    return 0;
}