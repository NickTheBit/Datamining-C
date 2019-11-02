#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(int argc,char **argv) {

	FILE *f;        			// Primary file pointer
	time_t times[2];			// Used for timing the algo
	char filename[100];			// Variable for filename
	int i,j,d;					// Counters for loops
	int flag;					// Used in 'flag' situations
	int k;						// Minimum amount of neighbors
	int dimensions = 0;			// Dimentions of the dataset
	int elementCounter = 0;		// Counts amount of datapoints
	char tempChar;				// Temporary storage for characters
	double tempDouble;			// Temporary storage for doubles
	int tempInt;				// Temporary storage for integers
	int curTeams = 0;			// Number of exsitant teams
	double curCls;				// Current closest point
	double curdis;				// CUrrent distance


	if (argc == 4) {
		f = fopen(argv[1],"r");
		k = atoi(argv[3]);
	} else {
		printf("Insert dataset: ");
		scanf("%s",filename);
		printf("Insert ninimum neighbors for a core: ");
		scanf("%d",&k);
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
	double elements[elementCounter][dimensions];		// Array for storage
	for (i=0; i<elementCounter; i++)
		for (j=0; j<dimensions; j++)
			fscanf(f,"%lf",&elements[i][j]);

	double knd[k];		// Array for sorting the closest points (distance)
	int knp[k];			// Array for storing points-neighbors

	// Array for storing the group a point belongs to
	int team[elementCounter];
	int tempTeam[k]; 		// Thats for figuring witch team to assign new elements
	int tempTeamPoints[k];	// Counter for team assignment

	// Setting all poits to 0
	for (i=0; i<elementCounter; i++)
		team[i] = 0;


	for (i=0; i<elementCounter; i++) {

		// Resetting dnd
		for (d=0; d<k; d++)
			knd[d] = 99999999;
		
		for (j=0; j<elementCounter; j++) {

			// Euclidian distance
			for (d=0; d<dimensions; d++){
				tempDouble += (elements[i][d] - elements[j][d])*(elements[i][d] - elements[j][d]);
			}
			curdis = sqrt(tempDouble);

			// Storing k closest distances and points
			for (d=0; d<k; d++)
				if (curdis <= knd[d]) {
					// Swapping distances
					tempDouble = knd[d];
					knd[d] = curdis;
					curdis = tempDouble;
					// Swapping points
					tempInt = knp[d];
					knp[d] = j;
					j = tempInt;
				}
		}

		// Determining where it belongs to
		for (j=0; j<k; j++) {
			// if a point is in the list increase its counter
			flag = 0;
			tempInt = 0;
			for (d=0; d<k; d++)
				if (knp[j] == tempTeam[d]) {
					tempTeamPoints[d] ++;
					flag = 1;
				}
			// if not add it to the list
			if (flag == 0) {
				tempTeam[tempInt] = knp[j];
				tempInt ++;
			}
		}
		// Assigning it
		tempInt = 0;
		for (j=0; j<k; j++)
			if (tempInt <= tempTeamPoints[j]) {
				tempInt = tempTeamPoints[j];
				flag = j;
			}
		team[i] = flag;

		
	}
}