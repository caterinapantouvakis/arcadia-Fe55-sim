#include "CommandLine.h"

#include <iostream>

using namespace std;

CommandLine* CommandLine::cl = nullptr;
bool CommandLine::fInit = false;
string CommandLine::fDefaultStr = "###";  // sistemare

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
    if( (it == word) || (it.find(word) != string::npos) )
      return true; 
  }
  return false;
}

float CommandLine::GetValueImpl(string keyWord) const{
  // if keyword is not found in argv values, check if there is a 
  // default value set. If a default value is set but another value for
  // keyWord is set in command line, it retrieves the last one
  if(!ContainsImpl(keyWord)){
    if(fDefaultValues.contains(keyWord)){
      return fDefaultValues.at(keyWord);
    }
    else if(!fDefaultValues.contains(keyWord)){
      cout << "No default value set for " << keyWord << " keyword" << endl; 
      return -1;
    }
    else {
      cout << "Keyword " << keyWord << " not found" << endl;
      return -1;
    }
  }

  string word;
  // loop over all the words passed in command line
  for(auto& w : fWords){
    // when word contains a certain keyword, it checks for "=" position
    if( w.find(keyWord) != string::npos ){
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

  return stof(word);
}

const string& CommandLine::GetInfoImpl(const string& keyWord) const {
  // if keyword is not found in argv values, check if there is a 
  // default value set. If a default value is set but another value for
  // keyWord is set in command line, it retrieves the last one
  if(!ContainsImpl(keyWord)){
    if(fDefaultInfo.contains(keyWord)){
      return fDefaultInfo.at(keyWord);
    }
    else if(!fDefaultInfo.contains(keyWord)){
      cout << "No default value set for " << keyWord << " keyword" << endl; 
      return fDefaultStr;
    }
    else {
      cout << "Keyword " << keyWord << " not found" << endl;
      return fDefaultStr;
    }
  }

  // return word after keyWord
  for(auto it = fWords.begin(); it < fWords.end(); ++it){
    if( it->find("--") != string::npos && it->find(keyWord) != string::npos )
      return *(it+1);
  }
  return fDefaultStr;
}