#
# A Makefile with convenience targets, primarily meant for automatic
# building and testing.
#

build-all: build-elasticsearch build-netmap build-pf_ring build-redis build-kafka
test-all:  test-elasticsearch test-netmap test-pf_ring test-redis test-kafka

build-elasticsearch:
	make -C elasticsearch

build-netmap:
	make -C netmap

build-pf_ring:
	make -C pf_ring

build-redis:
	make -C redis

build-kafka:
	make -C kafka

test-elasticsearch:
	make -C elasticsearch test

test-netmap:
	make -C netmap test

test-pf_ring:
	make -C pf_ring test

test-redis:
	make -C redis test

test-kafka:
	make -C kafka test
