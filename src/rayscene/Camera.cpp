  #include <iostream>
  #include <cmath>
  #include <thread>
  #include "Camera.hpp"
  #include "../raymath/Ray.hpp"

  struct RenderSegment
  {
  public:
    int rowMin;
    int rowMax;
    Image *image;
    double height;
    double intervalX;
    double intervalY;
    int reflections;
    Scene *scene;
  };

  Camera::Camera() : position(Vector3())
  {
  }

  Camera::Camera(Vector3 pos) : position(pos)
  {
  }

  Camera::~Camera()
  {
  }

  Vector3 Camera::getPosition()
  {
    return position;
  }

  void Camera::setPosition(Vector3 &pos)
  {
    position = pos;
  }

  /**
  * Render a segment (set of rows) of the image
  */

  //optimisation 4 :  pas de Vector3 origin recréé, pas de coord - origin à chaque pixel
  
 static void renderSegment(RenderSegment *segment)
{
    const Vector3 origin(0, 0, -1);

    for (int y = segment->rowMin; y < segment->rowMax; ++y)
    {
        double yCoord = (segment->height / 2.0) - (y * segment->intervalY);

        for (int x = 0; x < segment->image->width; ++x)
        {
            double xCoord = -0.5 + (x * segment->intervalX);

            Vector3 dir(xCoord - origin.x, yCoord - origin.y, -origin.z);
            dir = dir.normalize();

            Ray ray(origin, dir);

            Color pixel = segment->scene->raycast(ray, ray, 0, segment->reflections);
            segment->image->setPixel(x, y, pixel);
        }
    }
}


void Camera::render(Image &image, Scene &scene)
{
    scene.prepare(); // Optimisation 1: préparer la scène avant le rendu

    double ratio = (double)image.width / (double)image.height;
    double height = 1.0 / ratio;
    double intervalX = 1.0 / (double)image.width;
    double intervalY = height / (double)image.height;

#ifndef USE_THREADING
    // ---------- MODE SINGLE-THREAD ----------
    RenderSegment seg;
    seg.height = height;
    seg.image = &image;
    seg.scene = &scene;
    seg.intervalX = intervalX;
    seg.intervalY = intervalY;
    seg.reflections = Reflections;
    seg.rowMin = 0;
    seg.rowMax = image.height;

    renderSegment(&seg);
#else
    // ---------- MODE MULTITHREADING ----------
    unsigned int nthreads = std::thread::hardware_concurrency();
    if (nthreads == 0) nthreads = 4;

    int rowsPerThread = image.height / nthreads;

    std::vector<std::thread> threads;
    threads.reserve(nthreads);

    // Segments sur la pile
    std::vector<RenderSegment> segments(nthreads);

    for (unsigned int i = 0; i < nthreads; i++)
    {
        RenderSegment &seg = segments[i];
        seg.height = height;
        seg.image = &image;
        seg.scene = &scene;
        seg.intervalX = intervalX;
        seg.intervalY = intervalY;
        seg.reflections = Reflections;

        seg.rowMin = i * rowsPerThread;
        seg.rowMax = (i == nthreads - 1) ? image.height : (i + 1) * rowsPerThread;

        threads.emplace_back(renderSegment, &seg);
    }

    for (auto &t : threads)
        t.join();
#endif
}

std::ostream &operator<<(std::ostream &_stream, Camera &cam)
{
    Vector3 pos = cam.getPosition();
    return _stream << "Camera(" << pos << ")";
}