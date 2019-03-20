#ifndef MATHUTIL_H
#define MATHUTIL_H

float randomBetweenTwoFloats(float a, float b)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

#endif // !MATHUTIL_H
