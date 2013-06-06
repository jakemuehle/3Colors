#ifndef __3COLOR__CGRID_H
#define __3COLOR__CGRID_H

#include "cocos2d.h"
#include "ThreeColorTypes.h"

namespace three_color
{
	class ColorNode;

	class ColorGrid : public cocos2d::CCLayer
	{
	public:
		ColorGrid(
			const cocos2d::CCRect cbBounds,
			unsigned int nRows,
			unsigned int nColumns,
			unsigned int nColors,
			unsigned int nMinRows,
			unsigned int nMaxRows,
			unsigned int nMinColumns,
			unsigned int nMaxColumns)
			: m_cbBounds(cbBounds), 
			m_ppNodes(NULL),
			m_nRows(nRows),
			m_nColumns(nColumns),
			m_nColors(nColors),
			m_nMinRows(nMinRows),
			m_nMaxRows(nMaxRows),
			m_nMinColumns(nMinColumns),
			m_nMaxColumns(nMaxColumns) {}
		//CGrid(const CGrid& cbRHS);
		~ColorGrid(void);

		// Calculates the horizontal spacing between nodes.
        float getHorizontalSpacing() const;
        // Calculates the vertical spacing between nodes.
        float getVerticalSpacing() const;



		bool init();

	private:
		const cocos2d::CCRect m_cbBounds;
		ColorNode** m_ppNodes;
		PaletteIndex * m_cbRandomSequence;
		unsigned int m_nRows;
		unsigned int m_nColumns;
		unsigned int m_nColors;
		unsigned int m_nMinRows;
		unsigned int m_nMaxRows;
		unsigned int m_nMinColumns;
		unsigned int m_nMaxColumns;

		unsigned int setRandomSequence(ColorNode* pSequence);

		ColorGrid();
	};
}

#endif

