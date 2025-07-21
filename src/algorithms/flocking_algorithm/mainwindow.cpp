#include "mainwindow.h"

#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DRender/QCamera>

MainWindow::MainWindow(QWidget* parent) : QWidget(parent) {
  view = new Qt3DExtras::Qt3DWindow();
  view->defaultFrameGraph()->setClearColor(Qt::red);
  QWidget* container = QWidget::createWindowContainer(view);
  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->addWidget(container);

  rootEntity = new Qt3DCore::QEntity();

  // Camera
  auto* camera = view->camera();
  camera->lens()->setPerspectiveProjection(60.0f, 1.6f, 0.1f, 1000.0f);
  camera->setPosition(QVector3D(0, 0, 400));
  camera->setViewCenter(QVector3D(0, 0, 0));

  // Orbit control
  auto* camController = new Qt3DExtras::QOrbitCameraController(rootEntity);
  camController->setLinearSpeed(50.0f);
  camController->setLookSpeed(180.0f);
  camController->setCamera(camera);

  view->setRootEntity(rootEntity);

  // Create boids
  for (int i = 0; i < 50; ++i) {
    Boid* b = new Boid(rootEntity);
    boids.append(b);
  }

  // Simulation loop
  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &MainWindow::updateBoids);
  timer->start(16);  // ~60FPS
}

void MainWindow::updateBoids() {
  for (Boid* b : boids) {
    b->update(boids);
    b->applyEdges(limit);
  }
}
