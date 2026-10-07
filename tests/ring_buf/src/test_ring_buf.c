/*
 * Ring Buffer Module - Homework Test Skeleton
 *
 * test_fresh_state is provided as a worked example. Fill in the remaining
 * 7 ZTEST bodies according to TEST_SPEC.md. Stubs call ztest_test_skip()
 * so the binary builds and runs cleanly before each test is implemented.
 *
 * Run:
 *   west twister -T tests/ring_buf -p native_sim
 */

#include <zephyr/ztest.h>
#include <errno.h>

#include "ring_buf.h"

/*
 * Shared before hook: every suite reinitialises the ring buffer with a
 * capacity of 4 so tests start from a clean, known state. Capacity 4 is
 * enough to exercise FIFO order (push 1, 2, 3) and overflow (full at 4).
 */
static void before(void *f)
{
	ARG_UNUSED(f);
	rb_init(4);
}

/*
 * ============================================================================
 * Test Suite: ring_buf_init
 *
 * Initial state and re-initialization behaviour.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_init, NULL, NULL, before, NULL, NULL);

/* PROVIDED — study this test before writing the rest. */
ZTEST(ring_buf_init, test_fresh_state)
{
	zassert_true(rb_is_empty(), "Fresh buffer must be empty");
	zassert_equal(rb_count(), 0, "Fresh buffer count must be 0");
}

ZTEST(ring_buf_init, test_reinit_clears_state)
{

    zassert_equal(rb_push(99), 0, "push that fits should return 0" );
    zassert_equal(rb_init(4), 0, "init with valid cap should return 0" );
	zassert_true(rb_is_empty(), "Fresh buffer must be empty");
	zassert_equal(rb_count(), 0, "Fresh buffer count must be 0");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_push_pop
 *
 * Single push/pop round-trip, FIFO order, full error path.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_push_pop, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_push_pop, test_single_push_pop)
{   
    int v;

    zassert_equal(rb_push(42), 0, "push that fits should return 0" );
    zassert_equal(rb_pop(&v), 0, "pop with valid data should return 0" );
	zassert_true(rb_is_empty(), "Fresh buffer must be empty");
	zassert_equal(rb_count(), 0, "Fresh buffer count must be 0");
}

ZTEST(ring_buf_push_pop, test_fifo_order)
{
    int v;

    zassert_equal(rb_push(1), 0, "push that fits should return 0" );
    zassert_equal(rb_push(2), 0, "push that fits should return 0" );
    zassert_equal(rb_push(3), 0, "push that fits should return 0" );
    zassert_equal(rb_pop(&v), 0, "pop with valid data should return 0" );
    zassert_equal(v, 1, "First pop must be 1 since it was the oldest" );
    zassert_equal(rb_pop(&v), 0, "pop with valid data should return 0" );
    zassert_equal(v, 2, "First pop must be 2 since it was the second oldest" );
    zassert_equal(rb_pop(&v), 0, "pop with valid data should return 0" );
    zassert_equal(v, 3, "First pop must be 3 since it was the third oldest" );
	zassert_true(rb_is_empty(), "Fresh buffer must be empty");
	zassert_equal(rb_count(), 0, "Fresh buffer count must be 0");
}

ZTEST(ring_buf_push_pop, test_push_full_returns_enospc)
{
    zassert_equal(rb_push(1), 0, "push that fits should return 0" );
    zassert_equal(rb_push(2), 0, "push that fits should return 0" );
    zassert_equal(rb_push(3), 0, "push that fits should return 0" );
    zassert_equal(rb_push(4), 0, "push that fits should return 0" );
    zassert_equal(rb_push(99), -ENOSPC, "push that doesnt fits should return -ENOSPC" );
    zassert_equal(rb_count(), 4, "count should equal the amount we put in" );
}

/*
 * ============================================================================
 * Test Suite: ring_buf_boundaries
 *
 * Peek semantics and NULL-pointer boundary conditions.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_boundaries, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_boundaries, test_peek_does_not_consume)
{   
    int v;

    zassert_equal(rb_push(7), 0, "push that fits should return 0" );
    zassert_equal(rb_peek(&v), 0, "peek with valid data should return 0" );
    zassert_equal(v, 7, "peek returned correct value" );
    zassert_equal(rb_peek(&v), 0, "peek with valid data should return 0" );
    zassert_equal(v, 7, "peek did not consume" );
    zassert_equal(rb_count(), 1, "count should equal the amount we put in" );
}

ZTEST(ring_buf_boundaries, test_pop_null_returns_einval)
{
    zassert_equal(rb_pop(NULL), -EINVAL, "pop into NULL should return -EINVAL" );
}

ZTEST(ring_buf_boundaries, test_is_full_after_fill)
{
    zassert_equal(rb_push(1), 0, "push that fits should return 0" );
    zassert_equal(rb_push(1), 0, "push that fits should return 0" );
    zassert_equal(rb_push(1), 0, "push that fits should return 0" );
    zassert_equal(rb_push(1), 0, "push that fits should return 0" );
    zassert_true(rb_is_full(), "is_full should return true when full" );
    zassert_equal(rb_count(), 4, "count should equal the cap" );
}
