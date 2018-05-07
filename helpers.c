#include "helpers.h"

#include <stdlib.h>

t_nodeval **alloc_2d(const int m, const int n) {
	t_nodeval **arr = malloc(m * sizeof(*arr));
	int i;
	for (i = 0; i < m; ++i) {
		arr[i] = malloc(n * sizeof(t_nodeval));
	}
	return arr;
}

t_nodeval ****alloc_4d(const int m, const int n, const int o, const int p) {
	int i, j, k;
	t_nodeval ****arr = malloc(m * sizeof(***arr));
	for (i = 0; i < m; i++) {
		arr[i] = malloc(n * sizeof(**arr));
		for (j = 0; j < n; j++) {
			arr[i][j] = malloc(o * sizeof(*arr));
			for (k = 0; k < o; k++) {
				arr[i][j][k] = malloc(p * sizeof(t_nodeval));
			}
		}
	}
	return arr;
}
