#ifndef CPP3_SMARTCALC_V2_0_SRC_LEXEME_H
#define CPP3_SMARTCALC_V2_0_SRC_LEXEME_H

#include <string>

namespace s21 {

struct Lexeme {
 public:
  enum Code {
    kNumber = 0,
    kUnaryF = 1,
    kBinaryF = 2,
    kContext = 3,
    kBracketL = 4,
    kBracketR = 5,
    kVariable = 6,
    kError = 7,
  };
  Lexeme();
  explicit Lexeme(const std::string& name);
  Lexeme(const std::string& name, Code code);
  Lexeme(const Lexeme& other);
  Lexeme(Lexeme&& other) noexcept;
  Lexeme& operator=(const Lexeme& other);
  Lexeme& operator=(Lexeme&& other) noexcept;
  static Code DefineCode(const std::string& name);

  std::string name;
  Code code;
};

}  // namespace s21
#endif  // CPP3_SMARTCALC_V2_0_SRC_LEXEME_H
