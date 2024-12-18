#include "fill.h"
#include <stack>

void traverse_line(QImage *image, FillColors colors, std::stack<QPoint> *stack,
                   int left, int right, int y) {
  if (y < 0 || y >= image->height()) {
    return;
  }

  bool flag = false;
  int x = left;
  int temp;
  while (x <= right) {
    flag = false;
    while (x <= right && image->pixelColor(x, y) != colors.edge &&
           image->pixelColor(x, y) != colors.foreground) {
      if (!flag) {
        flag = true;
      }
      ++x;
    }

    if (flag) {
      if (x == right && image->pixelColor(x, y) != colors.edge &&
          image->pixelColor(x, y) != colors.foreground) {
        stack->push(QPoint(x, y));
      } else {
        stack->push(QPoint(x - 1, y));
      }
      flag = false;
    }

    temp = x;
    while (x < right && (image->pixelColor(x, y) == colors.edge ||
                         image->pixelColor(x, y) == colors.foreground)) {
      ++x;
    }
    if (x == temp) {
      ++x;
    }
  }
}

long fill(CoordinateScene *scene, FillColors colors,
            QPoint seed, int delay) {
  QImage *image = scene->getImage();
  std::stack<QPoint> stack;

  QElapsedTimer timer;
  timer.start();
  stack.push(seed);

  while (!stack.empty()) {
    QPoint current = stack.top();
    stack.pop();

    image->setPixelColor(current, colors.foreground);

    // Заполнение слева от заданной точки
    int x = current.x() - 1, y = current.y();
    while (x >= 0 && image->pixelColor(x, y) != colors.edge) {
      image->setPixelColor(x, y, colors.foreground);
      --x;
    }
    int left = x + 1;

    // Заполнение справа от заданной точки
    x = current.x() + 1;
    while (x < image->width() && image->pixelColor(x, y) != colors.edge) {
      image->setPixelColor(x, y, colors.foreground);
      ++x;
    }
    int right = x - 1;

    traverse_line(image, colors, &stack, left, right, y - 1);
    traverse_line(image, colors, &stack, left, right, y + 1);

    if (delay > 0) {
      scene->drawImage(image);
      QTime delay_time = QTime::currentTime().addMSecs(delay);
      while (QTime::currentTime() < delay_time) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, delay);
      }
    }
  }
  if (delay == 0) {
    scene->drawImage(image);
  }

  return timer.elapsed();
}
