#ifndef _SCENE_H_
#define _SCENE_H_

#include "ObjParser.h"
#include "Models/Camera.h"
#include "Models/Light.h"
#include "Models/Mesh.h"
#include "Models/Plane.h"
#include "Models/Sphere.h"

// STL
#include <vector>
#include <iostream>
using namespace std;

class Scene {
  public:
    Scene(json& _j);

    const std::shared_ptr<Camera> getCamera() {return m_camera;}
    const vector<BodyPtr>& getObjects() {return m_objects;}
    const vector<LightPtr>& getLights() {return m_lights;}
    const vector<MeshPtr>& getMesh() {return m_mesh;}
    const vector<BodyPtr>& getWindows() {return m_windows;}
    const vector<MeshPtr>& getLamps() {return m_lamps;}


  private:
    shared_ptr<Camera> m_camera;
    vector<BodyPtr> m_objects;   ///< objects 
    vector<BodyPtr> m_windows;   ///< transparent thing
    vector<LightPtr> m_lights;   ///< lights
    vector<MeshPtr> m_mesh;      ///< mesh
    vector<MeshPtr> m_lamps;
    ObjParser m_objParser;
};

#endif