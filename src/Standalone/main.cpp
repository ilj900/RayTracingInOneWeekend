#include "common_defines.h"
#include "camera.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

int main()
{
    FHittableList World;

    auto R = cos(M_PI * 0.25f);

    auto MaterialGround = std::make_shared<FLambertian>(FColor3(0.8f, 0.8f, 0.f));
    auto MaterialCenter = std::make_shared<FLambertian>(FColor3(0.1f, 0.2f, 0.5f));
    auto MaterialLeft = std::make_shared<FDielectric>(1.5f);
    auto MaterialBubble = std::make_shared<FDielectric>(1.f / 1.5f);
    auto MaterialRight = std::make_shared<FMetal>(FColor3(0.8f, 0.6f, 0.2f), 1.f);

    World.Add(std::make_shared<FSphere>(FPoint3(0.f, -100.5f, -1), 100.f, MaterialGround));
    World.Add(std::make_shared<FSphere>(FPoint3(0.f, 0.f, -1.2f), 0.5f, MaterialCenter));
    World.Add(std::make_shared<FSphere>(FPoint3(-1.f, 0.f, -1.f), 0.5f, MaterialLeft));
    World.Add(std::make_shared<FSphere>(FPoint3(-1.f, 0.f, -1.f), 0.4f, MaterialBubble));
    World.Add(std::make_shared<FSphere>(FPoint3(1.f, 0.f, -1.f), 0.5f, MaterialRight));

    FCamera Camera;
    Camera.AspectRatio = 16.f / 9.f;
    Camera.ImageWidth = 1920;
    Camera.IterationsPerPixel = 10;
    Camera.MaxDepth = 10;
    Camera.VFOV = 90.f;
    Camera.LookFrom = {-2.f, 2.f, 1.f};
    Camera.LookAt = {0.f, 0.f, -1.f};
    Camera.Up = {0.f, 1.f, 0.f};

    Camera.Render(World);

    Camera.SaveAsBMP("Result.bmp");
    Camera.SaveAsEXR("Result.exr");

    return 0;
}