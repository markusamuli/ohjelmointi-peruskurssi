// Markus Ryöti
// Ohjelmalla ovi käydä läpi kauppojen ja kauppaketjujen valikoimia ja tarkastella muun muassa hintoja. Data ladataan aluksi products.txt tiedostosta.

#include <iomanip>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>


using namespace std;


struct Tuote {
    string tuotenimi;
    double hinta;
};

// split funktio, jota käytetty aiemmin viikkoharjoituksessa
// ottaa parametrina merkkijonon, erotinmerkin ja totuusarvon sille,
// huomioidaanko tyhjiä rivejä
vector<string> split(const string& s, const char delimiter,
                     bool ignore_empty = false){

    vector<string> result;
    string tmp = s;

    while(tmp.find(delimiter) != string::npos)
    {
        string new_part = tmp.substr(0, tmp.find(delimiter));
        tmp = tmp.substr(tmp.find(delimiter)+1, tmp.size());
        if(not (ignore_empty and new_part.empty()))
        {
            result.push_back(new_part);
        }
    }
    if(not (ignore_empty and tmp.empty()))
    {
        result.push_back(tmp);
    }
    return result;
}

// funktio lukee tiedoston käyttäjältä
// samassa funktiossa on toteutettuna myös tietorakenteeseen tallennus
bool tiedostonLuku(map<string, map<string, vector<Tuote> >>& data) {

    string tiedostoNimi = "";
    cout << "Input file: ";
    getline(cin, tiedostoNimi);

    ifstream tiedosto(tiedostoNimi);

    if ( not tiedosto ) {
        cout << "Error: the input file cannot be opened" << endl;
        return false;
    }

    string rivi;

    while ( getline(tiedosto, rivi) ) {

        vector<string> osat = split(rivi, ';', true);

        if ( osat.size() != 4 ) {
            cout << "Error: the file has an erroneous line" << endl;
            return false;
        }

        string ketju = osat.at(0);
        string kauppa = osat.at(1);
        string tuote = osat.at(2);
        double hinta = -1;          // alustetaan -1, jotta voidaan tallentaa
                                    // kaikki hinnat double-arvona, myöhemmissä
                                    // funktioissa huomioitu toiminnassa

        if ( osat.at(3) != "out-of-stock" ) {
            hinta = stod(osat.at(3));
        }

        Tuote uusi_tuote = {tuote, hinta};      // tuotteen tietojen tallenta-
                                                // minen sen structiin

        if ( data.find(ketju) == data.end() ) {
            // jos ei entuudestaan löydetä, lisätään rakenteeseen
            data[ketju] = map<string, vector<Tuote>>();
        }

        if ( data.at(ketju).find(kauppa) == data.at(ketju).end() ) {
            // jos ei entuudestaan löydetä, lisätään rakenteeseen
            data[ketju][kauppa] = vector<Tuote>();
        }
        // lisätään tuote-struct kyseisen kaupan tuotevektoriin
        data.at(ketju).at(kauppa).push_back(uusi_tuote);
    }
    tiedosto.close();
    return true;
}

// funktio tulostaa läpi kaikki kauppaketjut
void tulostaKetjut(const map<string, map<string, vector<Tuote> >>& data) {
    for(auto ketju : data) {
        // ketju on map-rakenteinen, joten siitä otetaan vain avainarvo
        cout << ketju.first << endl;
    }
}

// funktio tulostaa läpi kaikki kaupat sen mukaan, mikä kauppaketju
// annetaan parametrina
void tulostaKaupat(const map<string, map<string, vector<Tuote> >>& data,
                   const string ketjunNimi) {

    if(data.find(ketjunNimi) == data.end()) {
        // jos ei löydetä, annetaan virheilmoitus
        cout << "Error: an unknown chain" << endl;
        return;
    }
    // jälleen kauppa on map-rakenteinen, otetaan vain avain
    for(auto kauppa : data.at(ketjunNimi)) {
        cout << kauppa.first << endl;
    }
}

// funktio tulostaa koko tuotevalikoiman kaupalle
// parametreina on kauppaketju ja itse kauppa
void tulostaValikoima(const map<string, map<string, vector<Tuote> >>& data,
                      const string ketjunNimi,
                      const string kaupanNimi) {

    if(data.find(ketjunNimi) == data.end()) {
        // jos kauppaketjua ei löydetä, annetaan virheilmoitus
        cout << "Error: unknown chain" << endl;
        return;
    }

    if(data.at(ketjunNimi).find(kaupanNimi) == data.at(ketjunNimi).end()) {
        // virheilmoitus annetaan myös, jos kauppaa ei löydetä
        cout << "Error: unknown store" << endl;
    }

    else {
        // käydään läpi mapin avulla, koska näin saadaan tulostettua
        // aakkosjärjestyksessä
        map<string, double> tuotetiedot = {};
        // käy läpi vektorin, alkioina Tuote structeja
        for(auto tuote : data.at(ketjunNimi).at(kaupanNimi)) {
            // lisätään arvot luotuun mappiin
            tuotetiedot[tuote.tuotenimi] = tuote.hinta;
        }
        // käydään läpi uudelleen luotu map
        for(auto tuote : tuotetiedot) {
            // -1 tarkoitti, että tuotetta ei saldossa
            if(tuote.second == -1) {
                // täten annetaan sen mukainen virheilmoitus
                cout << tuote.first << " "
                    << "out of stock" << endl;
            } else {
                // desimaalit kahden tarkkuudella, käytetty iomanip
                // kirjaston setprecisionia
                cout << tuote.first << " "    
                     << fixed << setprecision(2)
                     << tuote.second << endl;
            }
        }
    }
}

// funktio käy läpi jokaisen kaupan tuotevalikoiman ja etsii niistä kaikista
// halvimman, etsittävä tuote on annettu parametrina
void tulostaHalvin(const map<string, map<string, vector<Tuote> >>& data,
                   const string etsittavaTuote) {

    // halvimmalle hinnalle täytyy luoda muuttuja, jota ei voi alustaa nollaksi
    // kyseisen arvon kanssa voidaan olla varmoja että halvempi tuote löydetään
    double halvin = numeric_limits<double>::infinity();
    // saman hintaisia tuotteita voi olla useita, siksi niille
    // on luotu uusi säiliö
    set<string> halvimmat;
    int onValikoimassa = 0;     // poikkeustapausten kanssa myöhemmin
    // käydään läpi kauppaketjuja
    for(auto ketju : data) {
        // käydään läpi myymälöitä
        for(auto kauppa : data.at(ketju.first)) {
            // käydään läpi tuotevalikoiman vectoria
            for(auto tuote : kauppa.second) {
                // suoritetaan jos löydetään uusi halvempi hinta,
                // halvimmat joukko ensin tyhjennetään ja uusi hinta määritellään
                // lopuksi lisätään uusi kauppa halvimpien listaukseen
                if(tuote.tuotenimi == etsittavaTuote and
                        tuote.hinta < halvin and tuote.hinta > 0) {

                    halvin = tuote.hinta;
                    halvimmat.clear();
                    halvimmat.insert(ketju.first + " " + kauppa.first);
                    ++onValikoimassa;   // löydettiin valikoimasta, joten
                                        // kasvatetaan muuttujan arvoa
                }
                // jos tuotteen hinta on sama kuin jo olemassa olevan halvimman,
                // voidaan kauppa myös silloin lisätä samaan listaukseen
                else if(tuote.tuotenimi == etsittavaTuote and
                        tuote.hinta == halvin) {

                    halvimmat.insert(ketju.first + " " + kauppa.first);
                    ++onValikoimassa;   // löydettiin valikoimasta, joten
                                        // kasvatetaan muuttujan arvoa
                }
                // tuote löydettiin nimellä, joten se kuuluu myös valikoimaan
                // > kasvatetaan arvoa
                // halvimpien listalle ei kuitenkaan voida tällöin lisätä
                else if (tuote.tuotenimi == etsittavaTuote and
                         tuote.hinta == -1) {

                    ++onValikoimassa;
                }
            }
        }
    }

    // tuote kuuluu valikoimaan, mutta koska halvimpien listalle sitä ei ole
    //  voitu lisätä, annetaan sitä vastaava ilmoitus
    if(onValikoimassa > 0 and halvimmat.size() == 0) {
        cout << "The product is temporarily out of stock everywhere." << endl;
    }

    // jos millään luupin kierroksella ei löydetty tuotetta ja kasvatettu
    // onValikoimassa muuttujan arvoa, tiedätään, ettei se kuulu valikoimaan
    else if(onValikoimassa == 0) {
        cout << "Product is not part of product selection." << endl;
    }

    // jos kaikki onnistui niin kuin kuuluu, tulostetaan hinta ja kaupat
    // ohjeen mukaisesti
    else {
        // jälleen oikeilla desimaaleilla
        cout << fixed << setprecision(2) << halvin << " euros" << endl;
        for(auto kauppa : halvimmat) {
            cout << kauppa << endl;
        }
    }
}

// funktio käy läpi kaikkien kauppojen tuotteet, jotka lopuksi tulostetaan
void tulostaTuotteet(const map<string, map<string, vector<Tuote> >>& data) {
    // luodaan set rakenne, jolloin voidaan tulostaa aakkosjärjstyksessä, ja
    // listaukseen ei tule duplikaatteja
    set<string> tuotteet;

    for(auto ketju : data) {
        // käydään kauppojen valikoima läpi vastaavalla luupilla,
        // kuin edellisessä funktiossa
        for(auto kauppa : data.at(ketju.first)) {
            for(auto tuote : kauppa.second) {
                // lisätään uuteen settiin tuotteen nimi
                tuotteet.insert(tuote.tuotenimi);
            }
        }
    }

    for(auto tuote : tuotteet) {    // luupataan luotu set rakenne läpi
        cout << tuote << endl;
    }
}


int main() {

    // tehtävässä käytetty tietorakenne, map jonka arvona on toinen map
    // toisella mapilla arvoina on vectoria täynnä kunkin tuotteen aiemmin
    // määriteltyjä structeja
    map<string, map<string, vector<Tuote>>> data;

    // jos tiedoston luku ei onnistunut, keskeytetään ohjelma
    if(!tiedostonLuku(data)) {
        return EXIT_FAILURE;
    }


    // luupataan niin kauan, kunnes käyttäjä päättää quit komennolla
    while(true) {

        cout << "> ";
        string syote;
        getline(cin, syote);
        // käyttäjän syöte jaetaan osiin vectorin sisään split funktiolla
        vector<string> osat = split(syote, ' ');

        if(osat.size() == 0) {
            // virhe, jos käyttäjä ei anna tekstiä
            cout << "Error: unknown command" << endl;
            continue;
        }

        // ohjelma toimii komennoilla, jotka suorittavat aina oman funktionsa
        // komento on ensimmäinen osa käyttäjän kirjoittamasta tekstistä
        string komento = osat.at(0);


        // käyttäjän komennot jaettiin aiemmin vectoriin, nyt suoritetaan osan
        // mukainen komento erillisellä funktiolla ja lähetetään mahdollisesti
        // mukaan parametrina tarvittavia osia

        // tietorakenne on viety aina viitteellä funktioihin
        // jokaiselle komennolle täytyy tehdä virhetarkastelu
        if(komento == "quit") {     // keskeytään ohjelma
            return EXIT_SUCCESS;
        }
        else if(komento == "chains") {
            if(osat.size() > 1) {
                cout << "Error: error in command chains" << endl;
            } else {
                tulostaKetjut(data);
            }
        }
        else if(komento == "stores") {
            if(osat.size() != 2) {
                cout << "Error: error in command stores" << endl;
            } else {
                string ketju = osat.at(1);
                tulostaKaupat(data, ketju);
            }
        }
        else if(komento == "selection") {
            if(osat.size() != 3) {
                cout << "Error: error in command selection" << endl;
            } else {
                string ketju = osat.at(1);
                string kauppa = osat.at(2);
                tulostaValikoima(data, ketju, kauppa);
            }
        }
        else if(komento == "cheapest") {
            if(osat.size() != 2) {
                cout << "Error: error in command cheapest" << endl;
            } else {
                string tuote = osat.at(1);
                tulostaHalvin(data, tuote);
            }
        }
        else if(komento == "products") {
            if(osat.size() != 1) {
                cout << "Error: error in command products" << endl;
            } else {
                tulostaTuotteet(data);
            }
        }
        else {
             // virhe, jos komennossa oli tekstiä, mutta se ei  vastannut mitään
            // komentoa
            cout << "Error: unknown command" <<endl;
            continue;
        }
    }
}
