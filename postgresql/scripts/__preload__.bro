#
# This is loaded unconditionally at Bro startup before any of the BiFs that  the
# plugin defines become available.
#
# This is primarily for defining types that BiFs already depend on. If you need
# to do any other unconditional initialization (usually that's just for other BiF
# elemets), that should go into __load__.bro instead.
#

@load ./types.bro


