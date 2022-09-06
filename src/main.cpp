#include "main.hpp"
#include "Camera.hpp"
#include "Material.hpp"
#include "PixelBuffer.hpp"
#include "Ray.hpp"
#include "RayCaster.hpp"
#include "Vector3D.hpp"
#include "cairo.h"
#include <chrono>
#include <ratio>
#include <vector>

#include <gtk/gtk.h>

#include "GeometryRegister.hpp"
#include "Matrix3D.hpp"
#include "Sphere.hpp"

std::shared_ptr<Sphere<double>> centerSphere = std::make_shared<Sphere<double>>(
    Vector3D<double>(0.0, 0.0, 30.0),
    Material<double>(Vector3D<double>(255.0, 0.0, 0.0), 0.9), 20.0);
std::shared_ptr<Sphere<double>> orbitingSphere =
    std::make_shared<Sphere<double>>(
        Vector3D<double>(-30.0, 0.0, 0.0),
        Material<double>(Vector3D<double>(0.0, 255.0, 0.0), 0.9), 27.0);

std::shared_ptr<GeometryRegister<double>> geometryRegister;
PixelBuffer pixelBuffer(500, 500);
Camera<double> camera(Vector3D<double>(0.0, 0.0, -20.0),
                      Vector3D<double>(0.0, 0.0, 0.0), 500, 500);

static void setupRenderer() {
  geometryRegister = std::make_shared<GeometryRegister<double>>();

  pixelBuffer.Fill(255, 0, 0, 255);
  geometryRegister->Register(centerSphere).Register(orbitingSphere);
  geometryRegister->Print();
}

static void drawRenderer() {
  const size_t rayCasterCount = 1;

  std::vector<std::shared_ptr<RayCaster<double>>> rayCasters;
  rayCasters.reserve(6);

  // Creates all the ray casters (threads that will render segments of the
  // view).
  size_t raysPerCaster = camera.RayCount() / rayCasterCount;
  size_t raysRemaining = camera.RayCount() % rayCasterCount;
  for (size_t i = 0; i < rayCasterCount; ++i) {
    const size_t raysFrom = i * raysPerCaster;
    const size_t raysTo =
        raysFrom + raysPerCaster + (i + 1 < raysRemaining ? 0 : raysRemaining);
    rayCasters.push_back(std::make_shared<RayCaster<double>>(
        pixelBuffer, camera, geometryRegister, i * raysFrom, raysTo));
  }

  // Creates and starts all the ray casters.
  for (size_t i = 0; i < rayCasterCount; ++i) {
    rayCasters.at(i)->CreateThread();
  }

  // Joins all the ray casters, to await the rendering to finish.
  for (size_t i = 0; i < rayCasterCount; ++i) {
    rayCasters.at(i)->JoinThread();
  }
}

char timing[128];

static void activate(GtkApplication *app, gpointer user_data) {
  setupRenderer();

  const uint64_t startTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  drawRenderer();
  const uint64_t endTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  const uint64_t deltaTime = endTime - startTime;

  GtkWidget *window = nullptr;
  GtkWidget *box = nullptr;
  GtkWidget *button = nullptr;
  GtkWidget *image = nullptr;

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Luke's Ray-Tracing");
  gtk_window_set_default_size(GTK_WINDOW(window), 900, 500);

  box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_window_set_child(GTK_WINDOW(window), box);

  image = gtk_image_new_from_pixbuf(pixelBuffer.pixelBuffer);
  gtk_widget_set_hexpand(image, TRUE);
  gtk_widget_set_vexpand(image, TRUE);
  gtk_box_append(GTK_BOX(box), image);

  button = gtk_button_new_with_label("Press me");
  gtk_box_append(GTK_BOX(box), button);

  GtkWidget *frameTime = nullptr;
  frameTime = gtk_label_new("Rendering ...");
  gtk_box_append(GTK_BOX(box), frameTime);

  sprintf(timing, "%lu ms", deltaTime);
  gtk_label_set_text(GTK_LABEL(frameTime), timing);

  gtk_widget_show(window);
}

int main(int argc, char *argv[]) {
  GtkApplication *app = nullptr;
  int status;

  app = gtk_application_new("nl.fannst.raytrace", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), nullptr);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}
