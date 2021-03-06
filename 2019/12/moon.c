#include <string.h>

const int DIMENSIONS = 3;

struct Moon {
    int coord[DIMENSIONS];
    int velocity[DIMENSIONS];
};

struct Moon **read_system(const char *filename, int *number)
{
    const char *format = "<x=%d, y=%d, z=%d>\n";
    FILE *file = fopen(filename, "r");
    char current_line[100];
    int count = 0;
    while (fgets(current_line, sizeof(current_line), file) != NULL) {
        ++count;
    }
    fclose(file);
    *number = count;

    file = fopen(filename, "r");
    struct Moon **moons = malloc(sizeof(struct Moon *) * count);
    count = 0;
    int x, y, z;
    while (fgets(current_line, sizeof(current_line), file) != NULL) {
        sscanf(current_line, format, &x, &y, &z);
        struct Moon *new_moon = malloc(sizeof(struct Moon));
        new_moon->coord[0] = x;
        new_moon->coord[1] = y;
        new_moon->coord[2] = z;
        for (int i = 0; i < DIMENSIONS; ++i) {
            new_moon->velocity[i] = 0;
        }
        moons[count] = new_moon;
        ++count;
    }
    fclose(file);
    return moons;
}

static char *to_string(struct Moon *moon)
{
    char *result;
    const char *format = "pos=<x=%3d, y=%3d, z=%3d>, vel=<x=%3d, y=%3d, z=%3d>";
    asprintf(&result, format, moon->coord[0], moon->coord[1], moon->coord[2], moon->velocity[0], moon->velocity[1], moon->velocity[2]);
    return result;
}

char *to_string_system(int moon_count, struct Moon *moons[])
{
    char *result, *old_result;
    char *this_moon = to_string(moons[0]);
    asprintf(&result, "%s", this_moon);
    free(this_moon);
    for (int i = 1; i < moon_count; ++i) {
        old_result = result;
        char *this_moon = to_string(moons[i]);
        asprintf(&result, "%s\n%s", old_result, this_moon);
        free(this_moon);
        free(old_result);
    }
    return result;
}

char *to_vector_matrix(int moon_count, struct Moon *moons[], const int axis)
{
    char *result, *old_result;
    asprintf(&result, "pos=%3d, vel=%3d", moons[0]->coord[axis], moons[0]->velocity[axis]);
    for (int i = 1; i < moon_count; ++i) {
        old_result = result;
        asprintf(&result, "%s; pos=%3d, vel=%3d", old_result, moons[i]->coord[axis], moons[i]->velocity[axis]);
        free(old_result);
    }
    return result;
}

void gravity(int moon_count, struct Moon *moons[])
{
    for (int i = 0; i < moon_count; ++i) {
        for (int j = i + 1; j < moon_count; ++j) {
            for (int k = 0; k < DIMENSIONS; ++k) {
                if (moons[i]->coord[k] < moons[j]->coord[k]) {
                    ++moons[i]->velocity[k];
                    --moons[j]->velocity[k];
                } else if (moons[i]->coord[k] > moons[j]->coord[k]) {
                    --moons[i]->velocity[k];
                    ++moons[j]->velocity[k];
                }
            }
        }
    }
}

void velocity(int moon_count, struct Moon *moons[])
{
    for (int i = 0; i < moon_count; ++i) {
        for (int k = 0; k < DIMENSIONS; ++k) {
            moons[i]->coord[k] += moons[i]->velocity[k];
        }
    }
}
