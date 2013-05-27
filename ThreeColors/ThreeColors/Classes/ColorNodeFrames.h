#ifndef __3COLOR__COLOR_NODE_FRAMES_H
#define __3COLOR__COLOR_NODE_FRAMES_H

namespace cocos2d
{
class CCSpriteFrame;
} // cocos2d

namespace three_color
{
	class ColorNodeFrames
	{
	public:
		static ColorNodeFrames* get();
		static bool init();
		static void shutdown();

		cocos2d::CCSpriteFrame
			* const deselected_frame,
			* const selected_frame,
			* const captured_frame;

	private:

		static ColorNodeFrames* s_singleton;

		ColorNodeFrames(
				cocos2d::CCSpriteFrame* i_deselected_frame,
				cocos2d::CCSpriteFrame* i_selected_frame,
				cocos2d::CCSpriteFrame* i_captured_frame )
			: deselected_frame( i_deselected_frame ),
				selected_frame( i_selected_frame ),
				captured_frame( i_captured_frame ) {}

		~ColorNodeFrames();

		ColorNodeFrames();
		ColorNodeFrames(const ColorNodeFrames &);
		void operator=(const ColorNodeFrames &);

	}; // ColorNodeFrames
} // three_color

#endif // __3COLOR__COLOR_NODE_FRAMES_H
