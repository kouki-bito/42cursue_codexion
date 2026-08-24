#include "src/codexion.h"

static int	g_failures;

static void	check(int condition, const char *name)
{
	if (condition)
		printf("[OK]   %s\n", name);
	else
	{
		printf("[FAIL] %s\n", name);
		g_failures++;
	}
}

long long	get_burn_out(t_coder *coder)
{
	long long	deadline;

	pthread_mutex_lock(&coder->coder_mutex);
	deadline = coder->burn_out_time;
	pthread_mutex_unlock(&coder->coder_mutex);
	return (deadline);
}

static void	setup_test_coder(t_coder *coder, t_data *data, int id,
		long long deadline)
{
	memset(coder, 0, sizeof(*coder));
	coder->id = id;
	coder->data = data;
	coder->burn_out_time = deadline;
	pthread_mutex_init(&coder->coder_mutex, NULL);
}

static int	contains_coder(t_heap *heap, t_coder *coder)
{
	int	i;

	i = 0;
	while (i < heap->size)
	{
		if (heap->request[i].coder == coder)
			return (1);
		i++;
	}
	return (0);
}

static int	heap_is_valid(t_heap *heap)
{
	int	i;
	int	left;
	int	right;

	i = 0;
	while (i < heap->size)
	{
		left = i * 2 + 1;
		right = i * 2 + 2;
		if (left < heap->size
			&& heap_compare(&heap->request[left], &heap->request[i]))
			return (0);
		if (right < heap->size
			&& heap_compare(&heap->request[right], &heap->request[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	destroy_coders(t_coder *coders, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&coders[i].coder_mutex);
		i++;
	}
}

static void	test_empty_and_single(void)
{
	t_data	data;
	t_heap	heap;
	t_coder	coder;

	memset(&data, 0, sizeof(data));
	data.scheduler = "edf";
	pthread_mutex_init(&data.data_mutex, NULL);
	heap_init(&heap);
	setup_test_coder(&coder, &data, 1, 100);
	heap_pop(&heap, &coder);
	check(heap.size == 0, "empty: 空heapのpopでsizeを変更しない");
	heap_push(&heap, &coder);
	check(heap.size == 1, "single: 1要素をpushできる");
	check(heap.request[0].coder == &coder,
		"single: pushしたcoderが先頭になる");
	heap_pop(&heap, &coder);
	check(heap.size == 0, "single: 1要素をpopして空にできる");
	pthread_mutex_destroy(&coder.coder_mutex);
	pthread_mutex_destroy(&heap.lock);
	pthread_cond_destroy(&heap.cond);
	pthread_mutex_destroy(&data.data_mutex);
}

static void	test_edf_order(void)
{
	t_data	data;
	t_heap	heap;
	t_coder	coders[5];
	int		deadlines[5];
	int		i;

	memset(&data, 0, sizeof(data));
	data.scheduler = "edf";
	pthread_mutex_init(&data.data_mutex, NULL);
	heap_init(&heap);
	deadlines[0] = 500;
	deadlines[1] = 100;
	deadlines[2] = 300;
	deadlines[3] = 200;
	deadlines[4] = 400;
	i = 0;
	while (i < 5)
	{
		setup_test_coder(&coders[i], &data, i + 1, deadlines[i]);
		heap_push(&heap, &coders[i]);
		check(heap_is_valid(&heap), "EDF: push後もheap invariantを保つ");
		i++;
	}
	check(heap.size == 5, "EDF: sizeが5になる");
	check(heap.request[0].deadline == 100,
		"EDF: 最も早いdeadlineが先頭になる");
	heap_pop(&heap, &coders[1]);
	check(heap.size == 4, "EDF: 先頭削除でsizeが1減る");
	check(!contains_coder(&heap, &coders[1]),
		"EDF: 削除したcoderがheapから消える");
	check(heap.request[0].deadline == 200,
		"EDF: 先頭削除後に次のdeadlineが先頭になる");
	check(heap_is_valid(&heap), "EDF: 先頭削除後もheap invariantを保つ");
	heap_pop(&heap, &coders[4]);
	check(!contains_coder(&heap, &coders[4]),
		"EDF: 途中のcoderを削除できる");
	check(heap_is_valid(&heap), "EDF: 途中削除後もheap invariantを保つ");
	destroy_coders(coders, 5);
	pthread_mutex_destroy(&heap.lock);
	pthread_cond_destroy(&heap.cond);
	pthread_mutex_destroy(&data.data_mutex);
}

static void	test_equal_deadline(void)
{
	t_data	data;
	t_heap	heap;
	t_coder	coders[3];
	int		i;

	memset(&data, 0, sizeof(data));
	data.scheduler = "edf";
	pthread_mutex_init(&data.data_mutex, NULL);
	heap_init(&heap);
	i = 0;
	while (i < 3)
	{
		setup_test_coder(&coders[i], &data, i + 1, 100);
		heap_push(&heap, &coders[i]);
		i++;
	}
	check(heap.request[0].number == 0,
		"EDF: deadline同値なら最初のrequestを優先する");
	check(heap_is_valid(&heap),
		"EDF: deadline同値でもheap invariantを保つ");
	destroy_coders(coders, 3);
	pthread_mutex_destroy(&heap.lock);
	pthread_cond_destroy(&heap.cond);
	pthread_mutex_destroy(&data.data_mutex);
}

static void	test_fifo_order(void)
{
	t_data	data;
	t_heap	heap;
	t_coder	coders[3];

	memset(&data, 0, sizeof(data));
	data.scheduler = "fifo";
	pthread_mutex_init(&data.data_mutex, NULL);
	heap_init(&heap);
	setup_test_coder(&coders[0], &data, 1, 300);
	setup_test_coder(&coders[1], &data, 2, 100);
	setup_test_coder(&coders[2], &data, 3, 200);
	heap_push(&heap, &coders[0]);
	heap_push(&heap, &coders[1]);
	heap_push(&heap, &coders[2]);
	check(heap.request[0].coder == &coders[0],
		"FIFO: deadlineに関係なく最初のrequestを優先する");
	check(heap_is_valid(&heap), "FIFO: heap invariantを保つ");
	destroy_coders(coders, 3);
	pthread_mutex_destroy(&heap.lock);
	pthread_cond_destroy(&heap.cond);
	pthread_mutex_destroy(&data.data_mutex);
}

static void	test_remove_edges(void)
{
	t_data	data;
	t_heap	heap;
	t_coder	coders[4];
	t_coder	unknown;
	t_coder	*last;
	int		old_size;
	int		i;

	memset(&data, 0, sizeof(data));
	data.scheduler = "edf";
	pthread_mutex_init(&data.data_mutex, NULL);
	heap_init(&heap);
	i = 0;
	while (i < 4)
	{
		setup_test_coder(&coders[i], &data, i + 1, (i + 1) * 100);
		heap_push(&heap, &coders[i]);
		i++;
	}
	last = heap.request[heap.size - 1].coder;
	old_size = heap.size;
	heap_pop(&heap, last);
	check(heap.size == old_size - 1, "pop: 末尾削除でsizeが1減る");
	check(!contains_coder(&heap, last), "pop: 末尾のcoderが消える");
	check(heap_is_valid(&heap), "pop: 末尾削除後もheap invariantを保つ");
	setup_test_coder(&unknown, &data, 99, 50);
	old_size = heap.size;
	heap_pop(&heap, &unknown);
	check(heap.size == old_size,
		"pop: 存在しないcoderではsizeを変更しない");
	check(heap_is_valid(&heap),
		"pop: 存在しないcoderの処理後もheap invariantを保つ");
	pthread_mutex_destroy(&unknown.coder_mutex);
	destroy_coders(coders, 4);
	pthread_mutex_destroy(&heap.lock);
	pthread_cond_destroy(&heap.cond);
	pthread_mutex_destroy(&data.data_mutex);
}

int	main(void)
{
	test_empty_and_single();
	test_edf_order();
	test_equal_deadline();
	test_fifo_order();
	test_remove_edges();
	if (g_failures == 0)
	{
		printf("\nAll heap tests passed.\n");
		return (0);
	}
	printf("\n%d heap test(s) failed.\n", g_failures);
	return (1);
}
