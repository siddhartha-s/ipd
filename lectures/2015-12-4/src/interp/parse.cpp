#include "parse.hpp"
#include <iostream>

namespace parse
{

  list<string> tokenize(const string& input)
  {
    list<string> tokens;
    string_size i = 0;
    while (i != input.size()) {				

      // skip whitespace
      while (i != input.size() && isspace(input[i])) 
	i++;
      
      // if it's a paren, add it as a token
      if (isparen(input[i])) {
	tokens.push_back(input.substr(i,1));
	i++;
	continue;
      }

      // otherwise go until it's a paren or whitespace
      string_size j = i;
      while (j != input.size() && !isspace(input[j]) && !isparen(input[j]))
	j++;
      
      if (i != j) {
	tokens.push_back(input.substr(i, j - i));
	i = j;
      }
    }
    return tokens;
  }

  bool isparen(const char& c)
  {
    return c == '(' || c == ')';
  }

  Parser::Parser(string s) : tokens{tokenize(s)} {}

  unique_ptr<exp> Parser::parse()
  {
    cur = tokens.begin();
    end = tokens.end();
    return parse_exp();
  }

  unique_ptr<exp>
  Parser::parse_exp()
  {
    if (isNumeric(*cur)) {
      return unique_ptr<exp>{new num(std::stoi(*cur++))};
    } else if (*cur == "(") {
      string first = *++cur;
      cur++;
      list<unique_ptr<exp>> exps = move(parse_until_close());
      if (first == "+") {
	return unique_ptr<exp>{new add(move(exps))};
      } else if (first == "-") {
	return unique_ptr<exp>{new sub(move(exps))};
      } else if (first == "*") {
	return unique_ptr<exp>{new mul(move(exps))};
      } else if (first == "/") {
	return unique_ptr<exp>{new expressions::div(move(exps))};
      }
    }
  }

  list<unique_ptr<exp>>
  Parser::parse_until_close()
  {
    list<unique_ptr<exp>> exps;
    while ((*cur) != ")" && cur != end) {
      exps.push_back(move(parse_exp()));
    }
    if (cur == end)
      throw "unmatched parens";
    cur++;
    return exps;
  }
  
  bool isNumeric(string str)
  {
    return std::regex_match(str, std::regex("\\d+"));
  }


} // namespace parse
