#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"

int main()
{
    FHittableList World;

    World.Add(std::make_shared<FSphere>(FVector3{0, 0, -1}, 0.5f));
    World.Add(std::make_shared<FSphere>(FVector3{0, -100.5, -1}, 100.f));

    FCamera Camera;
    Camera.AspectRatio = 16.f / 9.f;
    Camera.ImageWidth = 1920;

    Camera.Render(World);

    Camera.SaveAsBMP("Result.bmp");
    Camera.SaveAsEXR("Result.exr");

    return 0;
}