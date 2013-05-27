#include "ColorSequence.h"

#include "my_math.h"
#include "ColorNode.h"

namespace three_color
{
	
unsigned int getRandomSequence(
		unsigned int i_number_of_colors,
		unsigned int i_min_length,
		unsigned int i_max_length,
		PaletteIndex o_sequence[] )
{
	unsigned int lengthRange = i_max_length - i_min_length,
		length = i_min_length;

	if( lengthRange != 0 )
	{
		length += my_utility::random( lengthRange + 1 ); // Add one so that the max can be obtained.
	}

	for( PaletteIndex * iter = o_sequence; iter != o_sequence + length; ++iter )
	{
		*iter = my_utility::random( i_number_of_colors );
	}

	return length;
}

unsigned int makeARandomChange(
		unsigned int i_number_of_colors,
		unsigned int i_min_length,
		unsigned int i_max_length,
		unsigned int i_current_length,
		PaletteIndex o_sequence[] )
{
	o_sequence[ my_utility::random( i_current_length ) ] = my_utility::random( i_number_of_colors );

	return i_current_length;
}

void setColorNodes(
		PaletteIndex i_sequence[],
		unsigned int i_length,
		ColorNode o_nodes[] )
{
	ColorNode* node_iter( o_nodes );
	for( PaletteIndex* sequence_iter = i_sequence; sequence_iter != i_sequence + i_length; ++sequence_iter, ++node_iter )
	{
		node_iter->setColor( *sequence_iter );
	}
}

bool startsEquivalent(
	ColorNode i_nodes[], unsigned int i_node_length,
	PaletteIndex i_sequence[], unsigned int i_sequence_length,
	bool * o_does_match_forward )
{
	if( i_node_length < i_sequence_length )
	{
		// Sequence is too long
		return false;
	}

	// Check forwards
	ColorNode* node_iter = i_nodes;
	for( PaletteIndex* sequence_iter = i_sequence; sequence_iter != i_sequence + i_sequence_length; ++sequence_iter, ++node_iter )
	{
		if( node_iter->getPaletteIndex() != *sequence_iter )
		{
			// Found a difference, so it doesn't match forwards.
			goto checkBackwards;
		}
	}

	// All of the elements of sequence match, so the nodes starts with the sequence.
	if( o_does_match_forward )
	{
		*o_does_match_forward = true;
	}
	return true;

checkBackwards:

	// Check backwards
	node_iter = i_nodes + i_node_length - 1;
	for( PaletteIndex* sequence_iter = i_sequence; sequence_iter != i_sequence + i_sequence_length; ++sequence_iter, --node_iter )
	{
		if( node_iter->getPaletteIndex() != *sequence_iter )
		{
			// Found a difference, so it doesn't match backwards either.
			return false;
		}
	}
	
	// All of the elements of sequence match, so the nodes ends with the sequence.
	if( o_does_match_forward )
	{
		*o_does_match_forward = false;
	}
	return true;
}

bool isEquivalent(
	ColorNode nodes[], unsigned int node_length,
	PaletteIndex sequence[], unsigned int sequence_length )
{
	// If they have the same length and starts equivalent, then they are completly eqivalent.
	return node_length == sequence_length && startsEquivalent( nodes, node_length, sequence, sequence_length );
}

bool startsEquivalent(
	ColorNode i_nodes_a[], unsigned int i_node_length_a,
	ColorNode i_nodes_b[], unsigned int i_node_length_b,
	bool * o_does_match_forward )
{
	if( i_node_length_a < i_node_length_b )
	{
		// Sequence b is longer, so a isn't long enough to start equivalent to b.
		return false;
	}

	// Check forwards
	ColorNode* node_a_iter = i_nodes_a;
	for( ColorNode* node_b_iter = i_nodes_b; node_b_iter != i_nodes_b + i_node_length_b; ++node_b_iter, ++node_a_iter )
	{
		if( node_a_iter->getPaletteIndex() != node_b_iter->getPaletteIndex() )
		{
			// Found a difference, so it doesn't match forwards.
			goto checkBackwards;
		}
	}

	// All of the elements of sequence match, so the nodes starts with the sequence.
	if( o_does_match_forward )
	{
		*o_does_match_forward = true;
	}
	return true;

checkBackwards:

	// Check backwards
	 node_a_iter = i_nodes_a + i_node_length_a - 1;
	for( ColorNode* node_b_iter = i_nodes_b; node_b_iter != i_nodes_b + i_node_length_b; ++node_b_iter, --node_a_iter )
	{
		if( node_a_iter->getPaletteIndex() != node_b_iter->getPaletteIndex() )
		{
			// Found a difference, so it doesn't match backwards either.
			return false;
		}
	}
	
	// All of the elements of sequence match, so the nodes ends with the sequence.
	if( o_does_match_forward )
	{
		*o_does_match_forward = false;
	}
	return true;
}

bool isEquivalent(
	ColorNode nodes_a[], unsigned int node_length_a,
	ColorNode nodes_b[], unsigned int node_length_b )
{
	// If they have the same length and starts equivalent, then they are completly eqivalent.
	return node_length_a == node_length_b && startsEquivalent( nodes_a, node_length_a, nodes_b, node_length_b );
}

}
