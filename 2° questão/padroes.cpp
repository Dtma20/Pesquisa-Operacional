#include <ilcplex/ilocplex.h>
#include <vector>
#include <iostream>

using namespace std;

/*
– Uma fábrica de latinhas possui 4 padrões de impressão em folhas de metal
(existem 2 tipos de folhas de metal diferentes).
– A fábrica possui 200 folhas de metal de tam 1 e 90 de tam 2.
– Cada latinhas é vendida a 50 u.
– Cada corpo não utilizado possui um custo de estocagem de 50 u e cada tampa
custa 3 u.
– Quantas impressões de cada padrão devem ser feitas para maximizar o lucro?*/

int main() {
    IloEnv env;
    try {
        IloModel model(env);

        
        // Variáveis

        IloIntVar x1(env), x2(env), x3(env), x4(env);
        IloIntVar y(env); // qnt de latas

        // Objetivo
        model.add(IloMaximize(env,
              50*y // quantidade de latas
            - 5*(x1+ 2*x2 + 4*x4 - y) //o que sobrou de corpo
            - 3*(7*x1 + 3*x2 + 9*x3+ 4*x4 - 2*y) // oque sobrou de tampa
        ));

        // Restrições
        model.add(x1 + x3 + x4 <= 200);
        model.add(x2 <= 90); 

        model.add(y <= (x1+ 2*x2 + 4*x4)); // y precisa ser menor ou igual a quantida de corpos
        model.add(y <= (7*x1 + 3*x2 + 9*x3+ 4*x4)/2); // quantidade de latas tem que se menor que quantidade de tampas/2

        model.add(x1 >= 0); 
        model.add(x2 >= 0); 
        model.add(x3 >= 0); 
        model.add(x4 >= 0);
        
        // Solução
        IloCplex cplex(model);
        if (cplex.solve()) {
            cout << "latas: " << cplex.getValue(y) << endl;
            cout << "tam1: " << cplex.getValue(x1) << endl;
            cout << "tam2: " << cplex.getValue(x2) << endl;
            cout << "tam3: " << cplex.getValue(x3) << endl;
            cout << "tam4: " << cplex.getValue(x4) << endl;
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
