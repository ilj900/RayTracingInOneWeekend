#include "bvh_node.h"
#include "camera.h"
#include "constant_media.h"
#include "hittable_list.h"
#include "material.h"
#include "quad.h"
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
    Camera.Background = {.7, 0.8, 1.};

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
    Camera.Background = {.7, 0.8, 1.};

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
    Camera.Background = {.7, 0.8, 1.};

    Camera.Render(FHittableList(Globe));

    Camera.SaveAsBMP("Result.bmp");
    Camera.SaveAsEXR("Result.exr");
}

void PerlinSpheres()
{
    FHittableList World;

    auto PerText = std::make_shared<FNoiseTexture>(4);
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
    Camera.Background = {.7, 0.8, 1.};

    Camera.Render(World);

    Camera.SaveAsBMP("Result.bmp");
    Camera.SaveAsEXR("Result.exr");
}

void Quads()
{
    FHittableList World;

    auto LeftRed = std::make_shared<FLambertian>(FColor3(1., 0.2, 0.2));
    auto BackGreen = std::make_shared<FLambertian>(FColor3(0.2, 1., 0.2));
    auto RightBlue = std::make_shared<FLambertian>(FColor3(0.2, 0.2, 1.));
    auto UpperOrange = std::make_shared<FLambertian>(FColor3(1., 0.5, 0.0));
    auto LowerTeal = std::make_shared<FLambertian>(FColor3(0.2, 0.8, 0.8));

    World.Add(std::make_shared<FQuad>(FPoint3(-3, -2, 5), FVector3(0, 0, -4), FVector3(0, 4,  0), LeftRed));
    World.Add(std::make_shared<FQuad>(FPoint3(-2, -2, 0), FVector3(4, 0,  0), FVector3(0, 4,  0), BackGreen));
    World.Add(std::make_shared<FQuad>(FPoint3( 3, -2, 1), FVector3(0, 0,  4), FVector3(0, 4,  0), RightBlue));
    World.Add(std::make_shared<FQuad>(FPoint3(-2,  3, 1), FVector3(4, 0,  0), FVector3(0, 0,  4), UpperOrange));
    World.Add(std::make_shared<FQuad>(FPoint3(-2, -3, 5), FVector3(4, 0,  0), FVector3(0, 0, -4), LowerTeal));

    FCamera Camera;
    Camera.AspectRatio = 16.0 / 9.0;
    Camera.ImageWidth = 1920;
    Camera.IterationsPerPixel = 30;
    Camera.MaxDepth = 10;
    Camera.VFOV = 80;
    Camera.LookFrom = {0, 0, 9};
    Camera.LookAt = {0, 0, 0};
    Camera.Up = {0, 1, 0};
    Camera.DefocusAngle = 0.;
    Camera.Background = {.7, 0.8, 1.};

    Camera.Render(World);

    Camera.SaveAsBMP("Result.bmp");
    Camera.SaveAsEXR("Result.exr");
}

void SimpleLight()
{
    FHittableList World;

    auto PerlinTexture = std::make_shared<FNoiseTexture>(4);

    World.Add(std::make_shared<FSphere>(FPoint3(0, -1000, 0), 1000, std::make_shared<FLambertian>(PerlinTexture)));
    World.Add(std::make_shared<FSphere>(FPoint3(0, 2, 0), 2, std::make_shared<FLambertian>(PerlinTexture)));

    auto DiffuseLight = std::make_shared<FDiffuseLight>(FColor3(4, 4, 4));
    World.Add(std::make_shared<FSphere>(FPoint3(0, 7, 0), 2, DiffuseLight));
    World.Add(std::make_shared<FQuad>(FPoint3(3, 1, -2), FVector3(2, 0 ,0), FVector3(0, 2, 0), DiffuseLight));

    FCamera Camera;
    Camera.AspectRatio = 16.0 / 9.0;
    Camera.ImageWidth = 1920;
    Camera.IterationsPerPixel = 30;
    Camera.MaxDepth = 10;
    Camera.VFOV = 20;
    Camera.LookFrom = {26, 3, 6};
    Camera.LookAt = {0, 2, 0};
    Camera.Up = {0, 1, 0};
    Camera.DefocusAngle = 0.;
    Camera.Background = {0., 0., 0.};

    Camera.Render(World);

    Camera.SaveAsBMP("Result.bmp");
    Camera.SaveAsEXR("Result.exr");
}

void CornellBox()
{
    FHittableList World;

    auto Red = std::make_shared<FLambertian>(FColor3(0.65, 0.05, 0.05));
    auto White = std::make_shared<FLambertian>(FColor3(0.73, 0.73, 0.73));
    auto Green = std::make_shared<FLambertian>(FColor3(0.12, 0.45, 0.15));
    auto Light = std::make_shared<FDiffuseLight>(FColor3(15, 15, 15));

    World.Add(std::make_shared<FQuad>(FPoint3(555,   0,   0), FVector3(   0, 555, 0), FVector3(0, 0, 555), Green));
    World.Add(std::make_shared<FQuad>(FPoint3(  0,   0,   0), FVector3(   0, 555, 0), FVector3(0, 0, 555), Red));
    World.Add(std::make_shared<FQuad>(FPoint3(343, 554, 332), FVector3(-130, 0, 0), FVector3(0, 0, -105), Light));
    World.Add(std::make_shared<FQuad>(FPoint3(  0,   0,   0), FVector3(   555, 0, 0), FVector3(0, 0, 555), White));
    World.Add(std::make_shared<FQuad>(FPoint3(555, 555, 555), FVector3(-555, 0, 0), FVector3(0, 0, -555), White));
    World.Add(std::make_shared<FQuad>(FPoint3(  0,   0, 555), FVector3(555, 0, 0), FVector3(0, 555, 0), White));

    std::shared_ptr<FHittable> Box1 = Box(FPoint3(0, 0, 0), FPoint3(165, 330, 165), White);
    Box1 = std::make_shared<FRotateY>(Box1, 15);
    Box1 = std::make_shared<FTranslate>(Box1, FVector3(265, 0, 295));
    World.Add(Box1);

    std::shared_ptr<FHittable> Box2 = Box(FPoint3(0, 0, 0), FPoint3(165, 165, 165), White);
    Box2 = std::make_shared<FRotateY>(Box2, -18);
    Box2 = std::make_shared<FTranslate>(Box2, FVector3(130, 0, 65));
    World.Add(Box2);

    FCamera Camera;
    Camera.AspectRatio = 1;
    Camera.ImageWidth = 1080;
    Camera.IterationsPerPixel = 20;
    Camera.MaxDepth = 10;
    Camera.Background = {0., 0., 0.};
    Camera.VFOV = 40;
    Camera.LookFrom = {278, 278, -800};
    Camera.LookAt = {278, 278, 0};
    Camera.Up = {0, 1, 0};
    Camera.DefocusAngle = 0.;

    Camera.Render(World);

    Camera.SaveAsBMP("Result.bmp");
    Camera.SaveAsEXR("Result.exr");
}

void CornellSmoke()
{
    FHittableList World;

    auto Red = std::make_shared<FLambertian>(FColor3(0.65, 0.05, 0.05));
    auto White = std::make_shared<FLambertian>(FColor3(0.73, 0.73, 0.73));
    auto Green = std::make_shared<FLambertian>(FColor3(0.12, 0.45, 0.15));
    auto Light = std::make_shared<FDiffuseLight>(FColor3(15, 15, 15));

    World.Add(std::make_shared<FQuad>(FPoint3(555,   0,   0), FVector3(   0, 555, 0), FVector3(0, 0, 555), Green));
    World.Add(std::make_shared<FQuad>(FPoint3(  0,   0,   0), FVector3(   0, 555, 0), FVector3(0, 0, 555), Red));
    World.Add(std::make_shared<FQuad>(FPoint3(343, 554, 332), FVector3(-130, 0, 0), FVector3(0, 0, -105), Light));
    World.Add(std::make_shared<FQuad>(FPoint3(  0,   0,   0), FVector3(   555, 0, 0), FVector3(0, 0, 555), White));
    World.Add(std::make_shared<FQuad>(FPoint3(555, 555, 555), FVector3(-555, 0, 0), FVector3(0, 0, -555), White));
    World.Add(std::make_shared<FQuad>(FPoint3(  0,   0, 555), FVector3(555, 0, 0), FVector3(0, 555, 0), White));

    std::shared_ptr<FHittable> Box1 = Box(FPoint3(0, 0, 0), FPoint3(165, 330, 165), White);
    Box1 = std::make_shared<FRotateY>(Box1, 15);
    Box1 = std::make_shared<FTranslate>(Box1, FVector3(265, 0, 295));
    World.Add(std::make_shared<FConstantMedia>(Box1, 0.01, FColor3(0, 0, 0)));

    std::shared_ptr<FHittable> Box2 = Box(FPoint3(0, 0, 0), FPoint3(165, 165, 165), White);
    Box2 = std::make_shared<FRotateY>(Box2, -18);
    Box2 = std::make_shared<FTranslate>(Box2, FVector3(130, 0, 65));
    World.Add(std::make_shared<FConstantMedia>(Box2, 0.01, FColor3(1, 1, 1)));

    FCamera Camera;
    Camera.AspectRatio = 1;
    Camera.ImageWidth = 1080;
    Camera.IterationsPerPixel = 200;
    Camera.MaxDepth = 10;
    Camera.Background = {0., 0., 0.};
    Camera.VFOV = 40;
    Camera.LookFrom = {278, 278, -800};
    Camera.LookAt = {278, 278, 0};
    Camera.Up = {0, 1, 0};
    Camera.DefocusAngle = 0.;

    Camera.Render(World);

    Camera.SaveAsBMP("Result.bmp");
    Camera.SaveAsEXR("Result.exr");
}

void FinalScene(int ImageWidth, int SamplesPerPixel, int MaxDepth)
{
    FHittableList Boxes1;
    auto Ground = std::make_shared<FLambertian>(FColor3(0.48, 0.83, 0.53));

    int BoxesPerSide = 20;
    for (int i = 0; i < BoxesPerSide; ++i)
    {
        for (int j = 0; j < BoxesPerSide; j++)
        {
            auto W = 100.;
            auto X0 = -1000. + i * W;
            auto Z0 = -1000. + j * W;
            auto Y0 = 0.;
            auto X1 = X0 + W;
            auto Y1 = RandomDouble(1, 101);
            auto Z1 = Z0 + W;

            Boxes1.Add(Box(FPoint3(X0, Y0, Z0), FPoint3(X1, Y1, Z1), Ground));
        }
    }

    FHittableList World;
    World.Add(std::make_shared<FBVHNode>(Boxes1));

    auto Light = std::make_shared<FDiffuseLight>(FColor3(7, 7, 7));
    World.Add(std::make_shared<FQuad>(FPoint3(123, 554, 147), FVector3(300, 0, 0), FVector3(0, 0, 265), Light));

    auto Center1 = FPoint3(400, 400, 200);
    auto Center2 = Center1 + FVector3(30, 0, 0);
    auto SphereMaterial = std::make_shared<FLambertian>(FColor3(0.7, 0.3, 0.1));
    World.Add(std::make_shared<FSphere>(Center1, Center2, 50, SphereMaterial));

    World.Add(std::make_shared<FSphere>(FPoint3(260, 150, 45), 50, std::make_shared<FDielectric>(1.5)));
    World.Add(std::make_shared<FSphere>(FPoint3(0, 150, 145), 50, std::make_shared<FMetal>(FColor3(0.8, 0.8, 0.9), 1.)));

    auto Boundary = std::make_shared<FSphere>(FPoint3(360, 150, 145), 70, std::make_shared<FDielectric>(1.5));
    World.Add(Boundary);
    World.Add(std::make_shared<FConstantMedia>(Boundary, 0.2, FColor3(0.2, 0.4, 0.9)));
    Boundary = std::make_shared<FSphere>(FPoint3(0, 0, 0), 5000, std::make_shared<FDielectric>(1.5));
    World.Add(std::make_shared<FConstantMedia>(Boundary, 0.0001, FColor3(1, 1, 1)));

    auto EarthMaterial = std::make_shared<FLambertian>(std::make_shared<FImageTexture>("earthmap.jpg"));
    World.Add(std::make_shared<FSphere>(FPoint3(400, 200, 400), 100, EarthMaterial));
    auto PerlinTexture = std::make_shared<FNoiseTexture>(0.2);
    World.Add(std::make_shared<FSphere>(FPoint3(220, 280, 300), 80, std::make_shared<FLambertian>(PerlinTexture)));

    FHittableList Boxes2;
    auto White = std::make_shared<FLambertian>(FColor3(0.73, 0.73, 0.73));
    int NS = 1000;
    for (int j = 0; j < NS; j++)
    {
        Boxes2.Add(std::make_shared<FSphere>(FPoint3(RandomDouble(0, 165), RandomDouble(0, 165), RandomDouble(0, 165)), 10, White));
    }

    World.Add(std::make_shared<FTranslate>(std::make_shared<FRotateY>(std::make_shared<FBVHNode>(Boxes2), 15), FVector3(-100, 270, 395)));

    FCamera Camera;
    Camera.AspectRatio = 1;
    Camera.ImageWidth = ImageWidth;
    Camera.IterationsPerPixel = SamplesPerPixel;
    Camera.MaxDepth = MaxDepth;
    Camera.Background = {0., 0., 0.};
    Camera.VFOV = 40;
    Camera.LookFrom = {478, 278, -600};
    Camera.LookAt = {278, 278, 0};
    Camera.Up = {0, 1, 0};
    Camera.DefocusAngle = 0.;

    Camera.Render(World);

    Camera.SaveAsBMP("Result.bmp");
    Camera.SaveAsEXR("Result.exr");
}

int main()
{
    switch (9)
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

        case 5:
        {
            Quads();
            break;
        }

        case 6:
        {
            SimpleLight();
            break;
        }

        case 7:
        {
            CornellBox();
            break;
        }

        case 8:
        {
            CornellSmoke();
            break;
        }

        case 9:
        {
            FinalScene(400, 250, 4);
            break;
        }

        case 10:
        {
            FinalScene(1080, 10000, 10);
            break;
        }

    }

    return 0;
}
