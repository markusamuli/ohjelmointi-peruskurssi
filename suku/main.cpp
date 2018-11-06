/*
#############################################################################
# TIE-02200 Ohjelmoinnin peruskurssi, K2018                                 #
# Project3: Suku on pahin                                                   #
# Program description: Queries user for CSV-file and allows simple searches #
#       based on the data.                                                  #
# File: main.cpp                                                            #
# Description: Main-module performs the followin operations:                #
#       * Query for input file.                                             #
#       * Parse the CSV-data                                                #
#       * Pass the parsed data to Familytree-module                         #
#       * Launch Cli-module                                                 #
# Notes: * This is an exercise program.                                     #
#        * Student's don't touch this file.                                 #
#############################################################################
*/
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include "familytree.hh"
#include "cli.hh"

// Constants to make CSV-parsing more readable.
const char CSV_DELIMITER = ';';
const int CSV_VALUES = 4;
const int CSV_NAME=0, CSV_HEIGHT=1, CSV_FATHER=2, CSV_MOTHER=3;


// Struct to group-up parent-child relations.
struct Relation
{
    std::string child_;
    std::vector<std::string> parents_;
};

bool populateDatabase(std::ifstream &datafile, std::shared_ptr<Familytree> database);
Person parsePerson(const std::string &line, std::vector<Relation> &relations);

int main()
{
    std::string cmd_string;
    std::shared_ptr<Familytree> database(new Familytree);

    // File query
    std::cout << "Input file: ";
    std::getline(std::cin, cmd_string);

    // Query result must be transformed from std::string to c_string so it can
    //  be passed to ifstream's constructor.
    std::ifstream datafile(cmd_string.c_str());

    if(not datafile)
    {
        std::cout << "Could not open file: " << cmd_string << std::endl;
        return EXIT_FAILURE;
    }

    // populateDatabase returns false if there was some error in file.
    if(not populateDatabase(datafile, database))
    {
        return EXIT_FAILURE;
    }

    // Constructin the commandlineinterpreter with the given main datastructure
    Cli commandline(database);

    // Empty loop that runs the CLI.
    // CLI returns false only on exit-command
    while(commandline.exec_prompt()){}

    return EXIT_SUCCESS;
}


/* Description: Reads throught the datafile and populates the database with it.
 * Parameters:
 *  Param1: Datafile-object
 *  Param2: Pointer to the database
 * Return values:
 *  True    - No errors in file
 *  False   - Errors in file
 */

bool populateDatabase(std::ifstream& datafile, std::shared_ptr<Familytree> database)
{
    std::string line = "";
    std::vector<std::string> parsed_line;
    std::vector<Relation> relations;
    int line_number = 0;

    while(std::getline(datafile, line))
    {
        // Linenumbering for error-printing.
        ++line_number;

        // Skip empty and commented lines.
        if(line.empty() or line[0] == '#')
        {
            continue;
        }

        // Parse the line to a new person and relation.
        Person new_person = parsePerson(line, relations);
        if( new_person.id_ == NO_ID or new_person.height_ == NO_HEIGHT )
        {
            std::cout << "Error in datafile, line " << line_number << std::endl;
            return false;
        }

        // Add the new person to the database.
        database->addNewPerson(new_person.id_, new_person.height_, std::cout);
    }

    // After the persons have been created. Add the child-parent relations.
    for( auto it = relations.begin(); it != relations.end(); ++it)
    {
        database->addRelation(it->child_, it->parents_, std::cout);
    }
    return true;
}

/* Description: Parse the string to a new person and child-parent relation.
 * Parameters:
 *  Param1: String to parse
 *  Param2: Container for Relations
 * Return values:
 *  Person-struct that was created.
 *      id_     - NO_NAME if wrong amount of parameters
 *      height  - NO_HEIGHT if wrong amount of parameters or the
 *                  string-to-int conversion failed.
 */

Person parsePerson(const std::string& line, std::vector<Relation> &relations)
{
    std::string tmp = line;
    std::vector<std::string> parsed_data;
    size_t pos = 0;
    Person new_person;
    Relation new_relation;

    // Split the ';' separated fields to a vector.
    while (true)
    {
        pos = tmp.find(CSV_DELIMITER);
        parsed_data.push_back(tmp.substr(0, pos));
        if(pos == std::string::npos){break;}
        tmp = tmp.substr(pos+1);
    }

    // Check the that there was correct amount of fields in the line.
    if( parsed_data.size() == CSV_VALUES )
    {
        new_person.id_ = parsed_data[CSV_NAME];
        // Exception safety.
        // The NO_HEIGHT assignment isn't necessary, but makes things clearer.
        try{
            new_person.height_ = std::stoi(parsed_data[CSV_HEIGHT]);
        }
        catch(std::invalid_argument)
        {
            std::cout << "Caught invalid_argument exception." << std::endl;
            new_person.height_ = NO_HEIGHT;
        }
        catch(std::out_of_range)
        {
            std::cout << "Caught out_of_range exception." << std::endl;
            new_person.height_ = NO_HEIGHT;
        }
        new_relation.child_ = parsed_data[CSV_NAME];
        new_relation.parents_.push_back(parsed_data[CSV_FATHER]);
        new_relation.parents_.push_back(parsed_data[CSV_MOTHER]);
        relations.push_back(new_relation);
    }

    return new_person;
}
