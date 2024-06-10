#include "controller.h"

#include <QDate>
#include <QObject>
#include <string>
#include <vector>

#include "calcoptions.h"

namespace s21 {

Controller::Controller(QObject* parent) : QObject{parent} {}

void Controller::AddLexemeToExpression(
    const std::string& lexeme_name) noexcept {
  model_.AddLexemeToExpression(lexeme_name);
}
void Controller::AddLexemeToVariable(const std::string& lexeme_name) noexcept {
  model_.AddLexemeToVariable(lexeme_name);
}

void Controller::DeleteLastExpressionLexeme() noexcept {
  model_.DeleteLastExpressionLexeme();
}
void Controller::DeleteLastVariableLexeme() noexcept {
  model_.DeleteLastVariableLexeme();
}

void Controller::ClearExpressionLexemes() noexcept {
  model_.ClearExpressionLexemes();
}
void Controller::ClearVariableLexemes() noexcept {
  model_.ClearVariableLexemes();
}

void Controller::Solve() noexcept { model_.Solve(); }

std::string Controller::GetStringExpression() const noexcept {
  return model_.GetStringExpression();
}
std::string Controller::GetStringVariable() const noexcept {
  return model_.GetStringVariable();
}

bool Controller::VariableEntered() const noexcept {
  return model_.VariableEntered();
}

void Controller::GetPlotData(double& x_min, double& x_max, double& y_min,
                             double& y_max, std::vector<double>& x,
                             std::vector<double>& y) const noexcept {
  model_.GetPlotData(x_min, x_max, y_min, y_max, x, y);
}

void Controller::MovePlotXAxis(double delta) noexcept {
  model_.MovePlotXAxis(delta);
}

void Controller::MovePlotYAxis(double delta) noexcept {
  model_.MovePlotYAxis(delta);
}

void Controller::ScalePlot(double scale) noexcept { model_.ScalePlot(scale); }

void Controller::ResetPlot() noexcept { model_.ResetPlot(); }

LoanCalcOutputOptions Controller::CalculateLoan(
    LoanCalcInputOptions opts) const noexcept {
  return bonus_model_.CalculateLoan(opts);
}

DepositCalcOutputOptions Controller::CalculateDeposit(
    DepositCalcInputOptions opts) const noexcept {
  return bonus_model_.CalculateDeposit(opts);
}

}  // namespace s21
