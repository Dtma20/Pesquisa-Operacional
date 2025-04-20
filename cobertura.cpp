#include <ilcplex/ilocplex.h>
#include <vector>
#include <iostream>

using namespace std;

/*
O governo planeja construir escolas de modo a satisfazer a demanda em uma
cidade. Ainda não se sabe quantas escolas são necessárias, mas a lei demanda que
todo bairro deve ter uma escolas ou estar próximo de uma. Em que bairros dever
ser construídas escolas de modo a minimizar o número de escolas construídas.

no fim o que ele quer é o conjunto dominante
*/

int main() {
    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj(n);

    for(int i = 0; i < m; i++){
        int u, v;
        cin >> u >> v;
        u--; v--;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    IloEnv env;
    try {
        IloModel model(env);

        
        // Variáveis
        IloBoolVarArray x(env, n);
        
        // Objetivo
        IloExpr sum(env);
        for(int i = 0; i < n; i++){
            sum+= x[i];
        }
        model.add(IloMinimize(env, sum));

        // Restrições
        for(int u = 0; u < n; u++){
            IloExpr sum(env);
            sum+= x[u];
            for(auto v : adj[u]){
                sum += x[v];
            }
            model.add(sum >= 1);
            sum.end();
        }

        // Solução
        IloCplex cplex(model);
        if (cplex.solve()) {
            for(int i = 0; i < n; i++){
                cout << i+1 << ": " << cplex.getValue(x[i]) << endl;
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
