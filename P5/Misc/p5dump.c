#include <stdio.h>
#include <errno.h>
#include <math.h>

#include <p5glove.h>
#include <p5common.h>

#define RAD2DEG 57.295779513083

double calc_dist(double x1, double y1, double z1) {
	return sqrt((x1*x1)+(y1*y1)+(z1*z1));
}

/* Brain-Dead P5 data dump.
 */
int main(int argc, char **argv)
{
	P5Glove glove;
	struct p5glove_data info;
	int sample;
	int num_visible;
	int visible_leds[3];
	double plane[4];

	glove=p5glove_open();
	if (glove == NULL) {
		fprintf(stderr, "%s: Can't open glove interface\n", argv[0]);
		return 1;
	}

	printf("Dumping the next 100 samples...\n");
	memset(&info,0,sizeof(info));
	for (sample=0; sample < 100; ) {
		int i,err;

		err=p5glove_sample(glove,&info);
		if (err < 0 && errno == EAGAIN)
			continue;

		printf("%2d: ",sample);
		sample++;
		if (err < 0) {
			perror("Glove Failure");
			exit(1);
		}

		/* Buttons */
		printf("%c%c%c ",
			(info.buttons & P5GLOVE_BUTTON_A) ? 'A' : '.',
			(info.buttons & P5GLOVE_BUTTON_B) ? 'B' : '.',
			(info.buttons & P5GLOVE_BUTTON_C) ? 'C' : '.');

		/* Fingers */
		printf("%2d,%2d.%2d.%2d.%2d ",
			info.finger[P5GLOVE_THUMB],
			info.finger[P5GLOVE_INDEX],
			info.finger[P5GLOVE_MIDDLE],
			info.finger[P5GLOVE_RING],
			info.finger[P5GLOVE_PINKY]);

		/* Visible IRs */
		num_visible = 0;
		for (i = 0; i < 8; i++) {
			if (! info.ir[i].visible)
				continue;

			printf("%d:(%4d,%4d,%4d) ",i,
					info.ir[i].x,info.ir[i].y,info.ir[i].z);
			visible_leds[num_visible++] = i;
		}

		printf("\n");
		printf("distance ");
		for (i=0; i<num_visible; i++) {
			printf(" %d:%f ", visible_leds[i], calc_dist(info.ir[visible_leds[i]].x, info.ir[visible_leds[i]].y,
				info.ir[visible_leds[i]].z));
		}
		printf("\n");
			
	}

	p5glove_close(glove);
}
