#ifndef _BODY_H_
#define _BODY_H_

#include "Ray.h"
#include "Material.h"
#include "Collision.h"

#include <string>
#include <memory>

// JSON
#include <nlohmann/json.hpp>
using json = nlohmann::json;
using Color4 = glm::vec4;


class Body {
  public:
    Body(std::string _type) : m_shapeName(_type) {}
    
    // @return -1 if no intersects
    virtual Collision intersects(Ray& _r) = 0;
    virtual void initialize(GLuint& _program) = 0;
    virtual void draw() = 0;
    Material getMaterial() {return m_material;}
    std::string getShape() {return m_shapeName;}

    Material m_material;

  private:
    std::string m_shapeName;
};

using BodyPtr = std::shared_ptr<Body>;

#endif