#include "interp/expressions.hpp"
#include "interp/parse.hpp"
#include <UnitTest++/UnitTest++.h>
#include <iostream>

namespace expressions
{

  using namespace parse;
  
  TEST(True)
  {
    CHECK(true);
  }
  
  TEST(EvalNum)
  {
    unique_ptr<exp> e1{new num{1}};
    CHECK_EQUAL(1, (e1->eval(nullptr))->getValue());
  }
  
  TEST(EvalPlus)
  {
    unique_ptr<exp> n1{new num{1}};
    unique_ptr<exp> n2{new num{2}};
    list<unique_ptr<exp>> ln;
    ln.push_back(move(n1));
    ln.push_back(move(n2));
    unique_ptr<exp> ae{new add{move(ln)}};
    CHECK_EQUAL(3, ae->eval(nullptr)->getValue());
  }

  TEST(EvalMult)
  {
    unique_ptr<exp> n1{new num{1}};
    unique_ptr<exp> n2{new num{2}};
    list<unique_ptr<exp>> ln;
    ln.push_back(move(n1));
    ln.push_back(move(n2));
    unique_ptr<exp> ae{new mul{move(ln)}};
    CHECK_EQUAL(2, ae->eval(nullptr)->getValue());
  }

  TEST(EvalNested)
  {
    unique_ptr<exp> n1{new num{1}};
    unique_ptr<exp> n2{new num{2}};
    unique_ptr<exp> n3{new num{3}};
    unique_ptr<exp> n4{new num{4}};
    list<unique_ptr<exp>> ln;
    ln.push_back(move(n1));
    ln.push_back(move(n2));
    unique_ptr<exp> s12{new sub{move(ln)}};
    list<unique_ptr<exp>> ln2;
    ln2.push_back(move(n3));
    ln2.push_back(move(n4));
    unique_ptr<exp> a34{new add{move(ln2)}};
    list<unique_ptr<exp>> ln3;
    ln3.push_back(move(s12));
    ln3.push_back(move(a34));
    unique_ptr<exp> muln17{new mul{move(ln3)}};
    CHECK_EQUAL(-7, muln17->eval(nullptr)->getValue());
  }

  TEST(WithParse)
  {
    Parser p{"(+ 1 2 3 4)"};
    unique_ptr<exp> e1 = p.parse();
    CHECK_EQUAL(10, e1->eval(nullptr)->getValue());
  }

  TEST(WithParseNested)
  {
    Parser p{"(* (/ 4 2) (- (+ 2 7) (+ 3 4)))"};
    unique_ptr<exp> e = p.parse();
    CHECK_EQUAL(4, e->eval(nullptr)->getValue());
  }

  TEST(Run)
  {
    CHECK_EQUAL("10", run("(+ 1 2 3 4)"));
  }

  TEST(Lambda)
  {
    CHECK_EQUAL("function", run("(lambda (x) x)"));
  }

  TEST(Apply)
  {
    CHECK_EQUAL("1", run("((lambda (x) x) 1)"));
  }

}  // namespace parse

int
main(int, const char* [])
{
    return UnitTest::RunAllTests();
}
