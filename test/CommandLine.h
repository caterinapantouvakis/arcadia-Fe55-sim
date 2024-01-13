#ifndef CommandLine_h
#define CommandLine_h

#include <string>
#include <vector>

class CommandLine{

    public:

    CommandLine(int argc, char** argv);
    CommandLine(const CommandLine&) = delete;
    ~CommandLine();

    // check if the word "word" is present
    bool contains(const std::string& word) const;

    // returns information for the analysis, which have "--" before the name
    //const std::vector<std::string>& GetInfoForAnalysis() const;

    // list of all words passed in command line
    inline const std::vector<std::string>& getAllInfo() const { return fWords; };

    // function to get values given a keyword, using the following syntax:
    // keyWord=value
    float getValue(std::string keyword) const;

    private:

    std::vector<std::string> fWords;

};

#endif
