#include "ColorGrid.h"

#include <cassert>
#include <algorithm>

#include "ColorNode.h"
#include "ColorSequence.h"

namespace three_color
{
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
	}

	bool ColorGrid::init()
	{
		m_ppNodes = new ColorNode*[m_nRows];
		assert(m_ppNodes && "New failed to create the sequence array for the grid.");
        
		m_cbRandomSequence = new PaletteIndex[m_nColumns];
		assert(m_cbRandomSequence && "New failed to create the random sequence for the grid.");
        
		float fHorizontalSpacing = getHorizontalSpacing();
		float fVerticalSpacing = getVerticalSpacing();
		float fInitialX = fHorizontalSpacing * 0.5f + m_cbBounds.getMinX();
        
		// Note that the origin for the points start in the lower left.
		cocos2d::CCPoint cbCurrentPoint(fInitialX, fVerticalSpacing * 0.5f + m_cbBounds.getMinY());
        
		int i = 0;

		for( ColorNode** sequence_iter = m_ppNodes; sequence_iter != m_ppNodes + m_nRows; ++sequence_iter )
		{
			*sequence_iter = new ColorNode[m_nColumns];
			assert(*sequence_iter && "New failed to create a sequence for the color grid.");

			i = 0;

			cbCurrentPoint.x = fInitialX;
			for( ColorNode* node_iter = *sequence_iter; node_iter != *sequence_iter + m_nColumns; ++node_iter )
			{
				bool bIsSuccessful = node_iter->init();
				
				if(bIsSuccessful)
				{
					node_iter->setPosition(cbCurrentPoint);
					node_iter->setParent(this, 1);
                
					if(i % 2 == 0)
						cbCurrentPoint.y += fVerticalSpacing / 4;
					else
						cbCurrentPoint.y -= fVerticalSpacing / 4;

					cbCurrentPoint.x += fHorizontalSpacing;
					i++;
				}
				else
					assert(bIsSuccessful && "Failed to initialize a color node for the grid.");
			}
            
			cbCurrentPoint.y += fVerticalSpacing;
		}
        
		//unsigned int* sequence_length_iter = m_sequences_length;
		for( ColorNode** sequence_iter = m_ppNodes; sequence_iter != m_ppNodes + m_nRows; ++sequence_iter )
		{
			setRandomSequence( *sequence_iter );
		}
        
		return true;
	}

	unsigned int ColorGrid::setRandomSequence(ColorNode* pSequence)
	{
        unsigned int nLength = getRandomSequence(m_nColors, m_nMinRows, m_nMaxRows, m_cbRandomSequence);
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
} //end namespace three_colors
	