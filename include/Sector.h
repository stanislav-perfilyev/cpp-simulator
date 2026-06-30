#pragma once
#include <memory>
#include "Fish.h"
#include "Boot.h"

struct Sector {
    std::unique_ptr<Fish> fish;
    std::unique_ptr<Boot> boot;
};
