#include "bonusmodel.h"

#include <QDate>
#include <QObject>
#include <QPair>
#include <QVector>
#include <QtMath>

#include "calcoptions.h"

namespace s21 {

BonusModel::BonusModel(QObject *parent) : QObject{parent} {}

LoanCalcOutputOptions BonusModel::CalculateLoan(
    LoanCalcInputOptions opts) const noexcept {
  if (!CheckLoanInputOptions(opts)) {
    return LoanCalcOutputOptions();
  }
  long double loan_amount = opts.loan_amount.toDouble();
  int loan_term = opts.loan_term.toInt();
  long double interest_rate = opts.interest_rate.toDouble();

  LoanCalcOutputOptions res;
  loan_term += loan_term * 11 * (opts.loan_term_type == "years");
  interest_rate /= 1200;
  if (opts.loan_type == LoanCalcInputOptions::kAmortized) {
    long double q = 1 + interest_rate;
    long double monthly_payment =
        qPow(q, loan_term) * loan_amount / ((1 - qPow(q, loan_term)) / (1 - q));
    long double total_payment = monthly_payment * loan_term;
    long double total_interest = total_payment - loan_amount;
    res.monthly_payment = QString::number(monthly_payment, 'f', 2);
    res.total_payment = QString::number(total_payment, 'f', 2);
    res.total_interest = QString::number(total_interest, 'f', 2);

    long double beginning_balance_value = loan_amount;
    for (int i = 0; i < loan_term; ++i) {
      long double ending_balance_value =
          beginning_balance_value * q - monthly_payment;
      res.payment_data.push_back(QPair<QString, QString>(
          QString::number(monthly_payment, 'f', 2),
          QString::number(i == loan_term - 1 ? 0 : ending_balance_value, 'f',
                          2)));
      beginning_balance_value = ending_balance_value;
    }
  } else if (opts.loan_type == LoanCalcInputOptions::kDifferential) {
    long double first_month_payment =
        loan_amount * (1.0 / loan_term + interest_rate);
    long double last_month_payment =
        (loan_amount + loan_amount * interest_rate) / loan_term;
    long double total_payment =
        loan_amount * (1 + interest_rate * (loan_term + 1) / 2);
    long double total_interest = total_payment - loan_amount;
    res.monthly_payment = QString::number(first_month_payment, 'f', 2) +
                          " ... " + QString::number(last_month_payment, 'f', 2);
    res.total_payment = QString::number(total_payment, 'f', 2);
    res.total_interest = QString::number(total_interest, 'f', 2);

    for (int i = 0; i < loan_term; ++i) {
      long double current_month_payment =
          loan_amount *
          (1.0 / loan_term + interest_rate - interest_rate * i / loan_term);
      long double ending_balance_value =
          loan_amount * (1 - (long double)i / loan_term);
      res.payment_data.push_back(QPair<QString, QString>(
          QString::number(current_month_payment, 'f', 2),
          QString::number(i == loan_term - 1 ? 0 : ending_balance_value, 'f',
                          2)));
    }
  }
  return res;
}

DepositCalcOutputOptions BonusModel::CalculateDeposit(
    DepositCalcInputOptions opts) const noexcept {
  if (!CheckDepositInputOptions(opts)) {
    return DepositCalcOutputOptions();
  }
  DepositCalcOutputOptions res;
  long double current_balance = opts.initial_investment.toDouble();
  long double interest_rate = opts.interest_rate.toDouble() / 100;
  long double tax_rate = opts.tax_rate.toDouble() / 100;
  long double current_interest = 0;
  long double total_interest = 0;
  long double total_tax = 0;
  int investment_term = opts.investment_term.toInt();
  investment_term +=
      investment_term * 11 * (opts.investment_term_type == "years");
  QDate first_date = opts.investment_date;
  QDate last_date = first_date.addMonths(investment_term);

  QDate compound_date =
      IncrementCompoundDate(first_date, opts.compounding_period);
  for (QDate cur_date = first_date.addDays(1); cur_date != last_date.addDays(1);
       cur_date = cur_date.addDays(1)) {
    int days_in_cur_year = cur_date.isLeapYear(cur_date.year()) ? 366 : 365;
    current_interest += current_balance * (interest_rate / days_in_cur_year);
    if (cur_date == compound_date) {
      if (opts.capitalization == DepositCalcInputOptions::kWithCapitalization) {
        current_balance += current_interest;
      }
      total_interest += current_interest;
      current_interest = 0;
      compound_date =
          IncrementCompoundDate(compound_date, opts.compounding_period);
    }
    for (int i = 0; i < opts.contributions.size(); ++i) {
      if (cur_date == opts.contributions[i].first) {
        current_balance += opts.contributions[i].second.toDouble();
        if (current_balance < 0) {
          return DepositCalcOutputOptions();
        }
      }
    }
  }
  total_interest += current_interest;
  total_tax = total_interest * tax_rate;
  total_interest -= total_tax;
  if (opts.capitalization == DepositCalcInputOptions::kWithCapitalization) {
    current_balance += current_interest;
    current_balance -= total_tax;
  }
  res.total_interest = QString::number(total_interest, 'f', 2);
  res.total_tax = QString::number(total_tax, 'f', 2);
  res.ending_balance = QString::number(current_balance, 'f', 2);
  return res;
}

bool BonusModel::CheckDepositInputOptions(
    DepositCalcInputOptions opts) const noexcept {
  bool current_balance_convertation_flag = false;
  bool interest_rate_convertation_flag = false;
  bool tax_rate_convertation_flag = false;
  bool investment_length_convertation_flag = false;

  double initial_investment =
      opts.initial_investment.toDouble(&current_balance_convertation_flag);
  double interest_rate =
      opts.interest_rate.toDouble(&interest_rate_convertation_flag);
  double tax_rate = opts.tax_rate.toDouble(&tax_rate_convertation_flag);
  int investment_term =
      opts.investment_term.toInt(&investment_length_convertation_flag);
  for (int i = 0; i < opts.contributions.size(); ++i) {
    bool contribution_convertation_flag = true;
    opts.contributions[i].second.toDouble(&contribution_convertation_flag);
    if (!contribution_convertation_flag) {
      return false;
    }
  }

  return (current_balance_convertation_flag &&
          interest_rate_convertation_flag && tax_rate_convertation_flag &&
          investment_length_convertation_flag && initial_investment > 0 &&
          interest_rate > 0 && tax_rate > 0 && investment_term > 0);
}

bool BonusModel::CheckLoanInputOptions(
    LoanCalcInputOptions opts) const noexcept {
  bool loan_amount_convertation_flag = false;
  bool loan_term_convertation_flag = false;
  bool interest_rate_convertation_flag = false;

  long double loan_amount =
      opts.loan_amount.toDouble(&loan_amount_convertation_flag);
  int loan_term = opts.loan_term.toInt(&loan_term_convertation_flag);
  long double interest_rate =
      opts.interest_rate.toDouble(&interest_rate_convertation_flag);

  return (loan_amount_convertation_flag && loan_term_convertation_flag &&
          interest_rate_convertation_flag && loan_amount > 0 && loan_term > 0 &&
          interest_rate > 0);
}

inline QDate BonusModel::IncrementCompoundDate(
    QDate date, QString compounding_period) const noexcept {
  if (compounding_period == "annually") return date.addYears(1);
  if (compounding_period == "semiannually") return date.addMonths(6);
  if (compounding_period == "quarterly") return date.addMonths(3);
  if (compounding_period == "monthly") return date.addMonths(1);
  if (compounding_period == "daily") return date.addDays(1);
  if (compounding_period == "at the end") return date.addDays(-1);
  return date;
}

}  // namespace s21
