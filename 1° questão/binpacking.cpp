#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <cmath>
#include <random>

using namespace std;
using namespace chrono;

int capacity = 0;

struct Item {
    int id;
    int size;
};

struct Bin {
    vector<Item> items;
    int total = 0;
};

using Solution = vector<Bin>;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());


// ---------- BFD: Best-Fit Decreasing ----------
Solution bestFitDecreasing(vector<Item> items) {
    sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
        return a.size > b.size;
    });
    Solution bins;
    for (const auto& item : items) {
        int best = -1;
        int min_space = capacity*2;

        for (int i = 0; i < bins.size(); i++) {
            int space = capacity - bins[i].total;
            if (item.size <= space && space < min_space) {
                min_space = space;
                best = i;
            }
        }

        if (best == -1) {
            bins.push_back(Bin());
            bins.back().items.push_back(item);
            bins.back().total += item.size;
        } else {
            bins[best].items.push_back(item);
            bins[best].total += item.size;
        }
    }
    return bins;
}


int cnt = 0;
// ---------- Busca Local ----------
bool localSearch(Solution& bins) {
    for (int i = 0; i < bins.size(); ++i) {
        for (int j = 0; j < bins.size(); ++j) {
            if (i == j) continue;

            for (int k = 0; k < bins[i].items.size(); ++k) {
                Item item = bins[i].items[k];
                if (bins[j].total + item.size <= capacity) {
                    bins[j].items.push_back(item);
                    bins[j].total += item.size;

                    bins[i].total -= item.size;
                    bins[i].items.erase(bins[i].items.begin() + k);

                    if (bins[i].items.empty()) {
                        bins.erase(bins.begin() + i);
                        cnt++;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}


// ---------- Eliminação progressiva de bins & LS ----------
void ILS(Solution& bins, steady_clock::time_point deadline) {
    Solution current = bins;

    while (steady_clock::now() < deadline) {
        if(current.size() == 1) return;

        int i = rng() % current.size(), j=i;

        while(j==i) j = rng() % current.size();

        Bin removed = current[i];
        Bin removed2 = current[j];

        Solution new_bins = current;

        new_bins.erase(new_bins.begin() + i);
        new_bins.erase(new_bins.begin() + j);

        for(auto &item : removed2.items){
            removed.items.push_back(item);
        }

        shuffle(removed.items.begin(), removed.items.end(), rng);
    
        bool success = true;
        for (auto& item : removed.items) {
            bool placed = false;
            for (auto& bin : new_bins) {
                if (bin.total + item.size <= capacity) {
                    bin.items.push_back(item);
                    bin.total += item.size;
                    placed = true;
                    break;
                }
            }
            if (!placed) {
                new_bins.push_back(Bin());
                new_bins.back().items.push_back(item);
                new_bins.back().total += item.size;
                success = false;
            }
        }
        
        if(localSearch(new_bins)){
            current = new_bins;
        }

        if (bins.size() > new_bins.size()) {
            bins = new_bins;
            current = new_bins;
        }
    }
}


// ---------- Programa principal ----------
int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Uso: ./binpacking <tempo_limite_segundos>\n";
        return 1;
    }

    int time_limit = atoi(argv[1]);
    int n;
    cin >> n >> capacity;

    vector<Item> items(n);
    for (int i = 0; i < n; i++) {
        cin >>  items[i].size;
        items[i].id = i;
    }

    auto deadline = steady_clock::now() + seconds(time_limit);

    Solution bins = bestFitDecreasing(items);
    shuffle(bins.begin(), bins.end(), rng);
    ILS(bins, deadline);
    
    cout << "Número de bins usados: " << bins.size() << "\n";
    for (int i = 0; i < bins.size(); ++i) {
        cout << "Bin " << i + 1 << "(" << bins[i].total << "): ";
        for (auto& item : bins[i].items)
        cout << item.id<< "(" << item.size << "), ";
        cout << "\n";
    }
    cout << "Número de bins usados: " << bins.size() << "\n";
    return 0;
}
