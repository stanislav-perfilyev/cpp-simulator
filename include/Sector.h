#pragma once
#include <memory>
#include "Fish.h"
#include "Boot.h"

/// Sector — one of the nine pond cells; holds at most one Fish or one Boot (never both).
struct Sector {
    std::unique_ptr<Fish> fish;
    std::unique_ptr<Boot> boot;
};
