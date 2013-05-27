
#ifndef __3COLOR__COLOR_NODE_H
#define __3COLOR__COLOR_NODE_H


#include "cocos2d.h"

#include "ThreeColorTypes.h"


namespace three_color
{

class ColorNode
{
public:

	ColorNode();
	
	~ColorNode(void);

	// Initialize a sprite created with the default constructor.
	// The sprite sheet should already be loaded by the CCSpriteFrameCache.
	// Returns if the initialization is successful.
	// Will fail if it is already initialized.
	bool init();

	cocos2d::CCSprite* getSprite() { return m_sprite; }
	
	void setPosition( cocos2d::CCPoint& position );

	// Sets the parent. The zOrder specifies the drawn depth or layer.
	void setParent( cocos2d::CCNode* parent, int z_order );
	// If the node is removed before its parent, this should be called to remove the sprite from being displayed.
	void removeParent( cocos2d::CCNode* parent );

	void setFrame( cocos2d::CCSpriteFrame* frame );

	bool isOnSprite( cocos2d::CCPoint& position ) const;

	PaletteIndex getPaletteIndex() const { return m_palette_index; }
	// The sprite should be setup before this is called.
	void setColor( PaletteIndex palette_index );

	bool isCaptured() const;
	void setIsCaptured( bool is_captured );
	void capture();
	void uncapture();
	void toggleCaptured();

	bool isSelected() const;
	void setIsSelected( bool is_selected );
	void select();
	void deselect();
	void toggleSelect();

private:

	cocos2d::CCSprite* m_sprite;
	PaletteIndex m_palette_index;
	unsigned char m_flags;

	// Disable copy and assignment
	ColorNode(const ColorNode &);
	ColorNode & operator=(const ColorNode &);
}; // ColorNode

} // three_color

#endif // __3COLOR__COLOR_NODE_H
