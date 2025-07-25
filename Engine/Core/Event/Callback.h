#pragma once

#include "eventpp/callbacklist.h"

template <typename Result, typename... Args> struct Callback {
  typedef eventpp::CallbackList<Result(Args...)> CallbackList;
  typedef typename CallbackList::Handle Handle;
  Callback() = default;

  Handle Add(Result (*Fn)(Args...)) { return mCallbackList.append(Fn); }

  template <typename T, typename... OtherArgs> Handle Add(T* Obj, Result (T::*Fn)(Args...)) {
    return mCallbackList.append([Obj, Fn](Args&&... args) -> Result { return (Obj->*Fn)(std::forward<Args>(args)...); });
  }

  Result Invoke(Args&&... InArgs) { return mCallbackList(std::forward<Args>(InArgs)...); }
  bool Remove(const Handle& InHandle) { return mCallbackList.remove(InHandle); }
  void Clear() { mCallbackList = {}; }

private:
  CallbackList mCallbackList;
};
