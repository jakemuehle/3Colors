#include "ColorGrid.h"

#include <cassert>
#include <algorithm>
#include "my_math.h"
#include "ColorNode.h"
#include "ColorSequence.h"
#include "ColorCount.h"

namespace three_color
{
	ColorGrid::ColorGrid(const cocos2d::CCRect cbBounds,
						 unsigned int nRows,
						 unsigned int nColumns,
	 					 unsigned int nColors,
						 unsigned int nComboSize,
						 double nInitCapturePercent) :
						 m_uncaptured_count(nColors)
	{
		m_cbBounds = cbBounds;
		m_nRows = nRows;
		m_nColumns = nColumns;
	 	m_nColors = nColors;
		m_nComboSize = nComboSize;
		m_pSelectedNodes = new ColorNode*[nComboSize];
		m_nCurrentSelectedNode = 0;
		m_nSelectedNodesSize = 0;
		m_pSelectedNodeNumbers = new unsigned int[nComboSize];
		m_cbSequenceColors = new PaletteIndex[nComboSize];
		m_nInitCapturePercent = nInitCapturePercent;
		m_nCapturedNodes = 0;
		this->registerWithTouchDispatcher();
	}

	ColorGrid::~ColorGrid()
	{
		delete m_pNodes;
		delete m_pSelectedNodes;
		delete m_pSelectedNodeNumbers;
		delete m_cbSequenceColors;
	}

	bool ColorGrid::init()
	{
		m_nNodes = m_nRows*m_nColumns;
		m_pNodes = new ColorNode[m_nNodes];
		assert(m_pNodes && "New failed to create the sequence array for the grid.");
        
		m_cbRandomSequence = new PaletteIndex[m_nRows*m_nColumns];
		assert(m_cbRandomSequence && "New failed to create the random sequence for the grid.");
        
		fHorizontalSpacing = getHorizontalSpacing();
		fVerticalSpacing = getVerticalSpacing();
		fInitialX = fHorizontalSpacing * 0.5f + m_cbBounds.getMinX();
		fInitialY = fVerticalSpacing * 0.5f + m_cbBounds.getMinY();

		// Note that the origin for the points start in the lower left.
		cocos2d::CCPoint cbCurrentPoint(fInitialX, fInitialY);

		for(int i = 0; i < m_nNodes; i++)
		{
			bool bIsSuccessful = m_pNodes[i].init();
				
			if(bIsSuccessful)
			{
				if(i / m_nRows % 2 == 0)
					cbCurrentPoint.y = fInitialY + ((i % m_nRows) * fVerticalSpacing);
				else
					cbCurrentPoint.y = fInitialY + (fVerticalSpacing / 2) + ((i % m_nRows) * fVerticalSpacing);

				cbCurrentPoint.x = fInitialX + ((i / m_nRows) * fHorizontalSpacing);
				
				m_pNodes[i].setPosition(cbCurrentPoint);
				m_pNodes[i].setParent(this, 1);
				m_pNodes[i].setColor(my_utility::random(m_nColors));
				m_uncaptured_count.tally(m_pNodes[i].getPaletteIndex());
			}
		}

		RandomCapture(m_nInitCapturePercent);

		for(int i = 0; i < m_nComboSize; i++)
		{
			m_pSelectedNodes[i] = 0;
		}

		for(int i = 0; i < m_nComboSize; i++)
		{
			m_pSelectedNodeNumbers[i] = 0;
		}
        
		return true;
	}

	bool ColorGrid::Reset()
	{
		if(!m_pNodes)
			return init();

		ClearPoints(0);
		m_uncaptured_count.resetAll();

		for(int i = 0; i < m_nNodes; ++i)
		{
			m_pNodes[i].uncapture();
			m_pNodes[i].setColor(my_utility::random(m_nColors));
			m_uncaptured_count.tally(m_pNodes[i].getPaletteIndex());
		}

		m_nCapturedNodes = 0;

		RandomCapture(m_nInitCapturePercent);
		
		return true;
	}

	void ColorGrid::RandomCapture(double nPercent)
	{
		int nIterations = m_nNodes * nPercent;

		for(; m_nCapturedNodes < nIterations;)
		{
			if(m_pNodes[my_utility::random(m_nNodes)].isCaptured())
				continue;
			else
				m_pNodes[my_utility::random(m_nNodes)].capture();

			m_nCapturedNodes++;
		}
	}

	unsigned int ColorGrid::setRandomSequence(ColorNode* pSequence, unsigned int nSize)
	{
		unsigned int nLength = getRandomSequence(m_nColors, nSize, nSize, m_cbRandomSequence);
		setColorNodes(m_cbRandomSequence, nLength, pSequence);
		return nLength;
	}

	float ColorGrid::getHorizontalSpacing() const
	{
		unsigned int nDivisions = m_nColumns + 1;
		return m_cbBounds.size.width / nDivisions;
	}
    
	float ColorGrid::getVerticalSpacing() const
	{
		unsigned int nDivisions = m_nRows + 1;
		return m_cbBounds.size.height / nDivisions;
	}

	void ColorGrid::registerWithTouchDispatcher()
	{
		cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,INT_MIN,false);
	}

	bool ColorGrid::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
	{
		cocos2d::CCPoint touchLocation = touch->getLocation();
		
		ClearPoints(0);

		if (m_cbBounds.containsPoint(touchLocation))
		{
			for(int i = 0; i < m_nNodes; i++)
			{
				if(m_pNodes[i].isOnSprite(touchLocation))
				{
					m_pNodes[i].select();
					m_pSelectedNodes[m_nSelectedNodesSize] = &m_pNodes[i];
					m_cbSequenceColors[m_nSelectedNodesSize] = m_pNodes[i].getPaletteIndex();
					m_pSelectedNodeNumbers[m_nSelectedNodesSize] = i;
					m_nSelectedNodesSize = 1;
					return true;
				}
			}
		}

		return true;
	}

	bool ColorGrid::IsFull()
	{
		if(m_nSelectedNodesSize == m_nComboSize)
			return true;

		return false;
	}

	ColorNode** ColorGrid::GetPoints()
	{
		return m_pSelectedNodes;
	}

	PaletteIndex* ColorGrid::GetSequenceColors()
	{
		return m_cbSequenceColors;
	}

	void ColorGrid::ClearPoints(unsigned int nStart)
	{
		for(int i = nStart; i < m_nComboSize; i++)
		{
			if(m_pSelectedNodes[i] != 0)
				m_pSelectedNodes[i]->deselect();

			m_pSelectedNodes[i] = 0;
		}

		if(nStart <= m_nComboSize)
			m_nSelectedNodesSize = nStart;
	}

	unsigned int ColorGrid::GetComboSize()
	{
		return m_nComboSize;
	}

	unsigned int ColorGrid::GetNumberPoints()
	{
		return m_nSelectedNodesSize;
	}

	void ColorGrid::HandleNodes(bool bStatus)
	{
		PaletteIndex cbPrevColor;
		PaletteIndex cbRandomColor;

		for(int i = 0; i < m_nSelectedNodesSize; i++)
		{
			if(m_pSelectedNodes[i]->isCaptured() && bStatus)
			{
				cbPrevColor = m_pSelectedNodes[i]->getPaletteIndex();
				cbRandomColor = my_utility::random(m_nColors);
				
				while(cbPrevColor == cbRandomColor)
					cbRandomColor = my_utility::random(m_nColors);
				
				m_pSelectedNodes[i]->setColor(cbRandomColor);
				m_pSelectedNodes[i]->uncapture();
				m_nCapturedNodes--;
				// Add the node's new color back to the tally
				m_uncaptured_count.tally(m_pSelectedNodes[i]->getPaletteIndex());
			}
			else if(bStatus)
			{
				m_pSelectedNodes[i]->capture();
				m_nCapturedNodes++;
				// Remove the node from the count
				m_uncaptured_count.tally(m_pSelectedNodes[i]->getPaletteIndex(),-1);
			}
		}

		ClearPoints(0);
	}

	void ColorGrid::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
	{
		cocos2d::CCPoint touchLocation = touch->getLocation();

		if(!IsFull())
			ClearPoints(0);
	}

	void ColorGrid::ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
	{
	}

	void ColorGrid::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
	{
		cocos2d::CCPoint touchLocation = touch->getLocation();

		if (m_cbBounds.containsPoint(touchLocation))
		{
			for(int i = 0; i < m_nNodes; i++)
			{
				if(m_pNodes[i].isOnSprite(touchLocation)
					&& CheckList(i)
					&& CheckAdjacency(i)
					&& m_nSelectedNodesSize < m_nComboSize)
				{
					m_pNodes[i].select();
					m_pSelectedNodes[m_nSelectedNodesSize] = &m_pNodes[i];
					m_cbSequenceColors[m_nSelectedNodesSize] = m_pNodes[i].getPaletteIndex();
					m_pSelectedNodeNumbers[m_nSelectedNodesSize] = i;
					m_nSelectedNodesSize++;
				}
			}
		}
	}

	bool ColorGrid::CheckAdjacency(unsigned int nIndex)
	{
		if((m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] / m_nRows) % 2 == 0)
		{
			if(nIndex == m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] - m_nRows - 1 && m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] % m_nRows != 0)	//LL
				return true;

			if(nIndex == m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] - m_nRows)	//LU
				return true;

			if(nIndex == m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] + m_nRows - 1 && m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] % m_nRows != 0 && m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] != 0)	//RL
				return true;

			if(nIndex == m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] + m_nRows)	//RU
				return true;
		}
		else
		{
			if(nIndex == m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] - m_nRows + 1  && (m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] + 1) % m_nRows != 0)	//LU
				return true;

			if(nIndex == m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] - m_nRows)	//LL
				return true;

			if(nIndex == m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] + m_nRows)	//RL
				return true;

			if(nIndex == m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] + m_nRows + 1 && (m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] + 1) % m_nRows != 0)	//RU
				return true;
		}

		if(nIndex == m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] - 1 && m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] % m_nRows != 0)  //L
			return true;

		if(nIndex == m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] + 1 && (m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] + 1) % m_nRows != 0) //U
			return true;

		return false;
	}

	bool ColorGrid::CheckList(unsigned int nIndex)
	{
		for(int i = 0; i < m_nSelectedNodesSize; i++)
		{
			if(nIndex == m_pSelectedNodeNumbers[i] && m_pSelectedNodes[i] != 0)
			{
				m_nSelectedNodesSize = i + 1;
				ClearPoints(m_nSelectedNodesSize);
				return false;
			}
		}

		return true;
	}
} //end namespace three_colors
	
	