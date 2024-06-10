#ifndef CPP3_SMARTCALC_V2_0_SRC_MAINMODEL_H_
#define CPP3_SMARTCALC_V2_0_SRC_MAINMODEL_H_

#include <string>
#include <vector>

#include "lexeme.h"

namespace s21 {

class MainModel {
 public:
  MainModel();
  void AddLexemeToExpression(std::string lexeme_name) noexcept;
  void AddLexemeToVariable(std::string lexeme_name) noexcept;

  void DeleteLastExpressionLexeme() noexcept;
  void DeleteLastVariableLexeme() noexcept;

  void ClearExpressionLexemes() noexcept;
  void ClearVariableLexemes() noexcept;

  std::string GetStringExpression() const noexcept;
  std::string GetStringVariable() const noexcept;

  bool VariableEntered() const noexcept;
  void Solve() noexcept;
  void GetPlotData(double& x_min, double& x_max, double& y_min, double& y_max,
                   std::vector<double>& x,
                   std::vector<double>& y) const noexcept;
  void MovePlotXAxis(double delta) noexcept;
  void MovePlotYAxis(double delta) noexcept;
  void ScalePlot(double scale) noexcept;
  void ResetPlot() noexcept;

 private:
  void CalcPlotData(std::vector<double>& x,
                    std::vector<double>& y) const noexcept;
  std::vector<Lexeme> LexemesToPolishNotation() const noexcept;
  long double SolveWithoutVariable(
      std::vector<Lexeme> polish_lexemes) const noexcept;
  bool VerifyAddedLexeme() const noexcept;
  std::string RemoveTrailingZeros(std::string string_number) const noexcept;
  bool VerifyBeforeSolving() const noexcept;
  int GetBinaryOperatorPriority(
      const std::string& operator_name) const noexcept;

  std::vector<Lexeme> expression_;
  Lexeme variable_;

  double x_min_ = -5;
  double x_max_ = 5;
  double y_min_ = -5;
  double y_max_ = 5;
  double scale_ = 1;
};

}  // namespace s21
#endif  // CPP3_SMARTCALC_V2_0_SRC_MAINMODEL_H_
