#pragma once
#include <Windows.h>
#include <map>
#include <functional>

#define	UNDEFINED -1

namespace DevSlem::D2DSimpleSurvivalGame
{
	typedef int State;
	typedef int Event;

	class Transition
	{
	private:
		// Field
		int state;
		std::map<State, Event> transitions;

	public:
		// Constructor
		Transition(State state) { this->state = state; }

		// Getter
		State ResultState(Event inputEvent) const;
		int TransitionCount() const;
		State State() const;

		// Method
		void AddTransition(Event inputEvent, DevSlem::D2DSimpleSurvivalGame::State resultState);
		void RemoveTransition(Event inputEvent);
	};

	class FiniteStateMachine
	{
	private:
		// Field
		std::map<State, Transition*> states; // (key, value) = (state, transition)
		State currentState;

	public:
		// Constructor
		FiniteStateMachine();
		~FiniteStateMachine();

		// Getter
		State GetCurrentState() const;

		// Method
		void AddTransition(State state, Event inputEvent, State resultState);
		void RemoveTransition(State state, Event inputEvent);
		void TriggerEvent(Event inputEvent);
		void SetCurrentState(State currentState);
	};
}
