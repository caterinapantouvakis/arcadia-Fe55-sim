#ifndef CommandLine_h
#define CommandLine_h

#include <string>
#include <vector>

class CommandLine{

public:

    static CommandLine* Get();
    static void Init(int argc, char** argv);
    ~CommandLine();

    // check if the word "word" is present
    static bool Contains(const std::string& word){
        return Get()->ContainsImpl(word);
    }

    // returns information for the analysis, which have "--" before the name
    //const std::vector<std::string>& GetInfoForAnalysis() const;

    // list of all words passed in command line
    static const std::vector<std::string>& GetAllInfo(){ 
        return Get()->GetAllInfoImpl();
    }

    // function to get values given a keyword, using the following syntax:
    // keyWord=value
    static float GetValue(std::string keyword){
        return Get()->GetValueImpl(keyword);
    }

    // delete copy constructor and assignment operator to avoid copies
    CommandLine(const CommandLine&) = delete;
    CommandLine& operator=(const CommandLine&) = delete;

private:

    CommandLine(int argc, char** argv);

    static CommandLine* cl;
    static bool fInit;
    std::vector<std::string> fWords;

    bool ContainsImpl(const std::string& word) const;
    //const std::vector<std::string>& GetInfoForAnalysis() const;
    const std::vector<std::string>& GetAllInfoImpl() const;
    float GetValueImpl(std::string keyword) const;
    

};

#endif
