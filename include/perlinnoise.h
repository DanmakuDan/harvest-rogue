//----------------------------------------------------------------------------------------
//
//	siv::PerlinNoise
//	Perlin noise library for modern C++
//
//	Copyright (C) 2013-2016 Ryo Suzuki <reputeless@gmail.com>
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files(the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions :
//	
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//	
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.
//
//----------------------------------------------------------------------------------------

# pragma once
# include <cstdint>
# include <numeric>
# include <algorithm>
# include <random>

namespace siv {
   class PerlinNoise {
   private:

      int32_t p[512];

      static double fade(double t) noexcept {
         return t * t * t * (t * (t * 6 - 15) + 10);
      }

      static double lerp(double t, double a, double b) noexcept {
         return a + t * (b - a);
      }

      static double grad(int32_t hash, double x, double y, double z) noexcept {
         const auto h = hash & 15;
         const auto u = h < 8 ? x : y;
         const auto v = h < 4 ? y : h == 12 || h == 14 ? x : z;
         return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
      }

   public:

      explicit PerlinNoise(uint32_t seed = std::default_random_engine::default_seed) {
         reseed(seed);
      }

      void reseed(uint32_t seed) {
         for (auto i = 0; i < 256; ++i) {
            p[i] = i;
         }

         shuffle(std::begin(p), std::begin(p) + 256, std::default_random_engine(seed));

         for (size_t i = 0; i < 256; ++i) {
            p[256 + i] = p[i];
         }
      }

      double noise(double x) const {
         return noise(x, 0.0, 0.0);
      }

      double noise(double x, double y) const {
         return noise(x, y, 0.0);
      }

      double noise(double x, double y, double z) const {
         const auto X = static_cast<int32_t>(floor(x)) & 255;
         const auto Y = static_cast<int32_t>(floor(y)) & 255;
         const auto Z = static_cast<int32_t>(floor(z)) & 255;

         x -= floor(x);
         y -= floor(y);
         z -= floor(z);

         const auto u = fade(x);
         const auto v = fade(y);
         const auto w = fade(z);

         const auto A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z;
         const auto B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;

         return lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z),
            grad(p[BA], x - 1, y, z)),
            lerp(u, grad(p[AB], x, y - 1, z),
               grad(p[BB], x - 1, y - 1, z))),
            lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1),
               grad(p[BA + 1], x - 1, y, z - 1)),
               lerp(u, grad(p[AB + 1], x, y - 1, z - 1),
                  grad(p[BB + 1], x - 1, y - 1, z - 1))));
      }

      double octaveNoise(double x, int32_t octaves) const {
         auto result = 0.0;
         auto amp = 1.0;

         for (int32_t i = 0; i < octaves; ++i) {
            result += noise(x) * amp;
            x *= 2.0;
            amp *= 0.5;
         }

         return result;
      }

      double octaveNoise(double x, double y, int32_t octaves) const {
         auto result = 0.0;
         auto amp = 1.0;

         for (auto i = 0; i < octaves; ++i) {
            result += noise(x, y) * amp;
            x *= 2.0;
            y *= 2.0;
            amp *= 0.5;
         }

         return result;
      }

      double octaveNoise(double x, double y, double z, int32_t octaves) const {
         auto result = 0.0;
         auto amp = 1.0;

         for (auto i = 0; i < octaves; ++i) {
            result += noise(x, y, z) * amp;
            x *= 2.0;
            y *= 2.0;
            z *= 2.0;
            amp *= 0.5;
         }

         return result;
      }

      double noise0_1(double x) const {
         return noise(x) * 0.5 + 0.5;
      }

      double noise0_1(double x, double y) const {
         return noise(x, y) * 0.5 + 0.5;
      }

      double noise0_1(double x, double y, double z) const {
         return noise(x, y, z) * 0.5 + 0.5;
      }

      double octaveNoise0_1(double x, int32_t octaves) const {
         return octaveNoise(x, octaves) * 0.5 + 0.5;
      }

      double octaveNoise0_1(double x, double y, int32_t octaves) const {
         return octaveNoise(x, y, octaves) * 0.5 + 0.5;
      }

      double octaveNoise0_1(double x, double y, double z, int32_t octaves) const {
         return octaveNoise(x, y, z, octaves) * 0.5 + 0.5;
      }
   };
}
