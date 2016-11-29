#ifndef INCFILE1_H_
#define INCFILE1_H_

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
#endif /* INCFILE1_H_ */