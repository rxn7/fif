#pragma once

#include <algorithm>
#include <array>
#include <chrono>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <ratio>
#include <string>
#include <unordered_map>
#include <vector>

#include "fif/core/types.hpp"

#include "entt/entity/fwd.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/compatibility.hpp"

#include "fif/core/event/event.hpp"
#include "fif/core/event/event_dispatcher.hpp"
#include "fif/core/opengl.hpp"

#include "fif/core/util/assertion.hpp"
#include "fif/core/util/clock.hpp"
#include "fif/core/util/function_name.hpp"
#include "fif/core/util/logger.hpp"
#include "fif/core/util/rng.hpp"
#include "fif/core/util/timing.hpp"

#include "fif/core/application.hpp"
