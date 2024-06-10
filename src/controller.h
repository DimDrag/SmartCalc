#ifndef CPP3_SMARTCALC_V2_0_SRC_CONTROLLER_H
#define CPP3_SMARTCALC_V2_0_SRC_CONTROLLER_H

#include <QDate>
#include <QObject>
#include <string>
#include <vector>

#include "bonusmodel.h"
#include "calcoptions.h"
#include "mainmodel.h"

namespace s21 {

class Controller : public QObject {
  Q_OBJECT

 public:
  explicit Controller(QObject* parent = nullptr);
  // SMART
  void AddLexemeToExpression(const std::string& lexeme_name) noexcept;
  void AddLexemeToVariable(const std::string& lexeme_name) noexcept;
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

  // LOAN && DEPOSIT
  LoanCalcOutputOptions CalculateLoan(LoanCalcInputOptions opts) const noexcept;
  DepositCalcOutputOptions CalculateDeposit(
      DepositCalcInputOptions opts) const noexcept;

 private:
  MainModel model_;
  BonusModel bonus_model_;
};

}  // namespace s21
#endif  // CPP3_SMARTCALC_V2_0_SRC_CONTROLLER_H
