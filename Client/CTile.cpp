#include "pch.h"
#include "CTile.h"

CTile::CTile()
{
}

CTile::~CTile()
{
}

void CTile::render(HDC _dc)
{
	tSelectPen pen(_dc);
	pen.SetPenColor(PEN_COLOR::DEFAULT);

	tSelectBrush brush(_dc, BRUSH_COLOR::HOLLOW);

	Vec2 vPos = GetPos();

	Rectangle(_dc, (int)(vPos.x)
		, (int)(vPos.y)
		, (int)(vPos.x + TILE_SIZE)
		, (int)(vPos.y + TILE_SIZE));

}