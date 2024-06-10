#ifndef CPP3_SMARTCALC_V2_0_SRC_BONUSMODEL_H_
#define CPP3_SMARTCALC_V2_0_SRC_BONUSMODEL_H_

#include <QDate>
#include <QObject>
#include <QPair>
#include <QVector>

#include "calcoptions.h"

namespace s21 {

class BonusModel : public QObject {
  Q_OBJECT
 public:
  explicit BonusModel(QObject *parent = nullptr);
  LoanCalcOutputOptions CalculateLoan(LoanCalcInputOptions opts) const noexcept;
  DepositCalcOutputOptions CalculateDeposit(
      DepositCalcInputOptions opts) const noexcept;

 private:
  bool CheckLoanInputOptions(LoanCalcInputOptions opts) const noexcept;
  bool CheckDepositInputOptions(DepositCalcInputOptions opts) const noexcept;
  inline QDate IncrementCompoundDate(QDate date,
                                     QString compounding_period) const noexcept;
};

}  // namespace s21
#endif  // CPP3_SMARTCALC_V2_0_SRC_BONUSMODEL_H_
