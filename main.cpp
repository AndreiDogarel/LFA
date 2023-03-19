#include <bits/stdc++.h>

using namespace std;

ifstream fin("proiect1.in");

const int NMAX = 1e5 + 5;
int n;
vector<pair<int, char>> G[NMAX];
vector<int> drum, drum_NFA;
bool stare_finala[NMAX];
bool acceptat_NFA;


// DFA
bool check_DFA(string s){
    int curr = 1;
    drum.push_back(curr);
    int i = 0;
    while(i < s.size()){
        bool ok = false;
        for(auto legatura : G[curr]){
            if(legatura.second == s[i]){
                drum.push_back(legatura.first);
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
void check_NFA(string s, int curr, int poz_cuv){
    drum_NFA.push_back(curr);
    for(auto tranzitie : G[curr]){
        if(s[poz_cuv] == tranzitie.second){
            check_NFA(s, tranzitie.first, poz_cuv + 1);
            drum_NFA.pop_back();
        }
    }
    if(poz_cuv == s.size() && stare_finala[curr]){
        acceptat_NFA = true;
        cout << "acceptat\n";
        for(auto i : drum){
            cout << i << " ";
        }
        cout << "\n\n";
    }
}


int main(){
    fin >> n;
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
        else{
            if(cer == 1){
                if(check_DFA(cuvant)){
                    cout << "acceptat\n";
                    for(auto i : drum){
                        cout << i << " ";
                    }
                    cout << "\n\n";
                }
                else{
                    cout << "neacceptat\n\n";
                }
            }
            else{
                check_NFA(cuvant, 1, 0);
                if(!acceptat_NFA){
                    cout << "neacceptat\n\n";
                }
            }
        }
    }
    return 0;
}