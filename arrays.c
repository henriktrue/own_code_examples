//from ports.cpp
E_MA_FILTER_RESPONSES MA_filter_calculate(uint16_t input, uint16_t * output,
                                          T_MA_Filter * Filter)
{
    uint16_t old;
    if (Filter == NULL)
        return MA_FILTER_FILTER_WAS_NULLPTR;

    if (Filter->readings == NULL)
        return MA_FILTER_FILTER_READINGS_WAS_NULLPTR;

    uint16_t sum;
    //save old point in array for later subtraction
    old = Filter->readings[Filter->writeIndex];
    //replace old array point with new enter
    Filter->readings[Filter->writeIndex] = input;
    // subtract old reading, and add new one
    sum = (Filter->total - old) + input;
    Filter->total = sum;
    (*output) = sum / Filter->points;

    //increment for nest calculation
    Filter->writeIndex++;

    if (Filter->writeIndex >= Filter->points)
    {
        Filter->writeIndex = 0;
    }

    return MA_FILTER_OK;
}

T_MA_Filter * NewMA_Filter(uint16_t * buffer, uint16_t bufferLength,
                           T_MA_Filter * Filter)
{
    Filter->readings = buffer;
    Filter->points = bufferLength;
    Filter->total = 0;
    Filter->writeIndex = 0;
    memset(Filter->readings, 0, Filter->points * sizeof(uint16_t));
    return Filter;
}

from ports.h
typedef struct
{
    uint8_t points;
    uint16_t input;
    uint16_t total;
    uint8_t writeIndex;
    uint16_t * readings;
} T_MA_Filter;

T_MA_Filter MA_ADC;
T_MA_Filter MA_Temp;
T_MA_Filter MA_Duty;

static uint16_t adcBuffer[10];
static uint16_t tempBuffer[10];
static uint16_t dutyBuffer[100];

typedef enum
{
    MA_FILTER_OK = 0,
    MA_FILTER_FILTER_WAS_NULLPTR = -1,
    MA_FILTER_FILTER_READINGS_WAS_NULLPTR = -2,
} E_MA_FILTER_RESPONSES;


from main
    NewMA_Filter(adcBuffer, 10, &MA_ADC);
    NewMA_Filter(tempBuffer, 10, &MA_Temp);
    NewMA_Filter(dutyBuffer, 100, &MA_Duty);
