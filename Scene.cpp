#include "Scene.h"

Scene::
Scene(json& _j){
  // camera object
  json c = _j.at("Camera"); 
  m_camera = std::make_shared<Camera>(c);

  // Objects
  for (auto& plane : _j.at("Plane")) {
    m_objects.emplace_back(new Plane(plane, plane.at("name")));
  }

  // windows
  for (auto& plane : _j.at("Window")) {
    m_windows.emplace_back(new Plane(plane, plane.at("name")));
  }

  for (auto& sphere : _j.at("Sphere")) {
    m_objects.emplace_back(new Sphere(sphere, sphere.at("name")));
  }

  // lights
  for (auto& light : _j.at("Lights")) {
    m_lights.emplace_back(new Light(light));
  }

  // mesh
  for (auto& mesh : _j.at("Mesh")) {
    Mesh m = m_objParser.parse_obj_file(mesh.at("ObjFilename"));
    m.read(mesh);
    m_mesh.emplace_back(make_shared<Mesh>(m));
  }

  // lamp
  for (auto& mesh : _j.at("Lamp")) {
    Mesh m = m_objParser.parse_obj_file(mesh.at("ObjFilename"));
    m.read(mesh);
    m_lamps.emplace_back(make_shared<Mesh>(m));
  }


}

