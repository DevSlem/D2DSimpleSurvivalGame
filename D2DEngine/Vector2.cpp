#include <cmath>
#include "Vector2.h"

using namespace DevSlem::D2DEngine;

Vector2 DevSlem::D2DEngine::Vector2::Zero()
{
    return Vector2();
}

Vector2 DevSlem::D2DEngine::Vector2::One()
{
    return Vector2(1, 1);
}

Vector2 DevSlem::D2DEngine::Vector2::Left()
{
    return Vector2(-1, 0);
}

Vector2 DevSlem::D2DEngine::Vector2::Right()
{
    return Vector2(1, 0);
}

Vector2 DevSlem::D2DEngine::Vector2::Up()
{
    return Vector2(0, 1);
}

Vector2 DevSlem::D2DEngine::Vector2::Down()
{
    return Vector2(0, -1);
}

float DevSlem::D2DEngine::Vector2::Distance(const Vector2& a, const Vector2& b) noexcept
{
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    return std::sqrt(dx * dx + dy * dy);
}

float DevSlem::D2DEngine::Vector2::Dot(const Vector2& a, const Vector2& b) noexcept
{
    return a.x * b.x + a.y * b.y;
}

Vector2 DevSlem::D2DEngine::Vector2::Lerp(const Vector2& start, const Vector2& end, float t) noexcept
{
    return start + (end - start) * t;
}

DevSlem::D2DEngine::Vector2::Vector2()
{
    x = 0; y = 0;
}

DevSlem::D2DEngine::Vector2::Vector2(float x, float y)
{
    this->x = x;
    this->y = y;
}

DevSlem::D2DEngine::Vector2::Vector2(const D2D1_POINT_2F& point)
{
    x = point.x;
    y = point.y;
}

DevSlem::D2DEngine::Vector2::Vector2(const D2D1_SIZE_F& size)
{
    x = size.width;
    y = size.height;
}

float DevSlem::D2DEngine::Vector2::Magnitude() const
{
    return std::sqrt(x * x + y * y);
}

Vector2 DevSlem::D2DEngine::Vector2::Normalized() const
{
    return *this / this->Magnitude();
}

Vector2 DevSlem::D2DEngine::Vector2::Abs() const
{
    return Vector2(std::abs(x), std::abs(y));
}

Vector2 DevSlem::D2DEngine::Vector2::Direction(const Vector2& destination) const
{
    return (destination - (*this)).Normalized();
}

void DevSlem::D2DEngine::Vector2::Set(float x, float y)
{
    this->x = x; this->y = y;
}

Vector2 DevSlem::D2DEngine::Vector2::operator+(const Vector2& v) const noexcept
{
    return Vector2(x + v.x, y + v.y);
}

Vector2 DevSlem::D2DEngine::Vector2::operator-(const Vector2& v) const noexcept
{
    return Vector2(x - v.x, y - v.y);
}

Vector2 DevSlem::D2DEngine::Vector2::operator*(const Vector2& v) const noexcept
{
    return Vector2(x * v.x, y * v.y);
}

Vector2 DevSlem::D2DEngine::Vector2::operator*(float value) const noexcept
{
    return Vector2(x * value, y * value);
}

Vector2 DevSlem::D2DEngine::Vector2::operator/(const Vector2& v) const noexcept
{
    return Vector2(x / v.x, y / v.y);
}

Vector2 DevSlem::D2DEngine::Vector2::operator/(float value) const noexcept
{
    return Vector2(x / value, y / value);
}

Vector2 DevSlem::D2DEngine::Vector2::operator+() const noexcept
{
    return Vector2(x, y);
}

Vector2 DevSlem::D2DEngine::Vector2::operator-() const noexcept
{
    return Vector2(-x, -y);
}

bool DevSlem::D2DEngine::Vector2::operator==(const Vector2& v) const noexcept
{
    return x == v.x && y == v.y;
}

bool DevSlem::D2DEngine::Vector2::operator!=(const Vector2& v) const noexcept
{
    return x != v.x || y != v.y;
}

Vector2& DevSlem::D2DEngine::Vector2::operator+=(const Vector2& v) noexcept
{
    x += v.x;
    y += v.y;
    return *this;
}

Vector2& DevSlem::D2DEngine::Vector2::operator-=(const Vector2& v) noexcept
{
    x -= v.x;
    y -= v.y;
    return *this;
}

Vector2& DevSlem::D2DEngine::Vector2::operator*=(const Vector2& v) noexcept
{
    x *= v.x;
    y *= v.y;
    return *this;
}

Vector2& DevSlem::D2DEngine::Vector2::operator*=(float value) noexcept
{
    x *= value;
    y *= value;
    return *this;
}

Vector2& DevSlem::D2DEngine::Vector2::operator/=(const Vector2& v) noexcept
{
    x /= v.x;
    y /= v.y;
    return *this;
}

Vector2& DevSlem::D2DEngine::Vector2::operator/=(float value) noexcept
{
    x /= value;
    y /= value;
    return *this;
}

DevSlem::D2DEngine::Vector2::operator D2D1_POINT_2F() const noexcept
{
    return { x, y };
}

DevSlem::D2DEngine::Vector2::operator D2D1_SIZE_F() const noexcept
{
    return { x, y };
}

Vector2 DevSlem::D2DEngine::operator*(float value, const Vector2& v) noexcept
{
    return Vector2(value * v.x, value * v.y);
}

Vector2 DevSlem::D2DEngine::operator/(float value, const Vector2& v) noexcept
{
    return Vector2(value / v.x, value / v.y);
}
