#include "camera.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

int main()
{
    FHittableList World;

    auto MaterialGround = std::make_shared<FLambertian>(FColor3{0.8f, 0.8f, 0.8f});
    auto MaterialCenter = std::make_shared<FLambertian>(FColor3{0.1f, 0.2f, 0.5f});
    auto MaterialLeft = std::make_shared<FMetal>(FColor3{0.8f, 0.8f, 0.8f});
    auto MaterialRight = std::make_shared<FMetal>(FColor3{0.8f, 0.6f, 0.2f});

    World.Add(std::make_shared<FSphere>(FVector3{0, -100.5, -1}, 100.f, MaterialGround));
    World.Add(std::make_shared<FSphere>(FVector3{0, 0, -1.2}, 0.5f, MaterialCenter));
    World.Add(std::make_shared<FSphere>(FVector3{-1.f, 0, -1}, 0.5f, MaterialLeft));
    World.Add(std::make_shared<FSphere>(FVector3{1.f, 0, -1}, 0.5f, MaterialRight));

    FCamera Camera;
    Camera.AspectRatio = 16.f / 9.f;
    Camera.ImageWidth = 1920;
    Camera.IterationsPerPixel = 10;
    Camera.MaxDepth = 10;

    Camera.Render(World);

    Camera.SaveAsBMP("Result.bmp");
    Camera.SaveAsEXR("Result.exr");

    return 0;
}