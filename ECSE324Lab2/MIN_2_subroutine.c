#include <stdio.h>
extern	int	MIN_2(int x, int y);
int main()	{
		int	a[5] = {7, 20, 3, 4, 5};
		int min_val = a[0];					//initialize the minimum value		           
		for (int i = 0; i < 5; i++){
			min_val = MIN_2(min_val, a[i]);
		printf("The minimum value is %d\n", min_val);
		return	min_val;
}
