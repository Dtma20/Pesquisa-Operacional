#include <ilcplex/ilocplex.h>
#include <iostream>
#include <string>

ILOSTLBEGIN

/* 
    Uma cooperativa agrícola opera 3 fazendas, e a produção total de cada fazenda depende da área 
    disponível para o plantio e da quantidade de água disponível para irrigação. A cooperativa 
    planeja diversificar sua produção, cultivando 3 culturas em cada fazenda: milho, arroz e feijão. 
    Cada cultura demanda uma quantidade específica de água para irrigação. Existem limites de área 
    para o plantio de cada cultura, e para evitar concorrência entre os cooperados, foi acordado que 
    a proporção de área cultivada será a mesma em todas as fazendas. O objetivo é determinar a área 
    plantada de cada cultura em cada fazenda, de modo a otimizar o lucro da cooperativa.

    Dados das fazendas:
    Fazenda   | Área (acres) | Água (litros)
    ------------------------------------------
    Fazenda 1 | 400         | 1800
    Fazenda 2 | 650         | 2200
    Fazenda 3 | 350         | 950

    Dados das culturas:
    Cultura   | Área Máx (acres) | Água (litros por área) | Lucro (por área)
    ------------------------------------------------------------------------
    Milho     | 660             | 5.5                  | 5000
    Arroz     | 880             | 4.0                  | 4000
    Feijão    | 400             | 3.5                  | 1800
*/


int main() {
    IloEnv env;
    IloModel model(env);

    int area_fazenda[3] = {400, 650, 350};
    int area_cultura[3] = {660, 880, 400};
    int agua_fazenda[3] = {1800, 2200, 950};
    double coef_agua[3] = {5.5, 4.0, 3.5};
    int preco[3] = {5000, 4000, 1800};

    // variáveis de decisão
    IloArray<IloNumVarArray> X(env, 3);
    for (int i = 0; i < 3; i++) {
        X[i] = IloNumVarArray(env, 3);
        for (int j = 0; j < 3; j++) {
        std::string name = "X" + std::to_string(i+1) + (j==0?"m":j==1?"a":"f");
        X[i][j] = IloNumVar(env, 0.0, IloInfinity, ILOINT, name.c_str());
        }
    }

    // função objetivo (maximizar lucro)
    IloExpr lucro(env);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        lucro += preco[j] * X[i][j];
    model.add(IloMaximize(env, lucro));
    lucro.end();

    // área por fazenda
    for (int i = 0; i < 3; i++) {
        IloExpr soma(env);
        for (int j = 0; j < 3; j++)
        soma += X[i][j];
        model.add(soma <= area_fazenda[i]);
        soma.end();
    }

    // área por cultura
    for (int j = 0; j < 3; j++) {
        IloExpr soma(env);
        for (int i = 0; i < 3; i++)
        soma += X[i][j];
        model.add(soma <= area_cultura[j]);
        soma.end();
    }

    // limite de água
    for (int i = 0; i < 3; i++) {
        IloExpr uso(env);
        for (int j = 0; j < 3; j++)
        uso += coef_agua[j] * X[i][j];
        model.add(uso <= agua_fazenda[i]);
        uso.end();
    }

    // proporção igual entre fazendas
    IloExpr soma0(env), soma1(env), soma2(env);
    for (int j = 0; j < 3; j++) {
        soma0 += X[0][j];
        soma1 += X[1][j];
        soma2 += X[2][j];
    }
    model.add(650.0 * soma0 == 400.0 * soma1);
    model.add(350.0 * soma0 == 400.0 * soma2);
    soma0.end(); soma1.end(); soma2.end();

    // Resolver o modelo
    IloCplex cplex(model);
    if (!cplex.solve()) {
        std::cerr << "Sem solucao\n";
        return 1;
    }

    std::cout << "Lucro = " << cplex.getObjValue() << std::endl;
    for (int i = 0; i < 3; i++) {
        std::cout << "Fazenda " << (i + 1) << ": ";
        std::cout << "Milho = "  << cplex.getValue(X[i][0]) << ", ";
        std::cout << "Arroz = "  << cplex.getValue(X[i][1]) << ", ";
        std::cout << "Feijao = " << cplex.getValue(X[i][2]) << std::endl;
    }
    
    env.end();
    return 0;
}
