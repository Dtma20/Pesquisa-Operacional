#include <ilcplex/ilocplex.h>
#include <iostream>

using namespace std;

/*
Dado um grafo G = (V , E), determinar a clique máxima de G, ou seja, o maior
subgrafo completo em G
*/

int main() {
    int n = 0, m = 0;
    char ch;

    // usamos o https://iridia.ulb.ac.be/~fmascia/maximum_clique/#detC125.9
    // até encontrar a linha com 'p col n m' ou 'p edge n m'
    while (cin >> ch) {
        if (ch == 'c') {
            while (cin.get() != '\n');
        } else if (ch == 'p') {
            string tmp;
            cin >> tmp >> n >> m; 
            break;
        }
    }

    int adj[n][n]{};


    while (cin >> ch) {
        if (ch == 'e') {
            int u, v;
            cin >> u >> v;
            u--, v--;
            adj[u][v] = 1;
            adj[v][u] = 1;
        }
    }

    cout << "Grafo lido com " << n << " vértices e " << m << " arestas.\n";

    IloEnv env;
    try {
        IloModel model(env);

        
        // Variáveis
        IloBoolVarArray x(env, n);

        // Objetivo
        IloExpr qntVertex(env);
        for(int i = 0; i < n; i++){
            qntVertex += x[i];
        }

        model.add(IloMaximize(env, qntVertex));

        // Restrições
        // se o cara tiver na clique, todo mundo que estiver, deve ter aresta
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                if(j==i) continue;
                if(!adj[i][j]){
                    model.add(x[i]+x[j] <= 1);
                }
            }

            model.add(x[i] >= 0);
        }

        
        // Solução
        IloCplex cplex(model);
        if (cplex.solve()) {
            cout << "Valor ótimo: " << cplex.getObjValue() << endl;
            for(int i = 0; i < n; i++){
                cout << i+1 << ": " << cplex.getValue(x[i]) << endl;
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
