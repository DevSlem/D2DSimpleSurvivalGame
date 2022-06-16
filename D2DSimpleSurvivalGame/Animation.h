#pragma once

#include <cmath>
#include <algorithm>

/* =================================================================================== */
/* Animations */
/* =================================================================================== */

namespace DevSlem::D2DSimpleSurvivalGame
{
	template <typename T>
	class Animation abstract
	{
	public:
		Animation(T start, T end, T duration) :
			start(start), end(end), duration(duration)
		{
		}

		void SetStart(T start) { this->start = start; }
		T GetStart() const { return start; }
		void SetEnd(T end) { this->end = end; }
		T GetEnd() const { return end; }
		void SetDuration(T duration) { this->duration = duration; }
		T GetDuration() const { return duration; }
		T GetValue(T time) { return ComputeValue(std::clamp(time, (T)0, duration)); }

	protected:
		virtual T ComputeValue(T time) = 0;

	protected:
		T start;
		T end;
		T duration;
	};

	template <typename T>
	class AnimationLinear : public Animation<T>
	{
	public:
		AnimationLinear(T start, T end, T duration) :
			Animation<T>(start, end, duration) {}

	protected:
		virtual T ComputeValue(T time) override
		{
			return (T)(this->start + ((this->end - this->start) * (time / this->duration)));
		}
	};

	template <typename T>
	class AnimationEaseOut : public Animation<T>
	{
	public:
		AnimationEaseOut(T start, T end, T duration) :
			Animation<T>(start, end, duration) {}

	protected:
		virtual T ComputeValue(T time) override
		{
			return (T)(this->start + (this->end - this->start) * (-std::pow(2, -10 * time / this->duration) + 1));
		}
	};

	template <typename T>
	class AnimationEaseIn : public Animation<T>
	{
	public:
		AnimationEaseIn(T start, T end, T duration) :
			Animation<T>(start, end, duration) {}

	protected:
		virtual T ComputeValue(T time) override
		{
			return (T)(this->start + (this->end - this->start) * std::pow(2, 10 * ((time / this->duration) - 1)));
		}
	};

	template <typename T>
	class AnimationEaseInOut : public Animation<T>
	{
	public:
		AnimationEaseInOut(T start, T end, T duration) :
			Animation<T>(start, end, duration) {}

	protected:
		virtual T ComputeValue(T time) override
		{
			//compute the current time relative to the midpoint
			time = time / (this->duration / 2);

			//if we haven't reached the midpoint, we want to do the ease-in portion
			if (time < 1)
				return (T)(this->start + (this->end - this->start) / 2 * std::pow(2, 10 * (time - 1)));

			//otherwise, do the ease-out portion
			return (T)(this->start + (this->end - this->start) / 2 * (-std::pow(2, -10 * --time) + 2));
		}
	};
}
