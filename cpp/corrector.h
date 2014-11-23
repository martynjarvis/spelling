/*
 *   corrector.h
*/

#ifndef corrector_h
#define corrector_h

#include <iostream>
#include <fstream>
#include <map>
#include <regex>
#include <vector>
#include <string>

class Corrector
{
    public:
        void train(std::string training_data_file);
        std::string most_common_word();
        std::string correct(std::string word);

    private:
        std::map<std::string,int> word_count;
        std::vector<std::string> * edits(std::string word);

};

#endif
