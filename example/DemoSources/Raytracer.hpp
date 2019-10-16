#pragma once

#include <cmath>
#include <limits>
#include <cstdint>
#include <vector>
#include <algorithm>

namespace Raytracer
{


	template <size_t DIM, typename T> struct vec
	{
		vec() { for (size_t i{ DIM }; i--; data_[i] = T()); }
		T& operator[](const size_t i) { return data_[i]; }
		const T& operator[](const size_t i) const { return data_[i]; }
	private:
		T data_[DIM];
	};

	using Vec3f = vec<3, float>;
	using Vec4f = vec<4, float>;

	template <typename T> struct vec<3, T>
	{
		vec() : x(T()), y(T()), z(T()) {}
		vec(T X, T Y, T Z) : x(X), y(Y), z(Z) {}
		T& operator[](const size_t i) { return i <= 0 ? x : (1 == i ? y : z); }
		const T& operator[](const size_t i) const { return i <= 0 ? x : (1 == i ? y : z); }
		float norm() { return std::sqrt(x * x + y * y + z * z); }
		vec<3, T>& normalize(T l = 1) { *this = (*this) * (l / norm()); return *this; }
		T x, y, z;
	};

	template <typename T> struct vec<4, T>
	{
		vec() : x(T()), y(T()), z(T()), w(T()) {}
		vec(T X, T Y, T Z, T W) : x(X), y(Y), z(Z), w(W) {}
		T& operator[](const size_t i) { return i <= 0 ? x : (1 == i ? y : (2 == i ? z : w)); }
		const T& operator[](const size_t i) const { return i <= 0 ? x : (1 == i ? y : (2 == i ? z : w)); }
		T x, y, z, w;
	};

	template<size_t DIM, typename T> T operator*(const vec<DIM, T>& lhs, const vec<DIM, T>& rhs)
	{
		T ReturnValue{ T() };

		for (size_t i{ DIM }; i--;)
		{
			ReturnValue += lhs[i] * rhs[i];
		}

		return ReturnValue;
	}

	template<size_t DIM, typename T>vec<DIM, T> operator+(vec<DIM, T> lhs, const vec<DIM, T>& rhs)
	{
		for (size_t i{ DIM }; i--;)
		{
			lhs[i] += rhs[i];
		}

		return lhs;
	}

	template<size_t DIM, typename T>vec<DIM, T> operator-(vec<DIM, T> lhs, const vec<DIM, T>& rhs)
	{
		for (size_t i{ DIM }; i--;)
		{
			lhs[i] -= rhs[i];
		}

		return lhs;
	}

	template<size_t DIM, typename T, typename U> vec<DIM, T> operator*(const vec<DIM, T>& lhs, const U& rhs)
	{
		vec<DIM, T> ReturnValue;

		for (size_t i{ DIM }; i--;)
		{
			ReturnValue[i] = lhs[i] * rhs;
		}

		return ReturnValue;
	}

	template<size_t DIM, typename T> vec<DIM, T> operator-(const vec<DIM, T>& lhs)
	{
		return lhs * T(-1);
	}

	struct Light
	{
		Light(const Vec3f& p, const float i) : Position(p), Intensity(i) {}
		Vec3f Position;
		float Intensity{};
	};

	struct Material
	{
		Material(const float r, const Vec4f& a, const Vec3f& color, const float spec) : Albedo(a), DiffuseColor(color), SpecularExponent(spec), RefractiveIndex(r) {}
		Material() : Albedo(1.0F, 0.0F, 0.0F, 0.0F), RefractiveIndex(1) {}
		Vec4f Albedo;
		Vec3f DiffuseColor;
		float SpecularExponent{};
		float RefractiveIndex{};
	};

	struct Sphere
	{
		Vec3f Center;
		Material material;
		float Radius{};

		Sphere(const Vec3f& c, const float r, const Material& m) : Center(c), material(m), Radius(r)  {}

		inline bool RayIntersect(const Vec3f& Origin, const Vec3f& Direction, float& t0) const
		{
			const Vec3f L{ Center - Origin };
			const float tca{ L * Direction };
			const float d2{ L * L - tca * tca };

			if (d2 > Radius * Radius)
			{
				return false;
			}

			const float thc{ std::sqrtf(Radius * Radius - d2) };
			const float t1{ tca + thc };

			t0 = tca - thc;

			if (t0 < 0.0F)
			{
				t0 = t1;
			}

			if (t0 < 0.0F)
			{
				return false;
			}

			return true;
		}
	};

	inline Vec3f Reflect(const Vec3f& I, const Vec3f& N)
	{
		return I - N * 2.0F * (I * N);
	}

	inline Vec3f Refract(const Vec3f& I, const Vec3f& N, const float eta_t, const float eta_i = 1.0F)
	{
		const float CosI{ -(std::max)(-1.0F, (std::min)(1.0F, I * N)) };

		if (CosI < 0)
		{
			return Refract(I, -N, eta_i, eta_t);
		}

		const float ETA{ eta_i / eta_t };
		const float k{ 1.0F - ETA * ETA * (1.0F - CosI * CosI) };

		return k < 0.0F ? Vec3f(1.0F, 0.0F, 0.0F) : I * ETA + N * (ETA * CosI - std::sqrtf(k));
	}

	inline bool SceneIntersect(const Vec3f& Origin, const Vec3f& Direction, const std::vector<Sphere>& spheres, Vec3f& Hit, Vec3f& N, Material& material)
	{
		float SpheresDist{ (std::numeric_limits<float>::max)() };

		for (const auto& Sphere : spheres)
		{
			float DistI{};

			if (Sphere.RayIntersect(Origin, Direction, DistI) && DistI < SpheresDist)
			{
				SpheresDist = DistI;
				Hit = Origin + Direction * DistI;
				N = (Hit - Sphere.Center).normalize();
				material = Sphere.material;
			}
		}

		return SpheresDist < 1000.0F;
	}

	inline std::vector<Vec3f> SphericalMap;
	inline std::int_fast32_t SphericalMapWidth{};
	inline std::int_fast32_t SphericalMapHeight{};

	inline Vec3f CastRay(const Vec3f& Origin, const Vec3f& Direction, const std::vector<Sphere>& spheres, const std::vector<Light>& lights, size_t Depth = 0)
	{
		Vec3f Point;
		Vec3f N;
		Material material;

		if (Depth > 4 || !SceneIntersect(Origin, Direction, spheres, Point, N, material))
		{
			std::int_fast32_t x{ (std::max)(0, (std::min)(SphericalMapWidth - 1, static_cast<std::int_fast32_t>((lwmf::Atan2Approx(Direction.z, Direction.x) / (lwmf::DoublePI) + 0.5F) * SphericalMapWidth))) };
			std::int_fast32_t y{ (std::max)(0, (std::min)(SphericalMapHeight - 1, static_cast<std::int_fast32_t>(std::acosf(Direction.y) / lwmf::PI * SphericalMapHeight))) };

			return SphericalMap[y * SphericalMapWidth + x];
		}

		const Vec3f ReflectDir{ Reflect(Direction, N).normalize() };
		const Vec3f RefractDir{ Refract(Direction, N, material.RefractiveIndex).normalize() };
		const Vec3f ReflectOrigin{ ReflectDir * N < 0.0F ? Point - N * 1e-3F : Point + N * 1e-3F };
		const Vec3f RefractOrigin{ RefractDir * N < 0.0F ? Point - N * 1e-3F : Point + N * 1e-3F };
		const Vec3f ReflectColor{ CastRay(ReflectOrigin, ReflectDir, spheres, lights, Depth + 1) };
		const Vec3f RefractColor{ CastRay(RefractOrigin, RefractDir, spheres, lights, Depth + 1) };

		float DiffuseLightIntensity{};
		float SpecularLightIntensity{};

		for (const auto& Light : lights)
		{
			const Vec3f LightDir{ (Light.Position - Point).normalize() };
			const float LightDistance{ (Light.Position - Point).norm() };

			const Vec3f ShadowOrigin{ LightDir * N < 0.0F ? Point - N * 1e-3F : Point + N * 1e-3F };
			Vec3f ShadowPoint;
			Vec3f ShadowN;
			Material TempMaterial;

			if (SceneIntersect(ShadowOrigin, LightDir, spheres, ShadowPoint, ShadowN, TempMaterial) && (ShadowPoint - ShadowOrigin).norm() < LightDistance)
			{
				continue;
			}

			DiffuseLightIntensity += Light.Intensity * (std::max)(0.0F, LightDir * N);
			SpecularLightIntensity += std::powf((std::max)(0.0F, -Reflect(-LightDir, N) * Direction), material.SpecularExponent) * Light.Intensity;
		}

		return material.DiffuseColor * DiffuseLightIntensity * material.Albedo[0] + Vec3f(1.0F, 1.0F, 1.0F) * SpecularLightIntensity * material.Albedo[1] + ReflectColor * material.Albedo[2] + RefractColor * material.Albedo[3];
	}

	inline std::vector<Sphere> UsedSpheres;
	inline std::vector<Light> UsedLights;

	inline void Init()
	{
		UsedLights.emplace_back(Light(Vec3f(-20.0F, 20.0F, 20.0F), 1.5F));
		UsedLights.emplace_back(Light(Vec3f(30.0F, 50.0F, -25.0F), 1.8F));
		UsedLights.emplace_back(Light(Vec3f(30.0F, 20.0F, 30.0F), 1.7F));

		lwmf::TextureStruct SphericalMapPNG;
		lwmf::LoadPNG(SphericalMapPNG, "./DemoGFX/Spherical.png");

		SphericalMapWidth = SphericalMapPNG.Width;
		SphericalMapHeight = SphericalMapPNG.Height;
		SphericalMap = std::vector<Vec3f>(static_cast<size_t>(SphericalMapWidth) * static_cast<size_t>(SphericalMapHeight));

		for (std::int_fast32_t Offset{}; Offset < SphericalMapPNG.Size; ++Offset)
		{
			const lwmf::ColorStruct Color{ lwmf::INTtoRGBA(SphericalMapPNG.Pixels[Offset]) };
			SphericalMap[Offset] = Vec3f(Color.Red * (1 / 255.0F), Color.Green * (1 / 255.0F), Color.Blue * (1 / 255.0F));
		}
	}

	inline void Draw()
	{
		static const Material Glass(1.5F, Vec4f(0.0F, 0.5F, 0.1F, 0.8F), Vec3f(0.6F, 0.7F, 0.8F), 125.0F);
		static const float DirZ{ -ScreenTexture.Height / (2.0F * std::tanf(lwmf::PI / 6.0F)) };
		static Vec3f SpherePos{ -5.0F, 0.0F, -20.0F };
		static Vec3f SphereDir{ 0.5F, 0.5F, 1.5F };
		static float Zoom{ 50.0F };
		static float ZoomSpeed{ 10.0F };

		UsedSpheres.clear();
		UsedSpheres.shrink_to_fit();
		UsedSpheres.emplace_back(Sphere(Vec3f(SpherePos.x, SpherePos.y, SpherePos.z), 4.0F, Glass));

		#pragma omp parallel for
		for (std::int_fast32_t y{}; y < ScreenTexture.Height; ++y)
		{
			const float DirY{ -(y + 0.5F) + ScreenTexture.HeightMid };

			for (std::int_fast32_t x{}; x < ScreenTexture.Width; ++x)
			{
				const float DirX{ (x + 0.5F) - ScreenTexture.WidthMid };

				Vec3f Result{ CastRay(Vec3f(0.0F, 0.0F, 0.0F), Vec3f(DirX, DirY, DirZ + Zoom).normalize(), UsedSpheres, UsedLights) };
				const float Max{ (std::max)(Result[0], (std::max)(Result[1], Result[2])) };

				if (Max > 1.0F)
				{
					Result = Result * (1.0F / Max);
				}

				lwmf::SetPixel(ScreenTexture, x, y, lwmf::RGBAtoINT(static_cast<std::int_fast32_t>(Result[0] * 255.0F), static_cast<std::int_fast32_t>(Result[1] * 255.0F), static_cast<std::int_fast32_t>(Result[2] * 255.0F), 255));
			}
		}

		SpherePos.x += SphereDir.x;
		SpherePos.y += SphereDir.y;
		SpherePos.z += SphereDir.z;
		Zoom += ZoomSpeed;

		if (SpherePos.x > 8.0F|| SpherePos.x < -8.0F)
		{
			SphereDir.x *= -1.0F;
		}

		if (SpherePos.y > 8.0F || SpherePos.y < -8.0F)
		{
			SphereDir.y *= -1.0F;
		}

		if (SpherePos.z > -10.0F || SpherePos.z < -40.0F)
		{
			SphereDir.z *= -1.0F;
		}

		if (Zoom > 200.0F || Zoom < 50.0F)
		{
			ZoomSpeed *= -1.0F;
		}

		lwmf::RenderText(ScreenTexture, "OpenMP accelerated realtime raytracing", 10, 10, 0xFFFFFFFF);
	}


} // namespace Raytracer