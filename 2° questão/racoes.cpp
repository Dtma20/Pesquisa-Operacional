#include <ilcplex/ilocplex.h>
#include <iostream>

using namespace std;

/*
Uma empresa de alimentos caninos produz 2 tipos de ração:
– All Mega Giga Suprema (AMGS) e Ração das Estrelas (RE).
– Para a sua manufatura são utilizadas cereais e carne.
– O AMGS utiliza 5 kg de cereais e 1 kg de carne e o RE utiliza 4 kg de carne e 2 kg
de cereais.
– O preço do AMGS é 20 reais e o do RE é 30 reais
– 1 kg de carne custa 4 reais e 1 kg de cereal custa 1 reais.
– Existe a disposição 10.000 kg de carne e 30.000 kg de cereais.
– Como deve ser a produção da empresa para ganhar o máximo de dinheiro?

Lucro de AMG = 20 - 5*1 - 4*1
Lucro de RE = 30 - 4*4 - 2*1
*/

int main() {
    IloEnv env;
    try {
        IloModel model(env);

        // Variáveis
        IloNumVar AMGS(env);
        IloNumVar RE(env);

        // Objetivo
        model.add(IloMaximize(env, 11*AMGS + 12*RE));

        // Restrições
        model.add(AMGS >= 0);
        model.add(RE >= 0);
        model.add(AMGS + 4*RE <= 10000);
        model.add(5*AMGS + 2*RE <= 30000);

        // Solução
        IloCplex cplex(model);
        if (cplex.solve()) {
            cout << "Valor ótimo: " << cplex.getObjValue() << endl;
            cout << "AMGS = " << cplex.getValue(AMGS) << ", RE = " << cplex.getValue(RE) << endl;
        } else {
            cout << "Sem solução viável." << endl;
        }
    } catch (IloException& e) {
        cerr << "Erro: " << e.getMessage() << endl;
    }
    env.end();
    return 0;
}
