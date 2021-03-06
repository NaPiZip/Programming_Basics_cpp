// Copyright 2019, Nawin
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "rand.h"
#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "camera.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"

#include "hittable.h"
#include "hittable_list.h"
#include "utils.h"

#include <iostream>
#include <memory>
#include <algorithm>
#include <vector>
#include <numeric>
#include <execution>
#include <chrono>

using std::string_literals::operator""s;
using ::testing::Eq;
using ::testing::Ne;


namespace {

  TEST(Vec3GeneralTests, ShouldPass) {
    vec3 v1{ 1., 2., 3.},
         v2{ 1., 2., 3.};

    // const copy ctor
    vec3 v3(v1);
    EXPECT_DOUBLE_EQ(v3[0], v1[0]);
    EXPECT_DOUBLE_EQ(v3[1], v1[1]);
    EXPECT_DOUBLE_EQ(v3[2], v1[2]);

    EXPECT_DOUBLE_EQ(v1[0], 1.0);
    EXPECT_DOUBLE_EQ(v1[1], 2.0);
    EXPECT_DOUBLE_EQ(v1[2], 3.0);

    // const = operator
    vec3 v4 = v1;
    EXPECT_DOUBLE_EQ(v4[0], v1[0]);
    EXPECT_DOUBLE_EQ(v4[1], v1[1]);
    EXPECT_DOUBLE_EQ(v4[2], v1[2]);

    EXPECT_DOUBLE_EQ(v1[0], 1.0);
    EXPECT_DOUBLE_EQ(v1[1], 2.0);
    EXPECT_DOUBLE_EQ(v1[2], 3.0);


    // const operator[]
    EXPECT_DOUBLE_EQ(v1[0], 1.0);
    EXPECT_DOUBLE_EQ(v1[1], 2.0);
    EXPECT_DOUBLE_EQ(v1[2], 3.0);

    // operator[]
    v1[0] = 3.;
    v1[1] = 3.;
    v1[2] = 3.;
    EXPECT_DOUBLE_EQ(v1[0], 3.0);
    EXPECT_DOUBLE_EQ(v1[1], 3.0);
    EXPECT_DOUBLE_EQ(v1[2], 3.0);

    // operator+=
    v1 += v1;
    EXPECT_DOUBLE_EQ(v1[0], 6.0);
    EXPECT_DOUBLE_EQ(v1[1], 6.0);
    EXPECT_DOUBLE_EQ(v1[2], 6.0);

    // operator-=
    v1 -= v1;
    EXPECT_DOUBLE_EQ(v1[0], 0.0);
    EXPECT_DOUBLE_EQ(v1[1], 0.0);
    EXPECT_DOUBLE_EQ(v1[2], 0.0);

    // operator*=
    v1 = vec3{ 1., 2., 3. };
    v1 *= v1;
    EXPECT_DOUBLE_EQ(v1[0], 1.0);
    EXPECT_DOUBLE_EQ(v1[1], 4.0);
    EXPECT_DOUBLE_EQ(v1[2], 9.0);

    // operator/=
    v1 /= v1;
    EXPECT_DOUBLE_EQ(v1[0], 1.0);
    EXPECT_DOUBLE_EQ(v1[1], 1.0);
    EXPECT_DOUBLE_EQ(v1[2], 1.0);

    // operator t *=
    v1 *= 2.0;
    EXPECT_DOUBLE_EQ(v1[0], 2.0);
    EXPECT_DOUBLE_EQ(v1[1], 2.0);
    EXPECT_DOUBLE_EQ(v1[2], 2.0);

    // operator t /=
    v1 /= 2.0;
    EXPECT_DOUBLE_EQ(v1[0], 1.0);
    EXPECT_DOUBLE_EQ(v1[1], 1.0);
    EXPECT_DOUBLE_EQ(v1[2], 1.0);

    // operator*/=
    v1 /= v1;
    EXPECT_DOUBLE_EQ(v1[0], 1.0);
    EXPECT_DOUBLE_EQ(v1[1], 1.0);
    EXPECT_DOUBLE_EQ(v1[2], 1.0);

    // x,y,z r,g,b functions
    EXPECT_DOUBLE_EQ(v1.x(), v1.r());
    EXPECT_DOUBLE_EQ(v1.y(), v1.g());
    EXPECT_DOUBLE_EQ(v1.z(), v1.b());

    // lenght()
    v1 = vec3{ 2., 2., 1. };
    EXPECT_DOUBLE_EQ(v1.length(), 3.0);

    // squared_length()
    EXPECT_DOUBLE_EQ(v1.squared_length(), 9.0);

    // make_unit_vector()
    v1.make_unit_vector();
    EXPECT_DOUBLE_EQ(v1.length(), 1.0);

    // operator>>
    std::istringstream is{ "5. 6. 7."s };
    is >> v1;
    EXPECT_DOUBLE_EQ(v1[0], 5.0);
    EXPECT_DOUBLE_EQ(v1[1], 6.0);
    EXPECT_DOUBLE_EQ(v1[2], 7.0);

    // operator<<
    std::ostringstream oss;
    oss << v1;
    EXPECT_THAT(oss.str(), "(x,y,z): 5, 6, 7\n"s);

    // operator+ -
    v1 = vec3{ 1., 2., 3. };
    v2 = -v1;
    EXPECT_DOUBLE_EQ(-v1[0], v2[0]);
    EXPECT_DOUBLE_EQ(-v1[1], v2[1]);
    EXPECT_DOUBLE_EQ(-v1[2], v2[2]);
    
    v2 = +v1;
    EXPECT_DOUBLE_EQ(v1[0], v2[0]);
    EXPECT_DOUBLE_EQ(v1[1], v2[1]);
    EXPECT_DOUBLE_EQ(v1[2], v2[2]);

    // operator ==
    v1 = v2;
    EXPECT_THAT(v1, Eq(v2));

    // operator *
    v2 = v1 * v1;
    EXPECT_DOUBLE_EQ(v2[0], 1);
    EXPECT_DOUBLE_EQ(v2[1], 4);
    EXPECT_DOUBLE_EQ(v2[2], 9);


    // operator /
    v2 = v1 / v1;
    EXPECT_DOUBLE_EQ(v2[0], 1);
    EXPECT_DOUBLE_EQ(v2[1], 1);
    EXPECT_DOUBLE_EQ(v2[2], 1);

    // operator * t
    v2 = v1 * 2.;
    EXPECT_DOUBLE_EQ(v2[0], 2);
    EXPECT_DOUBLE_EQ(v2[1], 4);
    EXPECT_DOUBLE_EQ(v2[2], 6);

    // operator t * 
    v2 = 2. *v1 ;
    EXPECT_DOUBLE_EQ(v2[0], 2);
    EXPECT_DOUBLE_EQ(v2[1], 4);
    EXPECT_DOUBLE_EQ(v2[2], 6);
  
    // operator /
    v2 = 1. / v1;
    EXPECT_DOUBLE_EQ(v2[0], 1);
    EXPECT_DOUBLE_EQ(v2[1], 1./2.);
    EXPECT_DOUBLE_EQ(v2[2], 1./3.);

    // operator /
    v2 = v1 / 1.;
    EXPECT_DOUBLE_EQ(v2[0], 1);
    EXPECT_DOUBLE_EQ(v2[1], 2.);
    EXPECT_DOUBLE_EQ(v2[2], 3.);
  }

  TEST(Vec3Accumulate, Positive) {
    vec3<double> a[10], 
                 sum;

    for (auto it = std::begin(a); it != std::end(a); ++it) {
      *it = vec3{ 1.0, 1.0, 1.0 };
    }
    
    sum = std::accumulate(std::begin(a), std::end(a), vec3<double>{0.0, 0.0, 0.0});

    EXPECT_THAT(sum, Eq(vec3{ 10.0, 10.0, 10.0 }));
  }

  TEST(RayTest, Positive) {
    vec3 A{ 0.0, 0.0, 0.0 },
         B{ 0.0, 1.0, 0.0 };

    ray r(A, B);
    EXPECT_THAT(r.origin(), Eq(A));
    EXPECT_THAT(r.direction(), Eq(B));

    EXPECT_THAT(r.point_at_paprameter(1), Eq(A+B));
  }
  
  TEST(Hitrecord, Positive) {
    sphere s{ vec3{0.0, 0.0, -1.0}, 1.0 };
    ray r{ vec3{0.0, 0.0, 0.0}, vec3{0.0, 0.0, -1.0} };
    hit_record<double> record{};

    s.hit(r, 0.0, 1000.0, record);
    EXPECT_THAT(record.normal, Eq(vec3{ 0.0, 0.0, 1.0 }));
    EXPECT_THAT(record.p, Eq(vec3{ 0.0, 0.0, 0.0 }));
    EXPECT_THAT(record.t, Eq(0.0));
  };

  TEST(Hitlsit, Positive) {
    hittable<double>* list[2];
    list[0] = new  sphere{ vec3{0.0, 0.0, -1.0}, 1.0 };
    list[1] = new sphere{ vec3{0.0, 0.0, -1.0}, 1.0 };
    ray r{ vec3{0.0, 0.0, 0.0}, vec3{0.0, 0.0, -1.0} };

    hittable<double>* world = new hittable_list<double>(list, 2);
    
    hit_record<double> record{};
    EXPECT_TRUE(world->hit(r, 0.0, 1000.0, record));
    EXPECT_THAT(record.normal, Eq(vec3{ 0.0, 0.0, 1.0 }));
    EXPECT_THAT(record.p, Eq(vec3{ 0.0, 0.0, 0.0 }));
    EXPECT_THAT(record.t, Eq(0.0));
  }
  TEST(Rand, Positive) {
    std::vector v(1000, 0.0);

    for(auto& e: v)
      e = random<double>();
    
    std::sort(v.begin(), v.end());
    auto it = std::unique(v.begin(), v.end());
    EXPECT_THAT(it, Eq(v.end()));    
  }

  TEST(Camera, Positive) {
    camera cam;
    vec3 lower_left_corner = vec3(-2.0, -1.0, -1.0);
    vec3 horizontal        = vec3(4.0, 0.0, 0.0);
    vec3 vertical          = vec3(0.0, 2.0, 0.0);
    vec3 origin            = vec3(0.0, 0.0, 0.0);

    EXPECT_THAT(cam.get_ray(0, 0).origin(),  origin);
    EXPECT_THAT(cam.get_ray(0, 0).direction(), lower_left_corner);
    
    EXPECT_THAT(cam.get_ray(0, 1).origin(), origin);
    EXPECT_THAT(cam.get_ray(0, 1).direction(), lower_left_corner + vertical);

    EXPECT_THAT(cam.get_ray(1, 0).origin(), origin);
    EXPECT_THAT(cam.get_ray(1, 0).direction(), lower_left_corner + horizontal);

    EXPECT_THAT(cam.get_ray(1, 1).origin(), origin);
    EXPECT_THAT(cam.get_ray(1, 1).direction(), lower_left_corner + horizontal +vertical);
  }

  TEST(RandSphere, Positive) {
    EXPECT_TRUE(random_in_unit_sphere() != random_in_unit_sphere());
  }

  TEST(Lambertian, Positive){
    hit_record<double> hr{ 1.0, vec3{1.0, 1.0, 1.0},vec3{1.0, 1.0, 1.0}, nullptr };
    ray r{ vec3{0.0, 0.0, 0.0}, vec3{1.0, 1.0, 1.0} },
      scatter{ vec3{0.0, 0.0, 0.0}, vec3{0.0, 0.0, 0.0} };
    vec3 attenuation{ 0.0, 0.0, 0.0 };

    lambertian l(vec3{ 1.0, 1.0, 1.0 });
    EXPECT_TRUE(l.scatter(r,hr, attenuation, scatter ));
    EXPECT_THAT(attenuation, Eq(vec3{ 1.0, 1.0, 1.0 }));
    EXPECT_THAT(scatter.origin(), Eq(hr.p));
    EXPECT_THAT(scatter.direction(), Ne(hr.p+hr.normal));
  }
  TEST(Metal, positive) {
    hit_record<double> hr{ 1.0, vec3{1.0, 1.0, 1.0},vec3{0.0, 1./sqrt(2.), 1./sqrt(2.)}, nullptr };
    ray r{ vec3{0.0, 0.0, 0.0}, vec3{0.0, 0.0, -1.0} },
      scatter{ vec3{0.0, 0.0, 0.0}, vec3{0.0, 0.0, 0.0} };
    vec3 attenuation{ 0.0, 0.0, 0.0 };

    metal l(vec3{ 1.0, 1.0, 1.0 }, 1.0);
    vec3 res = l.reflect(vec3{ 0.0, 0.0, -1.0 }, vec3{ 0.0, 1. / sqrt(2.), 1. / sqrt(2.) });
    EXPECT_NEAR(res.x(), 0, 0.002);
    EXPECT_NEAR(res.y(), 1, 0.002);
    EXPECT_NEAR(res.z(), 0, 0.002);

    EXPECT_TRUE(l.scatter(r, hr, attenuation, scatter));
    EXPECT_THAT(attenuation, Eq(vec3{ 1.0, 1.0, 1.0 }));
    EXPECT_THAT(scatter.origin(), Eq(hr.p));
  }

  TEST(Dielectric, positive) {
    hit_record<double> hr{ 1.0, vec3{1.0, 1.0, 1.0},vec3{0.0, 1. / sqrt(2.), 1. / sqrt(2.)}, nullptr };
    ray r{ vec3{0.0, 0.0, 0.0}, vec3{0.0, 1. / sqrt(2.), -1. / sqrt(2.)} },
      scatter{ vec3{0.0, 0.0, 0.0}, vec3{0.0, 0.0, 0.0} };
    vec3 refracted{ 0.0, 0.0, 0.0 };

    dielectric d(1.5);
    EXPECT_TRUE(d.refract(r.direction(), vec3{ 0.0, 0.0, +1.0 }, 0.66666, refracted));
    EXPECT_NEAR(refracted.x(), 0, 0.002);
    EXPECT_NEAR(refracted.y(), 0.471, 0.002);
    EXPECT_NEAR(refracted.z(), -0.881, 0.002);

    EXPECT_TRUE(d.scatter(r, hr, refracted, scatter));
    EXPECT_THAT(scatter.origin(), Eq(hr.p));
  }

  TEST(Optimize, Positive) {
    std::chrono::high_resolution_clock::time_point t0, t1;

    constexpr int nx = 20,
                  ny = 20,
                  ns = 2000;
    double R = cos(M_PI / 4);


    vec3 col(0.0, 0.0, 0.0);
    camera cam(vec3(-2.0, 2.0, 1.0),
               vec3(0.0, 0.0, -1.0),
               vec3(0.0, 1.0, 0.0),
               60.0, static_cast<double>(nx / ny));

    hittable<double>* list[2];
    list[0] = new sphere(vec3(-R, 0.0, -1.0), R, new lambertian(vec3(0.0, 0.0, 1.0)));
    list[1] = new sphere(vec3(R, 0.0, -1.0), R, new lambertian(vec3(1.0, 0.0, 0.0)));

    hittable<double>* world = new hittable_list(list, 2);
    vec3<double> a[ns];
      
   
    t0 = std::chrono::high_resolution_clock::now();
    std::for_each(std::begin(a), std::end(a), [&](vec3<double> in) {
      double u = static_cast<double>(1 + random<double>()) / static_cast<double>(nx),
              v = static_cast<double>(1 + random<double>()) / static_cast<double>(ny);
      col += color(cam.get_ray(u, v), world, 0);
      });

    col /= ns;  

    t1 = std::chrono::high_resolution_clock::now();

    std::cout << "Diff in ms: ";
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>((t1 - t0)).count() << "\n";
  }
}  // namespace
