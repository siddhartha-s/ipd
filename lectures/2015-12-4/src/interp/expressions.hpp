#pragma once

#include<list>
#include<memory>
#include<iostream>

namespace expressions
{

  template <typename T>
  using list = std::list<T>;

  template <typename T>
  using unique_ptr = std::unique_ptr<T>;

  struct value
  {
    virtual int getValue() {
      throw "not a number";
    } 
  };

  struct numVal : public value
  {
    int num;
    numVal(int num) : num{num} {}
    int getValue() {
      return num;
    }
  };

  struct exp
  {
    virtual unique_ptr<value> eval() const = 0;
  };

  class num : public exp
  {

  public:
    unique_ptr<value> eval() const {
      return std::unique_ptr<value>{new numVal(num_)};
    }
    num(int num) : num_{num} {}
    
  private:
    
    int num_;
    
  };

  struct numeric_op : public exp 
  {

    virtual int combine(int, int) const = 0;
    unique_ptr<value> eval() const;
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
  
} // namespace expressions
