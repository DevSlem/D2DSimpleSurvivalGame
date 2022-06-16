#pragma once

#include <string>
#include <Windows.h>
#include <d2d1.h>
#include <wincodec.h>
#include <algorithm>
#include <cmath>

#include "SoundManager.h"
#include "Vector2.h"
#include "Bounds.h"
#include "BoundsSetter.h"
#include "Random.h"

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p) = NULL; }}
#endif // !SAFE_RELEASE

namespace DevSlem::D2DEngine
{
	std::wstring format_arg_list(const wchar_t* fmt, va_list args);
	std::wstring format_wstring(const wchar_t* fmt, ...);
	D2D1_RECT_F GetRectF(const D2D1_POINT_2F& center, const D2D1_POINT_2F& extents);
	D2D1_POINT_2F GetCenterOfRect(const D2D1_RECT_F& rect);
	D2D1_POINT_2F GetExtentsOfRect(const D2D1_RECT_F& rect);
	bool ContainedInRect(const D2D1_RECT_F& rect, const D2D1_POINT_2F& position);
	float Lerp(float from, float to, float t);
	float InverseLerp(float from, float to, float value);
	POINT Point2L(LONG x = 0, LONG y = 0);
	float Distance(D2D1_POINT_2F end);
	float Distance(D2D1_POINT_2F start, D2D1_POINT_2F end);
	bool IntersectRect(const D2D1_RECT_F& r1, const D2D1_RECT_F& r2);
	D2D1_POINT_2F Add(D2D1_POINT_2F p1, D2D1_POINT_2F p2);

	class Time
	{
	private:
		static LARGE_INTEGER freq;
		static LARGE_INTEGER prevTime;
		static float deltaTime;
		static float fixedDeltaTime;

	public:
		static void Initialize()
		{
			::QueryPerformanceFrequency(&freq);
			::QueryPerformanceCounter(&prevTime);
			fixedDeltaTime = 1.0f / 50.0f;
		}
		static void SetDeltaTime()
		{
			LARGE_INTEGER currentTime;
			::QueryPerformanceCounter(&currentTime);

			deltaTime = (float)((double)(currentTime.QuadPart - prevTime.QuadPart) / (double)(freq.QuadPart));
			prevTime = currentTime;
		}

		static void SetFixedDeltaTime(float value)
		{
			fixedDeltaTime = value;
		}

		static float DeltaTime() { return deltaTime; }
		static float FixedDeltaTime() { return fixedDeltaTime; }

	private:
		Time() {}
	};

	/* Time static members initialize */
	inline LARGE_INTEGER Time::freq = {};
	inline LARGE_INTEGER Time::prevTime = {};
	inline float Time::deltaTime = 0;
	inline float Time::fixedDeltaTime = 0;

	class Timer
	{
	private:
		float duration;
		float time;

	public:
		// Constructor
		Timer(float duration)
		{
			SetDuration(duration);
		}

		// Getter
		float Duration() const { return duration; }
		float Time() const { return max(0, time); }
		bool TimeOver() const { return time <= 0; }

		// Setter
		void Initialize() { time = duration; }
		void SetDuration(float duration)
		{
			this->duration = max(0, duration);
			Initialize();
		}

		// Method
		bool Tick(float deltaTime)
		{
			time = max(0, time - deltaTime);
			if (time <= 0)
			{
				return true;
			}

			return false;
		}
	};

	class Sound
	{
	/* === Static Members === */
	private:
		// Field
		static CSoundManager* soundManager;
		static const int minVolume;
		static const int maxVolume;
		static bool isMute;
		static int systemVolume;

	public:
		// Getter
		static int SystemVolume();
		static bool IsMute();

		// Setter
		static void SetSoundManager(CSoundManager* value);
		static void SetSystemVolume(int value);
		static void ChangeSystemVolume(int deltaValue);
		static void SetMute(bool value);

	/* === Instance Members === */
	private:
		// Field
		int id;

	public:
		// Constructor
		Sound();

		// Getter
		int ID() const;

		// Setter
		void SetVolume(int value);

		// Method
		bool CreateSound(LPTSTR path);
		void Play(bool loop = false);
		void Stop();
	};


}
