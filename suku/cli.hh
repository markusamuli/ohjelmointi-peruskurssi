/*
#############################################################################
# TIE-02200 Ohjelmoinnin peruskurssi, K2018                                 #
# Project3: Suku on pahin                                                   #
# File: cli.hh                                                              #
# Description: Commandline interface for the program.                       #
# Notes: * This is a part of an exercise program                            #
#        * Student's don't touch this file.                                 #
#        * Understanding how the module works isn't necessary to complete   #
#          this exercise.                                                   #
#        * This is also an example how you can use function pointers to     #
#          a class' functions.                                              #
#        * This could be upgraded to have commandhistory with a relative    #
#          ease.                                                            #
#############################################################################
*/
#ifndef CLI_H
#define CLI_H

#include <string>
#include <vector>
#include <memory>

#include "familytree.hh"

/* In order to define a function pointer to a class you have to add
 * class as the visibility range before the pointer declaring * and
 * after that comes the type-name for the pointer.
 * In this case we have named the types according to the amount of
 * parameters the function receives.
 */
typedef void (Familytree::*oneparam)(std::ostream&) const;
typedef void (Familytree::*twoparam)(const std::string&, std::ostream&) const;
typedef void (Familytree::*threeparam)(const std::string&, const int, std::ostream&) const;

// CommandInfo contains the list of strings that select it and
// Function-pointers to the function that is going to be called through it.
struct CommandInfo
{
    std::vector<std::string> lines_;
    oneparam func0_;
    twoparam func1_;
    threeparam func2_;
};


class Cli
{
public:
    // The database that is going to be used is given as a parameter.
    Cli(std::shared_ptr<Familytree> db);

    /* Description: Runs the interface.
     * Return values:
     *  True    - Normal command
     *  False   - Exit command
     */
    bool exec_prompt();

private:
    std::shared_ptr<Familytree> database_;

    // Stores the arguments from current command.
    std::vector<std::string> args_;

    // Pointer to the recognized command.
    const CommandInfo* recognized_;

    // Printed for every query
    const std::string PROMPT = "> ";

    // Parses the given input to different arguments
    void parseCommand(const std::string& line);

    /* Description: Sets the recognized_ to point at the correct
     * CommandInfo.
     * Return values:
     *  Amount of arguments - Command can be executed
     *  -1                  - Exit command
     *  -2                  - Invalid amount of parameters
     *  -3                  - Command not recognized.
     */
    int checkCommandType();

    // To make code more readable. Could also be different constants but this
    // groups them better.
    enum Commandtypes {QUIT = -1, NO_PARAMETERS = 1, ONE_PARAMETER = 2,
                       TWO_PARAMETER = 3, EMPTY = 0, UNKNOWN = -3,
                       WRONG_PARAMETERS = -2};


    // Constant table that contains the different commands that the CLI can
    // recognize.
    CommandInfo const COMMANDS[11] = {
        {{"EXIT","QUIT","Q","LOPETA"}, nullptr, nullptr, nullptr},
        {{"TREE","FAMILYTREE","SUKUPUU","PUU", "PRINT"}, &Familytree::printPersons, nullptr,nullptr},
        {{"CHILDREN", "LAPSET"}, nullptr, &Familytree::printChildren, nullptr},
        {{"COUSINS", "SERKUT"}, nullptr, &Familytree::printCousins, nullptr},
        {{ "SIBLINGS", "SISARUKSET"},nullptr,&Familytree::printSiblings,nullptr},
        {{ "PARENTS", "VANHEMMAT"},nullptr,&Familytree::printParents,nullptr},
        {{ "TALLEST", "PISIN"}, nullptr,&Familytree::printTallestInLineage,nullptr},
        {{ "SHORTEST", "LYHYIN", "LYHIN"}, nullptr, &Familytree::printShortestInLineage, nullptr},
        {{ "GRANDCHILDREN", "LAPSENLAPSET", "GC", "LL"}, nullptr, nullptr,&Familytree::printGrandChildrenN},
        {{ "GRANDPARENTS", "ISOVANHEMMAT", "GP", "IV"}, nullptr,nullptr,&Familytree::printGrandParentsN},
        {{},nullptr,nullptr,nullptr}
    };


};

#endif // CLI_H
