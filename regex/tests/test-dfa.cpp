#include <assert.h>
#include <iostream>
#include <utility>
#include <map>
#include <set>
#include <string>

#include "../dfa.h"

using namespace std;

/**
 * DFA - with all transitions filled - that
 * accepts any string with AB substring.
 */
void test_1 ()
{
  int start = 0;
  dfa_tmap transition =
  {
    {make_pair(0, 'a'), 2},
    {make_pair(0, 'b'), 0},
    {make_pair(1, 'a'), 1},
    {make_pair(1, 'b'), 1},
    {make_pair(2, 'a'), 2},
    {make_pair(2, 'b'), 1},
  };
  set<int> final_states {1};

  Dfa dfa (start, transition, final_states);

  assert(dfa.validate("ab"));
  assert(dfa.validate("aab"));
  assert(dfa.validate("aaba"));

  assert(!dfa.validate("ba"));
  assert(!dfa.validate("a"));
  assert(!dfa.validate(" "));
}

Dfa make_aPLUSbPLUS_dfa()
{
  int start = 0;
  dfa_tmap transition =
  {
    {make_pair(0, 'a'), 1},
    {make_pair(1, 'a'), 3},
    {make_pair(1, 'b'), 2},
    {make_pair(2, 'b'), 4},
    {make_pair(3, 'a'), 3},
    {make_pair(3, 'b'), 4},
    {make_pair(4, 'b'), 4},
  };
  set<int> final_states {2, 4};

  return Dfa (start, transition, final_states);;
}

/**
 * DFA - without all transitions filled - that
 * recognizes a+b+ regex (+ == one ore more)
 */
void test_2 ()
{
  Dfa dfa = make_aPLUSbPLUS_dfa();

  assert(dfa.validate("ab"));
  assert(dfa.validate("aaaaab"));
  assert(dfa.validate("aabbb"));
  assert(dfa.validate("abbbb"));

  assert(!dfa.validate("ba"));
  assert(!dfa.validate("a"));
  assert(!dfa.validate("b"));
  assert(!dfa.validate(" "));
  assert(!dfa.validate("c"));
}

void test_get_states ()
{
  Dfa dfa = make_aPLUSbPLUS_dfa();

  assert(dfa.get_states() == set<int> ({0, 1, 2, 3, 4}));
}

void test_get_states_vector ()
{
  Dfa dfa = make_aPLUSbPLUS_dfa();
  vector<int> v = dfa.get_states_vector();
  assert(v == vector<int> ({0, 1, 2, 3, 4}));
}

void test_get_alphabet()
{
  Dfa dfa = make_aPLUSbPLUS_dfa();

  assert(dfa.get_alphabet() == "ab");
}

/**
 * Two dfas are equivalent if they recognize the
 * same language. Test this.
 */
void test_minimize_1 ()
{
  Dfa dfa = make_aPLUSbPLUS_dfa();
  dfa.minimize();

  assert(dfa.validate("ab"));
  assert(dfa.validate("aaaaab"));
  assert(dfa.validate("aabbb"));
  assert(dfa.validate("abbbb"));

  assert(!dfa.validate("ba"));
  assert(!dfa.validate("a"));
  assert(!dfa.validate("b"));
  assert(!dfa.validate(" "));
  assert(!dfa.validate("c"));
}

int main(int argc, char const *argv[])
{
  test_1();
  test_2();

  test_get_states();
  test_get_states_vector();

  test_get_alphabet();
  test_minimize_1();

  return 0;
}
