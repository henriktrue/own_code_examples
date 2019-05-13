/*
	This first part demonstrates how to store a struct in the stack memory section
*/

//main setup:
    uint16_t adcBuffer[10];
    uint16_t tempBuffer[10];
    uint16_t dutyBuffer[100];

    NewMA_Filter(adcBuffer, 10, &MA_ADC);
    NewMA_Filter(tempBuffer, 10, &MA_Temp);
    NewMA_Filter(dutyBuffer, 100, &MA_Duty);
	
//from ports.cpp
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
		(*WritePtr) = input;

		//.. do math
		// (*output) = sum(readings, 0 , points) / points

		uint16_t test = old;
		uint16_t test2 = Filter->total;

		if( test > test2)
		{
			sum = input;
		}
		else
		{
			sum = (Filter->total - old) + input ;
		}

		Filter->total = sum;
		(*output) = (sum) / Filter->points;

		//increment for nest calculation
		Filter->writeIndex++;

		return MA_FILTER_OK;
	}

	MA_Filter * NewMA_Filter( uint16_t * buffer, uint8_t bufferLength, MA_Filter * Filter )
	{
		Filter->readings = buffer;
		Filter->points = bufferLength;
		Filter->total = 0;
		Filter->writeIndex = 0;

		memset(Filter->readings, 0, Filter->points * sizeof(uint16_t));

		return Filter;
	}

//usage of functions from ISR's, inputs arguments is of form uint16_t
    MA_filter_calculate(battery, &battery_sum, &MA_ADC);
	MA_filter_calculate(temp, &temp_sum, &MA_Temp);
	
	
