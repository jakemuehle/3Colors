#include "ColorNode.h"

#include <cassert>

#include "ColorNodeFrames.h"

namespace three_color
{
    
    namespace
    {
        cocos2d::ccColor3B palette[] =
        {
            { 255, 0, 0 },
            { 0, 255, 0 },
            { 0, 0, 255 }
        };
        
        enum ColorNodeMasks
        {
            IsCapturedMask = 1,
            IsSelectedMask = IsCapturedMask << 1,
        }; // ColorNodeMasks
    }
    
    ColorNode::ColorNode(void)
	: m_sprite( NULL ),
    m_flags( 0 )
    {
    }
    
    ColorNode::~ColorNode(void)
    {
        if( m_sprite )
        {
            m_sprite->release();
            m_sprite = NULL;
        }
    }
    
    bool ColorNode::init()
    {
        assert( ! m_sprite && "Cannot initialize a color node that already has a sprite." );
        
        m_sprite = cocos2d::CCSprite::createWithSpriteFrame( ColorNodeFrames::get()->deselected_frame );
        
        if( ! m_sprite )
        {
            return false;
        }
        
        m_sprite->retain();
        
        return true;
    }
    
    void ColorNode::setPosition( cocos2d::CCPoint& position )
    {
        assert( m_sprite && "Cannot set the position of a color node that has no sprite." );
        
        m_sprite->setPosition( position );
    }
    
    void ColorNode::setPositionX( float x )
    {
        m_sprite->setPositionX(x);
    }
    
    void ColorNode::setPositionY( float y )
    {
        m_sprite->setPositionY(y);
    }
    
    const cocos2d::CCPoint & ColorNode::getPosition()
    {
        return m_sprite->getPosition();
    }
    
    void ColorNode::setParent( cocos2d::CCNode* parent, int z_order )
    {
        parent->addChild( m_sprite, z_order );
    }
    
    void ColorNode::removeParent( cocos2d::CCNode* parent )
    {
        parent->removeChild( m_sprite );
    }
    
    void ColorNode::setFrame( cocos2d::CCSpriteFrame* frame )
    {
        assert( m_sprite && "Cannot set the frame of a color node that has no sprite." );
        
        m_sprite->setDisplayFrame( frame );
    }
    
    bool ColorNode::isOnSprite( cocos2d::CCPoint& position ) const
    {
        assert( m_sprite && "Cannot check if a point is on a color node that has no sprite." );
        
        return m_sprite->boundingBox().containsPoint( position );
    }
    
    void ColorNode::setColor( PaletteIndex palette_index )
    {
        assert( m_sprite && "Cannot set the color of a color node that has no sprite." );
        
        m_sprite->setColor( palette[ palette_index ] );
        m_palette_index = palette_index;
    }
    
    bool ColorNode::isCaptured() const
    {
        return m_flags & IsCapturedMask;
    }
    
    void ColorNode::setIsCaptured( bool is_captured )
    {
        if( is_captured )
        {
            capture();
        }
        else
        {
            uncapture();
        }
    }
    
    void ColorNode::capture()
    {
        m_flags |= IsCapturedMask;
        
        setFrame( ColorNodeFrames::get()->captured_frame );
    }
    
    void ColorNode::uncapture()
    {
        m_flags &= ~IsCapturedMask;
        
        setFrame( ColorNodeFrames::get()->deselected_frame );
    }
    
    void ColorNode::toggleCaptured()
    {
        m_flags ^= IsCapturedMask;
        
        setFrame( isCaptured() ? ColorNodeFrames::get()->captured_frame : ColorNodeFrames::get()->deselected_frame );
    }
    
    bool ColorNode::isSelected() const
    {
        return (m_flags & IsSelectedMask) != 0;
    }
    
    void ColorNode::setIsSelected( bool is_selected )
    {
        if( is_selected )
        {
            select();
        }
        else
        {
            deselect();
        }
    }
    
    void ColorNode::select()
    {
        m_flags |= IsSelectedMask;
        
        setFrame( ColorNodeFrames::get()->selected_frame );
    }
    
    void ColorNode::deselect()
    {
        m_flags &= ~IsSelectedMask;
        
        setFrame( isCaptured() ? ColorNodeFrames::get()->captured_frame : ColorNodeFrames::get()->deselected_frame );
    }
    
    void ColorNode::toggleSelect()
    {
        m_flags ^= IsSelectedMask;
        
        setFrame(
                 isSelected() ?
                 ColorNodeFrames::get()->selected_frame :
                 isCaptured() ?
                 ColorNodeFrames::get()->captured_frame :
                 ColorNodeFrames::get()->deselected_frame );
    }
    
} // three_color
