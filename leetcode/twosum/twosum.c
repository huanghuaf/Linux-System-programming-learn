#include <stdio.h>
#include <stdlib.h>
/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* twoSum(int* nums, int numsSize, int target, int* returnSize){
    int i,j;
    int other;
    int *target_array;

    target_array = (int *)malloc(2 * sizeof(int));

    for (i = 0; i < numsSize; i++) {
		for (j = i + 1; j < numsSize; j++) {
			if (nums[i] + nums[j] == target) {
				target_array[0] = i;
				target_array[1] = j;
				*returnSize = 2;
				return target_array;
			}
		}
		if (j == numsSize) {
			*returnSize = 0;
		}
    }
    return target_array;
}

int main()
{
	int nums[] = {3,2,4};
	int target = 6;
	int *p_target = NULL;
	int return_size = 0;
	int i;
	
	for (i = 0; i < 3; i++)
		printf("num:%d\n", nums[i]);
	
	p_target = twoSum(nums, 3, target, &return_size);
	
	printf("return seze:%d\n", return_size);
	
	if (p_target) {
		for (i = 0; i < return_size; i++)
			printf("get num:%d\n", p_target[i]);
	}
	
	return 0;
}