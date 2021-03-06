/*

MIT License

Copyright (c) 2018 SAE Institute Switzerland AG

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

#include <tilemap_creator.h>
#include <graphics/graphics2d.h>
#include <utility/log.h>

namespace sfge::tools
{
void TilemapCreator::Init()
{
	Log::GetInstance()->Msg("Init TilemapCreator");
	m_TilemapImguiManager.Init(this);

	m_TilemapManager = m_Engine.GetGraphics2dManager()->GetTilemapSystem()->GetTilemapManager();
	m_TileTypeManager = m_Engine.GetGraphics2dManager()->GetTilemapSystem()->GetTileTypeManager();

	m_IsInit = true;
}

void TilemapCreator::Update(float dt)
{
	if (!m_IsInit)
		Init();

	m_TilemapImguiManager.Update(dt);
}


void TilemapCreator::Draw()
{
	if (!m_IsInit)
		return;

	m_TilemapImguiManager.Draw();
}

TilemapImguiManager* TilemapCreator::GetTilemapImguiManager()
{
	return &m_TilemapImguiManager;
}

TilemapManager* TilemapCreator::GetTilemapManager()
{
	return m_TilemapManager;
}

TileTypeManager* TilemapCreator::GetTileTypeManager()
{
	return m_TileTypeManager;
}

}
