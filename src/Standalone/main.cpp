#include "common_defines.h"
#include "camera.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

int main()
{
    FHittableList World;

    auto R = cos(M_PI * 0.25f);

    auto MaterialLeft = std::make_shared<FLambertian>(FColor3{0, 0, 1});
    auto MaterialRight = std::make_shared<FLambertian>(FColor3{1, 0, 0});

    World.Add(std::make_shared<FSphere>(FPoint3(-R, 0, -1), R, MaterialLeft));
    World.Add(std::make_shared<FSphere>(FPoint3(R, 0, -1), R, MaterialRight));

    FCamera Camera;
    Camera.AspectRatio = 16.f / 9.f;
    Camera.ImageWidth = 1920;
    Camera.IterationsPerPixel = 10;
    Camera.MaxDepth = 10;
    Camera.VFOV = 90.f;

    Camera.Render(World);

    Camera.SaveAsBMP("Result.bmp");
    Camera.SaveAsEXR("Result.exr");

    return 0;
}