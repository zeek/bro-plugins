# - Try to find kernel includes.
#
# Based on FindKernelHeaders.cmake by David Sansome (me@davidsansome.com)
#
# Variables used by this module, they can change the default behaviour and need
# to be set before calling find_package:
#
#  KERNELHEADERS_ROOT_DIR    Set this variable to the root directory of the
#                            kernel sources if the module has problems finding
#                            the proper path.
#
# Variables defined by this module:
#
#  KERNELHEADERS_FOUND         System has kernel headers.

# Find the kernel release
execute_process(
  COMMAND uname -r
  OUTPUT_VARIABLE KERNEL_RELEASE
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

find_path(KERNELHEADERS_ROOT_DIR
  NAMES include/linux/user.h
  PATHS /usr/src/linux-headers-${KERNEL_RELEASE}
        /usr/src/kernels/${KERNEL_RELEASE}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(KernelHeaders DEFAULT_MSG
    KERNELHEADERS_ROOT_DIR
)

mark_as_advanced(
    KERNELHEADERS_ROOT_DIR
)