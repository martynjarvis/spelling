#include "corrector.h"

void Corrector::train(std::string training_data_file)
{
    std::cout<<"Training."<<std::endl;
    std::regex word_regex("[a-zA-Z]+");  

    std::string line;
    std::ifstream word_file (training_data_file);

    if (word_file.is_open())
    {
        while ( getline(word_file,line) )
        {
            std::regex_iterator<std::string::iterator> regex_it (line.begin(), line.end(), word_regex);
            std::regex_iterator<std::string::iterator> regex_end;

            while (regex_it!=regex_end) {
                std::map<std::string,int>::iterator it;
                it = word_count.find(regex_it->str());

                if (it != word_count.end())
                {
                    word_count[regex_it->str()] += 1;
                }
                else
                {
                    word_count[regex_it->str()] = 1;
                }
                ++regex_it;
            }
        }
        word_file.close();
    }
    std::cout<<"Training done."<<std::endl;
}

std::string Corrector::most_common_word()
{
    // Check most common word (should be 'the' or something similar)
    std::map<std::string,int>::iterator it;
    it = word_count.begin();

    int max_val = it->second;
    std::string max_val_key = it->first;

    for (; it!= word_count.end(); ++it)
    {
        if (it->second > max_val){
            max_val = it->second;
            max_val_key = it->first;
        }
    }
    return max_val_key;
}

std::string Corrector::correct(std::string word)
{
    // is it correct?
    std::map<std::string,int>::iterator word_count_it;
    word_count_it = word_count.find(word);

    if (word_count_it != word_count.end())
    {
        return word;
    }

    int max_val = 0;
    std::string max_val_key;

    // try 1st degree edits
    std::vector<std::string> * first_edits = edits(word);
    for (std::vector<std::string>::iterator first_edits_it = first_edits->begin(); 
         first_edits_it != first_edits->end(); 
         ++first_edits_it)
    {
        word_count_it = word_count.find(*first_edits_it);
        if (word_count_it != word_count.end())
        {
            if (word_count_it->second > max_val){
                max_val = word_count_it->second;
                max_val_key = word_count_it->first;
            }
        }
    }
    if (max_val > 0)
    {
        delete first_edits;
        return max_val_key;
    }

    // second degree edits
    for (std::vector<std::string>::iterator first_edits_it = first_edits->begin(); 
         first_edits_it != first_edits->end(); 
         ++first_edits_it)
    {
        std::vector<std::string> * second_edits = edits(*first_edits_it);
        for (std::vector<std::string>::iterator second_edits_it = second_edits->begin(); 
             second_edits_it != second_edits->end(); 
             ++second_edits_it)
        {
            word_count_it = word_count.find(*second_edits_it);
            if (word_count_it != word_count.end())
            {
                if (word_count_it->second > max_val){
                    max_val = word_count_it->second;
                    max_val_key = word_count_it->first;
                }
            }
        }
        delete second_edits;
    }
    delete first_edits;
    if (max_val > 0)
    {
        return max_val_key;
    }
    return "FAIL";
}

std::vector<std::string> * Corrector::edits(std::string word){
    std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
    std::vector<std::string> * new_words = new std::vector<std::string>;
    // delete
    for (int i=1; i<=word.size(); i++){
        std::string a = word.substr(0,i-1);
        std::string b = word.substr(i);
        new_words->push_back(a+b);
    }
    // transpose
    for (int i=1; i<word.size(); i++){
        std::string a = word.substr(0,i-1);
        std::string b = word.substr(i-1,1);
        std::string c = word.substr(i,1);
        std::string d = word.substr(i+1);
        new_words->push_back(a+c+b+d);
    }
    // replace
    for (int i=1; i<=word.size(); i++){
        std::string a = word.substr(0,i-1);
        std::string b = word.substr(i);
        for( char c : alphabet) {
            new_words->push_back(a+c+b);
        }
    }
    // insert
    for (int i=0; i<=word.size(); i++){
        std::string a = word.substr(0,i);
        std::string b = word.substr(i);
        for( char c : alphabet) {
            new_words->push_back(a+c+b);
        }
    }
    return new_words;
}

