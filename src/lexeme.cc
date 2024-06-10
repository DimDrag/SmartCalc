#include "lexeme.h"

#include <string>
#include <utility>

namespace s21 {

Lexeme::Lexeme() {}
Lexeme::Lexeme(const std::string& name) : name(name) {
  code = DefineCode(name);
}
Lexeme::Lexeme(const std::string& name, Code code) : name(name), code(code) {}
Lexeme::Lexeme(const Lexeme& other) : name(other.name), code(other.code) {}
Lexeme::Lexeme(Lexeme&& other) noexcept : name(other.name), code(other.code) {}
Lexeme& Lexeme::operator=(const Lexeme& other) {
  name = other.name;
  code = other.code;
  return *this;
}
Lexeme& Lexeme::operator=(Lexeme&& other) noexcept {
  name = std::move(other.name);
  code = other.code;
  return *this;
}

Lexeme::Code Lexeme::DefineCode(const std::string& name) {
  if (name == "sin" || name == "cos" || name == "tan" || name == "asin" ||
      name == "acos" || name == "atan" || name == "log" || name == "ln" ||
      name == "sqrt")
    return kUnaryF;
  if (name == "pow" || name == "mod" || name == "div" || name == "mul")
    return kBinaryF;
  if (name == "plus" || name == "minus") return kContext;
  if (name == "bracketL") return kBracketL;
  if (name == "bracketR") return kBracketR;
  if (name == "variable") return kVariable;
  if (name == "MATH ERROR" || name == "SYNTAX ERROR") return kError;
  return kNumber;
}

}  // namespace s21
