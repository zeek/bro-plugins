#
# A Makefile with convenience targets, primarily meant for automatic
# building and testing.
#

build-all: build-dataseries build-elasticsearch build-netmap
test-all:  test-dataseries test-elasticsearch test-netmap

build-dataseries:
	make -C dataseries

build-elasticsearch:
	make -C elasticsearch

build-netmap:
	make -C netmap

test-dataseries:
	make -C dataseries test

test-elasticsearch:
	make -C elasticsearch test

test-netmap:
	make -C netmap test
