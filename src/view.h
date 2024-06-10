#ifndef CPP3_SMARTCALC_V2_0_SRC_VIEW_H
#define CPP3_SMARTCALC_V2_0_SRC_VIEW_H

#include <QCloseEvent>
#include <QCoreApplication>
#include <QEvent>
#include <QMainWindow>
#include <QObject>
#include <QWidget>

#include "controller.h"
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace s21 {

class View : public QMainWindow {
  Q_OBJECT

 public:
  explicit View(QWidget* parent = nullptr);
  ~View();

 private slots:
  // SMART
  void DisplayExpression() const noexcept;
  void CloseMenu() const noexcept;
  void OpenMenu() const noexcept;
  void LexemeButtonClicked() noexcept;
  void ButtonDeleteClicked() noexcept;
  void ButtonClearClicked() noexcept;
  void ButtonEqualClicked() noexcept;
  void UpdateVariableMode() noexcept;
  void ButtonGraphClicked() noexcept;
  void DrawGraph() noexcept;
  // LOAN
  void ButtonClearLoanClicked() const noexcept;
  void ButtonCalculateLoanClicked() const noexcept;
  // DEPOSIT
  void ButtonClearDepositClicked() const noexcept;
  void ButtonCalculateDepositClicked() const noexcept;
  void UpdateContributionTable(int i) const noexcept;

 protected:
  void closeEvent(QCloseEvent* event) override;

 private:
  void UpdateGraph() const noexcept;
  bool eventFilter(QObject* watched, QEvent* event) noexcept;
  Controller controller_;
  Ui::MainWindow* ui_;
  bool variable_mode_;
  bool graph_mode_;
  QCustomPlot* plot_;

  QPoint mouse_pressed_pos_;
  QPoint mouse_moved_pos_;
};

}  // namespace s21
#endif  // CPP3_SMARTCALC_V2_0_SRC_VIEW_H
