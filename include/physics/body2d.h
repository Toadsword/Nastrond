/*
MIT License

Copyright (c) 2017 SAE Institute Switzerland AG

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef SFGE_BODY2D_H
#define SFGE_BODY2D_H


#include <Box2D/Box2D.h>
#include <engine/component.h>
#include <engine/transform2d.h>
#include <editor/editor.h>

namespace sfge
{

class Body2d: public Offsetable
{
public:
	Body2d();
	Body2d(Transform2d *transform, Vec2f offset);

	b2Vec2 GetLinearVelocity();
	void SetLinearVelocity(b2Vec2 velocity);
	void ApplyForce(b2Vec2 force);
	b2BodyType GetType();
	float GetMass();
	void SetBody(b2Body* body);
	b2Body* GetBody();
private:
	b2Body * m_Body = nullptr;
};

namespace editor
{
struct Body2dInfo : ComponentInfo
{
	void DrawOnInspector() override;
	void AddVelocity(b2Vec2 velocity);
	std::deque<b2Vec2>& GetVelocities();
	Body2d* body = nullptr;
private:
	std::deque<b2Vec2> m_Velocities;
	const size_t m_VelocitiesMaxSize = 120;
};
}

class Body2dManager : public SingleComponentManager<Body2d, editor::Body2dInfo, ComponentType::BODY2D>
{
public:
	using SingleComponentManager::SingleComponentManager;
	void Init() override;
	void FixedUpdate() override;
	Body2d* AddComponent(Entity entity) override;
	void CreateComponent(json& componentJson, Entity entity) override;
	void DestroyComponent(Entity entity) override;

	void OnResize(size_t new_size) override;

private:
	Transform2dManager* m_Transform2dManager;
	std::weak_ptr<b2World> m_WorldPtr;
};



}

#endif