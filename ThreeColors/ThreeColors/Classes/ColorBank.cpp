#include "ColorBank.h"

#include <cassert>
#include <algorithm>

#include "ColorNode.h"
#include "ColorSequence.h"

namespace three_color
{
    
    ColorBank::~ColorBank()
    {
        if( m_sequences )
        {
            for( ColorNode** sequence_iter = m_sequences; sequence_iter != m_sequences + m_number_of_sequences; ++sequence_iter )
            {
                if( *sequence_iter )
                {
                    delete [] *sequence_iter;
                    *sequence_iter = NULL;
                }
            }
            delete [] m_sequences;
            m_sequences = NULL;
        }
        if( m_sequences_length )
        {
            delete [] m_sequences_length;
            m_sequences_length = NULL;
        }
    }
    
    void initNode( ColorNode & node )
    {
        bool is_successful = node.init();
        assert( is_successful && "Failed to initialize a color node for the bank." );
    }
    
    bool ColorBank::init()
    {
        m_sequences = new ColorNode*[m_number_of_sequences];
        assert( m_sequences && "New failed to create the sequence array for the bank." );
        
        m_sequences_length = new unsigned int[m_number_of_sequences];
        assert( m_sequences_length && "New failed to create the sequence length array for the bank." );
        
        m_random_sequence = new PaletteIndex[m_max_sequence_length];
        assert( m_random_sequence && "New failed to create the random sequence for the bank." );
        
        float horizontal_spacing = getHorizontalSpacing(),
		vertical_spacing = getVerticalSpacing(),
		initial_x = horizontal_spacing * 0.5f + m_bounds.getMinX();
        
        // Note that the origin for the points start in the lower left.
        cocos2d::CCPoint current_point( initial_x, vertical_spacing * 0.5f + m_bounds.getMinY() );
        
        for( ColorNode** sequence_iter = m_sequences; sequence_iter != m_sequences + m_number_of_sequences; ++sequence_iter )
        {
            *sequence_iter = new ColorNode[m_max_sequence_length];
            assert( *sequence_iter && "New failed to create a sequence for the color bank." );
            
            std::for_each( *sequence_iter, *sequence_iter + m_max_sequence_length, initNode );
            
            current_point.x = initial_x;
            for( ColorNode* node_iter = *sequence_iter; node_iter != *sequence_iter + m_max_sequence_length; ++node_iter )
            {
                node_iter->setPosition( current_point );
                node_iter->setParent( this, 1 );
                
                current_point.x += horizontal_spacing;
            }
            
            current_point.y += vertical_spacing;
        }
        
        unsigned int* sequence_length_iter = m_sequences_length;
        for( ColorNode** sequence_iter = m_sequences; sequence_iter != m_sequences + m_number_of_sequences; ++sequence_iter )
        {
            *sequence_length_iter++ = setRandomSequence( *sequence_iter );
        }
        
        return true;
    }
    
    float ColorBank::getHorizontalSpacing() const
    {
        unsigned int divisions = m_max_sequence_length + 1;
        return m_bounds.size.width / divisions;
    }
    
    float ColorBank::getVerticalSpacing() const
    {
        unsigned int divisions = m_number_of_sequences + 1;
        return m_bounds.size.height / divisions;
    }
    
    bool ColorBank::select( PaletteIndex selected_colors[], unsigned int length )
    {
        bool found_selection = false;
        
        unsigned int* length_iter = m_sequences_length;
        for( ColorNode** sequence_iter = m_sequences; sequence_iter != m_sequences + m_number_of_sequences; ++sequence_iter, ++length_iter )
        {
            bool matches_forward = false, matches_backward = false;
            
            found_selection |= startsEquivalent( *sequence_iter, *length_iter, selected_colors, length, &matches_forward, &matches_backward);
            
            if( matches_forward )
            {
                for(ColorNode* node_iter = *sequence_iter; node_iter != *sequence_iter + length; ++node_iter)
                {
                    node_iter->select();
                }
            }
            
            if( matches_backward )
            {
                for(ColorNode* node_iter = *sequence_iter + *length_iter - length; node_iter != *sequence_iter + *length_iter; ++node_iter)
                {
                    node_iter->select();
                }
                    
            }
        }
        
        return found_selection;
    }
    
    bool ColorBank::select( ColorNode selected_nodes[], unsigned int length )
    {
        bool found_selection = false;
        
        unsigned int* length_iter = m_sequences_length;
        for( ColorNode** sequence_iter = m_sequences; sequence_iter != m_sequences + m_number_of_sequences; ++sequence_iter, ++length_iter )
        {
            bool matches_forward = false, matches_backward = false;
            
            found_selection |= startsEquivalent( *sequence_iter, *length_iter, selected_nodes, length, &matches_forward, &matches_backward);
            
            if( matches_forward )
            {
                for(ColorNode* node_iter = *sequence_iter; node_iter != *sequence_iter + length; ++node_iter)
                {
                    node_iter->select();
                }
            }
            
            if( matches_backward )
            {
                for(ColorNode* node_iter = *sequence_iter + *length_iter - length; node_iter != *sequence_iter + *length_iter; ++node_iter)
                {
                    node_iter->select();
                }
                
            }
        }
        
        return found_selection;
    }
    
    void ColorBank::deselectAll()
    {
        for( ColorNode** sequence_iter = m_sequences; sequence_iter != m_sequences + m_number_of_sequences; ++sequence_iter )
        {
            for( ColorNode* node_iter = *sequence_iter; node_iter != *sequence_iter + m_max_sequence_length; ++node_iter)
            {
                node_iter->deselect();
            }
        }
    }
    
    bool ColorBank::unselect( PaletteIndex remaining_selected_colors[], unsigned int length)
    {
        deselectAll();
        return select(remaining_selected_colors, length);
    }
    
    bool ColorBank::unselect( ColorNode remaining_selected_nodes[], unsigned int length)
    {
        deselectAll();
        return select(remaining_selected_nodes, length);
    }
    
    bool ColorBank::makeSelection( PaletteIndex selected_colors[], unsigned int length )
    {
        ColorNode** sequence_iter = m_sequences;
        for( unsigned int* length_iter = m_sequences_length; sequence_iter != m_sequences + m_number_of_sequences; ++sequence_iter, ++length_iter )
        {
            if( isEquivalent( *sequence_iter, *length_iter, selected_colors, length ))
            {
                removeSequence(sequence_iter, length_iter);
                // Sets the last sequence random
                m_sequences_length[m_number_of_sequences-1]=setRandomSequence(m_sequences[m_number_of_sequences-1]);
                return true;
            }
        }
        return false;
    }
    
    bool ColorBank::makeSelection( ColorNode selected_nodes[], unsigned int length )
    {
        ColorNode** sequence_iter = m_sequences;
        for( unsigned int* length_iter = m_sequences_length; sequence_iter != m_sequences + m_number_of_sequences; ++sequence_iter, ++length_iter )
        {
            if( isEquivalent( selected_nodes, length, *sequence_iter, *length_iter ))
            {
                removeSequence(sequence_iter, length_iter);
                // Sets the last sequence random
                m_sequences_length[m_number_of_sequences-1]=setRandomSequence(m_sequences[m_number_of_sequences-1]);
                return true;
            }
        }
        return false;
    }
    
    unsigned int ColorBank::setRandomSequence( ColorNode* sequence )
    {
        unsigned int* sequence_length_iter;
        
        unsigned int length =
        getRandomSequence( m_number_of_colors, m_min_sequence_length, m_max_sequence_length, m_random_sequence );
        
    checkIfSequenceIsUnique:
        
        sequence_length_iter = m_sequences_length;
        // Checks up to the sequence being set, since new sequences are at higher indexes.
        // This is so in initialization can use this code with no change.
        for( ColorNode** sequence_iter = m_sequences; *sequence_iter != sequence; ++sequence_iter )
        {
            if( isEquivalent( *sequence_iter, *sequence_length_iter, m_random_sequence, length ) )
            {
                length = makeARandomChange( m_number_of_colors, m_min_sequence_length, m_max_sequence_length, length, m_random_sequence );
                goto checkIfSequenceIsUnique;
            }
        }
        
        // Is unique, so it can be added.
        setColorNodes( m_random_sequence, length, sequence );
        
        return length;
    }
    
    void ColorBank::removeSequence( ColorNode** sequence_ptr, unsigned int* length_ptr )
    {
        ColorNode* to_be_last_sequence = *sequence_ptr;
        unsigned int to_be_last_sequence_length = *length_ptr;
        float current_y = (**sequence_ptr).getPosition().y;
        
        unsigned int* length_iter = length_ptr;
        ColorNode** sequence_iter = sequence_ptr;
        // Shift all of the sequences from 
        for(; sequence_iter != m_sequences + m_number_of_sequences - 1; ++sequence_iter, ++length_iter )
        {
            *sequence_iter = *(sequence_iter+1);
            *length_iter = *(length_iter+1);
            
            float next_y = (**sequence_iter).getPosition().y;
            for( ColorNode* node_iter = *sequence_iter; node_iter != *sequence_iter + m_max_sequence_length; ++node_iter)
            {
                node_iter->setPositionY(current_y);
            }
            current_y = next_y;
        }
        // Loop exits at the last sequence and current_y is set to the height of the
        // old last sequence.
        *sequence_iter = to_be_last_sequence;
        *length_iter = to_be_last_sequence_length;
        
        for( ColorNode* node_iter = to_be_last_sequence; node_iter != to_be_last_sequence + m_max_sequence_length; ++node_iter)
        {
            node_iter->setPositionY(current_y);
        }
    }
    
} // three_color
