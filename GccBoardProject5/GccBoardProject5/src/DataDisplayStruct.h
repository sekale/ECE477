#ifndef INCFILE1_H_
#define INCFILE1_H_

typedef struct weather
{
	int numDay; // 1 - Today 2 - Tomorrow 3 - Day After
	char *typeDay;
	char *temperature;
	char *windSpeed;
};

typedef struct news
{
	char *postedBy; //source
	char *headline; //source
};

typedef struct time_now
{
	//Minute Hand <x,y> coordinates
	char *x_min;
	char *y_min;
	
	//Hour Hand <x,y> coordinates
	char *x_hr;
	char *y_hr;
};








#endif /* INCFILE1_H_ */