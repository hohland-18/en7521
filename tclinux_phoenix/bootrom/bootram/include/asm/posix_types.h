/* $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/bootrom/bootram/include/asm/posix_types.h#1 $
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1996, 1997, 1998, 2000 by Ralf Baechle
 */
#ifndef _ASM_POSIX_TYPES_H
#define _ASM_POSIX_TYPES_H

/*
 * This file is generally used by user-level software, so you need to
 * be a little careful about namespace pollution etc.  Also, we cannot
 * assume GCC is being used.
 */

typedef unsigned int	__kernel_dev_t;
typedef unsigned long	__kernel_ino_t;
typedef unsigned int	__kernel_mode_t;
typedef int		__kernel_nlink_t;
typedef long		__kernel_off_t;
typedef int		__kernel_pid_t;
typedef int		__kernel_ipc_pid_t;
typedef int		__kernel_uid_t;
typedef int		__kernel_gid_t;
typedef unsigned int	__kernel_size_t;
typedef int		__kernel_ssize_t;
typedef int		__kernel_ptrdiff_t;
typedef long		__kernel_time_t;
typedef long		__kernel_suseconds_t;
typedef long		__kernel_clock_t;
typedef long		__kernel_daddr_t;
typedef char *		__kernel_caddr_t;

typedef unsigned short	__kernel_uid16_t;
typedef unsigned short	__kernel_gid16_t;
typedef int		__kernel_uid32_t;
typedef int		__kernel_gid32_t;
typedef __kernel_uid_t	__kernel_old_uid_t;
typedef __kernel_gid_t	__kernel_old_gid_t;

#ifdef __GNUC__
typedef long long      __kernel_loff_t;
#endif

typedef struct {
        long    val[2];
} __kernel_fsid_t;

#if defined(__KERNEL__) || !defined(__GLIBC__) || (__GLIBC__ < 2)

#undef __FD_SET
static __inline__ void __FD_SET(unsigned long __fd, __kernel_fd_set *__fdsetp)
{
	unsigned long __tmp = __fd / __NFDBITS;
	unsigned long __rem = __fd % __NFDBITS;
	__fdsetp->fds_bits[__tmp] |= (1UL<<__rem);
}

#undef __FD_CLR
static __inline__ void __FD_CLR(unsigned long __fd, __kernel_fd_set *__fdsetp)
{
	unsigned long __tmp = __fd / __NFDBITS;
	unsigned long __rem = __fd % __NFDBITS;
	__fdsetp->fds_bits[__tmp] &= ~(1UL<<__rem);
}

#undef __FD_ISSET
static __inline__ int __FD_ISSET(unsigned long __fd, const __kernel_fd_set *__p)
{ 
	unsigned long __tmp = __fd / __NFDBITS;
	unsigned long __rem = __fd % __NFDBITS;
	return (__p->fds_bits[__tmp] & (1UL<<__rem)) != 0;
}

/*
 * This will unroll the loop for the normal constant case (8 ints,
 * for a 256-bit fd_set)
 */
#undef __FD_ZERO
static __inline__ void __FD_ZERO(__kernel_fd_set *__p)
{
	unsigned long *__tmp = __p->fds_bits;
	int __i;

	if (__builtin_constant_p(__FDSET_LONGS)) {
		switch (__FDSET_LONGS) {
		case 16:
			__tmp[ 0] = 0; __tmp[ 1] = 0;
			__tmp[ 2] = 0; __tmp[ 3] = 0;
			__tmp[ 4] = 0; __tmp[ 5] = 0;
			__tmp[ 6] = 0; __tmp[ 7] = 0;
			__tmp[ 8] = 0; __tmp[ 9] = 0;
			__tmp[10] = 0; __tmp[11] = 0;
			__tmp[12] = 0; __tmp[13] = 0;
			__tmp[14] = 0; __tmp[15] = 0;
			return;

		case 8:
			__tmp[ 0] = 0; __tmp[ 1] = 0;
			__tmp[ 2] = 0; __tmp[ 3] = 0;
			__tmp[ 4] = 0; __tmp[ 5] = 0;
			__tmp[ 6] = 0; __tmp[ 7] = 0;
			return;

		case 4:
			__tmp[ 0] = 0; __tmp[ 1] = 0;
			__tmp[ 2] = 0; __tmp[ 3] = 0;
			return;
		}
	}
	__i = __FDSET_LONGS;
	while (__i) {
		__i--;
		*__tmp = 0;
		__tmp++;
	}
}

#endif /* defined(__KERNEL__) || !defined(__GLIBC__) || (__GLIBC__ < 2) */

#endif /* _ASM_POSIX_TYPES_H */
