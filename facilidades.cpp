#include <ilcplex/ilocplex.h>
#include <iostream>

using namespace std;

/*
– Dado um conjunto de N centros que podem ser construídos depósitos ou não,
e um conjunto de M cliente que precisam suprir suas demandas.
– Minimizar o custo de instalação de depósitos e o custo de atendimento, onde:
– fi é o custo de instalação do depósito i
– cij é o custo de atendimento entre o depósito i e o cliente j.*/

int main() {
    int n, m;
    cin >> n >> m;
    int f[n], c[n][m];
    for(int i = 0; i < n; i++){
        cin >> f[i];
    }
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            cin >> c[i][j];
        }
    }
    IloEnv env;
    try {
        IloModel model(env);

        
        // Variáveis
        IloArray<IloBoolVarArray> X(env, n);
        for (int i = 0; i < n; i++) {
            X[i] = IloBoolVarArray(env, m); 
        }
        
        IloBoolVarArray y(env, n);
        // comno eu calculo o custo?
        // para todo i, y[i]*f[i] 
        // mais x[i][j]*c[i][j] se existe um atendimento

        // eai eu posso colocar na restrição que x[i][j] só pode atender j
        // se y[i] estiver ativo
        // entao é para todo j, soma de x[i]*x[i][j] precisa ser 1
        // pq eu quero garantir a demanda e ao mesmo tempo garantir que seja minima
        // ajuda o solver a nao tentar uma solucao com 2 

        // Objetivo
        IloExpr sum(env);
        for (int i = 0; i < n; i++) {
            sum += y[i]*f[i];
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                sum += X[i][j]*c[i][j];
            }
        }
        model.add(IloMinimize(env, sum));

        // Restrições
        for(int j = 0; j < m; j++){
            IloExpr demandaM(env);
            for(int i = 0; i < n; i++){
                demandaM+=X[i][j];
                // estamos linearizando para garantir que x[i][j] só pode ser 1
                // se y[i] tambem for 1
                model.add(X[i][j] <= y[i]); 
            }
            model.add(demandaM == 1);
            demandaM.end();
        }


        
        // Solução
        IloCplex cplex(model);
        if (cplex.solve()) {
            for(int i = 0; i < n; i++){
                cout << i+1 << ": ";
                if(cplex.getValue(y[i])) cout << "placed ";
                else cout << "------ ";
                for(int j = 0; j < m; j++){
                    cout << int(round(cplex.getValue(X[i][j]))) << ", ";
                }
                cout << endl;
            }
            cout << "Valor ótimo: " << cplex.getObjValue() << endl;
        } else {
            cout << "Sem solução viável." << endl;
        }
    } catch (IloException& e) {
        cerr << "Erro: " << e.getMessage() << endl;
        env.end();
        return 0;
    }
    env.end();
    return 0;
}
