#include <stdio.h>  /* for puts() */
#include <stdlib.h> /* for EXIT_SUCCESS */

void your_fcn(int *nums)
{
    nums[0] = -2147483647;
    nums[1] = -2079014691;
}

int main(int argc, char *argv[])
{
    int nums[2] = {0};
    your_fcn(nums);

    if (nums[0] >= 0 || nums[1] >= 0 || nums[0] + nums[1] != 0x414c0de)
        puts("You lost!");
    else
        puts("You won!");

    return EXIT_SUCCESS;
}
