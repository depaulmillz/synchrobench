.PHONY:	all

BENCHS = src/trees/sftree src/linkedlists/lockfree-list src/hashtables/lockfree-ht src/trees/rbtree src/skiplists/sequential
LBENCHS = src/trees/tree-lock src/linkedlists/lock-coupling-list src/linkedlists/lazy-list src/hashtables/lockbased-ht src/skiplists/skiplist-lock
LFBENCHS = src/hashtables/lockfree-ht #src/trees/lfbstree src/linkedlists/lockfree-list src/hashtables/lockfree-ht src/skiplists/rotating src/skiplists/fraser src/skiplists/nohotspot src/skiplists/numask

# Only compile C11/GNU11 algorithms with compatible compiler
GCC_GTEQ_490 := $(shell expr `gcc -dumpversion | sed -e 's/\.\([0-9][0-9]\)/\1/g' -e 's/\.\([0-9]\)/0\1/g' -e 's/^[0-9]\{3,4\}$$/&00/'` \>= 40900)
ifeq "$(GCC_GTEQ_490)" "1"
	BENCHS +=
	LBENCHS += src/linkedlists/versioned
	LFBENCHS += src/linkedlists/selfish
endif

#MAKEFLAGS+=-j4

MALLOC=TC

.PHONY:	clean all $(BENCHS) $(LBENCHS)

all:	lock spinlock lockfree estm sequential

lock: clean-build
	$(MAKE) "LOCK=MUTEX" $(LBENCHS)

spinlock: clean-build
	$(MAKE) "LOCK=SPIN" $(LBENCHS)

sequential: clean-build
	$(MAKE) "STM=SEQUENTIAL" $(BENCHS)

lockfree: clean-build
	for dir in $(LFBENCHS); do \
	$(MAKE) "STM=LOCKFREE" -C $$dir; \
	done

estm: clean-build
	$(MAKE) -C src/utils/estm-0.3.0
	$(MAKE) "STM=ESTM" $(BENCHS)

clean-build: 
	rm -rf build

clean:
	$(MAKE) -C src/utils/estm-0.3.0 clean
	rm -rf build bin

$(BENCHS):
	$(MAKE) -C $@ $(TARGET)

$(LBENCHS):
	$(MAKE) -C $@ $(TARGET)
