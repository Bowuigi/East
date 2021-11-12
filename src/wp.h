#include <stdlib.h>
#include <assert.h>

#define WP_ERR(msg) do {fprintf(stderr,"East, fatal error: %s", msg);exit(1);} while (0)

typedef struct {
	size_t *items;
	size_t length;
	size_t size;
} wp_t;

wp_t WP_Create() {
	wp_t tmp;

	tmp.length = 0;
	tmp.size = 10;
	tmp.items = calloc(sizeof(size_t), tmp.size);

	if (!tmp.items)
		WP_ERR("Out of memory");

	return tmp;
}

void WP_Delete(wp_t *W) {
	free(W->items);
}

static void WPDouble(wp_t *W) {
	size_t *tmp;

	W->size *= 2;
	tmp = realloc(W->items, sizeof(size_t)*W->size);

	if (!tmp)
		WP_ERR("Out of memory");

	W->items = tmp;
}

void WP_Push(wp_t *W, size_t waypoint) {
	if (W->length+2 > W->size)
		WPDouble(W);

	W->items[W->length] = waypoint;
	W->length++;
	W->items[W->length+1] = 0;
}

size_t WP_Pop(wp_t *W) {
	W->length--;
	size_t tmp = W->items[W->length];
	W->items[W->length] = 0;
	return tmp;
}
