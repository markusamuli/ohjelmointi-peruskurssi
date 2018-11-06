#include <vector>
#include <iostream>
#include <algorithm>

// Vakiot eri merkeille
const char KIINNI_STR = '.';
const char LIPPU_STR = 'P';
const char POMMI_STR = '*';
const char TYHJA_STR = '0';

class Ruutu
{
public:
    // Nämä metodit ovat luotu sen mukaan, mitä mainissa alunperin on kutsuttu
    Ruutu(int x, int y, bool miina, std::vector< std::vector< Ruutu > >* lauta);
    bool onkoLippu();
    void poistaLippu();
    bool avaa();
    void lisaaLippu();
    bool onkoValmis();
    void laskeViereiset();
    void tulosta(std::ostream& virta);
    void tulostaDebug(std::ostream& virta);
private:
    int x_;
    int y_;
    bool ruudussa_miina_;
    std::vector< std::vector< Ruutu > >* lauta_;
    // Lisäämällä nämä muuttujat kullekin ruudulle, onnistuu luokan toteuttaminen
    // Bool muuttujien alustukset ovat olennaisia
    bool ruudussa_lippu_ = false;
    int miinojaVieressa_ = 0;
    bool onkoAvattu_ = false;
};
