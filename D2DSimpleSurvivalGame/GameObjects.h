#pragma once
#include <d2d1.h>
#include <vector>

#include "D2DEngine.h"
#include "Entity.h"
#include "FiniteStateMachine.h"

namespace DevSlem::D2DSimpleSurvivalGame
{
	class BitmapRendererGameObject abstract : public DevSlem::D2DEngine::GameObject
	{
	protected:
		DevSlem::D2DEngine::BitmapRenderer renderer;
		ID2D1Bitmap* image;

		virtual ID2D1Bitmap* GetBitmapImage(IWICImagingFactory* WICFactory, ID2D1RenderTarget* renderTarget) = 0;

	public:
		BitmapRendererGameObject();

		virtual void CreateDeviceResources();
		virtual void ReleaseDeviceResources() override;
	};

	class Background : public BitmapRendererGameObject
	{
	private:
		DevSlem::D2DEngine::Rigidbody rigidbody;
		DevSlem::D2DEngine::RectCollider collider;

	protected:
		// BitmapRendererGameObject을(를) 통해 상속됨
		virtual ID2D1Bitmap* GetBitmapImage(IWICImagingFactory* WICFactory, ID2D1RenderTarget* renderTarget) override;

	public:
		Background();
	};

	class CannonBulletAttack : public DevSlem::D2DEngine::Component, public DevSlem::D2DEngine::ICollision
	{
	private:
		DevSlem::D2DEngine::RectCollider* collider;
		Entity* target;
		DevSlem::D2DEngine::Sound attackSound;

	public:
		CannonBulletAttack(DevSlem::D2DEngine::GameObject* gameObject, DevSlem::D2DEngine::RectCollider* collider) : Component(gameObject)
		{
			this->collider = collider;
			target = nullptr;
			canAttack = true;

			collider->AddListener(this);
		}
		~CannonBulletAttack()
		{
			collider->RemoveListener(this);
		}

		// ICollision을(를) 통해 상속됨
		virtual void OnCollision(DevSlem::D2DEngine::Collision& collision) override;
		void SetTarget(Entity* target) { this->target = target; }
		//void SetAttackSound(Sound* sound) { attackSound = sound; }
		DevSlem::D2DEngine::Sound* AttackSound() { return &attackSound; }

		bool canAttack;
	};

	class CannonBullet : public BitmapRendererGameObject
	{
	private:
		DevSlem::D2DEngine::Rigidbody rigidbody;
		DevSlem::D2DEngine::RectCollider collider;

	protected:
		virtual void OnActiveSet(bool isActive) override;
		// BitmapRendererGameObject을(를) 통해 상속됨
		virtual ID2D1Bitmap* GetBitmapImage(IWICImagingFactory* WICFactory, ID2D1RenderTarget* renderTarget) override;

	public:
		CannonBulletAttack attacker;

		CannonBullet();

		DevSlem::D2DEngine::Rigidbody* Rigidbody() { return &rigidbody; }
		DevSlem::D2DEngine::RectCollider* Collider() { return &collider; }

		virtual void CreateDeviceIndependentResources() override;
	};


	class CannonFire : public DevSlem::D2DEngine::Component, DevSlem::D2DEngine::IUpdate
	{
	private:
		DevSlem::D2DEngine::Timer timer;
		CannonBullet* bullet;
		DevSlem::D2DEngine::Sound fireSound;

	public:
		DevSlem::D2DEngine::Vector2 firePosition;

		CannonFire(DevSlem::D2DEngine::GameObject* gameObject);

		// IUpdate을(를) 통해 상속됨
		virtual void Update(float deltaTime) override;
		void SetCannonBullet(CannonBullet* bullet) { this->bullet = bullet; }
		DevSlem::D2DEngine::Sound* FireSound() { return &fireSound; }
	};

	class Cannon : public BitmapRendererGameObject
	{
	protected:
		// BitmapRendererGameObject을(를) 통해 상속됨
		virtual ID2D1Bitmap* GetBitmapImage(IWICImagingFactory* WICFactory, ID2D1RenderTarget* renderTarget) override;

	public:
		CannonFire cannonFire;

		Cannon();

		virtual void CreateDeviceIndependentResources() override;
	};

	class EntityComponent : public Entity, public DevSlem::D2DEngine::Component
	{
	public:
		EntityComponent(DevSlem::D2DEngine::GameObject* gameObject, float maxHealth) : Component(gameObject), Entity(maxHealth) {}
	};

	class PlayerInput : public DevSlem::D2DEngine::Component, public DevSlem::D2DEngine::IUpdate, public DevSlem::D2DEngine::IFixedUpdate
	{
	private:
		bool stop;
		DevSlem::D2DEngine::Rigidbody* rigidbody;
		D2D1_RECT_F movingArea;

	public:
		PlayerInput(DevSlem::D2DEngine::GameObject* gameObject, DevSlem::D2DEngine::Rigidbody* rigidbody) : Component(gameObject)
		{
			this->rigidbody = rigidbody;
			stop = false;
			movingArea = D2D1::RectF(300, 300, 980, 800);
		}

		// IUpdate을(를) 통해 상속됨
		virtual void Update(float deltaTime) override;

		// IFixedUpdate을(를) 통해 상속됨
		virtual void FixedUpdate(float fixedDeltaTime) override;
	};

	class Player : public BitmapRendererGameObject
	{
	public:
		DevSlem::D2DEngine::Rigidbody rigidbody;
		DevSlem::D2DEngine::RectCollider collider;
		EntityComponent entity;
		PlayerInput inputController;


	protected:
		// BitmapRendererGameObject을(를) 통해 상속됨
		virtual ID2D1Bitmap* GetBitmapImage(IWICImagingFactory* WICFactory, ID2D1RenderTarget* renderTarget) override;

	public:
		Player();
	};

	class TrapAttack : public DevSlem::D2DEngine::Component, public DevSlem::D2DEngine::ICollision, public DevSlem::D2DEngine::IUpdate
	{
	private:
		DevSlem::D2DEngine::RectCollider* collider;
		DevSlem::D2DEngine::Timer timer;
		Entity* target;
		bool canAttack;

	public:
		TrapAttack(DevSlem::D2DEngine::GameObject* gameObject, DevSlem::D2DEngine::RectCollider* collider);
		~TrapAttack()
		{
			collider->RemoveListener(this);
		}

		// ICollision을(를) 통해 상속됨
		virtual void OnCollision(DevSlem::D2DEngine::Collision& collision) override;

		// IUpdate을(를) 통해 상속됨
		virtual void Update(float deltaTime) override;
		void SetTarget(Entity* target) { this->target = target; }
	};

	class Trap : public DevSlem::D2DEngine::GameObject
	{
	public:
		DevSlem::D2DEngine::Rigidbody rigidbody;
		DevSlem::D2DEngine::RectCollider collider;
		TrapAttack attacker;
		DevSlem::D2DEngine::GeometryRenderer fillRenderer;
		DevSlem::D2DEngine::GeometryRenderer drawRenderer;

		ID2D1PathGeometry* trapGeometry;
		ID2D1SolidColorBrush* drawBrush;
		ID2D1SolidColorBrush* fillBrush;

		HRESULT CreateSharpTrap(const D2D1_RECT_F& rect, int count, ID2D1PathGeometry** trap);

	public:
		Trap();
		~Trap()
		{
			drawRenderer.SetGeometry(nullptr);
			SAFE_RELEASE(trapGeometry);
		}

		virtual void CreateDeviceIndependentResources() override;
		virtual void ReleaseDeviceIndependentResources() override;
		virtual void CreateDeviceResources() override;
		virtual void ReleaseDeviceResources() override;

	};

	class EnemyComponent : public EntityComponent, public DevSlem::D2DEngine::IUpdate, public DevSlem::D2DEngine::ICollision
	{
	public:
		enum State
		{
			Idle,
			Move,
			Follow,
			Attack,
			Die
		};

		enum Event
		{
			FindTarget,
			LostTarget,
			CanAttack,
			OutOfAttack,
			StopMove,
			Dubious,
			Dead,
			Reset
		};

	private:
		DevSlem::D2DEngine::Rigidbody* rigidbody;
		DevSlem::D2DEngine::RectCollider* collider;
		EntityComponent* target;
		FiniteStateMachine fsm;  // Finite State Machine
		DevSlem::D2DEngine::Timer timer;
		DevSlem::D2DEngine::Timer targetAttackTimer;
		float deltaTime;
		D2D1_RECT_F movingArea;

		DevSlem::D2DEngine::Sound attackSound;

	public:
		float viewRange;
		float attackRange;
		float minSpeed;
		float maxSpeed;
		float dashPower;
		float attackDamage;

	private:
		void InitializeFSM();
		void UpdateFSM();

		bool TryTriggerDie();
		bool IsTargetVisible();
		bool CanAttackTarget();
		void SetMoveSetting();
		void MoveToTarget();

		void ActionIdle();
		void ActionMove();
		void ActionFollow();
		void ActionAttack();
		void ActionDie();

		void CheckClicked();

	public:
		EnemyComponent(DevSlem::D2DEngine::GameObject* gameObject, DevSlem::D2DEngine::Rigidbody* rigidbody, DevSlem::D2DEngine::RectCollider* collider, float maxHealth);
		~EnemyComponent();

		// IUpdate을(를) 통해 상속됨
		virtual void Update(float deltaTime) override;

		void SetTarget(EntityComponent* target) { this->target = target; }
		//void SetAttackSound(Sound* sound) { this->attackSound = sound; }
		DevSlem::D2DEngine::Sound* AttackSound() { return &attackSound; }

		State CurrentState() const { return (State)fsm.GetCurrentState(); }

		// ICollision을(를) 통해 상속됨
		virtual void OnCollision(DevSlem::D2DEngine::Collision& collision) override;
	};

	class EnemyRenderer : public DevSlem::D2DEngine::Renderer
	{
	private:
		EnemyComponent* enemy;

	public:
		EnemyRenderer(DevSlem::D2DEngine::GameObject* gameObject, EnemyComponent* enemy) : Renderer(gameObject)
		{
			this->enemy = enemy;
		}

		// Renderer을(를) 통해 상속됨
		virtual void Render() override;
	};

	class Enemy : public DevSlem::D2DEngine::GameObject
	{
	private:
		DevSlem::D2DEngine::Rigidbody rigidbody;
		DevSlem::D2DEngine::RectCollider collider;
		EnemyComponent enemy;
		EnemyRenderer renderer;

	public:
		Enemy();

		EnemyComponent* EnemyComponent() { return &enemy; }

		virtual void CreateDeviceIndependentResources() override;
	};

	class GoodguyComponent : public DevSlem::D2DEngine::Component, public DevSlem::D2DEngine::IUpdate
	{
	public:
		enum State
		{
			Idle,
			Run
		};

		enum Event
		{
			FindEnemy,
			LostEnemy
		};

	private:
		FiniteStateMachine fsm;
		DevSlem::D2DEngine::Rigidbody* rigidbody;
		float deltaTime;
		std::vector<Enemy*> enemies;
		D2D1_RECT_F movingArea;

		void InitializeFSM();
		void UpdateFSM();

		void ActionIdle();
		void ActionRun();

	public:
		float viewRange;
		float speed;


	public:
		GoodguyComponent(DevSlem::D2DEngine::GameObject* gameObject, DevSlem::D2DEngine::Rigidbody* rigidbody);

		// IUpdate을(를) 통해 상속됨
		virtual void Update(float deltaTime) override;

		void AddEnemy(Enemy* enemy);

		State CurrentState() const { return (State)fsm.GetCurrentState(); }
	};

	class GoodguyRenderer : public DevSlem::D2DEngine::Renderer
	{
	private:
		GoodguyComponent* goodguy;

	public:
		GoodguyRenderer(DevSlem::D2DEngine::GameObject* gameObject, GoodguyComponent* goodguy);

		// Renderer을(를) 통해 상속됨
		virtual void Render() override;
	};

	class Goodguy : public DevSlem::D2DEngine::GameObject
	{
	private:
		DevSlem::D2DEngine::Rigidbody rigidbody;
		GoodguyComponent goodguy;
		GoodguyRenderer renderer;

	public:
		Goodguy();

		GoodguyComponent* GoodguyComponent() { return &goodguy; }
	};

	class GameObjects
	{
	public:
		Background background;
		Cannon leftCannon;
		Cannon rightCannon;
		Trap leftTrap;
		Trap rightTrap;
		Player player;
		CannonBullet leftCannonBullet;
		CannonBullet rightCannonBullet;
		Enemy enemy1;
		Enemy enemy2;
		Goodguy goodguy;

	public:
		GameObjects();

		void CreateDeviceIndependentResources();
		void CreateDeviceResources();
		void ReleaseDeviceResources();
		void ReleaseDeviceIndependentResources();

	private:
		std::vector<DevSlem::D2DEngine::GameObject*> gameObjects;
		void AddGameObjects();
		void InitializeGameObjects();
	};

}
