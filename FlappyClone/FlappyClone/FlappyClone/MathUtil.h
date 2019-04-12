#ifndef MATHUTIL_H
#define MATHUTIL_H


/// <summary>
/// Math utility header
/// </summary>

/// <summary>
/// Function used to get a random number between two floats
/// </summary>
/// <param name="low">The lower bounding value to be used in random function</param>
/// <param name="high">The upper bounding value to be used in random function</param>
/// <returns></returns>
static float randomBetweenTwoFloats(float low, float high)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = high - low;
	float r = random * diff;
	return low + r;
}

/// <summary>
/// Function used to get a random value between two ints
/// </summary>
/// <param name="low">The lower bounding value for the random function</param>
/// <param name="high">The upper bounding value for the random function</param>
/// <returns></returns>
static int randomBetweenTwoInts(int low, int high)
{
	return std::rand() % (high + 1 - low) + low;
}

#endif // !MATHUTIL_H
