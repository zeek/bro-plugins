#
# A Makefile with convenience targets, primarily meant for automatic
# building and testing.
#

build-all: build-dataseries build-elasticsearch build-netmap build-pf_ring build-redis build-myricom build-af_packet
test-all:  test-dataseries test-elasticsearch test-netmap test-pf_ring test-redis test-myricom test-af_packet

build-dataseries:
	make -C dataseries

build-elasticsearch:
	make -C elasticsearch

build-netmap:
	make -C netmap

build-pf_ring:
	make -C pf_ring

build-redis:
	make -C redis

build-myricom:
	make -C myricom

build-af_packet:
	make -C af_packet

test-dataseries:
	make -C dataseries test

test-elasticsearch:
	make -C elasticsearch test

test-netmap:
	make -C netmap test

test-pf_ring:
	make -C pf_ring test

test-redis:
	make -C redis test

test-myricom:
	make -C myricom test

test-myricom:
	make -C myricom test
