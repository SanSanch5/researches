#ifndef TIM_SORT_NOT_MINE_H
#define TIM_SORT_NOT_MINE_H

#ifndef SORT_CMP
#define SORT_CMP(x, y)  ((x) < (y) ? -1 : ((x) == (y) ? 0 : 1))
#endif

#ifndef TIM_SORT_STACK_SIZE
#define TIM_SORT_STACK_SIZE 128
#endif

#define SORT_SWAP(x,y) {int __SORT_SWAP_t = (x); (x) = (y); (y) = __SORT_SWAP_t;}

#ifdef __GNUC__
#define CLZ __builtin_clzll
#endif

#ifndef MAX
#define MAX(x,y) (((x) > (y) ? (x) : (y)))
#endif

#ifndef MIN
#define MIN(x,y) (((x) < (y) ? (x) : (y)))
#endif

/* timsort implementation, based on timsort.txt */

typedef struct {
  uint64_t start;
  uint64_t length;
} TIM_SORT_RUN_T;

static __inline int compute_minrun(const uint64_t size) {
  const int top_bit = 64 - CLZ(size);
  const int shift = MAX(top_bit, 6) - 6;
  const int minrun = size >> shift;
  const uint64_t mask = (1ULL << shift) - 1;

  if (mask & size) {
    return minrun + 1;
  }

  return minrun;
}

/* Function used to do a binary search for binary insertion sort */
static __inline int64_t BINARY_INSERTION_FIND(int *dst, const int x,
    const size_t size) {
  int64_t l, c, r;
  int cx;
  l = 0;
  r = size - 1;
  c = r >> 1;

  /* check for out of bounds at the beginning. */
  if (SORT_CMP(x, dst[0]) < 0) {
    return 0;
  } else if (SORT_CMP(x, dst[r]) > 0) {
    return r;
  }

  cx = dst[c];

  while (1) {
    const int val = SORT_CMP(x, cx);

    if (val < 0) {
      if (c - l <= 1) {
        return c;
      }

      r = c;
    } else { /* allow = for stability. The binary search favors the right. */
      if (r - c <= 1) {
        return c + 1;
      }

      l = c;
    }

    c = l + ((r - l) >> 1);
    cx = dst[c];
  }
}

/* Binary insertion sort, but knowing that the first "start" entries are sorted.  Used in timsort. */
static void BINARY_INSERTION_SORT_START(int *dst, const size_t start, const size_t size) {
  uint64_t i;

  for (i = start; i < size; i++) {
    int64_t j;
    int x;
    int64_t location;

    /* If this entry is already correct, just move along */
    if (SORT_CMP(dst[i - 1], dst[i]) <= 0) {
      continue;
    }

    /* Else we need to find the right place, shift everything over, and squeeze in */
    x = dst[i];
    location = BINARY_INSERTION_FIND(dst, x, i);

    for (j = i - 1; j >= location; j--) {
      dst[j + 1] = dst[j];
    }

    dst[location] = x;
  }
}

/* Binary insertion sort */
void BINARY_INSERTION_SORT(int *dst, const size_t size) {
  /* don't bother sorting an array of size 0 */
  if (size == 0) {
    return;
  }

  BINARY_INSERTION_SORT_START(dst, 1, size);
}

static __inline void REVERSE_ELEMENTS(int *dst, int64_t start, int64_t end) {
  while (1) {
    if (start >= end) {
      return;
    }

    SORT_SWAP(dst[start], dst[end]);
    start++;
    end--;
  }
}

static int64_t COUNT_RUN(int *dst, const uint64_t start, const size_t size) {
  uint64_t curr;

  if (size - start == 1) {
    return 1;
  }

  if (start >= size - 2) {
    if (SORT_CMP(dst[size - 2], dst[size - 1]) > 0) {
      SORT_SWAP(dst[size - 2], dst[size - 1]);
    }

    return 2;
  }

  curr = start + 2;

  if (SORT_CMP(dst[start], dst[start + 1]) <= 0) {
    /* increasing run */
    while (1) {
      if (curr == size - 1) {
        break;
      }

      if (SORT_CMP(dst[curr - 1], dst[curr]) > 0) {
        break;
      }

      curr++;
    }

    return curr - start;
  } else {
    /* decreasing run */
    while (1) {
      if (curr == size - 1) {
        break;
      }

      if (SORT_CMP(dst[curr - 1], dst[curr]) <= 0) {
        break;
      }

      curr++;
    }

    /* reverse in-place */
    REVERSE_ELEMENTS(dst, start, curr - 1);
    return curr - start;
  }
}

static int CHECK_INVARIANT(TIM_SORT_RUN_T *stack, const int stack_curr) {
  int64_t A, B, C;

  if (stack_curr < 2) {
    return 1;
  }

  if (stack_curr == 2) {
    const int64_t A1 = stack[stack_curr - 2].length;
    const int64_t B1 = stack[stack_curr - 1].length;

    if (A1 <= B1) {
      return 0;
    }

    return 1;
  }

  A = stack[stack_curr - 3].length;
  B = stack[stack_curr - 2].length;
  C = stack[stack_curr - 1].length;

  if ((A <= B + C) || (B <= C)) {
    return 0;
  }

  return 1;
}

typedef struct {
  size_t alloc;
  int *storage;
} TEMP_STORAGE_T;

static void TIM_SORT_RESIZE(TEMP_STORAGE_T *store, const size_t new_size) {
  if (store->alloc < new_size) {
    int *tempstore = (int *)realloc(store->storage, new_size * sizeof(int));

    if (tempstore == NULL) {
      fprintf(stderr, "Error allocating temporary storage for tim sort: need %lu bytes",
              sizeof(int) * new_size);
      exit(1);
    }

    store->storage = tempstore;
    store->alloc = new_size;
  }
}

static void TIM_SORT_MERGE(int *dst, const TIM_SORT_RUN_T *stack, const int stack_curr,
                           TEMP_STORAGE_T *store) {
  const int64_t A = stack[stack_curr - 2].length;
  const int64_t B = stack[stack_curr - 1].length;
  const int64_t curr = stack[stack_curr - 2].start;
  int *storage;
  int64_t i, j, k;
  TIM_SORT_RESIZE(store, MIN(A, B));
  storage = store->storage;

  /* left merge */
  if (A < B) {
    memcpy(storage, &dst[curr], A * sizeof(int));
    i = 0;
    j = curr + A;

    for (k = curr; k < curr + A + B; k++) {
      if ((i < A) && (j < curr + A + B)) {
        if (SORT_CMP(storage[i], dst[j]) <= 0) {
          dst[k] = storage[i++];
        } else {
          dst[k] = dst[j++];
        }
      } else if (i < A) {
        dst[k] = storage[i++];
      } else {
        dst[k] = dst[j++];
      }
    }
  } else {
    /* right merge */
    memcpy(storage, &dst[curr + A], B * sizeof(int));
    i = B - 1;
    j = curr + A - 1;

    for (k = curr + A + B - 1; k >= curr; k--) {
      if ((i >= 0) && (j >= curr)) {
        if (SORT_CMP(dst[j], storage[i]) > 0) {
          dst[k] = dst[j--];
        } else {
          dst[k] = storage[i--];
        }
      } else if (i >= 0) {
        dst[k] = storage[i--];
      } else {
        dst[k] = dst[j--];
      }
    }
  }
}

static int TIM_SORT_COLLAPSE(int *dst, TIM_SORT_RUN_T *stack, int stack_curr,
                             TEMP_STORAGE_T *store, const size_t size) {
  while (1) {
    int64_t A, B, C, D;
    int ABC, BCD, CD;

    /* if the stack only has one thing on it, we are done with the collapse */
    if (stack_curr <= 1) {
      break;
    }

    /* if this is the last merge, just do it */
    if ((stack_curr == 2) && (stack[0].length + stack[1].length == size)) {
      TIM_SORT_MERGE(dst, stack, stack_curr, store);
      stack[0].length += stack[1].length;
      stack_curr--;
      break;
    }
    /* check if the invariant is off for a stack of 2 elements */
    else if ((stack_curr == 2) && (stack[0].length <= stack[1].length)) {
      TIM_SORT_MERGE(dst, stack, stack_curr, store);
      stack[0].length += stack[1].length;
      stack_curr--;
      break;
    } else if (stack_curr == 2) {
      break;
    }

    B = stack[stack_curr - 3].length;
    C = stack[stack_curr - 2].length;
    D = stack[stack_curr - 1].length;

    if (stack_curr >= 4) {
      A = stack[stack_curr - 4].length;
      ABC = (A <= B + C);
    } else {
      ABC = 0;
    }

    BCD = (B <= C + D) || ABC;
    CD = (C <= D);

    /* Both invariants are good */
    if (!BCD && !CD) {
      break;
    }

    /* left merge */
    if (BCD && !CD) {
      TIM_SORT_MERGE(dst, stack, stack_curr - 1, store);
      stack[stack_curr - 3].length += stack[stack_curr - 2].length;
      stack[stack_curr - 2] = stack[stack_curr - 1];
      stack_curr--;
    } else {
      /* right merge */
      TIM_SORT_MERGE(dst, stack, stack_curr, store);
      stack[stack_curr - 2].length += stack[stack_curr - 1].length;
      stack_curr--;
    }
  }

  return stack_curr;
}

static __inline int PUSH_NEXT(int *dst,
                              const size_t size,
                              TEMP_STORAGE_T *store,
                              const uint64_t minrun,
                              TIM_SORT_RUN_T *run_stack,
                              uint64_t *stack_curr,
                              uint64_t *curr) {
  uint64_t len = COUNT_RUN(dst, *curr, size);
  uint64_t run = minrun;

  if (run > size - *curr) {
    run = size - *curr;
  }

  if (run > len) {
    BINARY_INSERTION_SORT_START(&dst[*curr], len, run);
    len = run;
  }

  run_stack[*stack_curr].start = *curr;
  run_stack[*stack_curr].length = len;
  (*stack_curr)++;
  *curr += len;

  if (*curr == size) {
    /* finish up */
    while (*stack_curr > 1) {
      TIM_SORT_MERGE(dst, run_stack, *stack_curr, store);
      run_stack[*stack_curr - 2].length += run_stack[*stack_curr - 1].length;
      (*stack_curr)--;
    }

    if (store->storage != NULL) {
      free(store->storage);
      store->storage = NULL;
    }

    return 0;
  }

  return 1;
}

void TIM_SORT(int *dst, const size_t size) {
  /* don't bother sorting an array of size 0 */
  if (size == 0) {
    return;
  }

  uint64_t minrun;
  TEMP_STORAGE_T _store, *store;
  TIM_SORT_RUN_T run_stack[TIM_SORT_STACK_SIZE];
  uint64_t stack_curr = 0;
  uint64_t curr = 0;

  if (size < 64) {
    BINARY_INSERTION_SORT(dst, size);
    return;
  }

  /* compute the minimum run length */
  minrun = compute_minrun(size);
  /* temporary storage for merges */
  store = &_store;
  store->alloc = 0;
  store->storage = NULL;

  if (!PUSH_NEXT(dst, size, store, minrun, run_stack, &stack_curr, &curr)) {
    return;
  }

  if (!PUSH_NEXT(dst, size, store, minrun, run_stack, &stack_curr, &curr)) {
    return;
  }

  if (!PUSH_NEXT(dst, size, store, minrun, run_stack, &stack_curr, &curr)) {
    return;
  }

  while (1) {
    if (!CHECK_INVARIANT(run_stack, stack_curr)) {
      stack_curr = TIM_SORT_COLLAPSE(dst, run_stack, stack_curr, store, size);
      continue;
    }

    if (!PUSH_NEXT(dst, size, store, minrun, run_stack, &stack_curr, &curr)) {
      return;
    }
  }
}

void timSortWrapper(int *arr, int p, int r)
{
    TIM_SORT(arr+p, r-p+1);
}

#endif // TIM_SORT_NOT_MINE_H
