#pragma once
#include <algorithm>

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

namespace DevSlem::D2DSimpleSurvivalGame
{
	class Entity
	{
	protected:
		float health;
		float maxHealth;
		bool isDead;

		virtual void Die() { this->isDead = true; }
	public:
		Entity(float maxHealth) : Entity(maxHealth, maxHealth) {}

		Entity(float health, float maxHealth)
		{
			SetMaxHealth(maxHealth);
			SetHealth(health);
		}

		virtual void SetHealth(float health)
		{
			this->health = std::clamp(health, 0.f, maxHealth);
			if (this->health <= 0)
			{
				if (!this->isDead)
					Die();
			}
			else
			{
				this->isDead = false;
			}
		}
		virtual void SetMaxHealth(float maxHealth) { this->maxHealth = max(0, maxHealth); }
		void AddDamage(float damage)
		{
			SetHealth(Health() - damage);
		}
		void Heal(float value)
		{
			SetHealth(Health() + value);
		}
		float Health() const { return health; }
		float MaxHealth() const { return maxHealth; }
		bool IsDead() const { return isDead; }
	};

}
