#include "view.h"

#include <QCloseEvent>
#include <QColor>
#include <QDateEdit>
#include <QEvent>
#include <QGridLayout>
#include <QList>
#include <QMainWindow>
#include <QMenu>
#include <QMouseEvent>
#include <QObject>
#include <QPair>
#include <QPen>
#include <QPushButton>
#include <QString>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVector>
#include <QWheelEvent>
#include <QWidget>
#include <string>
#include <vector>

#include "calcoptions.h"
#include "qcustomplot.h"
#include "ui_mainwindow.h"

namespace s21 {

View::View(QWidget *parent) : QMainWindow(parent), ui_(new Ui::MainWindow) {
  variable_mode_ = false;
  graph_mode_ = false;
  plot_ = nullptr;
  ui_->setupUi(this);
  // menu buttons initialize
  QPushButton *btn_sin = new QPushButton("sin");
  QPushButton *btn_cos = new QPushButton("cos");
  QPushButton *btn_tan = new QPushButton("tan");
  QPushButton *btn_asin = new QPushButton("asin");
  QPushButton *btn_acos = new QPushButton("acos");
  QPushButton *btn_atan = new QPushButton("atan");
  QPushButton *btn_log = new QPushButton("log");
  QPushButton *btn_ln = new QPushButton("ln");
  QPushButton *btn_mod = new QPushButton("mod");
  btn_sin->setShortcut(QKeySequence(Qt::Key_S, Qt::Key_I, Qt::Key_N));
  btn_cos->setShortcut(QKeySequence(Qt::Key_C, Qt::Key_O, Qt::Key_S));
  btn_tan->setShortcut(QKeySequence(Qt::Key_T, Qt::Key_A, Qt::Key_N));
  btn_asin->setShortcut(
      QKeySequence(Qt::Key_A, Qt::Key_S, Qt::Key_I, Qt::Key_N));
  btn_acos->setShortcut(
      QKeySequence(Qt::Key_A, Qt::Key_C, Qt::Key_O, Qt::Key_S));
  btn_atan->setShortcut(
      QKeySequence(Qt::Key_A, Qt::Key_T, Qt::Key_A, Qt::Key_N));
  btn_log->setShortcut(QKeySequence(Qt::Key_L, Qt::Key_O, Qt::Key_G));
  btn_ln->setShortcut(QKeySequence(Qt::Key_L, Qt::Key_N));
  btn_mod->setShortcut(QKeySequence(Qt::Key_M, Qt::Key_O, Qt::Key_D));
  btn_sin->setProperty("lexemeName", "sin");
  btn_cos->setProperty("lexemeName", "cos");
  btn_tan->setProperty("lexemeName", "tan");
  btn_asin->setProperty("lexemeName", "asin");
  btn_acos->setProperty("lexemeName", "acos");
  btn_atan->setProperty("lexemeName", "atan");
  btn_log->setProperty("lexemeName", "log");
  btn_ln->setProperty("lexemeName", "ln");
  btn_mod->setProperty("lexemeName", "mod");

  QMenu *trigonometry_menu = new QMenu(ui_->btnTrigonometryMenu);
  QGridLayout *trigonometry_menu_layout = new QGridLayout(trigonometry_menu);
  trigonometry_menu_layout->addWidget(btn_sin, 0, 0);
  trigonometry_menu_layout->addWidget(btn_cos, 0, 1);
  trigonometry_menu_layout->addWidget(btn_tan, 0, 2);
  trigonometry_menu_layout->addWidget(btn_asin, 1, 0);
  trigonometry_menu_layout->addWidget(btn_acos, 1, 1);
  trigonometry_menu_layout->addWidget(btn_atan, 1, 2);
  trigonometry_menu_layout->setMargin(7);
  trigonometry_menu->setAttribute(Qt::WA_TranslucentBackground);
  ui_->btnTrigonometryMenu->setMenu(trigonometry_menu);

  QMenu *functions_menu = new QMenu(ui_->btnFunctionsMenu);
  QGridLayout *functions_menu_layout = new QGridLayout(functions_menu);
  functions_menu_layout->addWidget(btn_log, 0, 0);
  functions_menu_layout->addWidget(btn_ln, 0, 1);
  functions_menu_layout->addWidget(btn_mod, 0, 3);
  functions_menu_layout->setMargin(7);
  functions_menu->setAttribute(Qt::WA_TranslucentBackground);
  ui_->btnFunctionsMenu->setMenu(functions_menu);

  // initialize button groups
  QList<QPushButton *> lexeme_buttons = {
      ui_->btnDigit0,   ui_->btnDigit1, ui_->btnDigit2,   ui_->btnDigit3,
      ui_->btnDigit4,   ui_->btnDigit5, ui_->btnDigit6,   ui_->btnDigit7,
      ui_->btnDigit8,   ui_->btnDigit9, ui_->btnBracketR, ui_->btnDot,
      ui_->btnSqrt,     ui_->btnPow,    ui_->btnExp,      ui_->btnDiv,
      ui_->btnPlus,     ui_->btnMinus,  ui_->btnMul,      ui_->btnX,
      ui_->btnBracketL, btn_sin,        btn_cos,          btn_tan,
      btn_asin,         btn_acos,       btn_atan,         btn_log,
      btn_ln,           btn_mod};
  foreach (QPushButton *btn, lexeme_buttons) {
    connect(btn, SIGNAL(clicked()), this, SLOT(LexemeButtonClicked()));
    connect(btn, SIGNAL(clicked()), this, SLOT(DisplayExpression()));
    connect(btn, SIGNAL(clicked()), this, SLOT(DrawGraph()));
  }
  QList<QPushButton *> menus_buttons = {btn_log,  btn_ln,   btn_sin,
                                        btn_cos,  btn_tan,  btn_asin,
                                        btn_acos, btn_atan, btn_mod};
  foreach (QPushButton *btn, menus_buttons) {
    btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(btn, SIGNAL(clicked()), this, SLOT(CloseMenu()));
  }
  connect(trigonometry_menu, SIGNAL(aboutToShow()), this, SLOT(OpenMenu()));
  connect(functions_menu, SIGNAL(aboutToShow()), this, SLOT(OpenMenu()));

  QList<QPushButton *> control_buttons = {ui_->btnDelete, ui_->btnClear,
                                          ui_->btnEqual, ui_->btnGraph};
  connect(ui_->btnDelete, SIGNAL(clicked()), this, SLOT(ButtonDeleteClicked()));
  connect(ui_->btnClear, SIGNAL(clicked()), this, SLOT(ButtonClearClicked()));
  connect(ui_->btnEqual, SIGNAL(clicked()), this, SLOT(ButtonEqualClicked()));
  connect(ui_->btnEqual, SIGNAL(clicked()), this, SLOT(UpdateVariableMode()));
  connect(ui_->btnGraph, SIGNAL(clicked()), this, SLOT(ButtonGraphClicked()));
  foreach (QPushButton *btn, control_buttons) {
    connect(btn, SIGNAL(clicked()), this, SLOT(DrawGraph()));
    connect(btn, SIGNAL(clicked()), this, SLOT(DisplayExpression()));
  }

  // PLOT BEAUTY
  mouse_pressed_pos_ = QPoint(0, 0);
  mouse_moved_pos_ = QPoint(0, 0);

  // LOAN
  ui_->paymentTable->horizontalHeader()->setSectionResizeMode(
      0, QHeaderView::ResizeToContents);
  ui_->paymentTable->horizontalHeader()->setSectionResizeMode(
      1, QHeaderView::Stretch);
  ui_->paymentTable->horizontalHeader()->setSectionResizeMode(
      2, QHeaderView::Stretch);

  connect(ui_->btnCalcLoan, SIGNAL(clicked()), this,
          SLOT(ButtonCalculateLoanClicked()));
  connect(ui_->btnAmortizedLoan, SIGNAL(clicked()), this,
          SLOT(ButtonCalculateLoanClicked()));
  connect(ui_->btnDifferentialLoan, SIGNAL(clicked()), this,
          SLOT(ButtonCalculateLoanClicked()));
  connect(ui_->btnClearLoan, SIGNAL(clicked()), this,
          SLOT(ButtonClearLoanClicked()));

  // DEPOSIT
  ui_->contributionTable->horizontalHeader()->setSectionResizeMode(
      0, QHeaderView::Stretch);
  ui_->contributionTable->horizontalHeader()->setSectionResizeMode(
      1, QHeaderView::Stretch);
  connect(ui_->contributionTable, SIGNAL(cellChanged(int, int)), this,
          SLOT(UpdateContributionTable(int)));
  connect(ui_->btnCalcDeposit, SIGNAL(clicked()), this,
          SLOT(ButtonCalculateDepositClicked()));
  connect(ui_->btnClearDeposit, SIGNAL(clicked()), this,
          SLOT(ButtonClearDepositClicked()));
}
View::~View() { delete ui_; }

void View::closeEvent(QCloseEvent *event) {
  qApp->quit();
  event->accept();
}

// SMART

void View::DisplayExpression() const noexcept {
  QString expression_text;
  if (!variable_mode_) {
    expression_text = QString::fromStdString(controller_.GetStringExpression());
  } else {
    expression_text = QString::fromStdString(controller_.GetStringVariable());
  }
  ui_->expressionField->setHtml("<p align=\"right\">" + expression_text);
  ui_->expressionField->moveCursor(QTextCursor::EndOfLine);
}

void View::CloseMenu() const noexcept {
  static_cast<QMenu *>(sender()->parent())->hide();
}

void View::OpenMenu() const noexcept {
  QPushButton *menu_button = static_cast<QPushButton *>(sender()->parent());
  QMenu *sender_menu = static_cast<QMenu *>(sender());
  sender_menu->setFixedWidth(menu_button->width());
  sender_menu->setFixedHeight(ui_->btnSqrt->pos().y() + ui_->btnSqrt->height() -
                              menu_button->pos().y() - menu_button->height());
}

void View::LexemeButtonClicked() noexcept {
  std::string lexeme_name = static_cast<QPushButton *>(sender())
                                ->property("lexemeName")
                                .toString()
                                .toStdString();
  if (!variable_mode_) {
    controller_.AddLexemeToExpression(lexeme_name);
  } else {
    controller_.AddLexemeToVariable(lexeme_name);
  }
}

void View::ButtonDeleteClicked() noexcept {
  if (!variable_mode_) {
    controller_.DeleteLastExpressionLexeme();
  } else {
    controller_.DeleteLastVariableLexeme();
  }
}

void View::ButtonClearClicked() noexcept {
  if (!variable_mode_) {
    controller_.ClearExpressionLexemes();
  } else {
    controller_.ClearVariableLexemes();
  }
}

void View::ButtonEqualClicked() noexcept { controller_.Solve(); }

void View::UpdateVariableMode() noexcept {
  variable_mode_ = controller_.VariableEntered();
  QList<QPushButton *> not_number_related_buttons = {ui_->btnSqrt,
                                                     ui_->btnPow,
                                                     ui_->btnDiv,
                                                     ui_->btnMul,
                                                     ui_->btnX,
                                                     ui_->btnBracketL,
                                                     ui_->btnBracketR,
                                                     ui_->btnTrigonometryMenu,
                                                     ui_->btnFunctionsMenu,
                                                     ui_->btnGraph};
  foreach (QPushButton *btn, not_number_related_buttons) {
    btn->setEnabled(!variable_mode_);
  }
}

void View::ButtonGraphClicked() noexcept {
  graph_mode_ = !graph_mode_;
  ui_->btnEqual->setEnabled(!graph_mode_);
  if (!plot_) {
    plot_ = new QCustomPlot();
    plot_->setGeometry(this->pos().x() + this->width() + 43,
                       this->pos().y() + 32, 640, 640);
    plot_->installEventFilter(this);
  }
  if (graph_mode_) {
    plot_->show();
  } else {
    plot_->close();
    controller_.ResetPlot();
    delete plot_;
    plot_ = nullptr;
  }
}

void View::DrawGraph() noexcept {
  if (!graph_mode_) {
    return;
  }
  plot_->addGraph();
  plot_->setBackground(QColor(152, 152, 152));
  QPen pen(QColor(43, 43, 131));
  pen.setWidthF(4);
  plot_->graph(0)->setPen(pen);
  plot_->xAxis->setLabel("x");
  plot_->yAxis->setLabel("y");
  UpdateGraph();
}

void View::UpdateGraph() const noexcept {
  double x_min, x_max, y_min, y_max;
  std::vector<double> x;
  std::vector<double> y;
  controller_.GetPlotData(x_min, x_max, y_min, y_max, x, y);
  plot_->graph(0)->setData(QVector<double>(x.begin(), x.end()),
                           QVector<double>(y.begin(), y.end()));
  plot_->xAxis->setRange(x_min, x_max);
  plot_->yAxis->setRange(y_min, y_max);
  plot_->replot();
}

// LOAN

void View::ButtonClearLoanClicked() const noexcept {
  ui_->loanTermBox->setCurrentIndex(0);
  ui_->loanAmountEdit->clear();
  ui_->loanTermEdit->clear();
  ui_->interestRateEdit->clear();
  ui_->monthlyPaymentField->clear();
  ui_->totalPaymentField->clear();
  ui_->totalInterestField->clear();
  ui_->paymentTable->setRowCount(0);
}

void View::ButtonCalculateLoanClicked() const noexcept {
  LoanCalcInputOptions in;
  in.loan_type = ui_->btnAmortizedLoan->isChecked()
                     ? LoanCalcInputOptions::kAmortized
                     : LoanCalcInputOptions::kDifferential;
  in.loan_amount = ui_->loanAmountEdit->text();
  in.loan_term = ui_->loanTermEdit->text();
  in.loan_term_type = ui_->loanTermBox->currentText();
  in.interest_rate = ui_->interestRateEdit->text();

  LoanCalcOutputOptions out = controller_.CalculateLoan(in);
  ui_->monthlyPaymentField->setText(out.monthly_payment);
  ui_->totalPaymentField->setText(out.total_payment);
  ui_->totalInterestField->setText(out.total_interest);
  ui_->paymentTable->setRowCount(out.payment_data.size());
  for (int i = 0; i < out.payment_data.size(); ++i) {
    QTableWidgetItem *payment_number = new QTableWidgetItem(QString::number(i));
    payment_number->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *payment = new QTableWidgetItem(out.payment_data[i].first);
    payment->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *ending_balance =
        new QTableWidgetItem(out.payment_data[i].second);
    ending_balance->setTextAlignment(Qt::AlignCenter);

    ui_->paymentTable->setItem(i, 0, payment_number);
    ui_->paymentTable->setItem(i, 1, payment);
    ui_->paymentTable->setItem(i, 2, ending_balance);
  }
}

// DEPOSIT

void View::UpdateContributionTable(int i) const noexcept {
  ui_->contributionTable->setRowCount(
      qMax(ui_->contributionTable->rowCount(), i + 2));
  QDateEdit *date = new QDateEdit();

  date->setAlignment(Qt::AlignCenter);
  date->setCalendarPopup(true);
  date->setDisplayFormat("dd.MM.yyyy");
  date->setDate(ui_->investmentDateEdit->date());

  if (!ui_->contributionTable->cellWidget(i, 0)) {
    ui_->contributionTable->setCellWidget(i, 0, date);
  }
  if (!ui_->contributionTable->item(i, 1)) {
    QTableWidgetItem *amount = new QTableWidgetItem("0");
    amount->setTextAlignment(Qt::AlignCenter);
    ui_->contributionTable->setItem(i, 1, amount);
  } else {
    ui_->contributionTable->item(i, 1)->setTextAlignment(Qt::AlignCenter);
  }
  for (int empty_row_index = 0;
       empty_row_index < ui_->contributionTable->rowCount();
       ++empty_row_index) {
    if (ui_->contributionTable->item(empty_row_index, 1) &&
        ui_->contributionTable->item(empty_row_index, 1)->text() == "") {
      ui_->contributionTable->removeRow(empty_row_index);
    }
  }
}

void View::ButtonClearDepositClicked() const noexcept {
  ui_->initialInvestmentEdit->clear();
  ui_->investmentTermEdit->clear();
  ui_->depositInterestRateEdit->clear();
  ui_->taxRateEdit->clear();
  ui_->depositTotalInterestField->clear();
  ui_->totalTaxField->clear();
  ui_->endingBalanceField->clear();
  ui_->capitalizationBox->setChecked(false);
  ui_->contributionTable->setRowCount(0);
  ui_->contributionTable->setRowCount(1);
  ui_->investmentDateEdit->setDate(QDate(2023, 11, 16));
  ui_->investmentTermBox->setCurrentIndex(0);
  ui_->compoundingPeriodBox->setCurrentIndex(0);
}

void View::ButtonCalculateDepositClicked() const noexcept {
  DepositCalcInputOptions in;
  in.initial_investment = ui_->initialInvestmentEdit->text();
  in.investment_term = ui_->investmentTermEdit->text();
  in.investment_term_type = ui_->investmentTermBox->currentText();
  in.investment_date = ui_->investmentDateEdit->date();
  in.interest_rate = ui_->depositInterestRateEdit->text();
  in.tax_rate = ui_->taxRateEdit->text();
  in.compounding_period = ui_->compoundingPeriodBox->currentText();
  in.capitalization = ui_->capitalizationBox->isChecked()
                          ? DepositCalcInputOptions::kWithCapitalization
                          : DepositCalcInputOptions::kNoCapitalization;
  for (int i = 0; i < ui_->contributionTable->rowCount() - 1; ++i) {
    in.contributions.push_back(QPair<QDate, QString>(
        static_cast<QDateEdit *>(ui_->contributionTable->cellWidget(i, 0))
            ->date(),
        ui_->contributionTable->item(i, 1)->text()));
  }
  DepositCalcOutputOptions out = controller_.CalculateDeposit(in);
  ui_->depositTotalInterestField->setText(out.total_interest);
  ui_->totalTaxField->setText(out.total_tax);
  ui_->endingBalanceField->setText(out.ending_balance);
}

bool View::eventFilter(QObject *watched, QEvent *event) noexcept {
  if (event->type() == QEvent::MouseButtonPress) {
    QMouseEvent *mouse_event = static_cast<QMouseEvent *>(event);
    mouse_pressed_pos_ = mouse_event->pos();
  } else if (event->type() == QEvent::MouseMove) {
    if (!mouse_pressed_pos_.isNull()) {
      QMouseEvent *mouse_event = static_cast<QMouseEvent *>(event);
      mouse_moved_pos_ = mouse_event->pos();
      controller_.MovePlotXAxis(mouse_pressed_pos_.x() - mouse_moved_pos_.x());
      controller_.MovePlotYAxis(mouse_moved_pos_.y() - mouse_pressed_pos_.y());
      UpdateGraph();
      mouse_pressed_pos_ = mouse_moved_pos_;
    }
  } else if (event->type() == QEvent::MouseButtonRelease) {
    mouse_pressed_pos_ = QPoint(0, 0);
  } else if (event->type() == QEvent::Wheel) {
    QWheelEvent *wheel_event = static_cast<QWheelEvent *>(event);
    double scale = 0.8;
    if (wheel_event->angleDelta().y() < 0) {
      scale = 1.2;
    }
    controller_.ScalePlot(scale);
    UpdateGraph();
  }
  return QMainWindow::eventFilter(watched, event);
}

}  // namespace s21
