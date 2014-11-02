#pragma once
#include <memory>

#define DEF_PTR(TYPE) typedef std::shared_ptr<class TYPE> TYPE##Ptr;
#define DEF_PTR_STRUCT(TYPE) typedef std::shared_ptr<struct TYPE> TYPE##Ptr;
