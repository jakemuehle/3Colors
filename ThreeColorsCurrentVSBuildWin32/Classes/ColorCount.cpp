//
//  ColorCount.cpp
//  ThreeColors
//
//  Created by Derek Higgs on 6/26/13.
//
//

#include "ColorCount.h"

namespace three_color
{
    ColorCount::ColorCount(unsigned int number_of_colors) :
        m_quantity_by_color(new int[number_of_colors]),
        m_number_of_colors(number_of_colors)
    {
        for(int * iter=m_quantity_by_color;iter!=m_quantity_by_color+number_of_colors;++iter)
        {
            *iter=0;
        }
    }
    
    int ColorCount::getTotal() const
    {
        unsigned int sum(0);
        for(int * iter=m_quantity_by_color; iter!=m_quantity_by_color+m_number_of_colors;++iter)
        {
            sum+=*iter;
        }
        return sum;
    }
}