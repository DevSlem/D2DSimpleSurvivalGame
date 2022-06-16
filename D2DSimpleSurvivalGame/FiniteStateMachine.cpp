#include "FiniteStateMachine.h"

using namespace DevSlem::D2DSimpleSurvivalGame;

FiniteStateMachine::FiniteStateMachine()
{
	currentState = UNDEFINED;
}

FiniteStateMachine::~FiniteStateMachine()
{
	for (auto state : states)
	{
		delete state.second;
	}
	states.clear();
}

void FiniteStateMachine::AddTransition(State state, Event inputEvent, State resultState)
{
	auto iter = states.find(state);
	if (iter == states.end())
		states[state] = new Transition(state);

	states[state]->AddTransition(inputEvent, resultState);
}

void FiniteStateMachine::RemoveTransition(State state, Event inputEvent)
{
	auto iter = states.find(state);
	if (iter == states.end())
		return;

	auto transition = iter->second;
	transition->RemoveTransition(inputEvent);
	if (transition->TransitionCount() == 0)
	{
		delete transition;
		states.erase(iter);
	}
}

void FiniteStateMachine::TriggerEvent(Event inputEvent)
{
	if (currentState == UNDEFINED)
		return;

	int resultState = states[currentState]->ResultState(inputEvent);
	if (resultState == UNDEFINED)
		return;
	currentState = resultState;
}

State DevSlem::D2DSimpleSurvivalGame::FiniteStateMachine::GetCurrentState() const
{
	return currentState;
}

void FiniteStateMachine::SetCurrentState(State currentState)
{
	auto iter = states.find(currentState);
	if (iter == states.end())
		return;

	this->currentState = currentState;
}

State DevSlem::D2DSimpleSurvivalGame::Transition::State() const
{
	return state;
}

int DevSlem::D2DSimpleSurvivalGame::Transition::TransitionCount() const
{
	return transitions.size();
}

void DevSlem::D2DSimpleSurvivalGame::Transition::AddTransition(Event inputEvent, DevSlem::D2DSimpleSurvivalGame::State resultState)
{
	transitions[inputEvent] = resultState;
}

void DevSlem::D2DSimpleSurvivalGame::Transition::RemoveTransition(Event inputEvent)
{
	transitions.erase(inputEvent);
}

DevSlem::D2DSimpleSurvivalGame::State DevSlem::D2DSimpleSurvivalGame::Transition::ResultState(Event inputEvent) const
{
	auto iter = transitions.find(inputEvent);
	if (iter == transitions.end())
		return UNDEFINED;

	return transitions.at(inputEvent);
}
