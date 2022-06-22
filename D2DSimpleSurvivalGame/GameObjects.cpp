#include "GameObjects.h"
#include "Input.h"
#include "D2DResource.h"

using namespace DevSlem::D2DSimpleSurvivalGame;
using namespace DevSlem::D2DEngine;

BitmapRendererGameObject::BitmapRendererGameObject() : renderer(this)
{
	image = nullptr;
}

void BitmapRendererGameObject::CreateDeviceResources()
{
	IWICImagingFactory* WICFactory = D2DResource::WICImagingFactory();
	ID2D1RenderTarget* renderTarget = D2DResource::RenderTarget();

	if (WICFactory == nullptr || renderTarget == nullptr)
		return;

	image = GetBitmapImage(WICFactory, renderTarget);
	if (image == nullptr)
		return;

	renderer.SetBitmap(image);
}

void BitmapRendererGameObject::ReleaseDeviceResources()
{
	SAFE_RELEASE(image);
}

ID2D1Bitmap* Background::GetBitmapImage(IWICImagingFactory* WICFactory, ID2D1RenderTarget* renderTarget)
{
	ID2D1Bitmap* temp;

	auto size = renderTarget->GetSize();

	auto hr = ::LoadBitmapFromResource(
		renderTarget,
		WICFactory,
		L"BACKGROUND",
		L"IMAGE",
		(UINT)size.width, (UINT)size.height,
		&temp
	);

	if (SUCCEEDED(hr))
	{
		this->Transform()->position = Vector2(size) / 2.0f;
		return temp;
	}

	return nullptr;
}

Background::Background() : rigidbody(this), collider(this, &rigidbody)
{
	rigidbody.gravityScale = 0;
	rigidbody.mass = 100;
	rigidbody.freezePositionX = true;
	rigidbody.freezePositionY = true;

	collider.SetOffset(Vector2(0, 285));
	collider.SetSize(Vector2(1280, 50));
	collider.physicsMaterial.SetBounciness(0.8f);
	collider.physicsMaterial.SetFriction(0.4f);
	collider.SetBoundsRendered(true);

	this->SetName(L"Background");
}


CannonBullet::CannonBullet() : rigidbody(this), collider(this, &rigidbody), attacker(this, &collider)
{
	renderer.SetSize(Vector2(50, 50));
	collider.SetSize(Vector2(50, 50));
	collider.SetBoundsRendered(true);
	collider.physicsMaterial.SetBounciness(0.8);
	rigidbody.gravityScale = 100;
	rigidbody.mass = 200.0f;
	this->SetName(L"Cannon Bullet");
}

void CannonBullet::CreateDeviceIndependentResources()
{
	attacker.AttackSound()->CreateSound(L"./assets/sounds/heavy-attack.wav");
}


void CannonBullet::OnActiveSet(bool isActive)
{
	rigidbody.SetEnabled(isActive);
	collider.SetEnabled(isActive);
	renderer.SetEnabled(isActive);
}

ID2D1Bitmap* CannonBullet::GetBitmapImage(IWICImagingFactory* WICFactory, ID2D1RenderTarget* renderTarget)
{
	ID2D1Bitmap* temp;

	auto hr = ::LoadBitmapFromResource(
		renderTarget,
		WICFactory,
		L"CANNON_BULLET",
		L"IMAGE",
		200, 200,
		&temp
	);

	return SUCCEEDED(hr) ? temp : nullptr;
}

Cannon::Cannon() : cannonFire(this)
{
	renderer.SetSize(Vector2(100, 100));
	//renderer.SetBoundsRendered(true);
}

void Cannon::CreateDeviceIndependentResources()
{
	cannonFire.FireSound()->CreateSound(L"./assets/sounds/cannon-fire.wav");
}

ID2D1Bitmap* Cannon::GetBitmapImage(IWICImagingFactory* WICFactory, ID2D1RenderTarget* renderTarget)
{
	ID2D1Bitmap* temp;

	auto hr = ::LoadBitmapFromResource(
		renderTarget,
		WICFactory,
		L"CANNON",
		L"IMAGE",
		200, 200,
		&temp
	);

	return SUCCEEDED(hr) ? temp : nullptr;
}

ID2D1Bitmap* Player::GetBitmapImage(IWICImagingFactory* WICFactory, ID2D1RenderTarget* renderTarget)
{
	ID2D1Bitmap* temp;

	auto hr = ::LoadBitmapFromResource(
		renderTarget,
		WICFactory,
		L"GHOST",
		L"IMAGE",
		200, 200,
		&temp
	);

	return SUCCEEDED(hr) ? temp : nullptr;
}

Player::Player() : 
	rigidbody(this), 
	collider(this, &rigidbody), 
	entity(this, 100), 
	inputController(this, &rigidbody)
{
	this->Transform()->position = Vector2(500, 500);
	collider.SetBoundsRendered(true);
	collider.SetOffset(Vector2(-2, -7));
	collider.SetSize(Vector2(40, 50));
	renderer.SetSize(Vector2(100, 100));
	rigidbody.gravityScale = 0;

	this->SetName(L"Player");
}

void PlayerInput::Update(float deltaTime)
{
	if (!stop)
		stop = Input::Space();
}

void PlayerInput::FixedUpdate(float fixedDeltaTime)
{
	if (stop)
	{
		rigidbody->SetVelocity(Vector2::Zero());
	}
	else
	{
		//rigidbody->AddForce(Vector2(Input::Horizontal() * 1000, -Input::Vertical() * 500));
		rigidbody->SetVelocity(Vector2(Input::Horizontal() * 500, -Input::Vertical() * 500));
	}

	auto pos = this->rigidbody->Position();
	// 이동 반경 제한
	this->rigidbody->SetPosition(Vector2(std::clamp(pos.x, movingArea.left, movingArea.right), std::clamp(pos.y, movingArea.top, movingArea.bottom)));

	stop = false;
}

void CannonBulletAttack::OnCollision(Collision& collision)
{
	if (canAttack && target != nullptr && collision.Collider()->GameObject()->Name() == L"Player")
	{
		target->SetHealth(target->Health() - 40);
		canAttack = false;
		attackSound.Play();
	}
}

Trap::Trap() : rigidbody(this), collider(this, &rigidbody), attacker(this, &collider), fillRenderer(this), drawRenderer(this)
{
	trapGeometry = nullptr;
	drawBrush = nullptr;
	fillBrush = nullptr;

	rigidbody.gravityScale = 0;
	rigidbody.freezePositionX = true;
	rigidbody.freezePositionY = true;
	collider.SetBoundsRendered(true);
	collider.SetSize(Vector2(100, 50));
	fillRenderer.SetDrawingMode(DrawingMode::Fill);
}


void Trap::CreateDeviceIndependentResources()
{
	// width 100
	// height 50
	auto hr = CreateSharpTrap(D2D1::RectF(-50, -25, 50, 25), 3, &trapGeometry);
	if (SUCCEEDED(hr))
	{
		fillRenderer.SetGeometry(trapGeometry);
		drawRenderer.SetGeometry(trapGeometry);
	}
}

void Trap::ReleaseDeviceIndependentResources()
{
	SAFE_RELEASE(trapGeometry);
}

void Trap::CreateDeviceResources()
{
	auto rt = D2DResource::RenderTarget();
	if (rt == nullptr)
		return;

	auto hr = rt->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black, 1.f), &drawBrush);
	hr = rt->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White, 1.f), &fillBrush);
	if (SUCCEEDED(hr))
	{
		fillRenderer.SetGeometryBrush(fillBrush);
		drawRenderer.SetGeometryBrush(drawBrush);
	}
}

void Trap::ReleaseDeviceResources()
{
	SAFE_RELEASE(drawBrush);
	SAFE_RELEASE(fillBrush);
	fillRenderer.SetGeometryBrush(nullptr);
	drawRenderer.SetGeometryBrush(nullptr);
}

HRESULT Trap::CreateSharpTrap(const D2D1_RECT_F& rect, int count, ID2D1PathGeometry** trap)
{
	if (count <= 0)
		return E_FAIL;

	auto factory = D2DResource::D2DFactory();
	if (factory == nullptr)
		return E_FAIL;

	HRESULT hr = factory->CreatePathGeometry(trap);
	if (SUCCEEDED(hr))
	{
		ID2D1GeometrySink* sink = nullptr;
		hr = (*trap)->Open(&sink);
		if (SUCCEEDED(hr))
		{
			sink->SetFillMode(D2D1_FILL_MODE_WINDING);
			sink->BeginFigure(D2D1::Point2F(rect.left, rect.bottom), D2D1_FIGURE_BEGIN_FILLED);

			float interval = (rect.right - rect.left) / count;
			std::vector<D2D1_POINT_2F> points;
			for (int i = 0; i < count; i++)
			{
				points.push_back(D2D1::Point2F(rect.left + interval / 2 + interval * i, rect.top));
				points.push_back(D2D1::Point2F(rect.left + interval * (i + 1), rect.bottom));
			}
			sink->AddLines(points.data(), points.size());
			sink->EndFigure(D2D1_FIGURE_END_CLOSED);
		}
		hr = sink->Close();
		SAFE_RELEASE(sink);
	}

	return hr;
}

TrapAttack::TrapAttack(::GameObject* gameObject, RectCollider* collider) : Component(gameObject), timer(1.0f)
{
	this->collider = collider;
	collider->AddListener(this);
	canAttack = true;
	target = nullptr;
}

void TrapAttack::OnCollision(Collision& collision)
{
	if (!canAttack || target == nullptr || collision.Collider()->GameObject()->Name() != L"Player")
		return;

	target->SetHealth(target->Health() - 10.0f);
	canAttack = false;
	timer.Initialize();
}

void TrapAttack::Update(float deltaTime)
{
	canAttack = timer.Tick(deltaTime);
}

CannonFire::CannonFire(::GameObject* gameObject) : Component(gameObject), timer(4)
{
	bullet = nullptr;
}

void CannonFire::Update(float deltaTime)
{
	if (bullet == nullptr)
		return;

	if (timer.Tick(deltaTime))
	{
		bullet->Rigidbody()->SetVelocity(Vector2::Zero());
		bullet->Transform()->position = firePosition;
		auto direaction = (firePosition - Transform()->position).Normalized();
		bullet->Rigidbody()->AddForce(
			bullet->Rigidbody()->mass * Vector2(direaction.x * Random::Range(10000, 30000), direaction.y * Random::Range(30000, 50000))
		);
		bullet->attacker.canAttack = true;
		timer.Initialize();

		fireSound.Play();
	}
}

EnemyComponent::EnemyComponent(::GameObject* gameObject, Rigidbody* rigidbody, RectCollider* collider, float maxHealth) : 
	EntityComponent(gameObject, maxHealth), timer(1), targetAttackTimer(1)
{
	this->rigidbody = rigidbody;
	this->collider = collider;
	collider->AddListener(this);

	target = nullptr;
	deltaTime = 0;
	movingArea = D2D1::RectF(300, 300, 980, 650);

	viewRange = 0;
	attackRange = 0;
	minSpeed = 0;
	maxSpeed = 0;
	dashPower = 0;
	attackDamage = 0;

	InitializeFSM();
}

EnemyComponent::~EnemyComponent()
{
	collider->RemoveListener(this);
}

void EnemyComponent::InitializeFSM()
{
	fsm.AddTransition(State::Idle, Event::Dubious, State::Move);
	fsm.AddTransition(State::Idle, Event::FindTarget, State::Follow);
	fsm.AddTransition(State::Idle, Event::CanAttack, State::Attack);
	fsm.AddTransition(State::Idle, Event::Dead, State::Die);
	fsm.AddTransition(State::Move, Event::StopMove, State::Idle);
	fsm.AddTransition(State::Move, Event::FindTarget, State::Follow);
	fsm.AddTransition(State::Move, Event::Dead, State::Die);
	fsm.AddTransition(State::Follow, Event::CanAttack, State::Attack);
	fsm.AddTransition(State::Follow, Event::LostTarget, State::Idle);
	fsm.AddTransition(State::Follow, Event::Dead, State::Die);
	fsm.AddTransition(State::Attack, Event::LostTarget, State::Idle);
	fsm.AddTransition(State::Attack, Event::OutOfAttack, State::Follow);
	fsm.AddTransition(State::Attack, Event::Dead, State::Die);
	fsm.AddTransition(State::Die, Event::Reset, State::Idle);

	fsm.SetCurrentState(State::Idle);
}

void EnemyComponent::UpdateFSM()
{
	switch (fsm.GetCurrentState())
	{
	case State::Idle:
	{
		ActionIdle();
		break;
	}
	case State::Move:
	{
		ActionMove();
		break;
	}
	case State::Follow:
	{
		ActionFollow();
		break;
	}
	case State::Attack:
	{
		ActionAttack();
		break;
	}
	case State::Die:
	{
		ActionDie();
		break;
	}
	default:
		break;
	}
}

bool EnemyComponent::TryTriggerDie()
{
	if (IsDead())
	{
		rigidbody->SetVelocity(Vector2::Zero());
		timer.SetDuration(5);
		fsm.TriggerEvent(Event::Dead);
		return true;
	}

	return false;
}

bool EnemyComponent::IsTargetVisible()
{
	return Vector2::Distance(this->Transform()->position, target->Transform()->position) <= viewRange;
}

bool EnemyComponent::CanAttackTarget()
{
	return Vector2::Distance(this->Transform()->position, target->Transform()->position) <= attackRange;
}


void EnemyComponent::SetMoveSetting()
{
	rigidbody->SetVelocity(Random::Range(minSpeed, maxSpeed) * Random::Direction());
}

void EnemyComponent::MoveToTarget()
{
	if (Vector2::Distance(rigidbody->Position(), target->Transform()->position) <= 5)
	{
		rigidbody->SetVelocity(Vector2::Zero());
	}
	else
	{
		auto direction = this->Transform()->position.Direction(target->Transform()->position);
		rigidbody->MovePosition(maxSpeed * direction * deltaTime);
	}
}

void EnemyComponent::ActionIdle()
{
	// 사망 체크
	if (TryTriggerDie())
		return;

	// 타겟 감지
	if (IsTargetVisible())
	{
		fsm.TriggerEvent(Event::FindTarget);
		return;
	}

	// 타이머가 끝나면 이동 상태로 변경
	if (timer.Tick(deltaTime))
	{
		timer.SetDuration(5); // 5초 동안 이동
		SetMoveSetting();
		fsm.TriggerEvent(Event::Dubious);
	}
}

void EnemyComponent::ActionMove()
{
	if (TryTriggerDie())
		return;

	if (IsTargetVisible())
	{
		fsm.TriggerEvent(Event::FindTarget);
		return;
	}

	if (timer.Tick(deltaTime))
	{
		timer.SetDuration(1); // 1초 동안 정지
		rigidbody->SetVelocity(Vector2::Zero());
		fsm.TriggerEvent(Event::StopMove);
	}
}

void EnemyComponent::ActionFollow()
{
	if (TryTriggerDie())
		return;

	if (!IsTargetVisible())
	{
		fsm.TriggerEvent(Event::LostTarget);
		return;
	}

	// 일단 이동 후 공격 시도
	MoveToTarget();

	if (CanAttackTarget())
	{
		timer.SetDuration(0);
		fsm.TriggerEvent(Event::CanAttack);
		return;
	}
}

void EnemyComponent::ActionAttack()
{
	if (TryTriggerDie())
		return;

	if (!IsTargetVisible())
	{
		fsm.TriggerEvent(Event::LostTarget);
		return;
	}

	if (!CanAttackTarget())
	{
		rigidbody->SetVelocity(Vector2::Zero());
		fsm.TriggerEvent(Event::OutOfAttack);
		return;
	}

	if (timer.Tick(deltaTime))
	{
		auto direction = this->Transform()->position.Direction(target->Transform()->position);
		rigidbody->SetVelocity(Vector2::Zero());
		rigidbody->AddForce(dashPower * direction);
		timer.SetDuration(1);
	}
}

void EnemyComponent::ActionDie()
{
	if (timer.Tick(deltaTime))
	{
		this->SetHealth(this->MaxHealth());
		timer.SetDuration(1);
		fsm.TriggerEvent(Event::Reset);
	}
}

void EnemyComponent::CheckClicked()
{
	if (Input::MouseLeftDown() && collider->Bounds().Contains(Input::MousePosition()))
	{
		this->AddDamage(1);
	}
}


void EnemyComponent::Update(float deltaTime)
{
	if (target == nullptr)
		return;

	this->deltaTime = deltaTime;
	UpdateFSM();
	
	auto lastPos = this->Transform()->position;
	this->Transform()->position.Set(std::clamp(lastPos.x, movingArea.left, movingArea.right), std::clamp(lastPos.y, movingArea.top, movingArea.bottom));
	
	targetAttackTimer.Tick(deltaTime);

	CheckClicked();
}

void EnemyComponent::OnCollision(Collision& collision)
{
	if (targetAttackTimer.TimeOver() && target != nullptr && collision.Collider()->GameObject()->Name() == L"Player")
	{
		target->AddDamage(attackDamage);
		targetAttackTimer.Initialize();
		attackSound.Play();
	}
}

void EnemyRenderer::Render()
{
	if (SolidBrush() == nullptr)
		return;

	auto solidBrush = SolidBrush();

	// Target 감지 영역과 공격 범위를 표시함
	D2D1_ELLIPSE viewCircle = { Vector2::Zero(), enemy->viewRange, enemy->viewRange};
	D2D1_ELLIPSE attackCircle = { Vector2::Zero(), enemy->attackRange, enemy->attackRange};

	solidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Blue));
	RenderTarget()->DrawEllipse(viewCircle, solidBrush);
	solidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
	RenderTarget()->DrawEllipse(attackCircle, solidBrush);

	// 현재 State에 따라 사각형의 색깔을 변경함
	switch (enemy->CurrentState())
	{
	case EnemyComponent::State::Idle:
	{
		solidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
		break;
	}
	case EnemyComponent::State::Move:
	{
		solidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Green));
		break;
	}
	case EnemyComponent::State::Follow:
	{
		solidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Yellow));
		break;
	}
	case EnemyComponent::State::Attack:
	{
		solidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
		break;
	}
	case EnemyComponent::State::Die:
	{
		solidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
		break;
	}
	default:
		solidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
		break;
	}

	RenderTarget()->FillRectangle(this->SizeBounds(), solidBrush);
}

Enemy::Enemy() : rigidbody(this), collider(this, &rigidbody, true), enemy(this, &rigidbody, &collider, 5), renderer(this, &enemy)
{
	// physics options
	rigidbody.gravityScale = 0;
	rigidbody.mass = 10;
	collider.physicsMaterial.SetFriction(0.4f);
	collider.physicsMaterial.SetBounciness(0.1f);
	collider.SetSize(Vector2(20, 20));
	collider.SetBoundsRendered(true);

	// enemy options
	enemy.attackRange = 100;
	enemy.viewRange = 250;
	enemy.minSpeed = 100;
	enemy.maxSpeed = 200;
	enemy.dashPower = 200000;
	enemy.attackDamage = 5;

	// rendering options
	renderer.SetSize(Vector2(20, 20));
}

void Enemy::CreateDeviceIndependentResources()
{
	enemy.AttackSound()->CreateSound(L"./assets/sounds/light-attack.wav");
}

GameObjects::GameObjects()
{
	AddGameObjects();
	InitializeGameObjects();
}

void GameObjects::CreateDeviceIndependentResources()
{
	for (auto gameObject : gameObjects)
	{
		gameObject->CreateDeviceIndependentResources();
	}
}

void GameObjects::CreateDeviceResources()
{
	for (auto gameObject : gameObjects)
	{
		gameObject->CreateDeviceResources();
	}
}

void GameObjects::ReleaseDeviceResources()
{
	for (auto gameObject : gameObjects)
	{
		gameObject->ReleaseDeviceResources();
	}
}

void GameObjects::ReleaseDeviceIndependentResources()
{
	for (auto gameObject : gameObjects)
	{
		gameObject->ReleaseDeviceIndependentResources();
	}
}

void GameObjects::AddGameObjects()
{
	gameObjects = {
		&background,
		&leftCannon,
		&rightCannon,
		&leftTrap,
		&rightTrap,
		&player,
		&leftCannonBullet,
		&rightCannonBullet,
		&enemy1,
		&enemy2,
		&goodguy
	};
}

void GameObjects::InitializeGameObjects()
{
	// Cannons
	leftCannon.Transform()->position = Vector2(100, 150);
	leftCannon.Transform()->scale.x = -1;
	rightCannon.Transform()->position = Vector2(1150, 150);

	// Cannon Bullets
	leftCannonBullet.Transform()->position.Set(160, 120);
	leftCannonBullet.attacker.SetTarget(&player.entity);
	rightCannonBullet.Transform()->position.Set(1090, 120);
	rightCannonBullet.attacker.SetTarget(&player.entity);

	// Cannon Fires
	leftCannon.cannonFire.SetCannonBullet(&leftCannonBullet);
	leftCannon.cannonFire.firePosition.Set(120, 125);
	rightCannon.cannonFire.SetCannonBullet(&rightCannonBullet);
	rightCannon.cannonFire.firePosition.Set(1090, 120);

	// Traps
	leftTrap.Transform()->position.Set(370, 575);
	rightTrap.Transform()->position.Set(800, 575);
	leftTrap.attacker.SetTarget(&player.entity);
	rightTrap.attacker.SetTarget(&player.entity);

	// Enemies
	enemy1.Transform()->position.Set(300, 300);
	enemy2.Transform()->position.Set(600, 600);
	enemy1.EnemyComponent()->SetTarget(&player.entity);
	enemy2.EnemyComponent()->SetTarget(&player.entity);

	// Goodguy
	goodguy.Transform()->position.Set(500, 500);
	goodguy.GoodguyComponent()->AddEnemy(&enemy1);
	goodguy.GoodguyComponent()->AddEnemy(&enemy2);
}

GoodguyComponent::GoodguyComponent(::GameObject* gameObject, Rigidbody* rigidbody) : Component(gameObject)
{
	deltaTime = 0;
	this->rigidbody = rigidbody;
	movingArea = D2D1::RectF(300, 300, 980, 650);

	InitializeFSM();
}

void GoodguyComponent::InitializeFSM()
{
	fsm.AddTransition(State::Idle, Event::FindEnemy, State::Run);
	fsm.AddTransition(State::Run, Event::LostEnemy, State::Idle);

	fsm.SetCurrentState(State::Idle);
}

void GoodguyComponent::UpdateFSM()
{
	switch (fsm.GetCurrentState())
	{
	case State::Idle:
	{
		ActionIdle();
		break;
	}
	case State::Run:
	{
		ActionRun();
		break;
	}
	default:
		break;
	}
}

void GoodguyComponent::ActionIdle()
{
	float distance = 1e10;
	for (auto enemy : enemies)
	{
		float temp = Vector2::Distance(this->Transform()->position, enemy->Transform()->position);
		distance = min(distance, temp);
	}
	if (distance <= viewRange)
	{
		fsm.TriggerEvent(Event::FindEnemy);
		return;
	}
}

void GoodguyComponent::ActionRun()
{
	float distance = 1e10;
	Enemy* tempEnemy = nullptr;
	for (auto enemy : enemies)
	{
		float temp = Vector2::Distance(this->Transform()->position, enemy->Transform()->position);
		if (temp < distance)
		{
			distance = temp;
			tempEnemy = enemy;
		}
	}

	if (distance <= viewRange)
	{
		auto direction = tempEnemy->Transform()->position.Direction(this->Transform()->position);
		rigidbody->SetVelocity(speed * direction);
	}
	else
	{
		rigidbody->SetVelocity(Vector2::Zero());
		fsm.TriggerEvent(Event::LostEnemy);
	}
}

void GoodguyComponent::Update(float deltaTime)
{
	this->deltaTime = deltaTime;
	UpdateFSM();

	auto lastPos = this->Transform()->position;
	this->Transform()->position.Set(std::clamp(lastPos.x, movingArea.left, movingArea.right), std::clamp(lastPos.y, movingArea.top, movingArea.bottom));
}

void GoodguyComponent::AddEnemy(Enemy* enemy)
{
	if (enemy == nullptr)
		return;

	auto iter = std::find(enemies.begin(), enemies.end(), enemy);
	if (iter == enemies.end())
	{
		enemies.push_back(enemy);
	}
}

GoodguyRenderer::GoodguyRenderer(::GameObject* gameObject, GoodguyComponent* goodguy) : Renderer(gameObject)
{
	this->goodguy = goodguy;
}

void GoodguyRenderer::Render()
{
	// 현재 State에 따라 사각형의 색깔을 변경함
	auto solidBrush = SolidBrush();
	if (solidBrush == nullptr)
		return;

	switch (goodguy->CurrentState())
	{
	case GoodguyComponent::State::Idle:
	{
		solidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
		break;
	}
	case GoodguyComponent::State::Run:
	{
		solidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
		break;
	}
	default:
		solidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
		break;
	}

	auto bounds = this->SizeBounds();
	D2D1_ELLIPSE ellipse = { Vector2::Zero(), bounds.extents.x, bounds.extents.y };
	RenderTarget()->FillEllipse(ellipse, solidBrush);
}

Goodguy::Goodguy() : rigidbody(this), goodguy(this, &rigidbody), renderer(this, &goodguy)
{
	rigidbody.gravityScale = 0;
	goodguy.viewRange = 200;
	goodguy.speed = 50;
	renderer.SetSize(Vector2(20, 20));
}
