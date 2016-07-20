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
    shared_ptr<exp> e1{new num{1}};
    CHECK_EQUAL(1, (e1->eval(nullptr))->getValue());
  }
  
  TEST(EvalPlus)
  {
    shared_ptr<exp> n1{new num{1}};
    shared_ptr<exp> n2{new num{2}};
    list<shared_ptr<exp>> ln;
    ln.push_back(move(n1));
    ln.push_back(move(n2));
    shared_ptr<exp> ae{new add{move(ln)}};
    CHECK_EQUAL(3, ae->eval(nullptr)->getValue());
  }

  TEST(EvalMult)
  {
    shared_ptr<exp> n1{new num{1}};
    shared_ptr<exp> n2{new num{2}};
    list<shared_ptr<exp>> ln;
    ln.push_back(move(n1));
    ln.push_back(move(n2));
    shared_ptr<exp> ae{new mul{move(ln)}};
    CHECK_EQUAL(2, ae->eval(nullptr)->getValue());
  }

  TEST(EvalNested)
  {
    shared_ptr<exp> n1{new num{1}};
    shared_ptr<exp> n2{new num{2}};
    shared_ptr<exp> n3{new num{3}};
    shared_ptr<exp> n4{new num{4}};
    list<shared_ptr<exp>> ln;
    ln.push_back(move(n1));
    ln.push_back(move(n2));
    shared_ptr<exp> s12{new sub{move(ln)}};
    list<shared_ptr<exp>> ln2;
    ln2.push_back(move(n3));
    ln2.push_back(move(n4));
    shared_ptr<exp> a34{new add{move(ln2)}};
    list<shared_ptr<exp>> ln3;
    ln3.push_back(move(s12));
    ln3.push_back(move(a34));
    shared_ptr<exp> muln17{new mul{move(ln3)}};
    CHECK_EQUAL(-7, muln17->eval(nullptr)->getValue());
  }

  
  TEST(WithParse)
  {
    Parser p{"(* (- 1 2) (+ 3 4))"};
    shared_ptr<exp> e1 = p.parse();
    CHECK_EQUAL(-7, e1->eval(nullptr)->getValue());
  }


  TEST(WithParse2)
  {
    Parser p{"(+ 1 2 3 4)"};
    shared_ptr<exp> e1 = p.parse();
    CHECK_EQUAL(10, e1->eval(nullptr)->getValue());
  }

  TEST(WithParseNested)
  {
    Parser p{"(* (/ 4 2) (- (+ 2 7) (+ 3 4)))"};
    shared_ptr<exp> e = p.parse();
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

  TEST(HigherOrder)
  {
    CHECK_EQUAL("3", run("((lambda (f) (f 1)) (lambda (n) (+ n 2)))"));
  }

  TEST(Closure)
  {
    string expr =
      "("
      " ((lambda (n)"
      "   (lambda (m) (+ n m)))"
      "  2)"
      " 4)";
    CHECK_EQUAL("6", run(expr));
  }

  TEST(Nary)
  {
    CHECK_EQUAL("20", run("((lambda (m n) (* m n)) 4 5)"));
  }

  TEST(If)
  {
    CHECK_EQUAL("1", run("(if0 0 1 2)"));
    CHECK_EQUAL("2", run("(if0 3 1 2)"));
  }

  TEST(Let)
  {
    CHECK_EQUAL("77", run("(let ([z 77]) z)"));
    CHECK_EQUAL("5", run("(let ([z 77]) (let ([z (+ 2 3)]) z))"));
  }

  TEST(Factorial)
  {
    string expr =
      "(let ([fact"
      "       (lambda (rec n)"
      "         (if0 n"
      "              1"
      "              (* n (rec rec (- n 1)))))])"
       "  (fact fact 5))";
    CHECK_EQUAL("120", run(expr));
  }

  TEST(AddM)
  {
    string expr =
      "(let ([make-addm (lambda (m) (lambda (n) (+ m n)))])"
      "  (let ([add2 (make-addm 2)])"
      "    (add2 1)))";
    CHECK_EQUAL("3", run(expr));
  }
  
  TEST(AddMN)
  {
    string expr =
      "(let ([make-addm (lambda (m) (lambda (n) (+ m n)))])"
      "  (let ([add2 (make-addm 2)])"
      "    (let ([add3 (make-addm 3)])"
      "      (- (add2 2) (add3 1)))))";
    CHECK_EQUAL("0", run(expr));
  }
   

}  // namespace expressions

int
main(int, const char* [])
{
    return UnitTest::RunAllTests();
}
