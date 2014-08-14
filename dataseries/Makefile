#
# Convenience Makefile providing a few common top-level targets.
#

cmake_build_dir=build
arch=`uname -s | tr A-Z a-z`-`uname -m`

all: build-it

build-it:
	@test -e $(cmake_build_dir)/config.status || ./configure
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

