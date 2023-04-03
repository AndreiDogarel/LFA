#include <bits/stdc++.h>

using namespace std;

ifstream f("proiect.in");
ofstream g("proiect.out");

const int NMAX = 1e5 + 5;

vector<pair<int, char>> Lambda_NFA[NMAX];
vector<pair<int, char>> NFA[NMAX];
vector<int> Inchidere_Lambda[NMAX];
map<string, vector<pair<string, char>>> DFA;
vector<char> alfabet;
bool stareFinalaLambdaNFA[NMAX];
map<string, bool> stareFinalaDFA;
int n, nrStariFinale;


void lambdaCompletion(){
    for(int nod = 0; nod < n; ++nod){
        bool ok = false;
        for(auto i : Inchidere_Lambda[nod]){
            pair<int, char> p = {i, '/'};
            if(i != nod && !count(Lambda_NFA[nod].begin(), Lambda_NFA[nod].end(), p)){
                Lambda_NFA[nod].push_back(p);
            }
            if(stareFinalaLambdaNFA[i]){
                ok = true;
            }
        }
        if(ok){
            stareFinalaLambdaNFA[nod] = true;
        }
    }
}


void lambdaTransRemoval(){
    for(int nod = 0; nod < n; ++nod){
        for(auto i : Lambda_NFA[nod]){
            if(i.second == '/'){
                for(auto j : Lambda_NFA[i.first]){
                    if(j.second != '/' && !count(NFA[nod].begin(), NFA[nod].end(), j)){
                        NFA[nod].push_back(j);
                    }
                }
            }
        }
    }
}


int main(){
    string alfa;
    f >> alfa;
    for(int i = 0; i < alfa.size(); ++i){
        alfabet.push_back(alfa[i]);
    }
    f >> n;
    f >> nrStariFinale;
    while(nrStariFinale){
        int stare;
        f >> stare;
        stareFinalaLambdaNFA[stare] = true;
        nrStariFinale--;
    }
    int stare1, stare2;
    char litera;
    while(f >> stare1 >> stare2 >> litera){
        Lambda_NFA[stare1].push_back({stare2, litera});         /// ** Litera lambda va fi notata cu '/' ** ///
        if(litera != '/'){
            NFA[stare1].push_back({stare2, litera});
        }
    }
    //////////////// Lambda-NFA -> NFA ////////////////
    for(int nod = 0; nod < n; ++nod){
        Inchidere_Lambda[nod].push_back(nod);
        queue<int> q;
        q.push(nod);
        while(!q.empty()){
            int curr = q.front();
            q.pop();
            for(auto i : Lambda_NFA[curr]){
                if(i.second == '/' && !count(Inchidere_Lambda[nod].begin(), Inchidere_Lambda[nod].end(), i.first)){
                    Inchidere_Lambda[nod].push_back(i.first);
                    q.push(i.first);
                }
            }
        }
    }
    lambdaCompletion();
    lambdaTransRemoval();
    /////////////////////////////////////////////

    //////////////// NFA -> DFA ////////////////
    map<string, bool> used;
    used["q0"] = true;
    pair<vector<int>, string> nod = {{0}, "q0"};
    if(stareFinalaLambdaNFA[0]){
        stareFinalaDFA["q0"] = true;
    }
    queue<pair<vector<int>, string>> q;
    q.push(nod);
    while(!q.empty()){
        pair<vector<int>, string> p = q.front();
        q.pop();
        for(auto nod : p.first){
            if(stareFinalaLambdaNFA[nod]){
                stareFinalaDFA[p.second] = true;
            }
        }
        for(auto litera : alfabet){
            vector<int> mult;
            string stare = "q";
            for(auto nod : p.first){
                for(auto leg : NFA[nod]){
                    if(leg.second == litera && !count(mult.begin(), mult.end(), leg.first)){
                        mult.push_back(leg.first);
                    }
                }
            }
            if(mult.size()){
                sort(mult.begin(), mult.end());
                for(auto i : mult){
                    stare += to_string(i) + ",";
                }
                stare.pop_back();
                DFA[p.second].push_back({stare, litera});
                if(!used[stare]){
                    q.push({mult, stare});
                    used[stare] = true;
                }
            }
            else{
                DFA[p.second].push_back({"-", litera});
            }
        }
    }
    for(auto stare : DFA){
        g << stare.first;
        if(stareFinalaDFA[stare.first]){
            g << " -> Stare finala";
        }
        g << "\n";
        for(auto legaturi : stare.second){
            g << legaturi.first << " " << legaturi.second << "\n";
        }
        g << "\n";
    }
    ///////////////////////////////////////////
    
    return 0;
}