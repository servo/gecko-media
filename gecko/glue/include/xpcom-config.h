/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/* Global defines needed by xpcom clients */

#ifndef _XPCOM_CONFIG_H_
#define _XPCOM_CONFIG_H_

/* Define this to throw() if the compiler complains about 
 * constructors returning NULL
 */
#define CPP_THROW_NEW throw()

/* Define if a dyanmic_cast to void* gives the most derived object */
#define HAVE_CPP_DYNAMIC_CAST_TO_VOID_PTR 1

/* Define to a string describing the XPCOM ABI in use */
#define TARGET_XPCOM_ABI "x86_64-gcc3"

#endif /* _XPCOM_CONFIG_H_ */
