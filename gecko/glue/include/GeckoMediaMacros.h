/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsTArray.h"

#define DEF_GECKO_MEDIA_REFLECTOR_CONTAINER(Class) \
  static nsTArray<Class> sReflectors;

#define IMPL_GECKO_MEDIA_REFLECTOR_GETTER(Class) \
  static Class *GetReflector(size_t aId)         \
  {                                              \
    for (Class & reflector : sReflectors)        \
    {                                            \
      if (reflector.mId == aId)                  \
      {                                          \
        return &reflector;                       \
      }                                          \
    }                                            \
    return nullptr;                              \
  }

#define IMPL_GECKO_MEDIA_REFLECTOR_CONSTRUCTOR(GMClass, Class, Callbacks)     \
  void GeckoMedia_##Class##_Create(size_t aId, Callbacks aCallbacks)          \
  {                                                                           \
    GMClass *reflector = sReflectors.AppendElement(GMClass(aId, aCallbacks)); \
    if (NS_WARN_IF(!reflector))                                               \
    {                                                                         \
      return;                                                                 \
    }                                                                         \
    for (size_t i = 0; i < sReflectors.Length(); i++)                         \
    {                                                                         \
      if (sReflectors[i].mId == aId)                                          \
      {                                                                       \
        sReflectors.RemoveElementAt(i);                                       \
        break;                                                                \
      }                                                                       \
    }                                                                         \
  }

#define IMPL_GECKO_MEDIA_REFLECTOR_GET(Class) \
  Class *reflector = GetReflector(aId);       \
  if (NS_WARN_IF(!reflector))                 \
  {                                           \
    return;                                   \
  }

#define IMPL_GECKO_MEDIA_REFLECTOR_SHUTDOWN_BEGIN(GMClass, Class) \
  void GeckoMedia_##Class##_Shutdown(size_t aId)                  \
  {                                                               \
    GMClass *reflector = GetReflector(aId);                       \
    if (NS_WARN_IF(!reflector))                                   \
    {                                                             \
      return;                                                     \
    }

#define IMPL_GECKO_MEDIA_REFLECTOR_SHUTDOWN_END     \
  for (size_t i = 0; i < sReflectors.Length(); i++) \
  {                                                 \
    if (sReflectors[i].mId == aId)                  \
    {                                               \
      sReflectors.RemoveElementAt(i);               \
      break;                                        \
    }                                               \
  }                                                 \
  }

#define IMPL_GECKO_MEDIA_REFLECTOR_SHUTDOWN(GMClass, Class) \
  IMPL_GECKO_MEDIA_REFLECTOR_SHUTDOWN_BEGIN(GMClass, Class) \
  IMPL_GECKO_MEDIA_REFLECTOR_SHUTDOWN_END

#define IMPL_GECKO_MEDIA_REFLECTOR(GMClass, Class, Callbacks)       \
  DEF_GECKO_MEDIA_REFLECTOR_CONTAINER(GMClass)                      \
  IMPL_GECKO_MEDIA_REFLECTOR_GETTER(GMClass)                        \
  IMPL_GECKO_MEDIA_REFLECTOR_CONSTRUCTOR(GMClass, Class, Callbacks) \
  IMPL_GECKO_MEDIA_REFLECTOR_SHUTDOWN(GMClass, Class)