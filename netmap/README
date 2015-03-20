
Bro::Netmap
===========

This plugin provides native `netmap
<http://info.iet.unipi.it/~luigi/netmap>`_ support for Bro.

Installation
------------

Follow netmap's instructions to get its kernel module and,
potentially, custom drivers installed. The following will then compile
and install the netmap plugin alongside Bro, assuming it can find the
netmap headers in a standard location::

    # ./configure && make && make install

If the headers are installed somewhere non-standard, add
``--with-netmap=<netmap-base-directory>`` to the ``configure``
command. If everything built and installed correctly, you should see
this::

    # bro -N Bro::Netmap
    Bro::Netmap - Packet acquisition via netmap (dynamic, version 1.0)

To use netmap, Bro needs read and write access to ``/dev/netmap``. If
you give that permission to a user, you can run Bro as non-root.

Usage
-----

Once installed, you can use netmap interfaces/ports by prefixing them
with either ``netmap::`` or ``vale::`` on the command line. For example,
to use netmap to monitor interface ``eth0``::

    bro -i netmap::eth0

Netmap does not enable promiscuous mode on interfaces,
you'll have to do that yourself. For example, on Linux::

    ifconfig eth0 promisc
