#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void generateWords(
    const std::string& known,  // original pattern "in"
    int index,  // current position being filled
    std::string& current,  // current word being made
    int floats[26],  // counts of floating letters
    int floatsleft,  // number of remaining floating letters 
    int blanksleft,  // number of blanks still not filled 
    const std::set<std::string>& dict,
    std::set<std::string>& results);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
  std::set<std::string> results;

  //populate float counts array
  int floats[26];
  for (int i = 0; i < 26; ++i) {
    floats[i] = 0;
  }

  int numfloats = 0;
  for (size_t i = 0; i < floating.size(); ++i) {
    char c = floating[i];
    if (c >= 'a' && c <= 'z') {
      floats[c - 'a']++;
      numfloats++;
    }
  }

  //count blanks
  int blanks = 0;
  for (size_t i = 0; i < in.size(); ++i) {
    if (in[i] == '-') {
      blanks++;
    }
  }

  //start recursion
  std::string current = in;
  generateWords(in, 0, current, floats, numfloats, blanks, dict, results);

  return results;
}

// Define any helper functions here
void generateWords(
    const std::string& known,
    int index,
    std::string& current,
    int floats[26],
    int floatsleft,
    int blanksleft,
    const std::set<std::string>& dict,
    std::set<std::string>& results)
{
  //word not possible
  if (floatsleft > blanksleft) {
    return;
  }
  
  //base case: full length word found in dict
  if (index == (int)known.size()) {
    if (floatsleft == 0 && dict.find(current) != dict.end()) {
      results.insert(current);
    }
    return;
  }

  //if this index is already fixed, move on to next index
  char knownchar = known[index];
  if (knownchar != '-') {
    current[index] = knownchar;
    generateWords(known, index + 1, current, floats, floatsleft, blanksleft, dict, results);
    return;
  }

  //this index is a blank, so fill it and recurse
  for (char c = 'a'; c <= 'z'; ++c) {
    current[index] = c;

    //try using c as floating letter
    if (floats[c - 'a'] > 0) {
      int nextfloats[26];
      for (int i = 0; i < 26; ++i) nextfloats[i] = floats[i];
      nextfloats[c - 'a']--;
      generateWords(known, index + 1, current, nextfloats, floatsleft - 1, blanksleft - 1, dict, results);
    }

    //try using c as non-floating filler if there are any
    if (blanksleft > floatsleft) {
      generateWords(known, index + 1, current, floats, floatsleft, blanksleft - 1, dict, results);
    }
  }
}