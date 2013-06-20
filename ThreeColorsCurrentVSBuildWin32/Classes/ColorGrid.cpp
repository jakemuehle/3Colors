#include "ColorGrid.h"

#include <cassert>
#include <algorithm>
#include "my_math.h"
#include "ColorNode.h"
#include "ColorSequence.h"

namespace three_color
{
	ColorGrid::ColorGrid(const cocos2d::CCRect cbBounds,
						 unsigned int nRows,
						 unsigned int nColumns,
	 					 unsigned int nColors,
						 unsigned int nComboSize)
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
		this->registerWithTouchDispatcher();
	}

	ColorGrid::~ColorGrid()
	{
		if(m_ppNodes)
		{
			for( ColorNode** sequence_iter = m_ppNodes; sequence_iter != m_ppNodes + m_nRows; ++sequence_iter )
			{
				if( *sequence_iter )
				{
					delete [] *sequence_iter;
					*sequence_iter = NULL;
				}
			}
			delete [] m_ppNodes;
			m_ppNodes = NULL;
		}

		delete m_pSelectedNodes;
		delete m_pSelectedNodeNumbers;
		delete m_cbSequenceColors;
	}

	bool ColorGrid::init()
	{
		m_ppNodes = new ColorNode*[m_nRows];
		assert(m_ppNodes && "New failed to create the sequence array for the grid.");
        
		m_cbRandomSequence = new PaletteIndex[m_nColumns];
		assert(m_cbRandomSequence && "New failed to create the random sequence for the grid.");
        
		fHorizontalSpacing = getHorizontalSpacing();
		fVerticalSpacing = getVerticalSpacing();
		fInitialX = fHorizontalSpacing * 0.5f + m_cbBounds.getMinX();

		// Note that the origin for the points start in the lower left.
		cocos2d::CCPoint cbCurrentPoint(fInitialX, fVerticalSpacing * 0.5f + m_cbBounds.getMinY());

		for(int i = 0; i < m_nRows; i++)
		{
			m_ppNodes[i] = new ColorNode[m_nColumns];
			assert(m_ppNodes[i] && "New failed to create a sequence for the color grid.");
			cbCurrentPoint.x = fInitialX;

			for(int j = 0; j < m_nColumns; j++)
			{
				bool bIsSuccessful = m_ppNodes[i][j].init();
				
				if(bIsSuccessful)
				{
					m_ppNodes[i][j].setPosition(cbCurrentPoint);
					m_ppNodes[i][j].setParent(this, 1);
                
					if(j % 2 == 0)
						cbCurrentPoint.y += fVerticalSpacing / 2;
					else
						cbCurrentPoint.y -= fVerticalSpacing / 2;

					cbCurrentPoint.x += fHorizontalSpacing;
				}
				else
					assert(bIsSuccessful && "Failed to initialize a color node for the grid.");
			}
			cbCurrentPoint.y += fVerticalSpacing;
		}

		for( ColorNode** sequence_iter = m_ppNodes; sequence_iter != m_ppNodes + m_nRows; ++sequence_iter )
		{
			setRandomSequence(*sequence_iter, m_nColumns);
		}

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
		int i = 0;
		
		ClearPoints();

		if (m_cbBounds.containsPoint(touchLocation))
		{
			for( ColorNode** sequence_iter = m_ppNodes; sequence_iter != m_ppNodes + m_nRows; ++sequence_iter )
			{
				for( ColorNode* node_iter = *sequence_iter; node_iter != *sequence_iter + m_nColumns; ++node_iter )
				{
					if(node_iter->isOnSprite(touchLocation))
					{
						node_iter->select();
						m_pSelectedNodes[m_nSelectedNodesSize] = node_iter;
						m_cbSequenceColors[m_nSelectedNodesSize] = node_iter->getPaletteIndex();
						m_pSelectedNodeNumbers[m_nSelectedNodesSize] = i;
						m_nSelectedNodesSize++;
						return true;
					}

					i++;
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

	void ColorGrid::ClearPoints()
	{
		for(int i = 0; i < m_nSelectedNodesSize; i++)
		{
			if(m_pSelectedNodes[i] != 0)
				m_pSelectedNodes[i]->deselect();

			m_pSelectedNodes[i] = 0;
		}

		m_nSelectedNodesSize = 0;
	}

	unsigned int ColorGrid::GetComboSize()
	{
		return m_nComboSize;
	}

	unsigned int ColorGrid::GetNumberPoints()
	{
		return m_nSelectedNodesSize;
	}

	void ColorGrid::CheckNodes(cocos2d::CCPoint cbTouchedRegion)
	{
		int i = 0;
		double nHorizontalEpsilon;
		double nVerticalEpsilon;

		for( ColorNode** sequence_iter = m_ppNodes; sequence_iter != m_ppNodes + m_nRows; ++sequence_iter )
		{
			for( ColorNode* node_iter = *sequence_iter; node_iter != *sequence_iter + m_nColumns; ++node_iter )
			{
				if(node_iter->isOnSprite(cbTouchedRegion) && !node_iter->isOnSprite(m_cbLastPoint))
				{
					if(m_nSelectedNodesSize == 0)
					{
						m_cbLastPoint = cbTouchedRegion;
						nLastTouchedNode = i;
					}
					else if(nLastTouchedNode == i)
						return;

					if(m_nSelectedNodesSize <= m_nComboSize 
					&& (((m_cbLastPoint.x == cbTouchedRegion.x && m_cbLastPoint.y + (fVerticalSpacing * 2) >= cbTouchedRegion.y)
					 || (m_cbLastPoint.x == cbTouchedRegion.x && m_cbLastPoint.y - (fVerticalSpacing * 2) >= cbTouchedRegion.y))
					 || ((m_cbLastPoint.x + fHorizontalSpacing >= cbTouchedRegion.x && m_cbLastPoint.y + fVerticalSpacing >= cbTouchedRegion.y)
					 || (m_cbLastPoint.x - fHorizontalSpacing >= cbTouchedRegion.x && m_cbLastPoint.y + fVerticalSpacing >= cbTouchedRegion.y)
					 || (m_cbLastPoint.x - fHorizontalSpacing >= cbTouchedRegion.x && m_cbLastPoint.y - fVerticalSpacing >= cbTouchedRegion.y)
					 || (m_cbLastPoint.x + fHorizontalSpacing >= cbTouchedRegion.x && m_cbLastPoint.y - fVerticalSpacing >= cbTouchedRegion.y))))
					{
						m_cbLastPoint = cbTouchedRegion;
						node_iter->select();
						m_pSelectedNodes[m_nSelectedNodesSize] = node_iter;
						m_cbSequenceColors[m_nSelectedNodesSize] = node_iter->getPaletteIndex();
						m_nSelectedNodesSize++;
						nLastTouchedNode = i;
					}

					return;
				}

				i++;
			}
		}
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
			}
			else if(bStatus)
				m_pSelectedNodes[i]->capture();
		}

		ClearPoints();
	}

	void ColorGrid::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
	{
		cocos2d::CCPoint touchLocation = touch->getLocation();

		if(!IsFull())
			ClearPoints();
	}

	void ColorGrid::ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
	{
	}

	void ColorGrid::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
	{
		cocos2d::CCPoint touchLocation = touch->getLocation();
		int i = 0;
		
		if (m_cbBounds.containsPoint(touchLocation))
		{
			for( ColorNode** sequence_iter = m_ppNodes; sequence_iter != m_ppNodes + m_nRows; ++sequence_iter )
			{
				for( ColorNode* node_iter = *sequence_iter; node_iter != *sequence_iter + m_nColumns; ++node_iter )
				{
					if(node_iter->isOnSprite(touchLocation)
					   && CheckList(i)
					   && CheckAdjacency(i)
					   && m_nSelectedNodesSize < m_nComboSize)
					{
						node_iter->select();
						m_pSelectedNodes[m_nSelectedNodesSize] = node_iter;
						m_cbSequenceColors[m_nSelectedNodesSize] = node_iter->getPaletteIndex();
						m_pSelectedNodeNumbers[m_nSelectedNodesSize] = i;
						m_nSelectedNodesSize++;
					}

					i++;
				}
			}
		}
	}

	bool ColorGrid::CheckAdjacency(unsigned int nIndex)
	{
		if(m_nRows % 2 == 0)  //Even number of rows
		{
			if(m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] % 2 == 1)  //Even Column, Odd last number
			{
				//Left Lower
				if(m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] - 1 == nIndex)
					return true;

				//Right Lower
				if(m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] + 1 == nIndex && (m_pSelectedNodeNumbers[m_nSelectedNodesSize-1]  + 1) % m_nRows != 0)
					return true;

				//Left Lower
				if(m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] + m_nRows - 1 == nIndex)
					return true;

				//Right Upper
				if(m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] + m_nRows + 1 == nIndex && (m_pSelectedNodeNumbers[m_nSelectedNodesSize-1]  + 1) % m_nRows != 0)
					return true;
			}
			else //Even column, even last number
			{
				//Left Lower
				if(m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] - m_nRows - 1 == nIndex  && m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] % m_nRows != 0)
					return true;

				//Right Lower
				if(m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] - m_nRows + 1 == nIndex)
					return true;

				//Left Upper
				if(m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] - 1 == nIndex  && m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] % m_nRows != 0)
					return true;

				//Right Upper
				if(m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] + 1 == nIndex)
					return true;
			}
		}
		else //Odd number of rows
		{
			if((m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] / m_nRows) % 2 == 0 && m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] % 2 == 0)  //Even column, even last number
			{
				//Left Upper
				if(m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] - 1 == nIndex && m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] % m_nRows != 0)
					return true;

				//Right Upper
				if(m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] + 1 == nIndex && (m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] + 1) % m_nRows != 0)
					return true;

				//Left Lower
				if(m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] + m_nRows - 1 == nIndex && m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] % m_nRows != 0)
					return true;

				//Right Lower
				if(m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] + m_nRows + 1 == nIndex && (m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] + 1) % m_nRows != 0)
					return true;
			}
			else if((m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] / m_nRows) % 2 == 1 && m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] % 2 == 0) //Odd column, even last number
			{
				//Left Upper
				if(m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] - m_nRows - 1 == nIndex)
					return true;

				//Right Upper
				if(m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] -m_nRows + 1 == nIndex)
					return true;

				//Left Lower
				if(m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] - 1 == nIndex)
					return true;

				//Right Lower
				if(m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] + 1 == nIndex)
					return true;
			}
			else if((m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] / m_nRows) % 2 == 1 && m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] % 2 == 1) //Odd column, odd last number
			{
				//Left Upper
				if(m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] - 1 == nIndex && m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] % m_nRows != 0)
					return true;

				//Right Upper
				if(m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] + 1 == nIndex && (m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] + 1) % m_nRows != 0)
					return true;

				//Left Lower
				if(m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] + m_nRows - 1 == nIndex && m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] % m_nRows != 0)
					return true;

				//Right Lower
				if(m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] + m_nRows + 1 == nIndex && (m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] + 1) % m_nRows != 0)
					return true;
			}
			else if((m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] / m_nRows) % 2 == 0  && m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] % 2 == 1) //Even column, odd last number
			{
				//Left Upper
				if(m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] + m_nRows - 1 == nIndex)
					return true;

				//Right Upper
				if(m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] + m_nRows + 1 == nIndex)
					return true;

				//Left Lower
				if(m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] - 1 == nIndex)
					return true;

				//Right Lower
				if(m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] + 1 == nIndex)
					return true;
			}
		}

		//Upper
		if(m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] - m_nRows == nIndex)
			return true;

		//Lower
		if(m_pSelectedNodeNumbers[m_nSelectedNodesSize-1] + m_nRows == nIndex)
			return true;

		return false;
	}

	bool ColorGrid::CheckList(unsigned int nIndex)
	{
		int nNewIndex = 1;
		bool bIsNotListed = true;

		for(unsigned int* sequence_iter = m_pSelectedNodeNumbers; sequence_iter != m_pSelectedNodeNumbers + m_nSelectedNodesSize; ++sequence_iter )
		{
			if(*sequence_iter == nIndex)
			{
				m_nSelectedNodesSize = nNewIndex;
				bIsNotListed = false;
				break;
			}
			nNewIndex++;
		}

		for(int i = m_nSelectedNodesSize; i < m_nComboSize; i++)
		{
			if(m_pSelectedNodes[i] != 0)
				m_pSelectedNodes[i]->deselect();
		}

		return bIsNotListed;
	}
} //end namespace three_colors
	