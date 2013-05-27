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

bool ColorBank::makeSelection( ColorNode nodes[], BankPossibleMatches & possible_matches )
{



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

} // three_color
