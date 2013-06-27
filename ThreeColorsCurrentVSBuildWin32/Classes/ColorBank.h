// Copywright May 2013
// Author: Derek Higgs
// Purpose: Handles the available color sequences.

#ifndef __3COLOR__COLOR_BANK_H
#define __3COLOR__COLOR_BANK_H

#include "cocos2d.h"

#include "ThreeColorTypes.h"

// Special features each have a macro to be easily taken out
#define PLAYER_CAN_CHOOSE_TO_SHUFFLE_A_NODE_ONCE_EVERY_TURN

namespace three_color
{
    
    class ColorNode;
    class ColorCount;
    
    class ColorBank : public cocos2d::CCLayer
    {
    public:
        
        ColorBank(
                  const cocos2d::CCRect bounds,
                  unsigned int number_of_sequences,
                  unsigned int number_of_colors,
                  unsigned int min_sequence_length,
                  unsigned int max_sequence_length )
        : m_bounds( bounds ),
        m_sequences( NULL ),
        m_sequences_length( NULL ),
        m_random_sequence( NULL ),
        m_number_of_sequences( number_of_sequences ),
        m_number_of_colors( number_of_colors ),
        m_min_sequence_length( min_sequence_length ),
        m_max_sequence_length( max_sequence_length )
#if defined(USE_WILD_CARDS_IN_BANK)
        , m_last_sent_wild_card( 0 )
#endif
#if defined(PLAYER_CAN_CHOOSE_TO_SHUFFLE_A_NODE_ONCE_EVERY_TURN)
        , m_can_shufle_node( false )
#endif
        {}
        
        ~ColorBank();
        
        // Call init before using.
        bool init();
        
        // Calculates the horizontal spacing between nodes.
        float getHorizontalSpacing() const;
        // Calculates the vertical spacing between nodes.
        float getVerticalSpacing() const;
        
        // Call to select the specified nodes.
        // Returns if the nodes is a valid incomplete selection.
        bool select( PaletteIndex selected_colors[], unsigned int length );
        bool select( ColorNode selected_nodes[], unsigned int length );
        
        // Deselects all nodes.
        void deselectAll();
        
        // Unselects to the specified nodes.
        // Returns if the nodes is a valid incomplete selection.
        bool unselect( PaletteIndex remaining_selected_colors[], unsigned int length);
        bool unselect( ColorNode remaining_selected_nodes[], unsigned int length);
        
        // Make the selection.
        // Returns if the nodes are valid.
        bool makeSelection( PaletteIndex selected_colors[], unsigned int length );
        bool makeSelection( ColorNode selected_nodes[], unsigned int length );
        
#if defined(PLAYER_CAN_CHOOSE_TO_SHUFFLE_A_NODE_ONCE_EVERY_TURN)
        bool ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
        
        void registerWithTouchDispatcher();
#endif
        
        inline void setUnusedColors(const ColorCount * unused_count)
        {
            m_unused_count = unused_count;
        }
        
    private:
        
        const cocos2d::CCRect m_bounds;
        
        const ColorCount * m_unused_count;
        
        // An array containing color sequences, which is also an array.
        // Two dimentional to allow swapping the sequences and shifting visual positions
        // when a sequence is selected.
        ColorNode** m_sequences;
        unsigned int* m_sequences_length;
        
        PaletteIndex * m_random_sequence;
        
        const unsigned int m_number_of_sequences,
		m_number_of_colors,
		m_min_sequence_length,
		m_max_sequence_length;
        
#if defined(USE_WILD_CARDS_IN_BANK)
        unsigned int m_last_sent_wild_card;
#endif
        
#if defined(PLAYER_CAN_CHOOSE_TO_SHUFFLE_A_NODE_ONCE_EVERY_TURN)
        bool m_can_shufle_node;
#endif
        
        unsigned int setRandomSequence( ColorNode* sequence );
        void removeSequence( ColorNode** sequence_ptr, unsigned int* length_ptr );
        
        ColorBank();
        ColorBank(const ColorBank &);
        void operator=(const ColorBank &);
        
    }; // ColorBank
    
} // three_color

#endif // __3COLOR__COLOR_BANK_H
