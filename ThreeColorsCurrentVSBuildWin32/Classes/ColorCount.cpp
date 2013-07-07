//
//  ColorCount.cpp
//  ThreeColors
//
//  Created by Derek Higgs on 6/26/13.
//
//

#include "ColorCount.h"

#include <math.h>
#include "my_math.h"

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
    
    PaletteIndex ColorCount::getRandomColor(float minimal_percent) const
    {
        float * current_percent = new float[m_number_of_colors],
            * percent_chance_array = new float[m_number_of_colors],
            * cumulialtive_percent = new float[m_number_of_colors];
        unsigned int number_below_min(0),
            total(getTotal());
        
        // Calculate the current percents and count how many are below the min
        float * float_write_iter;
        float_write_iter = current_percent;
        for(int * count_iter=m_quantity_by_color;count_iter!=m_quantity_by_color+m_number_of_colors;++count_iter,++float_write_iter)
        {
            *float_write_iter = ((float)*count_iter)/total;
            
            if(*float_write_iter<minimal_percent)
            {
                ++number_below_min;
            }
        }
        
        // Assign the percents based on either the min or a percent of what is left after assigning the mins
        {
            float remaining_percent = 1.0f - number_below_min*minimal_percent;
            float_write_iter = percent_chance_array;
            for(float*percent_iter=current_percent;percent_iter!=current_percent+m_number_of_colors;++percent_iter,++float_write_iter)
            {
                float current = *percent_iter;
                *float_write_iter = (current<minimal_percent) ?
                    minimal_percent :
                    current * remaining_percent;
            }
        }
        
        // Cumulate the percents
        cumulialtive_percent[0]=percent_chance_array[0];
        for(int i=1; i<=m_number_of_colors;++i)
        {
            cumulialtive_percent[i]=percent_chance_array[i]+cumulialtive_percent[i-1];
        }
        
        // Get the random color
        PaletteIndex random_color(0);
        {
            float rand = my_utility::random();
            for(float*chance_iter=cumulialtive_percent;chance_iter!=cumulialtive_percent+m_number_of_colors;++chance_iter,++random_color)
            {
                if(rand<*chance_iter)
                {
                    break;
                }
            }
        }
        
        // delete allocations
        delete [] cumulialtive_percent;
        delete [] percent_chance_array;
        delete [] current_percent;
        
        return random_color;
    }

	void ColorCount::resetAll()
	{
		for(int i = 0; i < m_number_of_colors; ++i)
			resetColor(i);
	}
}