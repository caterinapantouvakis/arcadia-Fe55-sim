#include "CommandLine.h"
#include<iostream>

using namespace std;

CommandLine::CommandLine(int argc, char** argv){

  fWords.reserve(argc);
  for(int i=1; i < argc; ++i)
    fWords.emplace_back(argv[i]);

}

CommandLine::~CommandLine(){
  fWords.clear();
}

bool CommandLine::contains(const string& word) const{
  for(auto it : fWords){
    if( (it == word) || (it.find(word)!=std::string::npos) )
      return true; 
  }
  return false;
}


float CommandLine::getValue(std::string keyword) const{
  
  if(!CommandLine::contains(keyword)){
    cout << "Keyword " << keyword << " not found" << endl; 
    return 0;
  }


  std::string word;
  // loop over all the words passed in command line
  for(auto w : fWords){
    // when word contains a certain keyword, it checks for "=" position
    if( w.find(keyword) != std::string::npos ){
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

