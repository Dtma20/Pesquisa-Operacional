#include <ilcplex/ilocplex.h>
#include <iostream>

using namespace std;

/*
Um produtor de gás natural s precisa enviar a maior quantidade de gás para a
fábrica t através de dutos.
Cada duto ij direcionado pode possuir uma capacidade associada.
*/

int main() {
    int n, m, s, t;
    cin >> n >> m;

    int adj[n][n] = {};

    for(int i = 0; i < m; i++){
        int u, v, w;
        u--; v--;
        cin >> u >> v >> w;
        adj[u][v] = w;
    }

    cin >> s >> t;
    IloEnv env;
    try {
        IloModel model(env);

        
        // Variáveis
        IloArray<IloNumVarArray> X(env, n);
        for (int i = 0; i < n; i++) {
            X[i] = IloNumVarArray(env, n, 0, IloInfinity, ILOFLOAT); 
        }
        

        // Objetivo
        IloExpr sum(env);
        for(int i = 0; i < n; i++){
            sum+= X[i][t];
        }
        cout << t << endl;
        model.add(IloMaximize(env, sum));

        // Restrições
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                model.add(X[i][j] >= 0);  // nao negatividade
                model.add(X[i][j] <= adj[i][j]); // capacidade de um vertice para outro
            }
        }

        for(int v = 0; v < n; v++){
            if(v==s || v==t) continue;

            IloExpr sumIn(env), sumOut(env);
            for(int j = 0; j < n; j++){
                sumIn += X[j][v];
                sumOut += X[v][j];
            }
            model.add(sumIn == sumOut);

            sumIn.end();
            sumOut.end();
        }

        
        // Solução
        IloCplex cplex(model);
        if (cplex.solve()) {
            for(int i = 0; i < n; i++){
                cout << i+1 << ": ";
                for(int j = 0; j < n; j++){
                    cout << cplex.getValue(X[i][j]) << ", ";
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
