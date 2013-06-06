// Copywright May 2013
// Author: Derek Higgs
// Purpose: Handles the available color sequences.

#ifndef __3COLOR__COLOR_BANK_H
#define __3COLOR__COLOR_BANK_H

#include "cocos2d.h"

#include "ThreeColorTypes.h"

namespace three_color
{
    
    class ColorNode;
    
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
        m_max_sequence_length( max_sequence_length ) {}
        
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
        
    private:
        
        const cocos2d::CCRect m_bounds;
        
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
        
        unsigned int setRandomSequence( ColorNode* sequence );
        void removeSequence( ColorNode** sequence_ptr, unsigned int* length_ptr );
        
        ColorBank();
        ColorBank(const ColorBank &);
        void operator=(const ColorBank &);
        
    }; // ColorBank
    
} // three_color

#endif // __3COLOR__COLOR_BANK_H
