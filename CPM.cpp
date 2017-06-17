#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// program liczy czas trwania najdluzszej sciezki, dla danych posortowanych topologicznie
// oraz dla danych nietopologicznych
// liczy rowniez ES EF LS LF dla danych topologicznych i nietopologicznych

    //TESTY - SORT
    //data 10 tak
    //data 20 tak
    //data 30 tak
    //data 40 tak
    //data 50 tak
    //data 60 tak
    //data 70 tak
    //data 80 tak

    //TESTY - NIESORT
    //dataN 10 tak
    //dataN 20 tak
    //dataN 30 tak
    //dataN 40 tak
    //dataN 50 tak
    //dataN 60 tak
    //dataN 70 tak
    //dataN 80 tak



    //zrodlo danych: http://mariusz.makuchowski.staff.iiar.pwr.wroc.pl/download/courses/komputerowe.wspomaganie.zarzadzania/lab.instrukcje/lab1.cpm/cpm.data/
class Zadanie
{
public:
    int id;
    int ES;
    int EF;
    int LS;
    int LF;
    int czas;
    vector <int> poprzednik;
    vector <int> nastepny;

};

int main ()
{
    int N,M;  // N-liczba zadan, M-liczba polaczen
    ifstream plik;

    //optymistycznie zakladam ze plik jest
    plik.open( "data10.txt" );
    plik>>N;
    plik>>M;

    //wgraj id zadania
    Zadanie Proces[N];
    for(int i=0; i<N; i++)
    {
        plik>>Proces[i].czas;
        Proces[i].id=i;
    }

    int polaczenie1;
    int polaczenie2;

    //wczytaj plik, tablica nastepnych i poprzednich
    for(int i=0; i<M; i++)
    {
        plik>>polaczenie1;
        plik>>polaczenie2;
        //bo numeracja od zera
        Proces[polaczenie1-1].nastepny.push_back(polaczenie2-1);
        Proces[polaczenie2-1].poprzednik.push_back(polaczenie1-1);
    }

    //zrob kopie
    Zadanie Procesy1[N];
    Zadanie Procesy2[N];
    for(int i=0; i<N; i++)
        Procesy1[i]=Proces[i];

    //wczytaj do wektroa G zadania
    vector <Zadanie> G;
    for (int i=0; i<N; i++)
    {
        G.push_back(Proces[i]);
    }

    //wektor rozwiazan - wynik topologiczny
    vector <int> g;

    //tymczasowy, malo istotny
    Zadanie a;

    //G jest tablica przechowujaca zadania
    //znajduje ulozenie topologiczne
    while(!G.empty())
    {

        for(size_t i=0; i<G.size(); i++)
        {
            a=G[i];

            //jesli zadanie juz bylo sciagniete, sprawdz
            for(int j=0; j<a.poprzednik.size(); j++)
            {
                for(size_t k=0; k<g.size(); k++)
                {
                    //sprawdz czy wystepuje w wektorze g, jesli tak to usun z G
                    if(g[k]==a.poprzednik[j])
                    {
                        a.poprzednik.erase(a.poprzednik.begin()+j);
                        G[i]=a;
                    }
                }
            }

            //jesli porzednicy pusci to wstaw je do wektora topologicznego
            if(a.poprzednik.empty())
            {
                g.push_back(a.id);
                G.erase(G.begin()+i);
            }
        }

    }

    //teraz uloz topologicznie w "grafie" w "kolejce"
    for(int i=0; i<N; i++)
    {
        Procesy2[i]=Procesy1[g[i]];
    }

    //early start
    int nrPoprzednika=0;
    int maxEF=0;
    int maxNrPoprzednika=0;
    for(int i=0; i<N; i++)
    {
        int ID;
        //pierwszy
        if(Procesy2[i].poprzednik.empty())
        {
            Procesy2[i].ES=0;
            Procesy2[i].EF=Procesy2[i].czas;
        }
        else
        {
            //jesli jest wiecej niz jeden poprzednik
            for(size_t j=0; j<Procesy2[i].poprzednik.size(); j++)
            {
                //numer poprzednika
                ID=Procesy2[i].poprzednik[j];
                for (int i=0; i<N; i++)
                    //znajdz pasujace numery id
                    if(Procesy2[i].id==ID)
                        //polcz maxEF
                        if(Procesy2[i].EF>maxEF)
                        {
                            maxEF=Procesy2[i].EF;
                        }
            }
            Procesy2[i].ES=maxEF;//Procesy2[maxNrPoprzednika].EF;
            Procesy2[i].EF=Procesy2[i].ES+Procesy2[i].czas;
            maxNrPoprzednika=0;
            maxEF=0;
        }

    }

    //policz process time
    int MAX=0;
    for( int i=0; i<N; i++)
    {
        if(Procesy2[i].EF>MAX)
            MAX=Procesy2[i].EF;
    }

    //late start
    int index=0;
    int mini=0;
    int c=0;
    int max_EF=MAX;
    mini=max_EF;
    for(int b=N-1; b>=0; b--)
    {
        if(Procesy2[b].nastepny.empty())
        {
            Procesy2[b].LF=max_EF;
            Procesy2[b].LS=Procesy2[b].LF-Procesy2[b].czas;
        }
        else
        {

            mini=max_EF;
            for(int d=0; d<Procesy2[b].nastepny.size(); d++)
            {
                index=Procesy2[b].nastepny[d]; //numer poprzednika
                for (int i=0; i<N; i++)
                    if(Procesy2[i].id==index)
                        if(Procesy2[i].LS<mini)
                        {
                            mini=Procesy2[i].LS;
                            //maxNrPoprzednika=nrPoprzednika;
                        }
            }
            Procesy2[b].LF=mini;
            Procesy2[b].LS=Procesy2[b].LF-Procesy2[b].czas;

        }
    }

    //sciezka
    int totalFloat=0;
    vector <Zadanie> total;
    for(int i=0; i<N; i++)
    {
        totalFloat=Procesy2[i].LS-Procesy2[i].ES;
        if(totalFloat==0)
        {
           // cout<<Procesy2[i].id;
           total.push_back(Procesy2[i]);
        }

    }

    //sortowanie bombelkowe po id, zeby zachowac kolejnosc wyswietlania
    for(int i=0; i<N; i++)
        for(int j=1; j<N-i; j++)
            if(Procesy2[j-1].id>Procesy2[j].id)
                swap(Procesy2[j-1], Procesy2[j]);

    //wyswietl
    cout<<endl;
    cout<<"Proces time:"<<MAX<<endl<<endl;
    cout << "NR: ES: EF: LS: LF"<<endl;
    for(int j=0; j<N; j++)
    {
        cout << Procesy2[j].id+1 << " " << Procesy2[j].ES << " " << Procesy2[j].EF<<" "<< Procesy2[j].LS << " " << Procesy2[j].LF <<endl;
    }

    cout<<endl<<"Sciezka Krytyczna:"<<endl;
    cout << "NR: ES: EF:"<<endl;
    for(int i=0; i<total.size(); i++)
    cout<<total[i].id+1<<" "<<total[i].ES<<" "<<total[i].EF<<endl;

    return 0;
}
