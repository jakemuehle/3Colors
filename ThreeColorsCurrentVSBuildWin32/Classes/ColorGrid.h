#ifndef __3COLOR__CGRID_H
#define __3COLOR__CGRID_H

#include "cocos2d.h"
#include "ColorNode.h"
#include "ThreeColorTypes.h"
#include "ColorCount.h"

namespace three_color
{
	class ColorNode;

	class ColorGrid : public cocos2d::CCLayer
	{
	public:
		ColorGrid(	const cocos2d::CCRect cbBounds,
					unsigned int nRows,
					unsigned int nColumns,
					unsigned int nColors,
					unsigned int nComboSize,
					double nInitCapturedPercent);
		~ColorGrid(void);

        float getHorizontalSpacing() const;
        float getVerticalSpacing() const;

		void registerWithTouchDispatcher();
		bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
		void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
		void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
		void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);

		void CheckNodes(cocos2d::CCPoint cbTouchedRegion);
		bool IsFull();
		ColorNode** GetPoints();
		PaletteIndex* GetSequenceColors();
		void ClearPoints(unsigned int nStart);
		unsigned int GetComboSize();
		unsigned int GetNumberPoints();
		void HandleNodes(bool bStatus);
		void RandomCapture(double nPercent);

		bool init();
		bool Reset();
        
        inline const ColorCount * getUncapturedColorCount() const { return &m_uncaptured_count; }
		inline const bool CheckWin() const { return (m_nCapturedNodes >= m_nNodes) ? true : false; }

	private:
		cocos2d::CCRect m_cbBounds;
		ColorNode* m_pNodes;
		PaletteIndex * m_cbRandomSequence;
		unsigned int m_nNodes;
		unsigned int m_nRows;
		unsigned int m_nColumns;
		unsigned int m_nColors;
		unsigned int m_nCurrentSelectedNode;
		unsigned int m_nSelectedNodesSize;
		ColorNode** m_pSelectedNodes;
		PaletteIndex* m_cbSequenceColors;
		unsigned int* m_pSelectedNodeNumbers;
		unsigned int m_nComboSize;
		float fHorizontalSpacing;
		float fVerticalSpacing;
		float fInitialX;
		float fInitialY;
		cocos2d::CCPoint m_cbLastPoint;
		unsigned int nLastTouchedNode;
		double m_nInitCapturePercent;
		unsigned int m_nCapturedNodes;
        
        ColorCount m_uncaptured_count;

		unsigned int setRandomSequence(ColorNode* pSequence, unsigned int nSize);
		bool CheckAdjacency(unsigned int nIndex);
		bool CheckList(unsigned int nIndex);

		ColorGrid();
	};
}

#endif

