#include <ilcplex/ilocplex.h>
#include <iostream>

using namespace std;

/*
Para o bom funcionamento do organismo, uma pessoa precisa ingerir uma
quantidade mínima de certas vitaminas, presentes em 6 diferentes ingredientes
que deverão ser combinados em um composto alimentar de custo mínimo.

Fab-Dep  1   2    3
  1      8 | 5  | 6
  2     15 | 10 | 12
  3      3 | 9  | 10
*/

int main() {
    IloEnv env;
    try {
        IloModel model(env);

        double C[3][3] = {
            { 8,  5,  6},
            {15, 10, 12},
            { 3,  9, 10},
        };

        double F[3] = {120, 80, 80};
        double D[3] = {150, 70, 60};

        // Variáveis
        IloArray<IloNumVarArray> X(env, 3);
        for (int i = 0; i < 3; i++) {
            X[i] = IloNumVarArray(env, 3, 0.0, IloInfinity, ILOFLOAT); 
        }
        

        // Objetivo
        IloExpr sumCusto(env);
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                sumCusto += X[i][j]*C[i][j];
            }
        }

        model.add(IloMinimize(env, sumCusto));

        // Restrições
        model.add(X[0][0] + X[0][1] + X[0][2] <= F[0]);
        model.add(X[1][0] + X[1][1] + X[1][2] <= F[1]);
        model.add(X[2][0] + X[2][1] + X[2][2] <= F[2]);

        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                model.add(X[i][j] >= 0);
            }
        }

        model.add(X[0][0] + X[1][0] + X[2][0] == D[0]);
        model.add(X[0][1] + X[1][1] + X[2][1] == D[1]);
        model.add(X[0][2] + X[1][2] + X[2][2] == D[2]);
        
        // Solução
        IloCplex cplex(model);
        if (cplex.solve()) {
            cout << "Valor ótimo: " << cplex.getObjValue() << endl;
            for(int i = 0; i < 3; i++){
                cout << i+1 << ": ";
                for(int j = 0; j < 3; j++){
                    cout << cplex.getValue(X[i][j]) << ", ";
                }
                cout << endl;
            }
        } else {
            cout << "Sem solução viável." << endl;
        }
    } catch (IloException& e) {
        cerr << "Erro: " << e.getMessage() << endl;
    }
    env.end();
    return 0;
}
