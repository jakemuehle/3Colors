#include "ColorBank.h"

#include <cassert>
#include <algorithm>

#include "ColorNode.h"
#include "ColorSequence.h"

#if defined(PLAYER_CAN_CHOOSE_TO_SHUFFLE_A_NODE_ONCE_EVERY_TURN) || defined(USE_WILD_CARDS_IN_BANK)
#include "my_math.h"
#endif

namespace three_color
{
#if defined(USE_WILD_CARDS_IN_BANK)
    const unsigned int k_turn_to_send_wild_card = 5;
#endif
    
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
        //////////////////////////////
        // 1. super init first
        if ( !CCLayer::init() )
        {
            return false;
        }
        
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
        
#if defined(PLAYER_CAN_CHOOSE_TO_SHUFFLE_A_NODE_ONCE_EVERY_TURN)
        m_bTouchEnabled = true;
#endif
        
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
            
            found_selection |= startsEquivalent( *sequence_iter, *length_iter, selected_colors, length,
#if defined(USE_WILD_CARDS_IN_BANK)
                                                true, NULL,
#endif
                                                &matches_forward, &matches_backward
                                                );
            
            if( matches_forward )
            {
                for(ColorNode* node_iter = *sequence_iter; node_iter != *sequence_iter + length; ++node_iter)
                {
                    node_iter->select();
                }
				continue;
            }
            else if( matches_backward )
            {
                for(ColorNode* node_iter = *sequence_iter + *length_iter - length; node_iter != *sequence_iter + *length_iter; ++node_iter)
                {
                    node_iter->select();
                }
                continue;
            }
        }
        
        return found_selection;
    }
    
    bool ColorBank::select( ColorNode selected_nodes[], unsigned int length )
    {
        // Do this for now until I go into searching to deselect
        deselectAll();
        
        bool found_selection = false;
        
        unsigned int* length_iter = m_sequences_length;
        for( ColorNode** sequence_iter = m_sequences; sequence_iter != m_sequences + m_number_of_sequences; ++sequence_iter, ++length_iter )
        {
            bool matches_forward = false, matches_backward = false;
            
            found_selection |= startsEquivalent( *sequence_iter, *length_iter, selected_nodes, length,
#if defined(USE_WILD_CARDS_IN_BANK)
                                                true, NULL,
#endif
                                                &matches_forward, &matches_backward
                                                );
            
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
#if defined(USE_WILD_CARDS_IN_BANK)
        ColorNode** sequence_matching_with_wild_card_ptr = NULL;
        unsigned int* sequence_matching_with_wild_card_length_ptr = NULL;
        bool has_wild_card_matching(false);
#endif
        for( unsigned int* length_iter = m_sequences_length; sequence_iter != m_sequences + m_number_of_sequences; ++sequence_iter, ++length_iter )
        {
            if( isEquivalent( *sequence_iter, *length_iter, selected_colors, length
#if defined(USE_WILD_CARDS_IN_BANK)
                             , true, &has_wild_card_matching
#endif
                             ))
            {
#if defined(USE_WILD_CARDS_IN_BANK)
                if( has_wild_card_matching )
                {
                    sequence_matching_with_wild_card_ptr = sequence_iter;
                    sequence_matching_with_wild_card_length_ptr = length_iter;
                    has_wild_card_matching = false;
                    continue; // Keep looking for a matching sequence without a wild card.
                }
                else
                {
                    // Increment the wild card counter here when a turn is passed
                    ++m_last_sent_wild_card;
                }
#endif
                removeSequence(sequence_iter, length_iter);
                // Sets the last sequence random
                m_sequences_length[m_number_of_sequences-1]=setRandomSequence(m_sequences[m_number_of_sequences-1]);
                
#if defined(PLAYER_CAN_CHOOSE_TO_SHUFFLE_A_NODE_ONCE_EVERY_TURN)
                m_can_shufle_node = true;
#endif
                return true;
            }
        }
#if defined(USE_WILD_CARDS_IN_BANK)
        if( sequence_matching_with_wild_card_ptr )
        {
            // Increment the wild card counter here when a turn is passed
            ++m_last_sent_wild_card;
            removeSequence(sequence_matching_with_wild_card_ptr, sequence_matching_with_wild_card_length_ptr);
            // Sets the last sequence random
            m_sequences_length[m_number_of_sequences-1]=setRandomSequence(m_sequences[m_number_of_sequences-1]);
        }
#endif
        return false;
    }
    
    bool ColorBank::makeSelection( ColorNode selected_nodes[], unsigned int length )
    {
        ColorNode** sequence_iter = m_sequences;
#if defined(USE_WILD_CARDS_IN_BANK)
        ColorNode** sequence_matching_with_wild_card_ptr = NULL;
        unsigned int* sequence_matching_with_wild_card_length_ptr = NULL;
        bool has_wild_card_matching(false);
#endif
        for( unsigned int* length_iter = m_sequences_length; sequence_iter != m_sequences + m_number_of_sequences; ++sequence_iter, ++length_iter )
        {
            if( isEquivalent( selected_nodes, length, *sequence_iter, *length_iter
#if defined(USE_WILD_CARDS_IN_BANK)
                             , true, &has_wild_card_matching
#endif
                             ))
            {
#if defined(USE_WILD_CARDS_IN_BANK)
                if( has_wild_card_matching )
                {
                    sequence_matching_with_wild_card_ptr = sequence_iter;
                    sequence_matching_with_wild_card_length_ptr = length_iter;
                    has_wild_card_matching = false;
                    continue; // Keep looking for a matching sequence without a wild card.
                }
                else
                {
                    // Increment the wild card counter here when a turn is passed
                    ++m_last_sent_wild_card;
                }
#endif
                removeSequence(sequence_iter, length_iter);
                // Sets the last sequence random
                m_sequences_length[m_number_of_sequences-1]=setRandomSequence(m_sequences[m_number_of_sequences-1]);
                
#if defined(PLAYER_CAN_CHOOSE_TO_SHUFFLE_A_NODE_ONCE_EVERY_TURN)
                m_can_shufle_node = true;
#endif
                return true;
            }
        }
#if defined(USE_WILD_CARDS_IN_BANK)
        if( sequence_matching_with_wild_card_ptr )
        {
            // Increment the wild card counter here when a turn is passed
            ++m_last_sent_wild_card;
            removeSequence(sequence_matching_with_wild_card_ptr, sequence_matching_with_wild_card_length_ptr);
            // Sets the last sequence random
            m_sequences_length[m_number_of_sequences-1]=setRandomSequence(m_sequences[m_number_of_sequences-1]);
        }
#endif
        return false;
    }
    
#if defined(PLAYER_CAN_CHOOSE_TO_SHUFFLE_A_NODE_ONCE_EVERY_TURN)
    bool ColorBank::ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
    {
        if( ! m_can_shufle_node )
        {
            return false;
        }
        
        cocos2d::CCPoint point = pTouch->getLocationInView();
        point = cocos2d::CCDirector::sharedDirector()->convertToGL(point);
        
        // Find the node that is selected
        unsigned int column = (unsigned int)((point.x - m_bounds.getMinX()) /getHorizontalSpacing()),
            row = (unsigned int)((point.y - m_bounds.getMinY()) / getVerticalSpacing()),
            * selected_row_length_ptr(m_sequences_length + row);
        
        // Casting a negative number to an unsigned int should make the number huge, so checking less than zero is covered.
        if( column >= *selected_row_length_ptr || row >= m_number_of_sequences )
        {
            // Out of bounds
            return false;
        }
        
        ColorNode** selected_row_ptr = m_sequences + row;
        ColorNode& selected_node = (*selected_row_ptr)[column];
        if( ! selected_node.isOnSprite(point) )
        {
            return false;
        }
        
        // Chose the new color
        PaletteIndex new_color = my_utility::random(m_number_of_colors-1);
        if( new_color >= selected_node.getPaletteIndex() )
        {
            ++new_color;
        }
        
        selected_node.setColor(new_color);
        m_can_shufle_node = false;
        
        // Check to see if the new sequence is unique.
        ColorNode** sequence_iter = m_sequences;
        for( unsigned int* length_iter = m_sequences_length; sequence_iter != m_sequences + m_number_of_sequences; ++sequence_iter, ++length_iter )
        {
            if(  sequence_iter != selected_row_ptr &&
               isEquivalent(*selected_row_ptr, *selected_row_length_ptr, *sequence_iter, *length_iter ))
            {
                // Found a match
                removeSequence(selected_row_ptr, selected_row_length_ptr);
                m_sequences_length[m_number_of_sequences-1]=setRandomSequence(m_sequences[m_number_of_sequences-1]);
                break;
            }
        }
        
        return true;
    }
    
    void ColorBank::registerWithTouchDispatcher()
    {
        cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,0,true);
    }
#endif
    
    
    unsigned int ColorBank::setRandomSequence( ColorNode* sequence )
    {
        unsigned int* sequence_length_iter;
        
        unsigned int length =
        getRandomSequence( m_number_of_colors, m_min_sequence_length, m_max_sequence_length, m_unused_count, 0.1f, m_random_sequence );
        
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
#if defined(USE_WILD_CARDS_IN_BANK)
        // The wild card counter is not incremented here because this is called in initialization. It is incremented when a turn is passed upon finding a sequence.
        if( m_last_sent_wild_card == k_turn_to_send_wild_card )
        {
            unsigned int index_to_put_wild_card = my_utility::random(length);
            m_random_sequence[ index_to_put_wild_card ] = k_wild_card;
            m_last_sent_wild_card = 0;
        }
#endif
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
