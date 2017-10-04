#pragma once

#include <entityx/entityx.h>

using CEntity = entityx::Entity;

template<class T>
using CComponentHandle = entityx::ComponentHandle<T>;
