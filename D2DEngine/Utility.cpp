#include <stdarg.h>
#include <tchar.h>
#include "Utility.h"

using namespace DevSlem::D2DEngine;

std::wstring DevSlem::D2DEngine::format_arg_list(const wchar_t* fmt, va_list args)
{
	if (!fmt) return L"";
	int   result = -1, length = 512;
	wchar_t* buffer = 0;
	while (result == -1)
	{
		if (buffer)
			delete[] buffer;
		buffer = new wchar_t[length + 1];
		memset(buffer, 0, (length + 1) * sizeof(wchar_t));

		// remove deprecate warning
		//result = _vsnprintf(buffer, length, fmt, args);

		result = _vsnwprintf_s(buffer, length, _TRUNCATE, fmt, args);
		length *= 2;
	}
	std::wstring s(buffer);
	delete[] buffer;
	return s;
}


std::wstring DevSlem::D2DEngine::format_wstring(const wchar_t* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	std::wstring s = format_arg_list(fmt, args);
	va_end(args);
	return s;
}

D2D1_RECT_F DevSlem::D2DEngine::GetRectF(const D2D1_POINT_2F& center, const D2D1_POINT_2F& extents)
{
	return D2D1::RectF(center.x - extents.x, center.y - extents.y, center.x + extents.x, center.y + extents.y);
}

D2D1_POINT_2F DevSlem::D2DEngine::GetCenterOfRect(const D2D1_RECT_F& rect)
{
	return D2D1::Point2F((rect.right + rect.left) / 2, (rect.bottom + rect.top) / 2);
}

D2D1_POINT_2F DevSlem::D2DEngine::GetExtentsOfRect(const D2D1_RECT_F& rect)
{
	return { (rect.right - rect.left) / 2.0f, (rect.bottom - rect.top) / 2.0f };
}

bool DevSlem::D2DEngine::ContainedInRect(const D2D1_RECT_F& rect, const D2D1_POINT_2F& position)
{
	return position.x >= rect.left && position.y >= rect.top && position.x <= rect.right && position.y <= rect.bottom;
}

float DevSlem::D2DEngine::Lerp(float from, float to, float t)
{
	return from + (to - from) * t;
}

float DevSlem::D2DEngine::InverseLerp(float from, float to, float value)
{
	return (value - from) / (to - from);
}

POINT DevSlem::D2DEngine::Point2L(LONG x, LONG y)
{
	POINT p;
	p.x = x;
	p.y = y;
	return p;
}

float DevSlem::D2DEngine::Distance(D2D1_POINT_2F end)
{
	return std::sqrt((float)std::pow(end.x, 2) + (float)std::pow(end.y, 2));
}

float DevSlem::D2DEngine::Distance(D2D1_POINT_2F start, D2D1_POINT_2F end)
{
	return (float)std::sqrt(std::pow(end.x - start.x, 2) + std::pow(end.y - start.y, 2));
}

bool DevSlem::D2DEngine::IntersectRect(const D2D1_RECT_F& r1, const D2D1_RECT_F& r2)
{
	return r1.left < r2.right && r1.right > r2.left && r1.top < r2.bottom&& r1.bottom > r2.top;
}

D2D1_POINT_2F DevSlem::D2DEngine::Add(D2D1_POINT_2F p1, D2D1_POINT_2F p2)
{
	return { p1.x + p2.x, p1.y + p2.y };
}

/* ===== Sound ===== */

CSoundManager* Sound::soundManager = nullptr;
const int Sound::minVolume = 0;
const int Sound::maxVolume = 100;
int Sound::systemVolume = 50;
bool Sound::isMute = false;

void DevSlem::D2DEngine::Sound::SetSoundManager(CSoundManager* value)
{
    soundManager = value;
}

int DevSlem::D2DEngine::Sound::SystemVolume()
{
    return systemVolume;
}

bool DevSlem::D2DEngine::Sound::IsMute()
{
    return isMute;
}

void DevSlem::D2DEngine::Sound::SetSystemVolume(int value)
{
    if (soundManager == nullptr || soundManager->pDSBPrimary == nullptr)
        return;

    systemVolume = std::clamp(value, minVolume, maxVolume);

    if (!IsMute())
        soundManager->pDSBPrimary->SetVolume(DSVOLUME_TO_DB(systemVolume));
}

void DevSlem::D2DEngine::Sound::ChangeSystemVolume(int deltaValue)
{
    SetSystemVolume(systemVolume + deltaValue);
}

void DevSlem::D2DEngine::Sound::SetMute(bool value)
{
    if (value)
    {
        int temp = systemVolume;
        SetSystemVolume(0);
        systemVolume = temp;
        isMute = true;
    }
    else
    {
        isMute = false;
        SetSystemVolume(systemVolume);
    }
}

DevSlem::D2DEngine::Sound::Sound()
{
    id = -1;
}

bool DevSlem::D2DEngine::Sound::CreateSound(LPTSTR path)
{
    if (soundManager == nullptr)
        return false;

    soundManager->add(path, &id);

    return true;
}

int DevSlem::D2DEngine::Sound::ID() const
{
    return id;
}

void DevSlem::D2DEngine::Sound::Play(bool loop)
{
    if (soundManager == nullptr)
        return;

    soundManager->play(id, loop);
}

void DevSlem::D2DEngine::Sound::Stop()
{
    if (soundManager == nullptr)
        return;

    soundManager->stop(id);
}

void DevSlem::D2DEngine::Sound::SetVolume(int value)
{
    if (soundManager == nullptr)
        return;

    soundManager->SetVolume(id, DSVOLUME_TO_DB(std::clamp(value, minVolume, maxVolume)));
}
