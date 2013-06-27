//
//  ColorCount.h
//  ThreeColors
//
//  Created by Derek Higgs on 6/26/13.
//
// Purpose: Keeps a tally of colors on the grid.
//          Used to bias the color bank.

#ifndef __ThreeColors__ColorCount__
#define __ThreeColors__ColorCount__

#include "ThreeColorTypes.h"

#ifndef NULL
#define NULL 0
#endif // NULL

namespace three_color
{
    class ColorCount
    {
    public:
        ColorCount(unsigned int number_of_colors);
        
        inline ~ColorCount()
        {
            if(m_quantity_by_color)
            {
                delete [] m_quantity_by_color;
                m_quantity_by_color = NULL;
            }
        }
        
        // Changes the quantity of the color by count.
        // Returns the resulting value.
        inline int tally(PaletteIndex color, int count=1)
        {
            return m_quantity_by_color[color]+=count;
        }
        
        inline int getQuantity(PaletteIndex color) const
        {
            return m_quantity_by_color[color];
        }
        
        int getTotal() const;
        
        // Gets a random color from the colors stored, but guarantees the minimal_percent for each color.
        PaletteIndex getRandomColor(float minimal_percent) const;
    
    private:
        int * m_quantity_by_color;
        const unsigned int m_number_of_colors;
        
        // Copy and assigned disabled until properly implemented with making a deep copy of the array.
        ColorCount(const ColorCount &);
        ColorCount operator=(const ColorCount &);
    };
}

#endif /* defined(__ThreeColors__ColorCount__) */
