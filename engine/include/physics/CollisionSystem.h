#include <vector>

#include "Rect2.h"
#include "Vec2.h"

namespace engine {
namespace physics {

class CollisionDetector
{
public:
    class Result
    {
    public:
        explicit Result(bool hasIntersection, const Vec2& intersection = Vec2(0,0)) : hasIntersection(hasIntersection), intersection(intersection) {}

        bool hasIntersection;
        Vec2 intersection;
    };

    Result rayIntersection(const Vec2& rayOrigin, const Vec2& rayDirection, const Rect2& object) 
    {
        Result hitResult(false);
        Vec2 closestHit;

        double tMin = -1;

        hitResult = rayIntersection(rayOrigin, rayDirection, object.topLeft(), object.topRight());
        if (hitResult.hasIntersection) {
            double t = (hitResult.intersection.x - rayOrigin.x) / rayDirection.x;
            if (t < tMin || tMin == -1) {
                tMin = t;
                closestHit = hitResult.intersection;
            }
        }

        hitResult = rayIntersection(rayOrigin, rayDirection, object.topLeft(), object.bottomLeft());
        if (hitResult.hasIntersection) {
            double t = (hitResult.intersection.x - rayOrigin.x) / rayDirection.x;
            if (t < tMin || tMin == -1) {
                tMin = t;
                closestHit = hitResult.intersection;
            }
        }

        hitResult = rayIntersection(rayOrigin, rayDirection, object.topRight(), object.bottomRight());
        if (hitResult.hasIntersection) {
            double t = (hitResult.intersection.x - rayOrigin.x) / rayDirection.x;
            if (t < tMin || tMin == -1) {
                tMin = t;
                closestHit = hitResult.intersection;
            }
        }

        hitResult = rayIntersection(rayOrigin, rayDirection, object.bottomLeft(), object.bottomRight());
        if (hitResult.hasIntersection) {
            double t = (hitResult.intersection.x - rayOrigin.x) / rayDirection.x;
            if (t < tMin || tMin == -1) {
                tMin = t;
                closestHit = hitResult.intersection;
            }
        }

        if (tMin == -1) {
            return Result(false);
        } else {
            return Result(true, closestHit);
        }
    }

    Result rayIntersection(const Vec2& rayOrigin, const Vec2& rayDirection, const Vec2& a, const Vec2& b)
    {
        Vec2 v1 = rayDirection.normalized();
        Vec2 v2 = (b - a).normalized();
        Vec2 p1 = rayOrigin;
        Vec2 p2 = a;
        
        double v1v2 = v1.cross(v2);

        if (v1v2 == 0)
        {
            return Result(false);
        }

        double t = v1.cross(v2) * (p2 - p1).cross(v1) / (v1v2 * v1v2);

        // either before a or after b
        if (t < 0 || t > (b - a).magnitude())
        {
            return Result(false);
        }

        Vec2 resultVec = p2 + t * v2;

        if ((resultVec - rayOrigin).x / rayDirection.x < 0){
            return Result(false);
        }

        return Result(true, p2 + t * v2);
    }

    std::vector<Vec2> rectIntersection(const Rect2& a, const Rect2& b) 
    {
        return std::vector<Vec2>();
    }
};

}
}
