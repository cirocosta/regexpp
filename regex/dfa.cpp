#include "dfa.h"

using namespace std;


void Dfa::show_dfa ()
{
  cout << endl;
  cout << "-----DFA-----" << endl;
  cout << endl;
  cout << "initial_state: " << initial_state << endl;

  s_states::iterator it = final_states.begin();

  cout << "final_states#" << final_states.size() << ": ";
  while (it != final_states.end())
    cout << "(" << *it++ << ") ";
  cout << endl;

  cout << "transition_function:" << endl;

  for (dfa_tmap::iterator iter = transitions.begin();
       iter != transitions.end();
       iter++) {
    cout << iter->first.first << ":'" << iter->first.second << "' | ";
    cout << iter->second << endl;
  }

  cout << endl;
  cout << "-----------" << endl;
  cout << endl;
}

void Dfa::add_final_state (int i)
{
  final_states.insert(i);
}

void Dfa::add_transition (int start_state, int next_state, char c)
{
  transitions[make_pair(start_state, c)] = next_state;
}

bool Dfa::validate (string input)
{
  int state = initial_state;

  for (char& c : input)
    state = transitions[make_pair(state, c)];

  return final_states.count(state) != 0;
}

set<int> Dfa::get_states ()
{
  set<int> res {initial_state};
  for (auto const &it : transitions)
    res.insert(it.first.first);

  return res;
}

string Dfa::get_alphabet ()
{
  string alphabet = "";
  for (auto const &it : transitions)
    if (alphabet.find_first_of(it.first.second) == string::npos)
      alphabet.insert(alphabet.end(), it.first.second);

  return alphabet;
}

vector<int> Dfa::get_states_vector ()
{
  vector<int> res {initial_state};
  for (auto const &it : transitions)
    if (find(res.begin(), res.end(), it.first.first) == res.end())
      res.push_back(it.first.first);

  return res;
}

void Dfa::minimize ()
{
  enum Type {
    VALID,
    INVALID
  };

  const vector<int> states = get_states_vector();
  const int num_states = states.size();
  char** matrix = new char*[num_states];
  bool table_change;
  string alphabet = get_alphabet();

  for (int i = 0; i < num_states; i++) {
    matrix[i] = new char[num_states];

    for (int j = i+1; j < num_states; j++) {
      if ((final_states.count(states[i]) && !final_states.count(states[j])) ||
          (final_states.count(states[j]) && !final_states.count(states[i]))) {
        matrix[i][j] = INVALID;
      } else {
        matrix[i][j] = VALID;
      }
    }
  }

  do {
    table_change = false;

    // iterate over our table of states (i.e, the
    // pairs of those states that are different)
    for (int i = 0; i < num_states; i++) {
      for (int j = i + 1; j < num_states; j++) {
        // if we haven't excluded the current
        // state
        if (matrix[i][j] != INVALID) {
          // iterate over all possible inputs
          for (char &input : alphabet) {
            int iTarget, jTarget;
            pair<int, char> pi = make_pair(i, input);
            pair<int, char> pj = make_pair(j, input);

            iTarget = transitions.count(pi) ? transitions[pi] : -1;
            jTarget = transitions.count(pj) ? transitions[pj] : -1;

            // if one of those states has a
            // transition but the other one doesnt
            if ((!~jTarget && ~iTarget) || (~jTarget && !~iTarget)) {
              matrix[i][j] = INVALID;
              table_change = true;
            // check if both of them go to a valid
            // merge state
            } else if (~jTarget && ~iTarget) {
              if (matrix[iTarget][jTarget] == INVALID) {
                matrix[i][j] = INVALID;
                table_change = true;
              }
            }
          }
        }
      }
    }
  } while (table_change);

  // implement merge of those able to be merged.
  // MERGE(s1, s2):  supose
  // ...........-->(1)-->(2)-->...................
  // => who comes to (1) now comes to the new
  // state.   => who goes out of (2) goes to the
  // new state (itself) or to the one it used to
  // go before the merge (dont change).

  for (int i = 0; i < num_states; i++)
    delete[] matrix[i];
  delete[] matrix;
}
