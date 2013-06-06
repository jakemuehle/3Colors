#include "ColorNodeFrames.h"

#include <cassert>

#include "cocos2d.h"

namespace three_color
{
	ColorNodeFrames* ColorNodeFrames::s_singleton = NULL;

	namespace
	{
		const char* color_node_sprite_sheet_name = "ColorNode.plist",
			* color_node_deselected_frame_name = "ColorNodeDeselected.png",
			* color_node_selected_frame_name = "ColorNodeSelected.png",
			* color_node_captured_frame_name = "ColorNodeSelected.png";
	}
	
	bool ColorNodeFrames::init()
	{
    cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile( color_node_sprite_sheet_name );

		cocos2d::CCSpriteFrame
			* deselected_frame =
					cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
							color_node_deselected_frame_name
					),
			* selected_frame =
					cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
							color_node_selected_frame_name
					),
			* captured_frame =
					cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
							color_node_captured_frame_name
					);

		s_singleton = new ColorNodeFrames( deselected_frame, selected_frame, captured_frame );
		assert( s_singleton && "Color Node Frames failed to initialize." );

		deselected_frame->retain();
		selected_frame->retain();
		captured_frame->retain();

		return true;
	}

	ColorNodeFrames::~ColorNodeFrames()
	{
		if( deselected_frame )
		{
			deselected_frame->release();
		}
		if( selected_frame )
		{
			selected_frame->release();
		}
		if( captured_frame )
		{
			captured_frame->release();
		}
	}

	ColorNodeFrames* ColorNodeFrames::get()
	{
		if( ! s_singleton )
		{
			init();
		}
		assert( s_singleton );

		return s_singleton;
	}

	void ColorNodeFrames::shutdown()
	{
		if( s_singleton )
		{
			delete s_singleton;
			s_singleton = NULL;
		}
	}

} // three_color
