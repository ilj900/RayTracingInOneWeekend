#include "common_defines.h"
#include "camera.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

int main()
{
    FHittableList World;

    auto GroundMaterial = std::make_shared<FLambertian>(FColor3(0.5, 0.5, 0.5));
    World.Add(std::make_shared<FSphere>(FPoint3(0, -1000, 0), 1000, GroundMaterial));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; ++b)
        {
            auto ChooseMaterial = RandomDouble();
            FPoint3 Center = FPoint3 (a + 0.9 * RandomDouble(), 0.2, b + 0.9 * RandomDouble());

            if ((Center - FPoint3(4, 0.2, 0)).Length() > 0.9)
            {
                std::shared_ptr<FMaterial> SphereMaterial;

                static RNG3D RNG3;
                if (ChooseMaterial < 0.8)
                {
                    auto [R, G, B] = RNG3();
                    SphereMaterial = std::make_shared<FLambertian>(FColor3(R, G, B));
                    auto Center2 = Center + FVector3(0, RandomDouble(0, 0.5), 0);
                    World.Add((std::make_shared<FSphere>(Center, Center2, 0.2, SphereMaterial)));
                }
                else if (ChooseMaterial < 0.95)
                {
                    auto [R, G, B] = RNG3();
                    auto Fuzz = RandomDouble(0, 0.5);
                    SphereMaterial = std::make_shared<FMetal>(FColor3(R, G, B), Fuzz);
                    World.Add((std::make_shared<FSphere>(Center, 0.2, SphereMaterial)));
                }
                else
                {
                    SphereMaterial = std::make_shared<FDielectric>(1.5);
                    World.Add((std::make_shared<FSphere>(Center, 0.2, SphereMaterial)));
                }
            }
        }
    }

    auto Material1 = std::make_shared<FDielectric>(1.5);
    World.Add((std::make_shared<FSphere>(FPoint3(0, 1, 0) , 1, Material1)));

    auto Material2 = std::make_shared<FLambertian>(FColor3(0.4, 0.2, 0.1));
    World.Add((std::make_shared<FSphere>(FPoint3(-4, 1, 0) , 1, Material2)));

    auto Material3 = std::make_shared<FMetal>(FColor3(0.7, 0.6, 0.5), 0);
    World.Add((std::make_shared<FSphere>(FPoint3(4, 1, 0) , 1, Material3)));

    FCamera Camera;
    Camera.AspectRatio = 16.0 / 9.0;
    Camera.ImageWidth = 1920;
    Camera.IterationsPerPixel = 30;
    Camera.MaxDepth = 10;
    Camera.VFOV = 20;
    Camera.LookFrom = {13, 2, 3};
    Camera.LookAt = {0, 0, 0};
    Camera.Up = {0, 1, 0};
    Camera.DefocusAngle = 0.6;
    Camera.FocusDistance = 10;

    Camera.Render(World);

    Camera.SaveAsBMP("Result.bmp");
    Camera.SaveAsEXR("Result.exr");

    return 0;
}
