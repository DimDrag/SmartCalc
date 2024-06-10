#include <QApplication>
#include <QDesktopWidget>

#include "view.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  s21::View w;

  QDesktopWidget *desktop = QApplication::desktop();
  int screen_width = desktop->width();
  int screen_height = desktop->height();
  // Получаем размер окна
  int window_width = w.width();
  int window_height = w.height();
  // Вычисляем координаты для центрирования окна
  int x = (screen_width - window_width) / 3;
  int y = (screen_height - window_height) / 3;
  w.move(x, y);

  w.show();
  return a.exec();
}
