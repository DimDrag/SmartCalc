#ifndef CPP3_SMARTCALC_V2_0_SRC_CALCOPTIONS_H
#define CPP3_SMARTCALC_V2_0_SRC_CALCOPTIONS_H

#include <QDate>
#include <QPair>
#include <QString>
#include <QVector>
#include <vector>

struct LoanCalcInputOptions {
  enum LoanType { kAmortized, kDifferential } loan_type;
  QString loan_amount;
  QString loan_term;
  QString loan_term_type;
  QString interest_rate;
};

struct LoanCalcOutputOptions {
  QString monthly_payment;
  QString total_payment;
  QString total_interest;
  QVector<QPair<QString, QString>> payment_data;
};

struct DepositCalcInputOptions {
  enum Capitalization { kWithCapitalization, kNoCapitalization } capitalization;
  QString initial_investment;
  QString investment_term;
  QString investment_term_type;
  QDate investment_date;
  QString interest_rate;
  QString tax_rate;
  QString compounding_period;
  QVector<QPair<QDate, QString>> contributions;
};

struct DepositCalcOutputOptions {
  QString total_interest;
  QString total_tax;
  QString ending_balance;
};

#endif  // CPP3_SMARTCALC_V2_0_SRC_CALCOPTIONS_H
