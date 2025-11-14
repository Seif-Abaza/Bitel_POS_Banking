#include <stdlib.h>

static unsigned long int Seed = 1;

int rand(void)
{
        Seed = Seed * 1103515245 + 12345;
	return (int)(Seed % RAND_MAX);
}

void srand(unsigned int seed)
{
	Seed = seed;
}
