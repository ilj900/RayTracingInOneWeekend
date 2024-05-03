#include "common_defines.h"
#include "camera.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

int main()
{
    FHittableList World;

    auto GroundMaterial = std::make_shared<FLambertian>(FColor3(0.5, 0.5, 0.5));
    World.Add(std::make_shared<FSphere>(FPoint3(0, -1000.f, 0), 1000.f, GroundMaterial));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; ++b)
        {
            auto ChooseMaterial = RandomFloat();
            FPoint3 Center = FPoint3 (a + 0.9 * RandomFloat(), 0.2, b + 0.9 * RandomFloat());

            if ((Center - FPoint3(4, 0.2, 0)).Length() > 0.9)
            {
                std::shared_ptr<FMaterial> SphereMaterial;

                static RNG3D RNG3;
                if (ChooseMaterial < 0.8)
                {
                    auto [R, G, B] = RNG3();
                    SphereMaterial = std::make_shared<FLambertian>(FColor3(R, G, B));
                }
                else if (ChooseMaterial < 0.95)
                {
                    auto [R, G, B] = RNG3();
                    auto Fuzz = RandomFloat(0, 0.5);
                    SphereMaterial = std::make_shared<FMetal>(FColor3(R, G, B), Fuzz);
                }
                else
                {
                    SphereMaterial = std::make_shared<FDielectric>(1.5f);
                }

                World.Add((std::make_shared<FSphere>(Center, 0.2f, SphereMaterial)));
            }
        }
    }

    auto Material1 = std::make_shared<FDielectric>(1.5f);
    World.Add((std::make_shared<FSphere>(FPoint3(0, 1, 0) , 1.f, Material1)));

    auto Material2 = std::make_shared<FLambertian>(FColor3(0.4, 0.2, 0.1));
    World.Add((std::make_shared<FSphere>(FPoint3(-4, 1, 0) , 1.f, Material2)));

    auto Material3 = std::make_shared<FMetal>(FColor3(0.7, 0.6, 0.5), 0.f);
    World.Add((std::make_shared<FSphere>(FPoint3(4, 1, 0) , 1.f, Material3)));

    FCamera Camera;
    Camera.AspectRatio = 16.f / 9.f;
    Camera.ImageWidth = 1920;
    Camera.IterationsPerPixel = 10;
    Camera.MaxDepth = 10;
    Camera.VFOV = 20.f;
    Camera.LookFrom = {13.f, 2.f, 3.f};
    Camera.LookAt = {0.f, 0.f, 0.f};
    Camera.Up = {0.f, 1.f, 0.f};
    Camera.DefocusAngle = 0.6f;
    Camera.FocusDistance = 10.f;

    Camera.Render(World);

    Camera.SaveAsBMP("Result.bmp");
    Camera.SaveAsEXR("Result.exr");

    return 0;
}