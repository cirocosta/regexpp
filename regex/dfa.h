#ifndef DFA_H
#define DFA_H

#include "common.h"
#include <string>
#include <algorithm>
#include <vector>
#include <utility>

using namespace std;

typedef set<int> s_states;

// TODO: get_states, get_alphabet and maybe others
// could cache their results in the class
// instance.


/**
 * Start out with the DFA in its start state
 * (q0).
 *
 * Consult the transition function to find the
 * state that de DFA enters after processing the
 * first input symbol a1 (`trans(q0,a1)`).
 *
 * Process the next input symbol a2 by
 * evaluating `trans(q1,a2)`.
 *
 * Repeat till an
 *
 * If qn is a member of F, then a1a2..an is
 * accepted (otherwise, rejected).
 */
class Dfa {
public:
  int initial_state;
  dfa_tmap transitions;
  set<int> final_states;

  Dfa () {};
  Dfa (int start) : initial_state(start) {}
  Dfa (int start, dfa_tmap trans,
       set<int> final) :
    initial_state(start),
    transitions(trans),
    final_states(final)
  {}

  /**
   * Shows the current DFA.
   */
  void show_dfa();

  /**
   * Gets the alphabet of the DFA from the
   * transition table
   */
  string get_alphabet();

  /**
   * Returns the set of states from the
   * transition function
   */
  set<int> get_states();
  /**
   * Returns a vector of the states from the
   * transition function (without repetition)
   */
  vector<int> get_states_vector();

  /**
   * Adds a state to the set of final states
   */
  void add_final_state (int i);
  /**
   * Adds a transition to the transitions table
   */
  void add_transition (int i, int j, char c);
  /**
   * Verifies if a given string is accepted or
   * not by the DFA constructed.
   */
  bool validate(string str);

  /**
   * Aims to remove two kinds of states:
   * unreachable and nondistinguishable.
   *
   * @see hopcroft's state minimization
   * algorithm
   */
  void minimize();
};

#endif
