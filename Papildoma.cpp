#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>
#include <deque>

using std::string;
using std::deque;
using std::vector;
using std::cerr;
using std::endl;
using namespace std::chrono;

struct stud{
string vardas;
string pavarde;
int egz;
vector<int> nd;
double vidurkis;
double mediana;
};

void nuskaitymasFaile(int& sk, vector<stud>& studentai){
    std::ifstream in("sugeneruota10000.txt");
    string eil;
    if(!in.good()){
        cerr << "Klaida. Toks failas neegzistuoja" << endl;
        exit(1);
    }
    int temp;
    sk = 0;
        while(in >> eil){
            stud studentas;
            studentas.vidurkis = 0;
            studentas.vardas = eil;
            in >> studentas.pavarde;
            while(in.peek()!='\n' && in >> temp){
                if(in.fail() || temp <= 0) {
                    cerr << "Klaida. Patikrinkite faila" << endl;
                    exit(1);
                }
                studentas.nd.push_back(temp);
                studentas.vidurkis += temp;
            }
            studentas.egz = studentas.nd[studentas.nd.size()-1];
            studentas.nd.pop_back();
            studentas.vidurkis = studentas.vidurkis/studentas.nd.size()*0.4 + studentas.egz*0.6;
            studentai.push_back(studentas);
            sk++;
        }
        in.close();
}

void nuskaitymasFaile(int& sk, deque<stud>& studentai){
    std::ifstream in("sugeneruota10000.txt");
    string eil;
    if(!in.good()){
        cerr << "Klaida. Toks failas neegzistuoja" << endl;
        exit(1);
    }
    int temp;
    sk = 0;
        while(in >> eil){
            stud studentas;
            studentas.vidurkis = 0;
            studentas.vardas = eil;
            in >> studentas.pavarde;
            while(in.peek()!='\n' && in >> temp){
                if(in.fail() || temp <= 0) {
                    cerr << "Klaida. Patikrinkite faila" << endl;
                    exit(1);
                }
                studentas.nd.push_back(temp);
                studentas.vidurkis += temp;
            }
            studentas.egz = studentas.nd[studentas.nd.size()-1];
            studentas.nd.pop_back();
            studentai.push_back(studentas);
            sk++;
        }
        in.close();
}

bool gavoSkola(stud studentas){
    if(studentas.vidurkis < 5.0) return true;
    else return false;
}

vector<stud> raskMinkstus(vector<stud>& studentai) {
    vector<stud> minksti;
    vector<stud>::size_type i = 0;
    // invariantas: vektoriaus `studentai` elementai [0, i) yra "kieti"
    while (i != studentai.size()) {
      if (gavoSkola(studentai[i])) {
        minksti.push_back(studentai[i]);
        studentai.erase(studentai.begin() + i);  // iðtrinti i-àjá stud.
      } else
        ++i;  // pereiti prie kito studento
    }
    return minksti;  // gràþina studentus gavusius skolà
}

vector<stud> iterpkKietus(vector<stud>& studentai){
    vector<stud> minksti;
    vector<stud>::size_type i = 0;
    vector<stud>::size_type it = 0;
    while (i != studentai.size()) {
      if (gavoSkola(studentai[i])) {
        minksti.push_back(studentai[i]);
      } else{
        studentai[it] = studentai[i];
        ++it;
      }
      ++i;
    }
    studentai.resize(it);
    studentai.shrink_to_fit();
    return minksti;
}

deque<stud> iterpkKietus(deque<stud>& studentai){
    deque<stud> minksti;
    int it = 0;
    int n = studentai.size();
    for(int i = 0; i < n; i++) {
      if(gavoSkola(studentai[i])){
        minksti.push_back(studentai[i]);
      } else{
        studentai.push_front(studentai[i]);
        ++it;
      }
    }
    studentai.resize(it);
    studentai.shrink_to_fit();
    return minksti;
}

int main(){
    ///Vector radimas

    vector<stud> studentai;
    int sk = -1;

    auto start = high_resolution_clock::now();
    nuskaitymasFaile(sk, studentai);

    vector<stud> vargs = raskMinkstus(studentai);

    auto ending = high_resolution_clock::now();
    duration<double> diff = ending-start;
    std::cout << sk << " studentu isskirstymas su vector raskMinkstus() uztruko: " << diff.count() << "s" << endl;


    ///Vector iterpimas

    vector<stud> studentaiVect;
    sk = -1;

    start = high_resolution_clock::now();
    nuskaitymasFaile(sk, studentaiVect);

    vector<stud> vargsVect = iterpkKietus(studentaiVect);

    ending = high_resolution_clock::now();
    duration<double> diffv = ending-start;
    std::cout << sk << " studentu isskirstymas su vector iterpkKietus() uztruko: " << diffv.count() << "s" << endl;

    ///Deque iterpimas

    deque<stud> studentaiDequ;
    sk = -1;

    start = high_resolution_clock::now();
    nuskaitymasFaile(sk, studentaiDequ);

    deque<stud> vargsDequ = iterpkKietus(studentaiDequ);

    ending = high_resolution_clock::now();
    duration<double> diffd = ending-start;
    std::cout << sk << " studentu isskirstymas su deque iterpkKietus() uztruko: " << diffd.count() << "s" << endl;
}
