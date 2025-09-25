#pragma once

#include "eventpp/callbacklist.h"

template <typename Result, typename... Args> struct Callback {
  typedef eventpp::CallbackList<Result(Args...)> CallbackList;
  typedef typename CallbackList::Handle Handle;
  Callback() = default;

  Handle Add(Result (*Fn)(Args...)) { return mCallbackList.append(Fn); }

  template <typename T> Handle Add(T* Obj, Result (T::*Fn)(Args...)) {
    return mCallbackList.append([Obj, Fn](Args... args) -> Result { return (Obj->*Fn)(std::forward<Args>(args)...); });
  }

  template <typename... OtherArgs>
    requires std::invocable<CallbackList, OtherArgs...>
  Result Invoke(OtherArgs&&... InArgs) {
    return mCallbackList(std::forward<OtherArgs>(InArgs)...);
  }

  bool Remove(const Handle& InHandle) { return mCallbackList.remove(InHandle); }
  void Clear() { mCallbackList = {}; }

private:
  CallbackList mCallbackList;
};
