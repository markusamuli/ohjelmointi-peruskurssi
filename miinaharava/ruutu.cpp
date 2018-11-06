// Markus Ryöti
// Tiedosto sisältää miinaharavakentän yhden ruutuolion eri metodit ja toiminnallisuudet.

#include "ruutu.hh"

// Perussetit
Ruutu::Ruutu(int x, int y, bool miina, std::vector< std::vector< Ruutu > >* lauta):
    x_(x), y_(y), ruudussa_miina_(miina), lauta_(lauta) {
}

// Metodi ainoastaan hakee tiedon luokan private muuttujasta
bool Ruutu::onkoLippu() {
    if ( ruudussa_lippu_ ) {
        return true;
    } else {
        return false;
    }
}

// Muutetaan private muuttujaa
void Ruutu::poistaLippu() {
    ruudussa_lippu_ = false;
}

// Avaa funktio palauttaa bool arvon ja määrää täten, jatkuuko peli vai ei
bool Ruutu::avaa() {
    if ( ruudussa_miina_ ) {
        onkoAvattu_ = true;
        return false;
    // Jos ruudussa ei ole miinaa, käytetään vastaavaa logiikkaa, kuin laskeViereiset metodissa
    } else if ( miinojaVieressa_ == 0 ) {

        int min_x = std::max(x_-1, 0);
        int max_x = std::min(x_+1, static_cast<int>(lauta_->at(y_).size()-1));
        int min_y = std::max(y_-1, 0);
        int max_y = std::min(y_+1, static_cast<int>(lauta_->size()-1));

        // Kummassakin if lauseessa tarkastetaan ensin ruudun private muuttujasta, onko ruutu jo avattu.
        // Jos tarkastusta ei tehdä, ei metodin uudelleen kutsunta toiminut kunnolla
        for(int y = min_y; y <= max_y; ++y) {
            for(int x = min_x; x <= max_x; ++x) {
                // Jos viereisiä miinoja löytyy, merkataan vain ruutu avatuksi
                if( miinojaVieressa_ > 0 and not lauta_->at(y).at(x).onkoAvattu_ ) {
                    lauta_->at(y).at(x).onkoAvattu_ = true;
                // Jos viereisiä miinoja ei ole, täytyy tutkia ruudut nykyisen ruudun ympäriltä
                // Täten kutsutaan siis uudelleen samaa metodia
                } else if ( miinojaVieressa_ == 0 and not lauta_->at(y).at(x).onkoAvattu_ ) {
                    lauta_->at(y).at(x).onkoAvattu_ = true;
                    lauta_->at(y).at(x).avaa();
                }
             }
        }
        return true;
    // Toteutaan ruudulle, jolle löytyykin viereisiä miinoja
    } else {
        onkoAvattu_ = true;
        return true;
    }
}

// Muutetaan private muuttujaa
void Ruutu::lisaaLippu() {
    ruudussa_lippu_ = true;
}

// Ruutu on valmis ainoastaan silloin, kun se sisältää miinan ja samaan aikaan lipun.
// Muilla arvoilla ei ole väliä
bool Ruutu::onkoValmis() {
    if ( not ruudussa_lippu_ and ruudussa_miina_ ) {
        return false;
    } else {
        return true;
    }
}

// Luupataan läpi x ja y suunnassa. Rajat määritetään laudan dimensioiden mukaan.
// miinojaVieressä metodi on private muuttuja, johon talletetaan vieressä olevien miinojen määrä
// kullekkin ruudulle
void Ruutu::laskeViereiset() {
    int min_x = std::max(x_-1, 0);
    int max_x = std::min(x_+1, static_cast<int>(lauta_->at(y_).size()-1));
    int min_y = std::max(y_-1, 0);
    int max_y = std::min(y_+1, static_cast<int>(lauta_->size()-1));

    for(int x = min_x; x <= max_x; ++x) {
        for(int y = min_y; y <= max_y; ++y) {
            if ( lauta_->at(y).at(x).ruudussa_miina_ ) {
               ++miinojaVieressa_;
            }
        }
    }
}

// Tulostetaan joka ruudulle oikea merkki. Apuna tarvitaan tietoa, onko ruutu avattu ja
// viereisten miinojen määrä
void Ruutu::tulosta(std::ostream& virta) {
    if ( ruudussa_lippu_ ) {
        virta << LIPPU_STR;
    } else if ( ruudussa_miina_ ) {
        if ( onkoAvattu_ ) {
            virta << POMMI_STR;
        } else {
            virta << KIINNI_STR;
        }
    } else if ( miinojaVieressa_ >= 0 and onkoAvattu_ ) {
        virta << miinojaVieressa_;
    }
    else {
        virta << KIINNI_STR;
    }
}

// Debugauksessa käytetty tulosta metodi, tehty muutoksia aina työn edetessä
void Ruutu::tulostaDebug(std::ostream& virta) {
    if ( ruudussa_lippu_ ) {
        virta << LIPPU_STR;
    } else if ( ruudussa_miina_ ) {
        virta << 'X';
    } else if ( onkoAvattu_ ) {
        virta << miinojaVieressa_;
    }
    else {
        virta << KIINNI_STR;
    }
}
