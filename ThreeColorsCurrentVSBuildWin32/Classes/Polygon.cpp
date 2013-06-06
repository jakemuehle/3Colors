#include "Polygon.h"

CPolygon::CPolygon(void)
{
	m_nPoints = 0;
	m_nAllocated = 0;
	m_pVertices = 0;
	m_bClosedPolygon = true;
}

CPolygon::CPolygon(const CPolygon& cbRHS)
{
	m_nPoints = cbRHS.m_nPoints;
	m_nAllocated = cbRHS.m_nAllocated;
	m_pVertices = 0;
	m_bClosedPolygon = cbRHS.m_bClosedPolygon;

}

CPolygon::~CPolygon(void)
{
	if(m_pVertices != 0)
	{
		free(m_pVertices);
		m_pVertices = 0;
	}
}

//void CPolygon::Resize(const unsigned int nNewSize)
//{
//	if(nNewSize < m_nPoints)
//	{
//		m_nPoints = nNewSize;
//	}
//
//	m_pVertices = 
//}

cocos2d::CCPoint* CPolygon::GetVertexHead()
{
	return m_pVertices;
}

unsigned int CPolygon::GetNumberVertices()
{
	return m_nPoints;
}

void CPolygon::AddVertex(const cocos2d::CCPoint& cbNewPoint)
{
	ResizeBounds();
	m_pVertices[m_nPoints] = cbNewPoint;
	m_nPoints++;
}

void CPolygon::AddVertexAt(const cocos2d::CCPoint& cbNewPoint, const unsigned int nAddPoint)
{
	m_nPoints++;
	ResizeBounds();

	if(CheckBounds(nAddPoint))
	{
		for(int i = m_nPoints; i > nAddPoint; i--)
		{
			m_pVertices[i] = m_pVertices[i-1];
		}

		m_pVertices[nAddPoint] = cbNewPoint;
	}
	else
	{
		m_nPoints--;
	}
}

void CPolygon::RemoveVertex(const cocos2d::CCPoint& cbRemovePoint)
{
	for(int i = 0; i < m_nPoints; i++)
	{
		if(m_pVertices[i].equals(cbRemovePoint))
		{
			RemoveVertexAt(i);
		}
	}
}

void CPolygon::RemoveVertexAt(const unsigned int nRemoveMarker)
{
	if(CheckBounds(nRemoveMarker))
	{
		for(int i = nRemoveMarker; i < m_nPoints - 1; i++)
		{
			m_pVertices[i] = m_pVertices[i+1];
		}

		m_nPoints--;
	}
}

void CPolygon::SetClosed(bool bNewClosure)
{
	m_bClosedPolygon = bNewClosure;
}

bool CPolygon::IsClosed()
{
	return m_bClosedPolygon;
}

bool CPolygon::CheckBounds(const unsigned int nLocation)
{
	if(nLocation > m_nPoints || nLocation < 0)
	{
		return false;
	}

	return true;
}

void CPolygon::ResizeBounds()
{
	if(m_nPoints <= m_nAllocated)
	{
		m_pVertices = (cocos2d::CCPoint*)realloc(m_pVertices, (m_nAllocated + 1) * m_nREALLOCATIONMULTIPLIER * sizeof(cocos2d::CCPoint*));
	}
}

void CPolygon::Clear()
{
	if(m_pVertices != 0)
	{
		free(m_pVertices);
		m_pVertices = 0;
	}
}