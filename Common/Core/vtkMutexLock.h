/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkMutexLock.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkMutexLock
 * @brief   mutual exclusion locking class
 *
 * vtkMutexLock allows the locking of variables which are accessed
 * through different threads.  This header file also defines
 * vtkSimpleMutexLock which is not a subclass of vtkObject.
 */

#ifndef vtkMutexLock_h
#define vtkMutexLock_h

#include "vtkCommonCoreModule.h" // For export macro
#include "vtkDeprecation.h"      // For VTK_DEPRECATED_IN_9_1_0
#include "vtkObject.h"
#include "vtkThreads.h" // for VTK_USE_PTHREADS and VTK_USE_WIN32_THREADS

#if defined(VTK_USE_PTHREADS)
#include <pthread.h> // Needed for PTHREAD implementation of mutex
typedef pthread_mutex_t vtkMutexType;
#endif

#ifdef VTK_USE_WIN32_THREADS
typedef vtkWindowsHANDLE vtkMutexType;
#endif

#ifndef VTK_USE_PTHREADS
#ifndef VTK_USE_WIN32_THREADS
typedef int vtkMutexType;
#endif
#endif

// Mutex lock that is not a vtkObject.

class VTK_DEPRECATED_IN_9_1_0("Use std::mutex instead.") VTKCOMMONCORE_EXPORT vtkSimpleMutexLock
{
public:
  // left public purposely
  vtkSimpleMutexLock();
  virtual ~vtkSimpleMutexLock();

  static vtkSimpleMutexLock* New();

  void Delete() { delete this; }

  /**
   * Lock the vtkMutexLock
   */
  void Lock();

  /**
   * Unlock the vtkMutexLock
   */
  void Unlock();

protected:
  friend class vtkSimpleConditionVariable;
  vtkMutexType MutexLock;

private:
  vtkSimpleMutexLock(const vtkSimpleMutexLock& other) = delete;
  vtkSimpleMutexLock& operator=(const vtkSimpleMutexLock& rhs) = delete;
};

class VTK_DEPRECATED_IN_9_1_0("Use std::mutex instead.") VTKCOMMONCORE_EXPORT vtkMutexLock
  : public vtkObject
{
public:
  static vtkMutexLock* New();

  vtkTypeMacro(vtkMutexLock, vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Lock the vtkMutexLock
   */
  void Lock();

  /**
   * Unlock the vtkMutexLock
   */
  void Unlock();

protected:
  friend class vtkConditionVariable; // needs to get at SimpleMutexLock.

  vtkSimpleMutexLock SimpleMutexLock;
  vtkMutexLock() = default;

private:
  vtkMutexLock(const vtkMutexLock&) = delete;
  void operator=(const vtkMutexLock&) = delete;
};

inline void vtkMutexLock::Lock()
{
  this->SimpleMutexLock.Lock();
}

inline void vtkMutexLock::Unlock()
{
  this->SimpleMutexLock.Unlock();
}

#endif

// VTK-HeaderTest-Exclude: vtkMutexLock.h
