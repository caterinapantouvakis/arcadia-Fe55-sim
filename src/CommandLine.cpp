#include "CommandLine.h"

#include <iostream>

using namespace std;

CommandLine* CommandLine::cl = nullptr;
bool CommandLine::fInit = false;

CommandLine::CommandLine(int argc, char** argv){
  fWords.reserve(argc);
  for(int i=1; i < argc; ++i)
    fWords.emplace_back(argv[i]);
}

CommandLine::~CommandLine(){
  fWords.clear();
  fDefaultValues.clear();
}

CommandLine* CommandLine::Get(){
  if(!fInit)
    cout << "CommandLine not initialized. Call Init to retrieve input arguments\n";
  return cl;
}

bool CommandLine::Init(int argc, char** argv){
  if(!fInit){
    cl = new CommandLine(argc, argv);
    fInit = true;
    return true;
  }
  else 
    return false;
}

bool CommandLine::ContainsImpl(const string& word) const{
  for(auto it : fWords){
    if( (it == word) || (it.find(word)!=std::string::npos) )
      return true; 
  }
  return false;
}

const vector<string>& CommandLine::GetAllInfoImpl() const {
  return fWords;
}

float CommandLine::GetValueImpl(std::string keyWord) const{
  // if keyword is not found in argv values, check if there is a 
  // default value set
  if(!ContainsImpl(keyWord)){
    // non posso usare il metodo contains perché ci vuole C++20 e root si lamenta
    if(fDefaultValues.contains(keyWord))
    cout << "Keyword " << keyWord << " not found" << endl; 
    return 0;
  }

  std::string word;
  // loop over all the words passed in command line
  for(auto& w : fWords){
    // when word contains a certain keyword, it checks for "=" position
    if( w.find(keyWord) != std::string::npos ){
      word = w;
      for(auto it = word.begin(); it != word.end(); ++it){
        if(*it == '='){
          // erase all the letters to get the value after the "=" sign
          word.erase(word.begin(), it + 1);
          break;
        }
      }
      break;
    }
  }

  return std::stoul(word);
}


