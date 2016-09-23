#
# A Makefile with convenience targets, primarily meant for automatic
# building and testing.
#

<<<<<<< HEAD
build-all: build-dataseries build-elasticsearch build-netmap build-pf_ring build-redis build-myricom build-af_packet
test-all:  test-dataseries test-elasticsearch test-netmap test-pf_ring test-redis test-myricom test-af_packet

build-dataseries:
	make -C dataseries
=======
build-all: build-elasticsearch build-netmap build-pf_ring build-redis build-myricom build-af_packet build-tcprs build-kafka build-postgresql
test-all:  test-elasticsearch test-netmap test-pf_ring test-redis test-myricom test-af_packet test-tcprs test-kafka test-postgresql
>>>>>>> bro/master

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

<<<<<<< HEAD
test-dataseries:
	make -C dataseries test
=======
build-tcprs:
	make -C tcprs

build-kafka:
	make -C kafka

build-postgresql:
	make -C postgresql
>>>>>>> bro/master

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

<<<<<<< HEAD
test-myricom:
	make -C myricom test
=======
test-af_packet:
	make -C af_packet test

test-tcprs:
	make -C tcprs test

test-kafka:
	make -C kafka test

test-postgresql:
	make -C postgresql test
>>>>>>> bro/master
