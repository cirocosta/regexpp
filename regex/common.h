#ifndef R_COMMON_H
#define R_COMMON_H

#include <string>
using std::string;

#include <set>
#include <map>
#include <utility>
#include <iostream>
using std::cout;
using std::endl;

typedef std::set<int> s_states;
typedef std::map<std::pair<int, char>, s_states> nfa_tmap;
typedef std::map<std::pair<int, char>, int> dfa_tmap;

inline bool is_digit (char c)
{
  return c >= 48 && c <= 57;
}

inline bool is_letter (char c)
{
  return (c >= 97 && c <= 122) || (c >= 65 && c <= 90);
}

inline int char_to_int (char c)
{
  return (int)c - 48;
}

inline void show_s_states (s_states states)
{
  s_states::iterator it = states.begin();

  cout << "{";
  while (it != states.end())
    cout << " " << *it++ << " ";
  cout << "}" << endl;
}

/**
 * Adds to the target string unique characters from the source.
 * @param  str1 target
 * @param  str2 source
 */
inline string& merge_to_unique_str (string& str1, string& str2)
{
  for (char& c : str2)
    if (str1.find_first_of(c) == string::npos)
      str1.insert(str1.end(), c);

  return str1;
}

template<class T>
void assign(T& a, T& b)
{
  a.insert(b.begin(), b.end());
}

template<class A, class... B>
void assign (A& head, B... tail)
{
  assign(head, tail...);
}


#endif
