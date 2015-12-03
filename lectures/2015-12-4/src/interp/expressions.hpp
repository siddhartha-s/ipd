#pragma once

#include <list>
#include <memory>
#include <iostream>
#include <map>

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
  using map = std::map<K,V>;

  /*********************
   values
  *********************/

  class exp;
  class environment;

  struct value : std::enable_shared_from_this<value>
  {
    virtual int getValue() {
      throw "not a number";
    } 
    virtual shared_ptr<value> apply(list<shared_ptr<value>> args) {
      throw "not a function";
    }
    virtual string toString() = 0;
  };

  struct numVal : public value
  {
    int num;
    numVal(int num) : num{num} {}
    int getValue() {
      return num;
    }
    string toString() {
      return std::to_string(num);
    }
  };

  struct closVal : public value
  {
    unique_ptr<exp> body;
    shared_ptr<environment> env;
    list<string> params;

    closVal(unique_ptr<exp> body, shared_ptr<environment> env, list<string> params) :
      body{move(body)}, env{env}, params{params} {}

    shared_ptr<value> apply(list<shared_ptr<value>>);

    string toString() {
      return "function";
    }
  };


  /*********************
   environments
  *********************/

  class environment : std::enable_shared_from_this<value>
  {
    
  public:
    
    shared_ptr<value> lookup(string) const;
    void bind(string, shared_ptr<value>);
    
    environment() : parent{nullptr} {}
    environment(shared_ptr<environment> par) : parent{par} {}
    
  private:
    
    shared_ptr<environment> parent;
    map<string, std::shared_ptr<value>> bindings;

  };

 /*********************
   expressions
  *********************/

  struct exp
  {
    virtual shared_ptr<value> eval(shared_ptr<environment>) = 0;
  };

  class num : public exp
  {

  public:
    shared_ptr<value> eval(shared_ptr<environment> env) {
      return std::shared_ptr<value>{new numVal(num_)};
    }
    num(int num) : num_{num} {}
    
  private:
    
    int num_;
    
  };

  struct numeric_op : public exp 
  {

    virtual int combine(int, int) const = 0;
    shared_ptr<value> eval(shared_ptr<environment>);
    numeric_op(list<unique_ptr<exp>> args) : args{move(args)} {}
   
    list<unique_ptr<exp>> args;
 
  };

  struct add : public numeric_op 
  {
    using numeric_op::numeric_op;
    int combine(int, int) const;
  };
  struct sub : public numeric_op
  {
    using numeric_op::numeric_op;
    int combine(int, int) const;
  };
  struct mul : public numeric_op
  {
    using numeric_op::numeric_op;
    int combine(int, int) const;
  };
  struct div : public numeric_op
  {
    using numeric_op::numeric_op;
    int combine(int, int) const;
  };

  class var : public exp
  {

  public:

    shared_ptr<value> eval(shared_ptr<environment>);

    var(string id) : id_{id} {}

    string getId() { return id_; }
    
  private:
    
    string id_;
    
  };

  class lambda : public exp
  {
    
  public:
    shared_ptr<value> eval(shared_ptr<environment>);

    lambda(list<string> params, unique_ptr<exp> body) :
      body_{move(body)}, params_{params} {}

  private:
    
    unique_ptr<exp> body_;
    list<string> params_;
    
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

  
} // namespace expressions
