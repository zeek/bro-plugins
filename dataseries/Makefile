
cmake_build_dir=build
arch=`uname -s | tr A-Z a-z`-`uname -m`
bro_default=../../..

all: build-it

check:
	@if [ ! -e "${bro_default}/bro-path-dev.in" ]; then \
		test -n "$$BRO" || (echo 'BRO not set/found; use "make BRO=<path-to-bro-source-tree>"'; exit 1); \
		test -e $$BRO/bro-path-dev.in || (echo "$$BRO is not a Bro source tree"; exit 1); \
		test -e $$BRO/build/bro-path-dev || (echo "$$BRO/build is not a Bro build tree; did you build Bro?"; exit 1) \
	fi

configure: check
	@mkdir -p $(cmake_build_dir)
	@if [ -e $$BRO/bro-path-dev.in ]; then \
	    ( cd $(cmake_build_dir) && cmake -DBRO_DIST=$$BRO -DCMAKE_MODULE_PATH=$$BRO/cmake .. ) \
	else \
		( cd $(cmake_build_dir) && cmake -DBRO_DIST=`cd ${bro_default}/.. && pwd` -DCMAKE_MODULE_PATH=`cd ${bro_default}/../cmake && pwd` .. ) \
	fi

build-it:
	@test -e $(cmake_build_dir) || make configure
	( cd $(cmake_build_dir) && make )

install:
	( cd $(cmake_build_dir) && make install )

clean:
	( cd $(cmake_build_dir) && make clean )

distclean:
	rm -rf $(cmake_build_dir) lib __bro_plugin__

test:
	make -C tests

sdist: build-it
	@cd build && \
	rm -rf sdist && \
	mkdir sdist && \
	cd sdist && \
	mkdir `cat ../../__bro_plugin__ | sed 's/::/_/g'` && \
	(cd ../.. && tar cf - --exclude lib --exclude build --exclude __bro_plugin__ --exclude '.?*' . ) \
		| ( cd `ls .` && tar xf - ) && \
	tar czvf `ls .`.tar.gz `ls .` && \
	echo Source distribution in build/sdist/`ls *.tar.gz`

bdist: build-it
	@( export DIR=`pwd`/build && \
	cd $(cmake_build_dir) && \
	rm -rf bdist && \
	DESTDIR="`pwd`/bdist" make install >/dev/null && \
	cd `find bdist -name __bro_plugin__ | sed 's/__bro_plugin__/../g'` && \
	tar czvf $$DIR/`ls . | head -1`-${arch}.tar.gz * && \
	echo Binary distribution in build/`ls .`-${arch}.tar.gz )

