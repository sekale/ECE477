/*
 * DataDisplayStruct.h
 *
 * Created: 12/1/2016 3:16:09 AM
 *  Author: pcadmin
 */ 


#ifndef DATADISPLAYSTRUCT_H_
#define DATADISPLAYSTRUCT_H_

typedef struct weather
{
	uint8_t typeDay;
	int temperature;
	int windSpeed;
	char typeWeather[12];	// for ex. - Rainy, Cloudy etc.
}Weather;

typedef struct news
{
	char headline[51]; //source
	int currentIndex;
}News;

typedef struct time_now
{
	//Minute Hand <x,y> coordinates
	int min;
	
	//Hour Hand <x,y> coordinates
	int hour;
}TimeNow;

#endif /* DATADISPLAYSTRUCT_H_ */