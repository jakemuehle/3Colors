#ifndef __3COLOR__COLOR_SEQUENCE_H
#define __3COLOR__COLOR_SEQUENCE_H

#include "cocos2d.h"

#include "ThreeColorTypes.h"

namespace three_color
{

class ColorNode;

// Gets a random sequence of colors ranging from pallet index 0 (inclusive) to i_numberOfColors (exclusive).
// The length of the sequence outputted is between i_min_length and i_max_length, both inclusive.
// The output, o_sequence, is an array, allocated by the caller, that contains the random sequence.
// Returns the lenght of the sequence.
unsigned int getRandomSequence(
		unsigned int i_number_of_colors,
		unsigned int i_min_length,
		unsigned int i_max_length,
		PaletteIndex o_sequence[] );

unsigned int makeARandomChange(
		unsigned int i_number_of_colors,
		unsigned int i_min_length,
		unsigned int i_max_length,
		unsigned int i_current_length,
		PaletteIndex o_sequence[] );

// Sets the color nodes with the specified colors of i_sequence of length i_length.
// The palette defines what the palette indexes mean.
// The output, o_nodes, should already be allocated and initialized by the caller.
void setColorNodes(
		PaletteIndex i_sequence[],
		unsigned int i_length,
		ColorNode o_nodes[] );

// Checks if the nodes' colors starts with or ends with the color sequence.
bool startsEquivalent(
	ColorNode i_nodes[], unsigned int i_node_length,
	PaletteIndex i_sequence[], unsigned int i_sequence_length,
	bool * o_does_match_forward = NULL, bool * o_does_match_backward = NULL );

// Checks if the nodes' colors is the same as the sequence going forwards or backwards.
bool isEquivalent(
	ColorNode i_nodes[], unsigned int i_node_length,
	PaletteIndex i_sequence[], unsigned int i_sequence_length );

// Checks if the nodes a's colors starts with or ends with the color sequence.
bool startsEquivalent(
	ColorNode i_nodes_a[], unsigned int i_node_length_a,
	ColorNode i_nodes_b[], unsigned int i_node_length_b,
	bool * o_does_match_forward = NULL, bool * o_does_match_backward = NULL );

// Checks if the nodes' colors is the same as the sequence going forwards or backwards.
bool isEquivalent(
	ColorNode i_nodes_a[], unsigned int i_node_length_a,
	ColorNode i_nodes_b[], unsigned int i_node_length_b );

}

#endif // __3COLOR__RANDOM_COLOR_H
