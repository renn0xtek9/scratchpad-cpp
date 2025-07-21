#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTimer>
#include <Qt3DCore/QEntity>
#include <Qt3DExtras/Qt3DWindow>
#include <QtWidgets>

#include "boid.h"

class MainWindow : public QWidget {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);

 private slots:
  void updateBoids();

 private:
  Qt3DExtras::Qt3DWindow* view;
  Qt3DCore::QEntity* rootEntity;
  QList<Boid*> boids;
  QTimer* timer;

  float limit = 150.0f;
};

#endif  // MAINWINDOW_H
