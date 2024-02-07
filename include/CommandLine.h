#ifndef CommandLine_h
#define CommandLine_h

#include <string>
#include <vector>
#include <map>

class CommandLine{

public:

    static CommandLine* Get();
    static bool Init(int argc, char** argv);
    ~CommandLine();

    // check if the word "word" is present
    static bool Contains(const std::string& word){
        return Get()->ContainsImpl(word);
    }

    // list of all words passed in command line
    static const std::vector<std::string>& GetAllInfo(){ 
        return Get()->GetAllInfoImpl();
    }

    // function to get values given a keyword, using the following syntax:
    // keyWord=value
    static float GetValue(std::string keyWord){
        return Get()->GetValueImpl(keyWord);
    }

    static void SetDefaultValue(const std::string& keyWord, float val){
        return Get()->SetDefaultValueImpl(keyWord, val);
    }

    static const std::string& GetInfo(const std::string& keyWord){
        return Get()->GetInfoImpl(keyWord);
    }

    static void SetDefaultInfo(const std::string& keyWord, const std::string& info){
        return Get()->SetDefaultInfoImpl(keyWord, info);
    }

    // delete copy constructor and assignment operator to avoid copies
    CommandLine(const CommandLine&) = delete;
    CommandLine& operator=(const CommandLine&) = delete;

private:

    CommandLine(int argc, char** argv);

    static CommandLine* cl;
    static bool fInit;
    std::vector<std::string> fWords;
    std::map<std::string, float> fDefaultValues;
    std::map<std::string, std::string> fDefaultInfo;
    static std::string fDefaultStr;

    bool ContainsImpl(const std::string& word) const;
    //const std::vector<std::string>& GetInfoForAnalysis() const;
    inline const std::vector<std::string>& GetAllInfoImpl() const { return fWords; }
    // returns values set as keyWord=value
    float GetValueImpl(std::string keyWord) const;
    // returns information for the simulation, which have "--" before the name
    const std::string& GetInfoImpl(const std::string& keyWord) const;
    inline void SetDefaultValueImpl(const std::string& keyWord, float val) { fDefaultValues[keyWord] = val; }
    inline void SetDefaultInfoImpl(const std::string& keyWord, const std::string& info) {
        fDefaultInfo[keyWord] = info;
    }
};

#endif
