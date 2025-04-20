#include <ilcplex/ilocplex.h>
#include <iostream>

using namespace std;

/*
Para o bom funcionamento do organismo, uma pessoa precisa ingerir uma
quantidade mínima de certas vitaminas, presentes em 6 diferentes ingredientes
que deverão ser combinados em um composto alimentar de custo mínimo.

vitaminas |  1 |  2 |  3 |  4 |  5 |  6 | Quantidade Mínima
A         |  1 |  0 |  2 |  2 |  1 |  2 |  9
C         |  0 |  1 |  3 |  1 |  3 |  2 | 19
Preço     | 35 | 30 | 60 | 50 | 27 | 22 | 

*/

int main() {
    IloEnv env;
    try {
        IloModel model(env);

        double a[6] = { 1,  0,  2,  2,  1,  2};
        double c[6] = { 0,  1,  3,  1,  3,  2};
        double p[6] = {35, 30, 60, 50, 27, 22};

        // Variáveis
        IloNumVarArray x(env, 6, 0, IloInfinity, ILOFLOAT);
        IloExpr sumA(env);
        IloExpr sumC(env);
        IloExpr sumValue(env);

        // Objetivo
        for(int i = 0; i < 6; i++)
            sumValue+=  x[i]*p[i];
        model.add(IloMinimize(env, sumValue));

        // Restrições
        for(int i = 0; i < 6; i++)
            model.add(x[i] >= 0);
        
        for(int i = 0; i < 6; i++)
            sumA+=  x[i]*a[i];

        for(int i = 0; i < 6; i++)
            sumC+=  x[i]*c[i];
        
        model.add(sumA >= 9);
        model.add(sumC >= 19);
        
        // Solução
        IloCplex cplex(model);
        if (cplex.solve()) {
            cout << "Valor ótimo: " << cplex.getObjValue() << endl;
            for(int i = 0; i < 6; i++) cout << i+1 <<": " <<cplex.getValue(x[i]) << endl;
        } else {
            cout << "Sem solução viável." << endl;
        }
    } catch (IloException& e) {
        cerr << "Erro: " << e.getMessage() << endl;
    }
    env.end();
    return 0;
}
