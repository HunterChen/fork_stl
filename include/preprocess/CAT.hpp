
#ifndef __FORK_STL_CAT_HPP__
#define __FORK_STL_CAT_HPP__
/* ****************************************************
 * Concat Symbol
 *
 * MY_CAT(a, b) a,b首先会被扩展然后再粘帖成一个符号
 *
 * Crate: 05/26/2014 
 *      : 符号连接 
 *
 * ****************************************************/
#
#define MY_CAT(a, b) MY_CAT_I(a, b)
#
#define MY_CAT_I(a, b) a ## b
#
#
#endif

