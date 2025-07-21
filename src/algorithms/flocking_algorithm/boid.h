#ifndef BOID_H
#define BOID_H

#include <QVector3D>
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QSphereMesh>

class Boid {
 public:
  Boid(Qt3DCore::QEntity* root);

  void update(const QList<Boid*>& boids);
  void applyEdges(float limit);

  Qt3DCore::QEntity* getEntity() const;

 private:
  QVector3D position;
  QVector3D velocity;
  QVector3D acceleration;

  Qt3DCore::QTransform* transform;
  Qt3DCore::QEntity* entity;

  float maxSpeed = 1.5f;
  float maxForce = 0.05f;
  float perception = 75.0f;

  QVector3D align(const QList<Boid*>& boids);
  QVector3D cohesion(const QList<Boid*>& boids);
  QVector3D separation(const QList<Boid*>& boids);
};

#endif  // BOID_H
