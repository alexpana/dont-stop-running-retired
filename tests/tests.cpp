#include "test_physics.h"

int main(int argc, char** argv)
{
    TestPysics tests;
    tests.add_test("physics.gravity_simple", &TestPysics::testGravitySimple);
    tests.add_test("physics.velocity", &TestPysics::testVelocity);
    tests.add_test("physics.collision.ray_line", &TestPysics::testRayLineCollision);

    return tests.run(argc, argv);
}