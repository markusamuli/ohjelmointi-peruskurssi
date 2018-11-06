// Markus Ryöti
// Luokan metodit tulostavat mainissa määriteltyjen komentojen avulla tuloksia
// Data koostuu henkilöstructeista, joiden sukulaissuhteet on määritelty niissä
// osoittimien avulla

#include "familytree.hh"
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <set>
#include <iostream>

using std::endl;
using std::string;


Familytree::Familytree() {

}

Familytree::~Familytree() {

}

// Funktio lisää joukkoon data_ uuden henkilön, jos ko henkilö ei siellä
// aiemmin ole
void Familytree::addNewPerson(const std::__cxx11::string &id, const int &height,
                              std::ostream &output) {

    // Tarkastetaan, löytyykö kyseinen henkilö datasta
    if(data_.find(id) != data_.end()) {
        output << "Error. Person already added." << endl;
        return;
    }

    // Luodaan uusi struct ja tallennetaan parametrina saadut arvot
    Person new_person;
    new_person.id_ = id;
    new_person.height_ = height;
    // Uusi henkilö lisätään structiin shared pointerina
    data_[id] = std::make_shared<Person>(new_person);

}

// Funktio käy datan läpi ja tulostaa jokaisen henkilön nimen ja pituuden
void Familytree::printPersons(std::ostream &output) const {

    for(auto person : data_) {
        output << person.second->id_ << ", " << person.second->height_ << endl;
    }
}

// Funktio lisää olemassa olevalle lapselle vanhempi-lapsisuhteen
void Familytree::addRelation(const std::__cxx11::string &child,
                             const std::vector<std::__cxx11::string> &parents,
                             std::ostream &output) {

    // Luodaan iteraattori ja katsotaan löytyykö lasta datasta
    Personmap::iterator child_iter = data_.find(child);
    if(child_iter == data_.end()) {
        printNotFound(child, output);
        return;
    }

    // Käydään vanhemmat läpi ja skipataan lisäys, jos vanhempaa ei löydetä
    for(std::vector<string>::size_type i = 0; i < parents.size(); ++i) {

        Personmap::iterator parent_iter = data_.find(parents.at(i));
        if(parent_iter == data_.end()) {
            continue;
        }

        // Tallennetaan vanhempi-pointteerin lapsi
        (*parent_iter).second->children_.push_back((*child_iter).second.get());
        // Tallennetaan lapselle vanhempi
        (*child_iter).second->parents_.at(i) = (*parent_iter).second.get();
    }
}

// Funktio tulostaa parametrina tulevan henkilön lapset
void Familytree::printChildren(const std::__cxx11::string &id,
                               std::ostream &output) const {

    // Tehdään virhekäsittely ja tulostetaan mahdollinen virhe
    if(!isInData(id, output)) {
        return;
    }

    // Lisätään lapset settiin, niin aakkosjärjestyksessä tulostus käy helposti
    std::set<string> children;
    for(auto child : data_.at(id)->children_) {
        children.insert(child->id_);
    }

    // Tulostetaan sen mukaan, oliko lapsia vai ei
    if(children.size() == 0) {
        output << id << " has no children." << endl;
    } else {
        output << id << " has " << children.size() << " children:" << endl;
        for(auto child : children) {
            output << child << endl;
        }
    }
}

// Funktio tulostaa parametrina tulevan henkilön vanhemmat
void Familytree::printParents(const std::__cxx11::string &id,
                              std::ostream &output) const {

    // Virhetarkastus
    if(!isInData(id, output)) {
        return;
    }

    // Lisätään vanhemmat settiin, jolloin aakkosjärjestyksessä tulostus helppoa
    std::set<string>sortedParents;
    for(auto parent : data_.at(id)->parents_) {
        if(parent != nullptr) {
            sortedParents.insert(parent->id_);
        }
    }

    // Tulostetaan tulokset sen mukaan, oliko vanhempia vai ei
    if(sortedParents.size() == 0) {
        hasNoSomething(id, "parents", output);
    } else {
        output << id << " has " << sortedParents.size() << " parents:" << endl;
        for(auto parent : sortedParents) {
            output << parent << endl;
        }
    }
}

// Funktio tulostaa id:n serkukset
void Familytree::printSiblings(const std::__cxx11::string &id,
                               std::ostream &output) const {
    // Virhetarkastus
    if(!isInData(id, output)) {
        return;
    }

    // Käytetään apufunktiota tulostukseen
    std::set<string>siblings = Siblings(id);

    // Tulostus sen mukaan, löytyikö sisaruksia vai ei
    if(siblings.size() == 0) {
        hasNoSomething(id, "siblings", output);
    } else {
        output << id << " has " << siblings.size() << " siblings:" << endl;
        for(auto sibling : siblings) {
            output << sibling << endl;
        }
    }
}

// Funktio tulostaa henkilön serkut
void Familytree::printCousins(const std::__cxx11::string &id,
                              std::ostream &output) const {

    // Virhetarkastelu
    if(!isInData(id, output)) {
        return;
    }

    // Serkusten tallennus settiin aakkosjärjestyksen vuoksi
    std::set<string> cousins;
    std::set<string> parents;

    // Täytyy tarkastaa, oliko vanhempaan osoitinta, muuten myöhempi luuppaus
    // ei toimi. Jos ei löydy osoitinta, jatketaan eteenpäin
    for(auto parent : data_.at(id)->parents_) {
        if(parent == nullptr) {
            continue;
        } else {
            parents.insert(parent->id_);
        }
    }

    // Luupissa käydään läpi vanhemmat ja haetaan heidän sisaruksena apufunktiona
    // Lopuksi lisätään settiin
    for(auto parent : parents) {
        for(auto parentSibling : Siblings(parent)) {
            for(auto cousin : getPointer(parentSibling)->children_) {
                cousins.insert(cousin->id_);
            }
        }
    }

    // Tulostus sen mukaan, löytyikö serkkuja vai ei
    if(cousins.size() == 0) {
        hasNoSomething(id, "cousins", output);
    } else {
        output << id << " has " << cousins.size() << " cousins:" << endl;
        for(auto cousin : cousins) {
            output << cousin << endl;
        }
    }
}


void Familytree::printTallestInLineage(const std::__cxx11::string &id,
                                       std::ostream &output) const {

    // Virhetarkastelu
    if(!isInData(id, output)) {
        return;
    }

    // Luodaan muuttujat, jotka välitetään parametrina apufunktiolle
    int tallestHeight = 0;
    string tallestPersonId = "";

    // Tehdään alkuarvaus etsittävän id:n omien tietojen mukaan
    tallestHeight = data_.at(id)->height_;
    tallestPersonId = data_.at(id)->id_;

    // Apufunktio etsii rekursiivisesti pisimmän henkilön
    tallestFinder(tallestPersonId, tallestHeight, data_.at(id).get());


    // Tulostus sen mukaan, oliko henkilö itse joukon pisin vai joku muu
    if(tallestPersonId == id) {
        output << "With the height of " << tallestHeight << ", "
               << tallestPersonId << " is the tallest person in his/her lineage."
               << endl;
    } else {
        output << "With the height of " << tallestHeight << ", "
               << tallestPersonId << " is the tallest person in "
               << id << "'s lineage." << endl;
    }
}

// Funktio toimii vastaavasti kuin pisimmän etsiväkin
void Familytree::printShortestInLineage(const std::__cxx11::string &id,
                                        std::ostream &output) const {
    // BONUS

    if(!isInData(id, output)) {
        return;
    }

    int shortestHeight = 0;
    string shortestPersonId = "";

    shortestHeight = data_.at(id)->height_;
    shortestPersonId = data_.at(id)->id_;

    shortestFinder(shortestPersonId, shortestHeight, data_.at(id).get());


    if(shortestPersonId == id) {
        output << "With the height of " << shortestHeight << ", "
               << shortestPersonId << " is the shortest person in his/her lineage."
               << endl;
    } else {
        output << "With the height of " << shortestHeight << ", "
               << shortestPersonId << " is the shortest person in "
               << id << "'s lineage." << endl;
    }
}


void Familytree::printGrandChildrenN(const std::__cxx11::string &id,
                                     const int n, std::ostream &output) const {
    // BONUS


    if(!isInData(id, output)) {
        return;
    }

    output << id << n <<endl;

}


void Familytree::printGrandParentsN(const std::__cxx11::string &id,
                                    const int n, std::ostream &output) const {
    // BONUS

    if(!isInData(id, output)) {
        return;
    }

    output << id << n << endl;

}

// Funktio hakee etsittävän henkilön osoittimen
Person* Familytree::getPointer(const string &id) const {
    if(data_.find(id) == data_.end()) {
        return nullptr;
    } else {
        return data_.at(id).get();
    }
}

// Tulostaa virheilmoituksen, kun henkilöä ei löydetty
// Kutsutaan alempana määritellystä funktiosta
void Familytree::printNotFound(const std::__cxx11::string &id,
                               std::ostream &output) const {

    output << "Error. " << id << " not found." << endl;   
}

// Tulostaa sen mukaan puutteita parametrin mukaisesti
void Familytree::hasNoSomething(const std::__cxx11::string &id,
                                std::string relation,
                                std::ostream &output) const {

    output << id << " has no " << relation << "." << endl;
}

// Funktio etsii pisimmän henkilön rekursiivisesti ja tallentaa ne viitteenä
// tuleviin muuttujiin
void Familytree::tallestFinder(std::string &tallestPerson,
                               int &tallestHeight, Person* person) const {

    for(auto child : person->children_) {
        if(child != nullptr) {
            if(child->height_ > tallestHeight) {
                tallestHeight = child->height_;
                tallestPerson = child->id_;
            }

            tallestFinder(tallestPerson, tallestHeight, child);
        }
    }
}

// Funktio toimii vastaavasti kuin pisimmän etsiväkin
void Familytree::shortestFinder(std::string &shortestPerson,
                               int &shortestHeight, Person* person) const {

    for(auto child : person->children_) {
        if(child != nullptr) {
            if(child->height_ < shortestHeight) {
                shortestHeight = child->height_;
                shortestPerson = child->id_;
            }

            shortestFinder(shortestPerson, shortestHeight, child);
        }
    }
}

// Funktio etsii henkilön sisarukset parametrina tulevan henkilön mukaan
// Palauttaa lopuksi setin sisaruksista
IdSet Familytree::Siblings(const string &id) const {

    std::set<string> siblings;
    std::set<string> parents;

    // Lisätään vanhempiin vain, jos osoitin ei ole nullptr
    // Luuppaus ei muuten toimi
    for(auto parentInfo : data_.at(id)->parents_) {
        if(parentInfo == nullptr) {
            continue;
        } else {
            parents.insert(parentInfo->id_);
        }
    }

    // Lisätään vanhempien lapset settiin
    for(auto parent : parents) {
        for(auto child : getPointer(parent)->children_) {
            // Etsittyä henkilöä ei tarvitse lisätä
            if(child->id_ != id) {
                siblings.insert(child->id_);
            }
        }
    }

    return siblings;
}

// Funktio tarkastaa, löytyykö haettu henkilö datasta, jos ei, tulostetaan virhe
bool Familytree::isInData(const std::__cxx11::string &id, std::ostream &output) const {
    if(data_.find(id) == data_.end()) {
        printNotFound(id, output);
        return false;
    } else {
        return true;
    }
}
