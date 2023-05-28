#include <bits/stdc++.h>
#include <typeinfo>

using namespace std;

ifstream f("gramatica.in");
ofstream g("cuvinte.out");


class IndexError : public exception {
public:
    const char* what() const throw(){
        return "Index invalid!";
    }
};


class Grammar {
private:
    map<char, vector<string>> productii;
    int nrNeterminale;

public:
    Grammar();
    int getNrTerminale() const {return this->nrNeterminale;}
    map<char, vector<string>> getProductii() const {return this->productii;}
    friend istream& operator >> (istream&, Grammar&);
    friend ostream& operator << (ostream& out, const Grammar&);
};

Grammar::Grammar(){
    this->nrNeterminale = 0;
}

istream& operator >> (istream& in, Grammar& obj){
    if(typeid(in) == typeid(istream)){
        cout << "Numarul de neterminale: ";
        in >> obj.nrNeterminale;

        for(int i = 1; i <= obj.nrNeterminale; ++i){
            char neterminala;
            cout << "Simbolul neterminalei: ";
            in >> neterminala;
            cout << "Productiile neterminalei: ";
            string prod;
            in >> prod;
            stringstream ss(prod);
            while(getline(ss, prod, '|')){
                obj.productii[neterminala].push_back(prod);
            }
        }
    }
    else if(typeid(in) == typeid(ifstream)){
        in >> obj.nrNeterminale;
        for(int i = 1; i <= obj.nrNeterminale; ++i){
            char neterminala;
            in >> neterminala;
            string prod;
            in >> prod;
            stringstream ss(prod);
            while(getline(ss, prod, '|')){
                obj.productii[neterminala].push_back(prod);
            }
        }
    }
    return in;
}

ostream& operator << (ostream& out, const Grammar& obj){
    for(auto line : obj.productii){
        out << line.first << " -> ";
        for(int i = 0; i < line.second.size(); ++i){
            if(i < line.second.size() - 1){
                out << line.second[i] << " | ";
            }
            else{
                out << line.second[i] << "\n";
            }
        }
    }
    return out;
}


class Meniu {
private:
    static Meniu* obiect;
    vector<string> cuvinte;
    int lungimeCuv;
    Grammar gramatica;
    Meniu(){
        this->lungimeCuv = 0;
        this->cuvinte = {};
    }
    Meniu(const Meniu&) = delete;
    ~Meniu() = default;

public:
    static Meniu* getInstance(){
        if(obiect == NULL){
            obiect = new Meniu();
        }
        return obiect;
    }
    void read();
    void start();
    bool finish(string);
    void back(char, string, map<char, vector<string>>);
    char findNeterminal(string);
    string deleteNeterminal(string);
    void getResults();
};

Meniu* Meniu::obiect = 0;

void Meniu::read(){
    cout << "Alegeti optiunea de input:\n 1 -> Tastatura\n 2 -> Fisier\n";
    int response;
    while(true){
        cout << "Raspuns: ";
        cin >> response;
        try{
            if(response < 1 || response > 2){
                throw IndexError();
            }
            else{
                break;
            }
        }
        catch(const IndexError& error){
            cout << error.what() << "\n";
        }
    }
    if(response == 1){
        cin >> gramatica;
        cout << "Lungimea cuvintelor: ";
        cin >> this->lungimeCuv;
    }
    else{
        f >> gramatica;
        f >> this->lungimeCuv;
    }
}

void Meniu::start(){
    read();
    map<char, vector<string>> mp = this->gramatica.getProductii();
    for(auto productie : mp['S']){
        char neterminal = findNeterminal(productie);
        back(neterminal, productie, mp);
    }
    getResults();
}

void Meniu::back(char neterminal, string cuvant, map<char, vector<string>> productii){
    if(cuvant.size() == this->lungimeCuv && findNeterminal(cuvant) == '0'){
        this->cuvinte.push_back(cuvant);
        return;
    }
    else if(cuvant.size() == this->lungimeCuv + 1 && findNeterminal(cuvant) != '0'){
        char neterminal = findNeterminal(cuvant);
        bool lambda = false;
        for(auto productie : productii[neterminal]){
            if(productie == "/"){
                lambda = true;
            }
        }
        if(lambda){
            this->cuvinte.push_back(deleteNeterminal(cuvant));
        }
        return;
    }
    else{
        for(auto productie : productii[neterminal]){
            string cand = "";
            if(productie == "/"){
                cand = deleteNeterminal(cuvant);
                if(cand.size() == this->lungimeCuv){
                    this->cuvinte.push_back(cand);
                }
            }
            else{
                for(auto simbol : cuvant){
                    if(simbol >= 'A' && simbol <= 'Z'){
                        cand += productie;
                    }
                    else{
                        cand += simbol;
                    }
                }
                char neterminal = findNeterminal(productie);
                back(neterminal, cand, productii);
            }
        }
    }
}

bool Meniu::finish(string cuvant){
    for(auto simbol : cuvant){
        if(simbol >= 'A' && simbol <= 'Z'){
            return false;
        }
    }
    return true;
}

char Meniu::findNeterminal(string cuvant){
    for(auto simbol : cuvant){
        if(simbol >= 'A' && simbol <= 'Z'){
            return simbol;
        }
    }
    return '0';
}

string Meniu::deleteNeterminal(string cuvant){
    string res = "";
    for(auto simbol : cuvant){
        if(simbol >= 'A' && simbol <= 'Z'){
            continue;
        }
        res += simbol;
    }
    return res;
}

void Meniu::getResults(){
    for(auto cuvant : this->cuvinte){
        g << cuvant << "\n";
    }
}

int main(){
    Meniu* m = Meniu::getInstance();
    m->start();
    return 0;
}