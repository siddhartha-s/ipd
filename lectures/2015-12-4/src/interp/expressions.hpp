#pragma once

#include <list>
#include <memory>
#include <iostream>
#include <unordered_map>

namespace expressions
{

  template <typename T>
  using list = std::list<T>;

  template <typename T>
  using unique_ptr = std::unique_ptr<T>;

  template <typename T>
  using shared_ptr = std::shared_ptr<T>;

  using string = std::string;

  template <typename K, typename V>
  using map = std::unordered_map<K,V>;

  /*********************
   values
  *********************/

  class exp;
  class environment;

  class value
  {
  public:
    virtual int getValue() {
      throw "not a number";
    } 
    virtual shared_ptr<value> apply(list<shared_ptr<value>> args) {
      throw "not a function";
    }
    virtual string toString() = 0;
  };

  class numVal : public value
  {

  public:
    numVal(int num) : num{num} {}
    int getValue() {
      return num;
    }
    string toString() {
      return std::to_string(num);
    }

  private: 
    int num;
  };

  struct closVal : public value
  {

  public:
    closVal(unique_ptr<exp> body, shared_ptr<environment> env, list<string> params) :
      body{move(body)}, env{env}, params{params} {}

    shared_ptr<value> apply(list<shared_ptr<value>>);

    string toString() {
      return "function";
    }

  private:    
    unique_ptr<exp> body;
    shared_ptr<environment> env;
    list<string> params;

  };


  /*********************
   environments
  *********************/
    
  class environment 
  {
    
  public:
    
    shared_ptr<value> lookup(string) const;
    void bind(string, shared_ptr<value>);
    
    environment() : parent{nullptr} {}
    environment(shared_ptr<environment> par) : parent{par} {}
    
  private:
    
    shared_ptr<environment> parent;
    map<string, shared_ptr<value>> bindings;

  };

 /*********************
   expressions
  *********************/

  class exp
  {
  public:
    virtual shared_ptr<value> eval(shared_ptr<environment>) = 0;
  };

  class num : public exp
  {

  public:
    shared_ptr<value> eval(shared_ptr<environment> env) {
      return std::shared_ptr<value>{new numVal(val)};
    }
    num(int n) : val{n} {}
    
  private:
    
    int val;
  };

  class numeric_op : public exp 
  {

  public:
    shared_ptr<value> eval(shared_ptr<environment>);
    numeric_op(list<unique_ptr<exp>> args) : args{move(args)} {}

  private:
    virtual int combine(int, int) const = 0;   
    list<unique_ptr<exp>> args;
 
  };

  class add : public numeric_op 
  {
  public:
    using numeric_op::numeric_op;
  private:
    int combine(int, int) const;
  };

  class sub : public numeric_op
  {
  public:
    using numeric_op::numeric_op;
  private:
    int combine(int, int) const;
  };

  class mul : public numeric_op
  {
  public:
    using numeric_op::numeric_op;
  private:
    int combine(int, int) const;
  };

  struct div : public numeric_op
  {
  public:
    using numeric_op::numeric_op;
  private:
    int combine(int, int) const;
  };

  class var : public exp
  {

  public:

    shared_ptr<value> eval(shared_ptr<environment>);
    var(string id) : id{id} {}
    
  private:
    
    string id;
    
  };

  class lambda : public exp
  {
    
  public:

    shared_ptr<value> eval(shared_ptr<environment>);
    lambda(list<string> params, unique_ptr<exp> body) :
      body{move(body)}, params{params} {}

  private:
    
    unique_ptr<exp> body;
    list<string> params;
    
  };

  class app : public exp
  {

  public:

    shared_ptr<value> eval(shared_ptr<environment>);
    
    app(list<unique_ptr<exp>> exps) :
      exps{move(exps)} {}

  private:
    
    list<unique_ptr<exp>> exps;
    
  };

  class if0 : public exp
  {
    
  public:

    shared_ptr<value> eval(shared_ptr<environment>);

    if0(unique_ptr<exp> t, unique_ptr<exp> tb, unique_ptr<exp> fb) :
      test{move(t)}, true_branch{move(tb)}, false_branch{move(fb)} {}
    
  private:

    unique_ptr<exp> test;
    unique_ptr<exp> true_branch;
    unique_ptr<exp> false_branch;
  };

  class let : public exp
  {
    
  public:

    shared_ptr<value> eval(shared_ptr<environment>);

    let(string id, unique_ptr<exp> bound, unique_ptr<exp>body) :
      id{id}, bound{move(bound)}, body{move(body)} {}
    
  private:

    string id;
    unique_ptr<exp> bound;
    unique_ptr<exp> body;

  };

  
} // namespace expressions
