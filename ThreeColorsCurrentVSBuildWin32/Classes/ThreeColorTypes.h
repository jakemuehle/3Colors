// Copywright May 2013
// Author: Derek Higgs
// Purpose: Common types for the three-color game.

#ifndef __3COLOR__TYPES_H
#define __3COLOR__TYPES_H


#define USE_WILD_CARDS_IN_BANK


namespace three_color
{
    
    // Represents which color for a color node to use.
    typedef unsigned char PaletteIndex;
    
    // Special features each have a macro to be easily taken out
#if defined(USE_WILD_CARDS_IN_BANK)
    const PaletteIndex k_wild_card = 3;
#endif
    
} // three_color


#endif // __3COLOR__TYPES_H
