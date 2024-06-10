#include <gtest/gtest.h>
#include <math.h>

#include <iostream>
#include <vector>

#include "../lexeme.h"
#include "../mainmodel.h"

const double EPS = 1e-7;

TEST(MainModel, TestSolve1) {
  // -sin(8.2) + 2 * 4
  s21::MainModel model;
  model.AddLexemeToExpression("minus");
  model.AddLexemeToExpression("sin");
  model.AddLexemeToExpression("8");
  model.AddLexemeToExpression(".");
  model.AddLexemeToExpression("2");
  model.AddLexemeToExpression("bracketR");
  model.AddLexemeToExpression("plus");
  model.AddLexemeToExpression("2");
  model.AddLexemeToExpression("mul");
  model.AddLexemeToExpression("4");
  model.Solve();
  double res = std::stod(model.GetStringExpression());
  EXPECT_NEAR(res, -sin(8.2) + 2 * 4, EPS);
}

TEST(MainModel, TestSolve2) {
  // asin(0.3) + acos(0.3) - sin(cos(tan(120)))
  s21::MainModel model;
  model.AddLexemeToExpression("asin");
  model.AddLexemeToExpression("0");
  model.AddLexemeToExpression(".");
  model.AddLexemeToExpression("3");
  model.AddLexemeToExpression("bracketR");
  model.AddLexemeToExpression("plus");
  model.AddLexemeToExpression("acos");
  model.AddLexemeToExpression("0");
  model.AddLexemeToExpression(".");
  model.AddLexemeToExpression("3");
  model.AddLexemeToExpression("bracketR");
  model.AddLexemeToExpression("minus");
  model.AddLexemeToExpression("sin");
  model.AddLexemeToExpression("cos");
  model.AddLexemeToExpression("tan");
  model.AddLexemeToExpression("1");
  model.AddLexemeToExpression("2");
  model.AddLexemeToExpression("0");
  model.AddLexemeToExpression("bracketR");
  model.AddLexemeToExpression("bracketR");
  model.AddLexemeToExpression("bracketR");
  model.Solve();
  double res = std::stod(model.GetStringExpression());
  EXPECT_NEAR(res, asin(0.3) + acos(0.3) - sin(cos(tan(120))), EPS);
}

TEST(MainModel, TestSolve3) {
  // atan(0.3) / log(0.3) - ln(+sqrt(120^0.8 mod 2))
  s21::MainModel model;
  model.AddLexemeToExpression("atan");
  model.AddLexemeToExpression("0");
  model.AddLexemeToExpression(".");
  model.AddLexemeToExpression("3");
  model.AddLexemeToExpression("bracketR");
  model.AddLexemeToExpression("div");
  model.AddLexemeToExpression("log");
  model.AddLexemeToExpression("0");
  model.AddLexemeToExpression(".");
  model.AddLexemeToExpression("3");
  model.AddLexemeToExpression("bracketR");
  model.AddLexemeToExpression("minus");
  model.AddLexemeToExpression("ln");
  model.AddLexemeToExpression("plus");
  model.AddLexemeToExpression("sqrt");
  model.AddLexemeToExpression("1");
  model.AddLexemeToExpression("2");
  model.AddLexemeToExpression("0");
  model.AddLexemeToExpression("pow");
  model.AddLexemeToExpression("0");
  model.AddLexemeToExpression(".");
  model.AddLexemeToExpression("8");
  model.AddLexemeToExpression("mod");
  model.AddLexemeToExpression("2");
  model.AddLexemeToExpression("bracketR");
  model.AddLexemeToExpression("bracketR");
  model.Solve();
  double res = std::stod(model.GetStringExpression());
  EXPECT_NEAR(
      res, atan(0.3) / log10(0.3) - log(+sqrt(fmod(pow(120, 0.8), 2.0))), EPS);
}

TEST(MainModel, TestSolve4) {
  // atan(X) / log(X) - ln(+sqrt(120^0.8 mod 2))
  // X = +3.1E-2
  s21::MainModel model;
  model.AddLexemeToExpression("atan");
  model.AddLexemeToExpression("variable");
  model.AddLexemeToExpression("bracketR");
  model.AddLexemeToExpression("div");
  model.AddLexemeToExpression("log");
  model.AddLexemeToExpression("variable");
  model.AddLexemeToExpression("bracketR");
  model.AddLexemeToExpression("minus");
  model.AddLexemeToExpression("ln");
  model.AddLexemeToExpression("plus");
  model.AddLexemeToExpression("sqrt");
  model.AddLexemeToExpression("1");
  model.AddLexemeToExpression("2");
  model.AddLexemeToExpression("0");
  model.AddLexemeToExpression("pow");
  model.AddLexemeToExpression("0");
  model.AddLexemeToExpression(".");
  model.AddLexemeToExpression("8");
  model.AddLexemeToExpression("mod");
  model.AddLexemeToExpression("2");
  model.AddLexemeToExpression("bracketR");
  model.AddLexemeToExpression("bracketR");
  model.Solve();
  model.AddLexemeToVariable("plus");
  model.AddLexemeToVariable("3");
  model.AddLexemeToVariable(".");
  model.AddLexemeToVariable("1");
  model.AddLexemeToVariable("E");
  model.AddLexemeToVariable("minus");
  model.AddLexemeToVariable("2");
  model.Solve();
  double res = std::stod(model.GetStringExpression());
  EXPECT_NEAR(
      res,
      atan(+3.1E-2) / log10(+3.1E-2) - log(+sqrt(fmod(pow(120, 0.8), 2.0))),
      EPS);
}

TEST(MainModel, TestVariable) {
  // X = +3.1E-5
  s21::MainModel model;
  model.AddLexemeToVariable("minus");
  model.AddLexemeToVariable("plus");
  model.AddLexemeToVariable("3");
  model.AddLexemeToVariable(".");
  model.AddLexemeToVariable(".");
  model.AddLexemeToVariable("1");
  model.AddLexemeToVariable("E");
  model.AddLexemeToVariable("E");
  model.AddLexemeToVariable("plus");
  model.AddLexemeToVariable("minus");
  model.AddLexemeToVariable("5");
  EXPECT_EQ(model.GetStringVariable(), "+3.1E-5");
}

TEST(MainModel, TestDeleteLastExpressionLexeme) {
  // -sin(8.2) + 2 * 4
  s21::MainModel model;
  model.AddLexemeToExpression("minus");
  model.AddLexemeToExpression("sin");
  model.AddLexemeToExpression("8");
  model.AddLexemeToExpression(".");
  model.AddLexemeToExpression("2");
  model.AddLexemeToExpression("bracketR");
  model.AddLexemeToExpression("plus");
  model.AddLexemeToExpression("2");
  model.AddLexemeToExpression("div");
  model.DeleteLastExpressionLexeme();
  model.AddLexemeToExpression("mul");
  model.AddLexemeToExpression("4");
  model.Solve();
  double res = std::stod(model.GetStringExpression());
  EXPECT_NEAR(res, -sin(8.2) + 2 * 4, EPS);
}

TEST(MainModel, TestDeleteLastVariableLexeme) {
  // atan(X)
  // X = +3.1E-2
  s21::MainModel model;
  model.AddLexemeToExpression("atan");
  model.AddLexemeToExpression("variable");
  model.AddLexemeToExpression("bracketR");
  model.Solve();
  model.AddLexemeToVariable("minus");
  model.DeleteLastVariableLexeme();
  model.AddLexemeToVariable("plus");
  model.AddLexemeToVariable("3");
  model.AddLexemeToVariable(".");
  model.AddLexemeToVariable("1");
  model.AddLexemeToVariable("E");
  model.AddLexemeToVariable("minus");
  model.AddLexemeToVariable("2");
  model.Solve();
  double res = std::stod(model.GetStringExpression());
  EXPECT_NEAR(res, atan(+3.1E-2), EPS);
}

TEST(MainModel, TestClearExpressionLexemes) {
  // 2 * 4
  s21::MainModel model;
  model.AddLexemeToExpression("2");
  model.AddLexemeToExpression("div");
  model.ClearExpressionLexemes();
  model.AddLexemeToExpression("2");
  model.AddLexemeToExpression("mul");
  model.AddLexemeToExpression("4");
  model.Solve();
  double res = std::stod(model.GetStringExpression());
  EXPECT_NEAR(res, 2 * 4, EPS);
}

TEST(MainModel, TestClearVariableLexemes) {
  // atan(X)
  // X = +3.1E-2
  s21::MainModel model;
  model.AddLexemeToExpression("atan");
  model.AddLexemeToExpression("variable");
  model.AddLexemeToExpression("bracketR");
  model.Solve();
  model.AddLexemeToVariable("minus");
  model.AddLexemeToVariable("2");
  model.ClearVariableLexemes();
  model.AddLexemeToVariable("plus");
  model.AddLexemeToVariable("3");
  model.AddLexemeToVariable(".");
  model.AddLexemeToVariable("1");
  model.AddLexemeToVariable("E");
  model.AddLexemeToVariable("minus");
  model.AddLexemeToVariable("2");
  model.Solve();
  double res = std::stod(model.GetStringExpression());
  EXPECT_NEAR(res, atan(+3.1E-2), EPS);
}

TEST(MainModel, TestGetPlotData) {
  // -sin(x) + 2 * 4
  s21::MainModel model;
  double x_min;
  double x_max;
  double y_min;
  double y_max;
  std::vector<double> x;
  std::vector<double> y;

  model.AddLexemeToExpression("minus");
  model.AddLexemeToExpression("sin");
  model.AddLexemeToExpression("variable");
  model.AddLexemeToExpression("bracketR");
  model.AddLexemeToExpression("plus");
  model.AddLexemeToExpression("2");
  model.AddLexemeToExpression("mul");
  model.AddLexemeToExpression("4");

  model.GetPlotData(x_min, x_max, y_min, y_max, x, y);

  EXPECT_EQ(x.size(), y.size());
  for (size_t i = 0; i < x.size(); ++i) {
    s21::MainModel m;
    m.AddLexemeToExpression("minus");
    m.AddLexemeToExpression("sin");
    m.AddLexemeToExpression(std::to_string(x[i]));
    m.AddLexemeToExpression("bracketR");
    m.AddLexemeToExpression("plus");
    m.AddLexemeToExpression("2");
    m.AddLexemeToExpression("mul");
    m.AddLexemeToExpression("4");
    m.Solve();
    EXPECT_NEAR(y[i], std::stod(m.GetStringExpression()), EPS);
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
