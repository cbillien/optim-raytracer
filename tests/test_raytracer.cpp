#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <cstdlib>
#include <string>
#include <vector>
#include "lodepng.h"

//exo1
// Fonction utilitaire pour lancer le raytracer
int run_raytracer(const std::string& scene, const std::string& output) {
    std::string cmd = "../build/raytracer " + scene + " " + output;
    return system(cmd.c_str());
}

//exo1
// Fonction pour comparer deux PNG pixel par pixel
bool compare_png(const std::string& file1, const std::string& file2) {
    std::vector<unsigned char> image1, image2;
    unsigned width1, height1, width2, height2;

    if (lodepng::decode(image1, width1, height1, file1) != 0) return false;
    if (lodepng::decode(image2, width2, height2, file2) != 0) return false;

    if (width1 != width2 || height1 != height2) return false;
    return image1 == image2;
}


//exo1
// ------------------ TESTS ------------------

TEST_CASE("IsoSphere scene", "[raytracer]") {
    std::string scene = "../scenes/iso-sphere-on-plane.json";
    std::string output = "../tests/test_iso.png";
    std::string reference = "../tests/reference_iso.png";

    REQUIRE(run_raytracer(scene, output) == 0);
    REQUIRE(compare_png(output, reference) == true);
}

TEST_CASE("Monkey scene", "[raytracer]") {
    std::string scene = "../scenes/monkey-on-plane.json";
    std::string output = "../tests/test_monkey.png";
    std::string reference = "../tests/reference_monkey.png";

    REQUIRE(run_raytracer(scene, output) == 0);
    REQUIRE(compare_png(output, reference) == true);
}

TEST_CASE("Two spheres scene", "[raytracer]") {
    std::string scene = "../scenes/two-spheres-on-plane.json";
    std::string output = "../tests/test_two_spheres.png";
    std::string reference = "../tests/reference_two_spheres.png";

    REQUIRE(run_raytracer(scene, output) == 0);
    REQUIRE(compare_png(output, reference) == true);
}

TEST_CASE("Empty scene (black image)", "[raytracer]") {
    std::string scene = "../scenes/empty.json";
    std::string output = "../tests/test_black.png";
    std::string reference = "../tests/reference_black.png";

    REQUIRE(run_raytracer(scene, output) == 0);
    REQUIRE(compare_png(output, reference) == true);
}
