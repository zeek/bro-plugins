#
# A Makefile with convenience targets, primarily meant for automatic
# building and testing.
#

build-all: build-dataseries build-elasticsearch build-netmap build-redis
test-all:  test-dataseries test-elasticsearch test-netmap test-redis

build-dataseries:
	make -C dataseries

build-elasticsearch:
	make -C elasticsearch

build-netmap:
	make -C netmap

build-redis:
	make -C redis

test-dataseries:
	make -C dataseries test

test-elasticsearch:
	make -C elasticsearch test

test-netmap:
	make -C netmap test

test-redis:
	make -C redis test
