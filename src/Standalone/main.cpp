#include "bvh_node.h"
#include "camera.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "texture.h"

void BouncingSpheres()
{
    FHittableList World;

    auto CheckerboarTexture = std::make_shared<FCheckerTexture>(0.32, FColor3(0.2, 0.3, 0.1), FColor3(0.9, 0.9, 0.9));
    World.Add(std::make_shared<FSphere>(FPoint3(0, -1000, 0), 1000, std::make_shared<FLambertian>(CheckerboarTexture)));

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

    World = FHittableList(std::make_shared<FBVHNode>(World));

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
}

void CheckeredSpheres()
{
    FHittableList World;

    auto Checker = std::make_shared<FCheckerTexture>(0.32, FColor3(0.2, 0.3, 0.1), FColor3(0.9, 0.9, 0.9));

    World.Add(std::make_shared<FSphere>(FPoint3(0, -10, 0), 10, std::make_shared<FLambertian>(Checker)));
    World.Add(std::make_shared<FSphere>(FPoint3(0, 10, 0), 10, std::make_shared<FLambertian>(Checker)));

    FCamera Camera;
    Camera.AspectRatio = 16.0 / 9.0;
    Camera.ImageWidth = 1920;
    Camera.IterationsPerPixel = 30;
    Camera.MaxDepth = 10;
    Camera.VFOV = 20;
    Camera.LookFrom = {13, 2, 3};
    Camera.LookAt = {0, 0, 0};
    Camera.Up = {0, 1, 0};
    Camera.DefocusAngle = 0.;

    Camera.Render(World);

    Camera.SaveAsBMP("Result.bmp");
    Camera.SaveAsEXR("Result.exr");
}

void Earth()
{
    auto EarthTexture = std::make_shared<FImageTexture>("earthmap.jpg");
    auto EarthSurface = std::make_shared<FLambertian>(EarthTexture);
    auto Globe = std::make_shared<FSphere>(FPoint3(0, 0, 0), 2, EarthSurface);

    FCamera Camera;
    Camera.AspectRatio = 16.0 / 9.0;
    Camera.ImageWidth = 1920;
    Camera.IterationsPerPixel = 30;
    Camera.MaxDepth = 10;
    Camera.VFOV = 20;
    Camera.LookFrom = {0, 0, 12};
    Camera.LookAt = {0, 0, 0};
    Camera.Up = {0, 1, 0};
    Camera.DefocusAngle = 0.;

    Camera.Render(FHittableList(Globe));

    Camera.SaveAsBMP("Result.bmp");
    Camera.SaveAsEXR("Result.exr");
}

void PerlinSpheres()
{
    FHittableList World;

    auto PerText = std::make_shared<FNoiseTexture>();
    World.Add(std::make_shared<FSphere>(FPoint3(0, -1000, 0), 1000, std::make_shared<FLambertian>(PerText)));
    World.Add(std::make_shared<FSphere>(FPoint3(0, 2, 0), 2, std::make_shared<FLambertian>(PerText)));

    FCamera Camera;
    Camera.AspectRatio = 16.0 / 9.0;
    Camera.ImageWidth = 1920;
    Camera.IterationsPerPixel = 30;
    Camera.MaxDepth = 10;
    Camera.VFOV = 20;
    Camera.LookFrom = {13, 2, 3};
    Camera.LookAt = {0, 0, 0};
    Camera.Up = {0, 1, 0};
    Camera.DefocusAngle = 0.;

    Camera.Render(World);

    Camera.SaveAsBMP("Result.bmp");
    Camera.SaveAsEXR("Result.exr");
}

int main()
{
    switch (4)
    {
        case 1:
        {
            BouncingSpheres();
            break;
        }

        case 2:
        {
            CheckeredSpheres();
            break;
        }

        case 3:
        {
            Earth();
            break;
        }

        case 4:
        {
            PerlinSpheres();
            break;
        }

    }

    return 0;
}
