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
#include <engine/engine.h>
#include <engine/log.h>
#include <engine/modules.h>
#include <python/pycomponent.h>
#include <utility/json_utility.h>
#include <graphics/shape.h>

int main()
{
	sfge::Engine engine;
	engine.Init(true);

	auto pythonManager = engine.GetPythonManager();

	json gameObjectJson = {
	{"name", "PyGameObject" },
	{"components",
	{
		{
			{"type", (int)sfge::ComponentType::TRANSFORM}
		},
		{
			{"type", (int)sfge::ComponentType::PYCOMPONENT },
			{"script_path", "scripts/component_test.py" }
		},
		{
			{ "type", (int)sfge::ComponentType::PYCOMPONENT },
			{ "script_path", "scripts/sprite_test.py" }
		},
		{
			{"type", (int)sfge::ComponentType::SHAPE },
			{"shape_type",(int)sfge::ShapeType::CIRCLE},
			{"position",{ 100,300 }},
			{ "radius", 500.0 }
		}
	}
	}
	};

	sfge::GameObject* gameObject = sfge::GameObject::LoadGameObject(engine, gameObjectJson);

	for (int i = 0; i < 10; i++)
	{
		sfge::Log::GetInstance()->Msg("GAME OBJECT UPDATE");
		gameObject->Update(sf::seconds(0.4));
	}
	delete(gameObject);
	engine.Destroy();

#ifdef WIN32
	system("pause");
#endif
	return EXIT_SUCCESS;
}
