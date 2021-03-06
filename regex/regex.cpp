#include "regex.h"
#include <iostream>

/**
 * As we need to parse a regex we must deal with
 * it like any other language. So, whe must
 * define a grammar for each and them utilize
 * the parsing techniques we know. For this
 * we're might use SDTS.
 */

/**
 * 1. Start with the Root, then repeatedly:
 *
 * 2. at node N, labeled with nonterminal A,
 * select one of the productions for A and
 * construct children at N for the symbols in
 * the production body.
 *
 * 2.1 : decide which A-productio to use by
 * examining the lookahead symbol: the
 * production body with alpha is used if the
 * lookahead symbol is in FIRST(alpha). If
 * there's an eps-production for A, it is used
 * if the lookahead is not found in any FIRST of
 * a-productions.
 *
 * 2.2 mimic the body of the chosen production.
 *
 * 3. find the next node at which a subtree is
 * to be constructed (leftmost unexpanded
 * nonterminal)
 */

Nfa Regex::r_single (int init, int final, char input)
{
  nfa_tmap transitions = {
    {make_pair(init, input), s_states {final}}
  };

  return Nfa (init, transitions, s_states {final}, string {input});
}

Nfa Regex::r_alternate (int init, Nfa a, Nfa b, int final)
{
  nfa_tmap transitions = {
    {make_pair(init, NFA_EPS), s_states {a.initial_state, b.initial_state}},
    {make_pair(*a.final_states.begin(), NFA_EPS), s_states {final}},
    {make_pair(*b.final_states.begin(), NFA_EPS), s_states {final}},
  };

  assign(transitions, a.transitions);
  assign(transitions, b.transitions);

  string alphabet = merge_to_unique_str(a.alphabet, b.alphabet);

  return Nfa (init, transitions, s_states {final}, alphabet);
}

Nfa Regex::r_concatenate (Nfa a, Nfa b)
{
  s_states b_initial_state = {b.initial_state};

  for (nfa_tmap::iterator it = a.transitions.begin();
       it != a.transitions.end();
       it++) {
    if (it->second == a.final_states)
      it->second = b_initial_state;
  }

  assign(a.transitions, b.transitions);

  string alphabet = merge_to_unique_str(a.alphabet, b.alphabet);

  return Nfa (a.initial_state, a.transitions, b.final_states, alphabet);
}

Nfa Regex::r_closure (int i, Nfa a, int f)
{
  nfa_tmap transitions = {
    {make_pair(i, NFA_EPS), s_states {a.initial_state, f}},
    {make_pair(*a.final_states.begin(), NFA_EPS), s_states {f, a.initial_state}},
  };

  assign(transitions, a.transitions);

  return Nfa (i, transitions, s_states {f}, a.alphabet);
}


/////////////
// PARSING //
/////////////

void Regex::_match (char t) {
  if (lookahead == t)
    while (lookahead = source[++index], lookahead == ' ')
      continue;
  else
    cerr << "Parser::match: Syntax Error." << endl;
}

void Regex::_start ()
{
  _re();
}

void Regex::_re ()
{
  _c();
  _re1();
}
void Regex::_re1 ()
{
  if (lookahead == '|') {
    _match('|');
    _c();
    // cout << "|";
    int from = counter++;
    int to = counter++;

    Nfa n1 = nfa_stack.front();
    nfa_stack.pop_front();
    Nfa n2 = nfa_stack.front();
    nfa_stack.pop_front();
    nfa_stack.push_front(r_alternate(from, n2, n1,to));

    _re1();
  }
}
void Regex::_c ()
{
  _b();
  _c1();
}
void Regex::_c1 ()
{
  if (lookahead == '.') {
    _match('.');
    _b();

    // cout << ".";

    Nfa n2 = nfa_stack.front();
    nfa_stack.pop_front();
    Nfa n1 = nfa_stack.front();
    nfa_stack.pop_front();

    nfa_stack.push_front(r_concatenate(n1, n2));

    _c1();
  }
}
void Regex::_b ()
{
  if (lookahead == '(') {
    _match('(');
    _re();
    _match(')');
    _b1();
  } else {
    _sim();
    _b1();
  }
}
void Regex::_b1 ()
{
  if (lookahead == '*') {
    _match('*');
    // cout << "*";

    Nfa n1 = nfa_stack.front();
    nfa_stack.pop_front();

    int from = counter++;
    int to = counter++;
    nfa_stack.push_front(r_closure(from, n1, to));

    _b1();
  }
}
void Regex::_sim ()
{
  char la = lookahead;

  if (is_digit(lookahead) || is_letter(lookahead)) {
    _match(lookahead);
    // cout << la;
    int from = counter++;
    int to = counter++;
    nfa_stack.push_front(r_single(from, to, la));
  }
}

void Regex::compile ()
{
  lookahead = source[index];

  _start();

  final_nfa = nfa_stack.front();
  nfa_stack.pop_front();

  final_nfa.convert();
  compiled = true;
}

bool Regex::match (string input)
{

  if (!compiled) {
    // cout << "i2p :: (" << source << ") => ";
    compile();
    // cout << endl;
  }


  // final_nfa.show_nfa();

  return final_nfa.validate(input);
}
