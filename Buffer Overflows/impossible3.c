#include <stdio.h>  /* for puts() */
#include <stdlib.h> /* for EXIT_SUCCESS */

void your_fcn(int *nums)
{
    int arr[2] = {0};
    arr[3] = 0x804849e;
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
