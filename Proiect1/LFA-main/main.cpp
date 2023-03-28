#include <bits/stdc++.h>

using namespace std;

ifstream fin("proiect1.in");

const int NMAX = 1e5 + 5;
vector<pair<int, char>> G[NMAX];
vector<int> drum_DFA;
vector<vector<int>> drum_NFA;
bool stare_finala[NMAX];
bool acceptat_NFA;


// DFA
bool check_DFA(string s){
    int curr = 1;
    drum_DFA.push_back(curr);
    int i = 0;
    while(i < s.size()){
        bool ok = false;
        for(auto legatura : G[curr]){
            if(legatura.second == s[i]){
                drum_DFA.push_back(legatura.first);
                ok = true;
                curr = legatura.first;
                break;
            }
        }
        if(!ok){
            return false;
        }
        i++;
    }
    if(stare_finala[curr]){
        return true;
    }
    return false;
}

// NFA
void check_NFA(vector<int> drum, string s, int curr, int poz_cuv){
    if(poz_cuv == s.size()){
        if(stare_finala[curr]){
            acceptat_NFA = true;
            drum_NFA.push_back(drum);
        }
        return;
    }
    else{
        for(auto nod : G[curr]){
            if(nod.second == s[poz_cuv]){
                drum.push_back(nod.first);
                check_NFA(drum, s, nod.first, poz_cuv + 1);
                drum.pop_back();
            }
        }
    }
}


int main(){
    int nrStariFinale;
    fin >> nrStariFinale;
    for(int i = 0; i < nrStariFinale; ++i){
        int stare;
        fin >> stare;
        stare_finala[stare] = true;
    }
    int x, y;
    char muchie;
    while(fin >> x >> y >> muchie){
        G[x].push_back({y, muchie});
    }
    cout << "Proiect 1 Limbaje Formale si Automate\n";
    cout << "Instructiuni:\n - 1 = stare initiala implicit\n - automatul este introdus in fisierul de intrare\n - pentru cuvantul vid se introduce LAMBDA la tastatura\n - pentru finalizarea testarii cuvintelor se introduce comanda .bye\n\n";
    int cer;
    cout << "Alege tipul de automat finit introdus\n";
    cout << "1 -> Determinist\n";
    cout << "2 -> Nederminist\n";
    cin >> cer;
    string cuvant;
    while(cin >> cuvant){
        if(cuvant == ".bye"){
            break;
        }
        else if(cuvant == "LAMBDA"){
            if(!stare_finala[1]){
                cout << "neacceptat\n\n";
            }
            else{
                cout << "acceptat\n\n";
            }
        }
        else{
            if(cer == 1){
                if(check_DFA(cuvant)){
                    cout << "acceptat\n";
                    for(auto i : drum_DFA){
                        cout << "q" << i << " -> ";
                    }
                    cout << "finish\n\n";
                }
                else{
                    cout << "neacceptat\n\n";
                }
                drum_DFA.clear();
            }
            else{
                vector<int> drum;
                drum.push_back(1);
                check_NFA(drum, cuvant, 1, 0);
                if(!acceptat_NFA){
                    cout << "neacceptat\n\n";
                }
                else{
                    cout << "acceptat\n";
                    for(auto drum : drum_NFA){
                        for(auto nod : drum){
                            cout << "q" << nod << " -> ";
                        }
                        cout << "finish\n";
                    }
                    cout << "\n";
                    acceptat_NFA = false;
                }
                drum_NFA.clear();
            }
        }
    }
    return 0;
}