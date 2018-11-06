/*
#############################################################################
# TIE-02200 Ohjelmoinnin peruskurssi, K2018                                 #
# Project3: Suku on pahin                                                   #
# File: familytree.h                                                        #
# Description: Familytree-class datastructure                               #
#       Datastructure is populated with Person-structs and provides some    #
#       query-functions.                                                    #
# Notes: * This is a part of an exercise program                            #
#        * Student's aren't allowed to alter public interface!              #
#        * All changes to private side are allowed.                         #
#############################################################################
*/
#ifndef FAMILYTREE_H
#define FAMILYTREE_H

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <set>
#include <iostream>


// Named constants to improve readability in other modules.
const std::string NO_ID = "";
const int NO_HEIGHT = -1;

// Struct for the persons data.
struct Person
{
    std::string id_ = NO_ID;
    int height_ = NO_HEIGHT;
    std::vector<Person*> parents_{nullptr, nullptr};
    std::vector<Person*> children_;
};


// <REMOVE FROM TEMPLATE>
//------------------------
using Personmap = std::map<std::string, std::shared_ptr<Person> >;
//------------------------
// </REMOVE FROM TEMPLATE>

using IdSet = std::set<std::string>;

/*  Class: Familytree
 *  Description: Datastructure for Person-structs
 */
class Familytree
{
public:
    Familytree();
    ~Familytree();

    /* Description: Adds a new Person to the datastructure.
     * Parameters:
     *  Param1: Person's ID string
     *  Param2: Person's height
     *  Param3: Output-stream for error-printing
     * Errormessages:
     *  If person's ID is already in datastructure:
     *      "Error. Person already added."
     */
    void addNewPerson(const std::string& id, const int& height,
                      std::ostream &output);

    /* Description: Prints all stored Persons' IDs and heights
     * Parameters:
     *  Param1: Output-stream for printing
     * Errormessages:
     *  None.
     */
    void printPersons(std::ostream &output) const;



    /*
     * All the following functions have the same error messages:
     *  If ID wasn't found in datastructure:
     *      "Error. <ID> not found."
     *  If the printing list is empty:
     *      "Error. <ID> has no <group's name>."
     * -------------------------------------------------------------------
     */

    /* Description: Adds new parent-child relation
     * Parameters:
     *  Param1: ID of the child
     *  Param2: Vector containing the parents' IDs
     *  Param3: Output-stream for error-printing
     */
    void addRelation(const std::string& child,
                     const std::vector<std::string>& parents,
                     std::ostream& output);

    /* Description: Prints all children for the person.
     * Parameters:
     *  Param1: ID of the person
     *  Param2: Output-stream for printing
     */
    void printChildren(const std::string& id,
                       std::ostream& output) const;

    /* Description: Prints all the parents for the person.
     * Parameters:
     *  Param1: ID of the person
     *  Param2: Output-stream for printing
     */
    void printParents(const std::string& id,
                      std::ostream& output) const;

    /* Description: Prints all siblings for the person.
     *  (Persons who shares a parent)
     * Parameters:
     *  Param1: ID of the person
     *  Param2: Output-stream for printing
     */
    void printSiblings(const std::string& id,
                       std::ostream& output) const;

    /* Description: Prints all the cousins for the person.
     *  (Persons who share same grandparents)
     * Parameters:
     *  Param1: ID of the person
     *  Param2: Output-stream for printing
     */
    void printCousins(const std::string& id,
                      std::ostream& output) const;

    /* Description: Prints the tallest person of the ID's lineage.
     * Parameters:
     *  Param1: ID of the person
     *  Param2: Output-stream for printing
     */
    void printTallestInLineage(const std::string& id,
                               std::ostream& output) const;

    /* Description:
     * Parameters:
     *  Param1: ID of the person
     *  Param2: Output-stream for printing
     */
    void printShortestInLineage(const std::string& id,
                                std::ostream& output) const;


    /* The following functions have additional errormessage:
     *  If Param2's value is less than 1:
     *      "Error. Level can't be less than 1."
     * ---------------------------------------------------------------
     */


    /* Description: Prints the amount of grandchildren in given distance
     *  from the person.
     * Parameters:
     *  Param1: ID of the person
     *  Param2: Distance from the person. (n-1 times "grandness")
     *  Param3: Output-stream for printing
     */
    void printGrandChildrenN(const std::string& id, const int n,
                             std::ostream& output) const;

    /* Description: Prints the amount of grandparents in given distance
     *  from the person.
     * Parameters:
     *  Param1: ID of the person
     *  Param2: Distance from the person. (n-1 times "grandness")
     *  Param3: Output-stream for printing
     */
    void printGrandParentsN(const std::string& id, const int n,
                            std::ostream& output) const;

private:
    Personmap data_;


    /* The following functions are meant to make project easier.
     * You can implement them if you want and/or create your own.
     * Anyway it would be a good idea to implement more functions
     * to make things easier and to avoid "copy-paste-coding"
     */

    // Return a pointer for ID.
    Person* getPointer(const std::string& id) const;

    // Printing errors.
    void printNotFound(const std::string& id,
                       std::ostream& output) const;

    // Turns a vector of persons to a set of IDs.
    IdSet VectorToIdSet(const std::vector<Person*> &container) const;

    // Prints the the data in a container.
    void printGroup(const std::string& id, const std::string& group,
                    const IdSet& container, std::ostream& output) const;


    void personsOnLevel(Person* const person,
                        IdSet &container,
                        const bool direction_child,
                        const int& n
                        ) const;

    void printGrand(Person* const person, const int& n,
                    const bool& dir_c, std::ostream&
                    output = std::cout) const;
    void printTallestOrShortest(Person* const person,
                                const bool& tallest,
                                std::ostream& output = std::cout) const;
    Person*  heightRecursive(Person* const person,
                             const bool& tallest) const;

    void hasNoSomething(const std::__cxx11::string &id, std::string relation,
                        std::ostream &output) const;   

    void tallestFinder(std::string& tallestPerson, int& tallestHeight,
                       Person *person) const;

    void shortestFinder(std::string &shortestPerson, int &shortestHeight,
                        Person* person) const;

    IdSet Siblings(const std::string& id) const;

    bool isInData(const std::__cxx11::string &id, std::ostream &output) const;

};

#endif // FAMILYTREE_H
