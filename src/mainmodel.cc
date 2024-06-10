#include "mainmodel.h"

#include <math.h>

#include <algorithm>
#include <iostream>
#include <regex>
#include <stack>
#include <string>
#include <vector>

namespace s21 {

MainModel::MainModel() { variable_.code = Lexeme::kNumber; }

void MainModel::AddLexemeToExpression(std::string lexeme_name) noexcept {
  if (!expression_.empty() && expression_.back().code == Lexeme::kError) {
    expression_.pop_back();
  }
  switch (Lexeme::DefineCode(lexeme_name)) {
    case Lexeme::kBinaryF:
    case Lexeme::kBracketR:
    case Lexeme::kBracketL:
    case Lexeme::kVariable:
    case Lexeme::kUnaryF:
      expression_.push_back(Lexeme(lexeme_name));
      break;
    case Lexeme::kNumber:
      if (!expression_.empty() && expression_.back().code == Lexeme::kNumber) {
        if (!expression_.back().name.empty() &&
            expression_.back().name.find('.') == std::string::npos &&
            lexeme_name != "." && expression_.back().name[0] == '0') {
          expression_.back().name = lexeme_name;
        } else {
          expression_.back().name += lexeme_name;
        }
      } else {
        expression_.push_back(Lexeme(lexeme_name));
      }
      break;
    case Lexeme::kContext:
      if (!expression_.empty() && expression_.back().name.back() == 'E') {
        if (lexeme_name == "minus") lexeme_name = "-";
        if (lexeme_name == "plus") lexeme_name = "+";
        expression_.back().name += lexeme_name;
      } else if (expression_.empty() ||
                 expression_.back().code == Lexeme::kUnaryF ||
                 expression_.back().code == Lexeme::kBinaryF ||
                 expression_.back().code == Lexeme::kBracketL) {
        expression_.push_back(Lexeme(lexeme_name, Lexeme::kUnaryF));
      } else {
        expression_.push_back(Lexeme(lexeme_name, Lexeme::kBinaryF));
      }
      break;
    default:
      break;
  }
  if (VerifyAddedLexeme()) {
    if (Lexeme::DefineCode(lexeme_name) == Lexeme::kUnaryF) {
      expression_.push_back(Lexeme("bracketL"));
    }
  } else {
    DeleteLastExpressionLexeme();
  }
}

void MainModel::AddLexemeToVariable(std::string lexeme_name) noexcept {
  if (variable_.code == Lexeme::kError) {
    variable_.code = Lexeme::kNumber;
    variable_.name = "";
  }
  // double dot in number (like 1.234.5)
  if (lexeme_name == "." && variable_.name.find('.') != std::string::npos) {
    return;
  }
  if (lexeme_name == "minus") lexeme_name = "-";
  if (lexeme_name == "plus") lexeme_name = "+";
  if (lexeme_name == "-" || lexeme_name == "+") {
    if (variable_.name.empty() || variable_.name.back() == 'E') {
      variable_.name += lexeme_name;
    } else if (variable_.name.back() == '-' || variable_.name.back() == '+') {
      variable_.name.back() = lexeme_name[0];
    }

  } else if (lexeme_name == "E") {
    if (!variable_.name.empty() && std::isdigit(variable_.name.back())) {
      variable_.name += lexeme_name;
    }
  } else {
    if (!variable_.name.empty() &&
        variable_.name.find('.') == std::string::npos && lexeme_name != "." &&
        (variable_.name[0] == '0' || variable_.name[1] == '0')) {
      if (variable_.name[0] == '0') variable_.name[0] = lexeme_name[0];
      if (variable_.name[1] == '0') variable_.name[1] = lexeme_name[0];
    } else {
      variable_.name += lexeme_name;
    }
  }
}

void MainModel::DeleteLastExpressionLexeme() noexcept {
  if (expression_.empty()) return;
  if (expression_.back().code == Lexeme::kNumber) {
    expression_.back().name.pop_back();
    if (expression_.back().name.empty()) {
      expression_.pop_back();
    }
    return;
  }
  if (expression_.back().code == Lexeme::kBracketL &&
      expression_[expression_.size() - 2].code == Lexeme::kUnaryF) {
    expression_.pop_back();
  }
  expression_.pop_back();
}

void MainModel::DeleteLastVariableLexeme() noexcept {
  if (!variable_.name.empty()) {
    variable_.name.pop_back();
  }
}

void MainModel::ClearExpressionLexemes() noexcept { expression_.clear(); }

void MainModel::ClearVariableLexemes() noexcept { variable_.name.clear(); }

std::string MainModel::GetStringExpression() const noexcept {
  if (expression_.empty()) return std::string("");
  if (expression_.back().code == Lexeme::kError) {
    return expression_.back().name;
  }
  std::string result = "";
  for (const auto& i : expression_) {
    std::string curLexemeName = i.name;
    if (i.code == Lexeme::kBinaryF) {
      curLexemeName = "&nbsp;" + curLexemeName + "&nbsp;";
    }
    result += curLexemeName;
  }
  result = std::regex_replace(result, std::regex("sqrt"), "√");
  result = std::regex_replace(result, std::regex("pow"), "^");
  result = std::regex_replace(result, std::regex("bracketL"), "(");
  result = std::regex_replace(result, std::regex("bracketR"), ")");
  result = std::regex_replace(result, std::regex("div"), "÷");
  result = std::regex_replace(result, std::regex("mul"), "×");
  result = std::regex_replace(result, std::regex("plus"), "+");
  result = std::regex_replace(result, std::regex("minus"), "-");
  result = std::regex_replace(result, std::regex("variable"), "X");
  return result;
}

std::string MainModel::GetStringVariable() const noexcept {
  return variable_.name;
}

bool MainModel::VariableEntered() const noexcept {
  return std::any_of(expression_.cbegin(), expression_.cend(),
                     [](const auto& i) { return i.code == Lexeme::kVariable; });
}

std::vector<Lexeme> MainModel::LexemesToPolishNotation() const noexcept {
  std::vector<Lexeme> result;
  std::stack<Lexeme> stack;
  for (const auto& i : expression_) {
    switch (i.code) {
      case Lexeme::kNumber:
      case Lexeme::kVariable:
        result.push_back(i);
        break;
      case Lexeme::kUnaryF:
      case Lexeme::kBracketL:
        stack.push(i);
        break;
      case Lexeme::kBracketR:
        while (stack.top().code != Lexeme::kBracketL) {
          result.push_back(stack.top());
          stack.pop();
        }
        stack.pop();
        break;
      case Lexeme::kBinaryF:
        while (!stack.empty() && (stack.top().code == Lexeme::kUnaryF ||
                                  GetBinaryOperatorPriority(stack.top().name) >=
                                      GetBinaryOperatorPriority(i.name))) {
          result.push_back(stack.top());
          stack.pop();
        }
        stack.push(i);
        break;
      default:
        break;
    }
  }
  while (!stack.empty()) {
    result.push_back(stack.top());
    stack.pop();
  }
  return result;
}

void MainModel::Solve() noexcept {
  if (!expression_.empty() && expression_.back().code == Lexeme::kError) {
    return;
  }
  if (!VerifyBeforeSolving()) {
    if (VariableEntered() && !variable_.name.empty()) {
      variable_.name = "SYNTAX ERROR";
      variable_.code = Lexeme::kError;
    } else {
      expression_.push_back(Lexeme("SYNTAX ERROR"));
    }
    return;
  }
  std::vector<Lexeme> polish_lexemes = LexemesToPolishNotation();
  if (VariableEntered()) {
    if (variable_.name == "") {
      variable_.name = "ENTER X:";
      variable_.code = Lexeme::kError;
      return;
    }
    std::replace_if(
        polish_lexemes.begin(), polish_lexemes.end(),
        [](const auto& i) { return i.code == Lexeme::kVariable; }, variable_);
  }
  double ans = SolveWithoutVariable(polish_lexemes);
  if (isinf(ans) || isnan(ans)) {
    if (variable_.name != "") {
      variable_ = Lexeme("MATH ERROR");
    } else {
      expression_.push_back(Lexeme("MATH ERROR"));
    }
  } else {
    ClearExpressionLexemes();
    ClearVariableLexemes();
    std::string s(100, '\0');
    auto written = std::snprintf(&s[0], s.size(), "%.7f", ans);
    s.resize(written);
    expression_.push_back(Lexeme(RemoveTrailingZeros(s)));
  }
}

long double MainModel::SolveWithoutVariable(
    std::vector<Lexeme> polish_lexemes) const noexcept {
  std::stack<long double> stack;
  for (const auto& i : polish_lexemes) {
    switch (i.code) {
      case Lexeme::kNumber:
      case Lexeme::kVariable: {
        stack.push(std::stold(i.name));
        break;
      }
      case Lexeme::kUnaryF: {
        long double operand = stack.top();
        stack.pop();
        if (i.name == "sin") {
          stack.push(sin(operand));
        } else if (i.name == "cos") {
          stack.push(cos(operand));
        } else if (i.name == "tan") {
          stack.push(tan(operand));
        } else if (i.name == "asin") {
          stack.push(asin(operand));
        } else if (i.name == "acos") {
          stack.push(acos(operand));
        } else if (i.name == "atan") {
          stack.push(atan(operand));
        } else if (i.name == "log") {
          stack.push(log10(operand));
        } else if (i.name == "ln") {
          stack.push(log(operand));
        } else if (i.name == "sqrt") {
          stack.push(sqrt(operand));
        } else if (i.name == "minus") {
          stack.push(operand * (-1));
        } else if (i.name == "plus") {
          stack.push(operand);
        }
        break;
      }
      case Lexeme::kBinaryF: {
        long double operand1 = stack.top();
        stack.pop();
        long double operand2 = stack.top();
        stack.pop();
        if (i.name == "pow") {
          stack.push(pow(operand2, operand1));
        } else if (i.name == "mod") {
          stack.push(fmodl(operand2, operand1));
        } else if (i.name == "div") {
          stack.push(operand2 / operand1);
        } else if (i.name == "plus") {
          stack.push(operand2 + operand1);
        } else if (i.name == "minus") {
          stack.push(operand2 - operand1);
        } else if (i.name == "mul") {
          stack.push(operand2 * operand1);
        }
        break;
      }
      default:
        break;
    }
  }
  long double ans = 0;
  if (!stack.empty()) {
    ans = stack.top();
  }
  return ans;
}

void MainModel::CalcPlotData(std::vector<double>& x,
                             std::vector<double>& y) const noexcept {
  if (expression_.empty() || !VerifyBeforeSolving()) {
    return;
  }
  double h = (x_max_ - x_min_) / 2000;
  std::vector<Lexeme> polish_lexemes = LexemesToPolishNotation();
  for (double curX = x_min_; curX < x_max_; curX += h) {
    for (auto& i : polish_lexemes) {
      if (i.code == Lexeme::kVariable) {
        i.name = std::to_string(curX);
      }
    }
    double curY = SolveWithoutVariable(polish_lexemes);
    x.push_back(curX);
    y.push_back(curY);
  }
}

void MainModel::MovePlotXAxis(double delta) noexcept {
  delta = delta * scale_ / 60.0;
  x_min_ += delta;
  x_max_ += delta;
}

void MainModel::MovePlotYAxis(double delta) noexcept {
  delta = delta * scale_ / 60.0;
  y_min_ += delta;
  y_max_ += delta;
}

void MainModel::ScalePlot(double scale) noexcept {
  scale_ *= scale;
  x_min_ *= scale;
  x_max_ *= scale;
  y_min_ *= scale;
  y_max_ *= scale;
}

void MainModel::GetPlotData(double& x_min, double& x_max, double& y_min,
                            double& y_max, std::vector<double>& x,
                            std::vector<double>& y) const noexcept {
  x_min = x_min_;
  x_max = x_max_;
  y_min = y_min_;
  y_max = y_max_;
  CalcPlotData(x, y);
}

void MainModel::ResetPlot() noexcept {
  x_min_ = -5;
  x_max_ = 5;
  y_min_ = -5;
  y_max_ = 5;
  scale_ = 1;
}

bool MainModel::VerifyAddedLexeme() const noexcept {
  // sequence matches ")..." or "/..."
  if (expression_[0].code == Lexeme::kBinaryF ||
      expression_[0].code == Lexeme::kBracketR) {
    return false;
  }
  // double dot in number (like 1.234.5)
  if (expression_.back().code == Lexeme::kNumber &&
      expression_.back().name.find_first_of(".") !=
          expression_.back().name.find_last_of(".")) {
    return false;
  }
  // number lexeme matches "E..."
  if (expression_.back().code == Lexeme::kNumber &&
      expression_.back().name[0] == 'E') {
    return false;
  }
  // number lexeme matches ".E"
  if (expression_.back().code == Lexeme::kNumber &&
      expression_.back().name.find(".E") != std::string::npos) {
    return false;
  }
  if (expression_.size() < 2) {
    return true;
  }

  Lexeme added = expression_.back();
  Lexeme previous = expression_[expression_.size() - 2];

  // lexeme name matches "."
  if (previous.code == Lexeme::kNumber && previous.name == ".") {
    return false;
  }
  // other cases
  switch (added.code) {
    case Lexeme::kUnaryF:
    case Lexeme::kNumber:
    case Lexeme::kBracketL:
    case Lexeme::kVariable:
      if (previous.code == Lexeme::kNumber) return false;
      if (previous.code == Lexeme::kVariable) return false;
      if (previous.code == Lexeme::kBracketR) return false;
      break;
    case Lexeme::kBinaryF:
    case Lexeme::kBracketR:
      if (previous.code == Lexeme::kUnaryF) return false;
      if (previous.code == Lexeme::kBinaryF) return false;
      if (previous.code == Lexeme::kBracketL) return false;
      break;
    default:
      break;
  }
  return true;
}

std::string MainModel::RemoveTrailingZeros(
    std::string string_number) const noexcept {
  while (string_number.back() == '0' && string_number.size() > 1) {
    string_number.pop_back();
  }
  if (string_number.back() == '.') {
    string_number.pop_back();
  }
  return string_number;
}

bool MainModel::VerifyBeforeSolving() const noexcept {
  if (VariableEntered() && !variable_.name.empty() &&
      (variable_.name == "-." || variable_.name == "+." ||
       variable_.name == ".")) {
    return false;
  }
  int brackets_balance = 0;
  for (const auto& i : expression_) {
    brackets_balance += (i.code == Lexeme::kBracketL);
    brackets_balance -= (i.code == Lexeme::kBracketR);
  }
  if (brackets_balance != 0 ||
      (!expression_.empty() && expression_.back().code != Lexeme::kNumber &&
       expression_.back().code != Lexeme::kBracketR &&
       expression_.back().code != Lexeme::kVariable)) {
    return false;
  }
  return true;
}

int MainModel::GetBinaryOperatorPriority(
    const std::string& operator_name) const noexcept {
  if (operator_name == "pow") {
    return 3;
  }
  if (operator_name == "mod" || operator_name == "mul" ||
      operator_name == "div") {
    return 2;
  }
  if (operator_name == "minus" || operator_name == "plus") {
    return 1;
  }
  return -1;
}

}  // namespace s21
