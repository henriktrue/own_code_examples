/*
	This code section demonstrates how to store a struct in the heap memory section
*/
	
//initialisation from main:
	MA_ADC = NewMA_Filter(10);
    MA_Temp = NewMA_Filter(10);
    MA_Duty = NewMA_Filter(100);
	
//initilisation from header file with struct details
	typedef enum{
    MA_FILTER_OK = 0,
    MA_FILTER_FILTER_WAS_NULLPTR = -1,
    MA_FILTER_FILTER_READINGS_WAS_NULLPTR = -2,
	} MA_FILTER_RESPONSES;

	typedef struct
	{
	  uint8_t points;
	  uint16_t input;
	  uint16_t total;
	  uint16_t writeIndex;
	  uint16_t * readings;
	} MA_Filter;

	
//main two functions used
	MA_FILTER_RESPONSES MA_filter_calculate(uint16_t input, uint16_t * output, MA_Filter * Filter)
	{
		//write pointer should point at readings start address + offset, where offset is element size * index
		//unless it's the last element or overflow, then start from beginning
		uint16_t * WritePtr;
		uint16_t old;

		if(Filter == NULL)
			return MA_FILTER_FILTER_WAS_NULLPTR;

		if(Filter->readings == NULL)
			return MA_FILTER_FILTER_READINGS_WAS_NULLPTR;

		if(Filter->writeIndex < Filter->points)
		{
			WritePtr = Filter->readings + ( Filter->writeIndex * sizeof(uint16_t));
		}
		else
		{
			WritePtr = Filter->readings;
			Filter->writeIndex = 0;
		}

		uint16_t sum;

		//assign value to the location pointed at
		old = (*WritePtr);
		if (old < sum)
			(*WritePtr) = input;

		//.. do math
		// (*output) = sum(readings, 0 , points) / points
		sum = (Filter->total + input - old);

		Filter->total = sum;
		(*output) = (sum * 10) / Filter->points;

		//increment for nest calculation
		Filter->writeIndex++;

		return MA_FILTER_OK;
	}

	MA_Filter * NewMA_Filter(uint8_t points )
	{
		MA_Filter * Filter = (MA_Filter*) malloc( sizeof(MA_Filter) );
		Filter->readings = (uint16_t*) malloc( points * sizeof(uint16_t) );
		Filter->points = points;
		Filter->total = 0;
		memset(Filter->readings, 0, Filter->points * sizeof(uint16_t));
		Filter->writeIndex = 0;

		return Filter;
	}

//usage of these 2 functions, inputs arguments are of type uint16_t
    MA_filter_calculate(battery, &battery_sum, MA_ADC);
	
