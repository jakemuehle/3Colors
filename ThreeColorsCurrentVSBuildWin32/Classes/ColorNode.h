// Copywright May 2013
// Author: Derek Higgs
// Purpose: Handles a color node such as its status and animations.

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
        
        // Gets the sprite
        cocos2d::CCSprite* getSprite() { return m_sprite; }
        
        // Sets the sprite's position
        void setPosition( cocos2d::CCPoint& position );
        
        // Set the sprite's x position.
        void setPositionX( float x );
        // Set the sprite's y position.
        void setPositionY( float y );
        
        // Get the sprite's position.
        const cocos2d::CCPoint & getPosition();
        
        // Sets the parent. The zOrder specifies the drawn depth or layer.  Or calling addChild on the sprite works too.
        void setParent( cocos2d::CCNode* parent, int z_order );
        // If the node is removed before its parent, this should be called to remove the sprite from being displayed.
        void removeParent( cocos2d::CCNode* parent );
        
        // Sets the frame of the sprite.
        void setFrame( cocos2d::CCSpriteFrame* frame );
        
        // Checks to see if the point is on the sprite.
        bool isOnSprite( cocos2d::CCPoint& position ) const;
        
        // Get the palette index.  Represents which color to show.
        PaletteIndex getPaletteIndex() const { return m_palette_index; }
        
        // Set the palette index.  Represents which color to show.
        // The sprite should be setup before this is called.
        void setColor( PaletteIndex palette_index );
        
        // Functions to handle the capture status.
        bool isCaptured() const;
        void setIsCaptured( bool is_captured );
        void capture();
        void uncapture();
        void toggleCaptured();
        
        // Functions to handle the selected status.
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
