#include "app.h"
#include "SDL.h"

#define BOXES_COUNT 200
#define BUFFER_AREA 200
struct MyBox
{
	float x;
	float y;
	int color;
	float speed;
	char size;
	char direction;
};

struct MyBox *boxes;

static int lerpColor(int a, int b, float amount);

unsigned long int next = 1;
/* rand: return pseudo-random integer on 0..32767 */
#define RAND_MAX 32767
int rand()
{
	next = next * 1103515245 + 12345;
	return (unsigned int)(next / 65536) % (RAND_MAX + 1);
}
void setSeed(unsigned int seed)
{
	next = seed;
}

int randomInt(int from, int to)
{
	return rand() % (to - from) + from;
}

void init()
{
	printf("Sizeof MyBox: %d", sizeof(struct MyBox));
	boxes = malloc(BOXES_COUNT * sizeof(struct MyBox));
	setSeed(137);

	for (int i = 0; i < BOXES_COUNT; i++)
	{
		char size = (char)randomInt(20, 40);
		boxes[i].x = randomInt(0, SCREEN_WIDTH - size - BUFFER_AREA);
		boxes[i].y = randomInt(0, SCREEN_HEIGHT - size);
		boxes[i].size =  size;
		boxes[i].speed = 1.2;
		boxes[i].color = 0xff00ff;
		boxes[i].direction = rand() > (RAND_MAX / 2);
	}
}

void draw_circle(SDL_Renderer* renderer, int x, int y, int radius)
{
	for (int w = 0; w < radius * 2; w++)
	{
		for (int h = 0; h < radius * 2; h++)
		{
			int dx = radius - w; // horizontal offset
			int dy = radius - h; // vertical offset
			if ((dx * dx + dy * dy) <= (radius * radius))
			{
				SDL_RenderDrawPoint(renderer, x + dx, y + dy);
			}
		}
	}
}

void DrawCircle(SDL_Renderer *renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
	const int32_t diameter = (radius * 2);

	int32_t x = (radius - 1);
	int32_t y = 0;
	int32_t tx = 1;
	int32_t ty = 1;
	int32_t error = (tx - diameter);

	while (x >= y)
	{
		//  Each of the following renders an octant of the circle
		SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
		SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
		SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
		SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
		SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
		SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
		SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
		SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

		if (error <= 0)
		{
			++y;
			error += ty;
			ty += 2;
		}

		if (error > 0)
		{
			--x;
			tx += 2;
			error += (tx - diameter);
		}
	}
}

void render(SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	// SDL_Rect rect = {0, 0, 0, 0};
	for (int i = 0; i < BOXES_COUNT; i++)
	{
		struct MyBox *box = &boxes[i];
		int c = box->color;
		// rect.x = (int)box->x;
		// rect.y = (int)box->y;
		// rect.h = box->size;
		// rect.w = box->size;

		SDL_SetRenderDrawColor(renderer, c >> 16, (c >> 8) & 0xff, c & 0xff, 255);

		// SDL_RenderFillRect(renderer, &rect);
		draw_circle(renderer, box->x, box->y, box->size);
	}
	SDL_RenderPresent(renderer);
}

void update(double deltaTime)
{
	for (int i = 0; i < BOXES_COUNT; i++)
	{
		struct MyBox *box = &boxes[i];
		box->x += box->speed * deltaTime * (box->direction == 0 ? 1 : -1);

		box->color = lerpColor(0x00ff00, 0xff0000, box->x / SCREEN_WIDTH);

		if (box->x > SCREEN_WIDTH - box->size && box->direction == 0)
			box->direction = 1;
		else if (box->x <= 0 && box->direction == 1)
			box->direction = 0;
	}
}

void teardown()
{
	free(boxes);
}

static int lerpColor(int a, int b, float amount)
{
	int ar = a >> 16,
		ag = (a >> 8) & 0xff,
		ab = a & 0xff,
		br = b >> 16,
		bg = (b >> 8) & 0xff,
		bb = b & 0xff,
		rr = ar + amount * (br - ar),
		rg = ag + amount * (bg - ag),
		rb = ab + amount * (bb - ab);

	return (rr << 16) + (rg << 8) + (rb | 0);
};