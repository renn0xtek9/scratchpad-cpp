#include "boid.h"

#include <QRandomGenerator>
#include <Qt3DExtras/QPhongMaterial>
#include <QtMath>

Boid::Boid(Qt3DCore::QEntity* root) {
  position =
      QVector3D(QRandomGenerator::global()->generate() % 200 - 100, QRandomGenerator::global()->generate() % 200 - 100,
                QRandomGenerator::global()->generate() % 200 - 100);

  velocity =
      QVector3D((QRandomGenerator::global()->generate() % 5 - 2), (QRandomGenerator::global()->generate() % 5 - 2),
                (QRandomGenerator::global()->generate() % 5 - 2));
  acceleration = QVector3D(0, 0, 0);

  auto* mesh = new Qt3DExtras::QSphereMesh();
  mesh->setRadius(2.0f);

  transform = new Qt3DCore::QTransform();
  transform->setTranslation(position);

  auto* material = new Qt3DExtras::QPhongMaterial();
  material->setDiffuse(QColor(Qt::blue));

  entity = new Qt3DCore::QEntity(root);
  entity->addComponent(mesh);
  entity->addComponent(transform);
  entity->addComponent(material);
}

void Boid::update(const QList<Boid*>& boids) {
  acceleration += align(boids);
  acceleration += cohesion(boids);
  acceleration += separation(boids);

  velocity += acceleration;
  if (velocity.length() > maxSpeed) velocity = velocity.normalized() * maxSpeed;

  position += velocity;
  acceleration = QVector3D(0, 0, 0);

  transform->setTranslation(position);
}

void Boid::applyEdges(float limit) {
  if (position.x() > limit) position.setX(-limit);
  if (position.x() < -limit) position.setX(limit);
  if (position.y() > limit) position.setY(-limit);
  if (position.y() < -limit) position.setY(limit);
  if (position.z() > limit) position.setZ(-limit);
  if (position.z() < -limit) position.setZ(limit);
}

Qt3DCore::QEntity* Boid::getEntity() const {
  return entity;
}

// ALIGNMENT Rule
QVector3D Boid::align(const QList<Boid*>& boids) {
  QVector3D steering;
  int total = 0;
  for (auto* other : boids) {
    if (other == this) continue;
    if ((other->position - position).length() < perception) {
      steering += other->velocity;
      total++;
    }
  }
  if (total > 0) {
    steering /= total;
    steering = steering.normalized() * maxSpeed;
    steering -= velocity;
    if (steering.length() > maxForce) steering = steering.normalized() * maxForce;
  }
  return steering;
}

// COHESION Rule
QVector3D Boid::cohesion(const QList<Boid*>& boids) {
  QVector3D center;
  int total = 0;
  for (auto* other : boids) {
    if (other == this) continue;
    if ((other->position - position).length() < perception) {
      center += other->position;
      total++;
    }
  }
  if (total > 0) {
    center /= total;
    QVector3D desired = center - position;
    desired = desired.normalized() * maxSpeed;
    QVector3D steering = desired - velocity;
    if (steering.length() > maxForce) steering = steering.normalized() * maxForce;
    return steering;
  }
  return QVector3D(0, 0, 0);
}

// SEPARATION Rule
QVector3D Boid::separation(const QList<Boid*>& boids) {
  QVector3D steering;
  int total = 0;
  for (auto* other : boids) {
    float d = (other->position - position).length();
    if (other != this && d < perception / 2.0f) {
      QVector3D diff = position - other->position;
      if (d != 0) diff /= d;
      steering += diff;
      ++total;
    }
  }
  if (total > 0) {
    steering /= total;
    steering = steering.normalized() * maxSpeed;
    steering -= velocity;
    if (steering.length() > maxForce) steering = steering.normalized() * maxForce;
  }
  return steering;
}
