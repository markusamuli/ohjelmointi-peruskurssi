#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include "ruutu.hh"

const int KOKO = 6;

void tulostaDebugLauta(std::vector< std::vector< Ruutu > >& lauta, std::ostream& virta)
{
    virta << "  ";
    for(int x = 0; x < KOKO; ++x)
    {
        virta << (x+1) % 10 << ' ';
    }
    virta << std::endl;

    for(int y = 0; y < KOKO; ++y)
    {
        virta << (y+1) % 10 << ' ';
        for(int x = 0; x < KOKO; ++x)
        {
            lauta.at(y).at(x).tulostaDebug(virta);
            virta << " ";
        }
        virta << std::endl;
    }
}

// Funktio tulostaa pelilaudan määritellyssä formaatissa.
void tulostaLauta(std::vector< std::vector< Ruutu > >& lauta, std::ostream& virta)
{
    // Tulostetaa jokaisen merkin jälkeen välilyönti, jotta ASCII-grafiikka olisi selkeämpää katsoa.

    // Tulostetaan laudan yläpuolelle X-akselin suuntaiset numerot
    virta << "  ";
    for(int x = 0; x < KOKO; ++x)
    {
        virta << (x+1) % 10 << ' ';  // Käytetään jakojäännöstä 10:n suhteen, jotta tuloste mahtuu vaikka lauta olisi suurempi kuin 10
    }
    virta << std::endl;

    // Tulostetaan lauta siten, että jokaiselle riville tulee Y-akselin numero ja ko.
    // rivillä olevia ruutuja pyydetään sen jälkeen tulostamaan itsensä metodikutsua käyttäen.
    for(int y = 0; y < KOKO; ++y)
    {
        virta << (y+1) % 10 << ' ';
        for(int x = 0; x < KOKO; ++x)
        {
            lauta.at(y).at(x).tulosta(virta);
            virta << " ";
        }
        virta << std::endl;
    }
}

// Funktio alustaa pelilaudan käyttökuntoon, eli muodostaa ruutuoliot ja laskee niihin tarvitut tiedot
void alustaLauta(std::vector< std::vector< Ruutu > >& lauta)
{
    std::cout << "Syota siemenluku tai tyhja rivi: ";
    std::string siemen = "";
    getline(std::cin, siemen);

    if(siemen == "") {
        // Jos käyttäjä ei syöttänyt siemenarvoa, käytetään siemenenä tietokoneen kellosta otettua satunnaislukua.
        srand (time(NULL));
    } else {
        // Jos käyttäjä syötti siemenarvon, käytetään sitä.
        srand(stoi(siemen));
    }

    for(int y = 0; y < KOKO; ++y)
    {
        std::vector< Ruutu > rivi;
        for(int x = 0; x < KOKO; ++x)
        {
            bool onkoMiinaa = false;
            if(rand()%5 == 0){
                onkoMiinaa = true;
            }
            Ruutu uusi_ruutu(x, y, onkoMiinaa, &lauta);
            rivi.push_back(uusi_ruutu);
        }
        lauta.push_back(rivi);
    }

    // Kun kaikki laudan ruudut ovat olemassa, pyydetään jokaista ruutua laskemaan itselleen tieto,
    // kuinka monta miinaa sen viereisissä ruuduissa on. Pelilauta on tämän jälkeen valmis.
    for(int y = 0; y < KOKO; ++y)
    {
        for(int x = 0; x < KOKO; ++x)
        {
            lauta.at(y).at(x).laskeViereiset();
        }
    }
}

// Funktio tarkistaa, onko peli päättynyt, palauttaa true, jos on, ja false, jos ei
bool tarkistaLauta(std::vector< std::vector< Ruutu > >& lauta)
{
    for(int y = 0; y < KOKO; ++y)
    {
        for(int x = 0; x < KOKO; ++x)
        {
            if(not lauta[y][x].onkoValmis())
            {
                return false;
            }
        }
    }
    return true;
}

// Funktio esittää parametrina annetun kysymyksen ja tulkitsee vastauksena saamansa merkkijonon,
// myöntävä vastaus true ja kieltävä false
bool lippukysymys(std::string kysymys){
    std::string vastaus = "";
    do
    {
        std::cout << kysymys;
        std::cin >> vastaus;
    } while(vastaus != "K" and vastaus != "k" and vastaus != "E" and vastaus != "e");

    return (vastaus == "K" or vastaus == "k");
}

// Funktio lukee arvot muuttujille x ja y, jotka välitetään viiteparametrina kutsujalle
bool lueKoordinaatit(int& x, int& y)
{
    std::cout << "x y> ";
    return std::cin >> x and std::cin >> y;
}

int main()
{
    std::vector< std::vector< Ruutu > > lauta;
    alustaLauta(lauta);
    tulostaLauta(lauta, std::cout);
    //tulostaDebugLauta(lauta, std::cout);

    int x = 0;
    int y = 0;

    while(not tarkistaLauta(lauta) and lueKoordinaatit(x, y))
    {
        if(lauta.at(y-1).at(x-1).onkoLippu())
        {
            if(lippukysymys("Haluatko poistaa lipun? (K/E) "))
            {
                lauta.at(y-1).at(x-1).poistaLippu();
            }
        } else if(not lippukysymys("Haluatko laittaa lipun? (K/E) ")) {
            if(not lauta.at(y-1).at(x-1).avaa()) {
                std::cout << "BOOOM! Peli loppui!" << std::endl;
                tulostaLauta(lauta, std::cout);
                std::cout << "Havisit..." << std::endl;
                return EXIT_FAILURE;
            }
        } else {
            lauta.at(y-1).at(x-1).lisaaLippu();
        }
        tulostaLauta(lauta, std::cout);
    }
    std::cout << "Voitit, jee!" << std::endl;
    return EXIT_SUCCESS;
}
