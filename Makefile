#
# A Makefile with convinience targets, primarily mean for automatic building and testing.
#

build-all: build-dataseries build-elasticsearch
test-all:  test-dataseries test-elasticsearch

build-dataseries:
	make -C dataseries

build-elasticsearch:
	make -C elasticsearch

test-dataseries:
	make -C dataseries test

test-elasticsearch:
	make -C elasticsearch test
