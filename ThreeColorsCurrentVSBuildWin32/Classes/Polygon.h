#pragma once

#include <stdlib.h>
#include "cocoa/CCGeometry.h";

class CPolygon
{
public:
	CPolygon(void);
	CPolygon(const CPolygon& cbRHS);
	~CPolygon(void);

	cocos2d::CCPoint* GetVertexHead();
	unsigned int GetNumberVertices();
	void AddVertex(const cocos2d::CCPoint& cbNewPoint);
	void AddVertexAt(const cocos2d::CCPoint& cbNewPoint, const unsigned int nAddPoint);
	void RemoveVertex(const cocos2d::CCPoint& cbRemovePoint);
	void RemoveVertexAt(const unsigned int nRemovePoint);
	void SetClosed(bool bNewClosure);
	void Resize(const unsigned int nNewSize);
	void Clear();
	bool IsClosed();

private:
	static const unsigned int m_nREALLOCATIONMULTIPLIER = 2;

	bool CheckBounds(const unsigned int nLocation);
	void ResizeBounds();

	unsigned int m_nPoints;
	unsigned int m_nAllocated;
	cocos2d::CCPoint* m_pVertices;
	bool m_bClosedPolygon;
};

