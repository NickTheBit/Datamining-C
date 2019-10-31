#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define buffer 10

int main(int argc,char **argv) {
	FILE *f;					// Variable for file
	time_t times[2];			// Timer bar for algo
	char filename[100];			// Filename alternative to main arguments
	char tempChar;				// temp variable for characters
	double tempDouble;			// temp variable for doubles
	int tempInt;				// temp variable for integers
	int i,j,d;					// Various counters
	int dimensions = 1;			// Counter for dimensions
	int elementCounter = 0;		// Counter for elements
	double coreRadius;			// User designated radius for core
	int minNeighbors;			// Minimum points for core status
	int curNeighbors;			// Temporary Variabe for neighbor assignment
	int arrayCounter;			// Counter used for correct assignment to arrays
	int clusterCounter  = 0;	// Variable for counting new clusters
	int dbg	= 0;				// Var for debugging printf("Dbg %d\n",dbg++);

	if (argc == 4) {
		f = fopen(argv[1],"r");
		coreRadius = atof(argv[2]);
		minNeighbors = atoi(argv[3]);
	} else {
		printf("Insert dataset: ");
		scanf("%s",filename);
		printf("Insert core radius: ");
		scanf("%lf",&coreRadius);
		printf("Insert ninimum neighbors for a core: ");
		scanf("%d",&minNeighbors);
		f = fopen(filename,"r");
	}
	if (!f) {
		printf("invalid dataset\n");
		return 0;
	}

	// Reading dataset
	fscanf(f,"%c",&tempChar);
	while (tempChar != '\n') {
		if ((tempChar == ' ')||(tempChar == '\t')||(tempChar == ',')) {
			dimensions ++;
		}
		fscanf(f,"%c",&tempChar);
	}
	rewind(f);
	while (!feof(f)) {
		fscanf(f,"%c",&tempChar);
		if (tempChar == '\n')
			elementCounter ++;
	}
	rewind(f);

	// Dataset report
	printf("Detected\n\tElements: %d\n\tdimensions: %d\n",elementCounter,dimensions);

	// Started timer
	times[0] = clock();

	// Storring Points
	double elements[elementCounter][dimensions];
	for (i=0; i<elementCounter; i++)
		for (j=0; j<dimensions; j++)
			fscanf(f,"%lf",&elements[i][j]);

	// Creating arrays for storage
	int *owner = calloc(elementCounter,sizeof(int));
	int *inRange = (int *)calloc(minNeighbors + buffer,sizeof(int));;
	int *explored = calloc(elementCounter,sizeof(int));

	// Starting Algorythm
	for (i=0; i<elementCounter; i++) {

		if (explored[i] != 0)
			continue;

		// Storage for range query
		curNeighbors = 0;
		arrayCounter = 0;

		//inRange query
		for (j=0; j<elementCounter; j++) {
			tempDouble = 0;
			tempInt = 0;
			
			// Element shouldnt check itself
			if (j == i)
				continue;

			// Euclidian distance
			for (d=0; d<dimensions; d++){
				tempDouble += (elements[i][d] - elements[j][d])*(elements[i][d] - elements[j][d]);
			}
			tempDouble = sqrt(tempDouble);


			if (tempDouble <= coreRadius) {
				// Possible seg fault
				curNeighbors ++;
				inRange[arrayCounter++] = j;
				if (owner[j] != 0)
					tempInt = owner[j];
			}
		}
		if (curNeighbors >= minNeighbors)
			if (tempInt == 0)
				owner[i] = clusterCounter++;
			else
				owner[i] = tempInt;
		else 
			owner[i] = -1;
		explored[i] = 1;
		for (d=0; d<arrayCounter; d++) {
			owner[inRange[d]] = owner[i];
			explored[inRange[d]] = 1;
		}
	}
	times[1] = clock();

	// Outputting in file
	printf("Execution time: %f\n",(double)(times[1]-times[0])/CLOCKS_PER_SEC);
	printf("\tWARNING\nOutput is %d clusters\nproceed ? [y/n]: ",clusterCounter);
	scanf(" %c",&tempChar);
	if (tempChar != 'y')
		return 0;

	fclose(f);
	for (i=-1; i<clusterCounter; i++) {
		sprintf(filename,"Cluster%d.csv",i);
		f = fopen(filename,"w");
		for (j=0; j<elementCounter; j++)
			if (owner[j] == i) {
				for (d=0; d<dimensions; d++)
					fprintf(f,"%lf ",elements[j][d]);
				fprintf(f,"\n");
			}
		fclose(f);
	}

	return 0;
	
}