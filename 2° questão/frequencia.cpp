#include <ilcplex/ilocplex.h>
#include <vector>
#include <iostream>

using namespace std;

/*
– Quando o raio de duas antenas se interceptam, haverá interferência caso as
antenas utilizem a mesma frequência
– Estabelecer frequências para as antenas de modo que não haja interferências
e que o número de frequências utilizadas seja minimizado.
*/

int main() {
    // podemos modelar o problema como um problema de grafos
    // onde cada antena é um vertice e a interferencia entre antenas são arestas
    int n, m;
    cin >> n >> m;
    vector<pair<int, int>> edge;
    for(int i = 0; i < m; i++){
        int u, v;
        cin >> u >> v;
        u--; v--;
        edge.push_back({u, v});
    }

    IloEnv env;
    try {
        IloModel model(env);

        
        // Variáveis

        IloArray<IloBoolVarArray> X(env, n); // se x tem a cor c, no pior caso temos n cores
        for (int i = 0; i < n; i++) {
            X[i] = IloBoolVarArray(env, n); 
        }
        IloBoolVarArray y(env, n); // se a cor c está sendo usada
       
        // Objetivo
        IloExpr sum(env);
        for (int c = 0; c < n; c++) {
            sum += y[c];
        }
        model.add(IloMinimize(env, sum));

        // Restrições
        for(int i = 0; i < n; i++){ // cada vertice tenha apenas uma cor
            IloExpr qntColor(env);
            for(int c = 0; c < n; c++){
                qntColor += X[i][c];
            }
            model.add(qntColor == 1);

            qntColor.end();
        }

        for(auto [u, v] : edge){ // garantir que ambos nao tenham a mesma cor
            for(int c = 0; c < n; c++){
                cout << u << " " << v << endl;
                model.add(X[u][c] + X[v][c] <= y[c]); // força para o model que Yc precisa ser preenchido
            }
        }
        
        // Solução
        IloCplex cplex(model);
        if (cplex.solve()) {
            for(int i = 0; i < n; i++){
                cout << i+1 << ": ";
                for(int c = 0; c < n; c++){
                    if(cplex.getValue(X[i][c]) > 0.5){
                        cout << c+1 << endl;
                        break;
                    }
                }
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
