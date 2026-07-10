#include <gtest/gtest.h>
#include "Game.h"
#include "Exceptions.h"

// ── Exception hierarchy ───────────────────────────────────────────────────

TEST(ExceptionHierarchy, FishCaughtIsSimulatorError) {
    EXPECT_NO_THROW({
        try {
            throw FishCaught(5);
        } catch (const SimulatorError& e) {
            EXPECT_STREQ(e.what(), "Fish caught!");
        }
    });
}

TEST(ExceptionHierarchy, BootCaughtIsSimulatorError) {
    EXPECT_NO_THROW({
        try {
            throw BootCaught(3);
        } catch (const SimulatorError& e) {
            EXPECT_STREQ(e.what(), "Boot caught!");
        }
    });
}

TEST(ExceptionHierarchy, FishCaughtIsStdException) {
    try {
        throw FishCaught(7);
    } catch (const std::exception& e) {
        EXPECT_STREQ(e.what(), "Fish caught!");
    }
}

TEST(ExceptionHierarchy, FishCaughtCarriesAttempts) {
    try { throw FishCaught(42); }
    catch (const FishCaught& fc) { EXPECT_EQ(fc.attempts, 42); }
}

TEST(ExceptionHierarchy, BootCaughtCarriesAttempts) {
    try { throw BootCaught(13); }
    catch (const BootCaught& bc) { EXPECT_EQ(bc.attempts, 13); }
}

// ── InvalidSectorError ───────────────────────────────────────────────────

TEST(ExceptionHierarchy, InvalidSectorCarriesIndex) {
    try { throw InvalidSectorError(99); }
    catch (const InvalidSectorError& e) {
        EXPECT_EQ(e.sector, 99);
        EXPECT_NE(std::string(e.what()).find("99"), std::string::npos);
    }
}

// ── Game::processInput boundary checks ───────────────────────────────────

TEST(GameProcessInput, NegativeSectorThrowsInvalidSector) {
    Game g;
    EXPECT_THROW(g.processInput(-1), InvalidSectorError);
}

TEST(GameProcessInput, TooLargeSectorThrowsInvalidSector) {
    Game g;
    EXPECT_THROW(g.processInput(static_cast<int>(Game::kFieldSize)), InvalidSectorError);
}

TEST(GameProcessInput, ValidSectorDoesNotThrowInvalidSector) {
    // A valid sector may throw FishCaught or BootCaught, but NOT InvalidSectorError.
    Game g;
    for (int i = 0; i < static_cast<int>(Game::kFieldSize); ++i) {
        try {
            g.processInput(i);
        } catch (const InvalidSectorError&) {
            FAIL() << "InvalidSectorError thrown for valid sector " << i;
        } catch (const SimulatorError&) {
            // FishCaught or BootCaught — expected
        }
    }
}

TEST(GameProcessInput, AttemptsIncrementOnEachCall) {
    // Keep clicking until we catch something; attempts must equal call count.
    Game g;
    int calls = 0;
    for (int i = 0; i < static_cast<int>(Game::kFieldSize); ++i) {
        try {
            g.processInput(i);
            ++calls;
        } catch (const FishCaught& fc) {
            EXPECT_EQ(fc.attempts, calls + 1);
            return;
        } catch (const BootCaught& bc) {
            EXPECT_EQ(bc.attempts, calls + 1);
            return;
        }
    }
}

// ── Constants ─────────────────────────────────────────────────────────────

TEST(GameConstants, FieldSizeIsNine) {
    EXPECT_EQ(Game::kFieldSize, 9u);
}

TEST(GameConstants, BootCountIsThree) {
    EXPECT_EQ(Game::kBootCount, 3);
}

// ── Exhaustive field scan (covers empty-sector path + placement invariants) ─

TEST(GameField, ExactlyOneFishAndThreeBoots) {
    // Process every sector; count catches — verifies placeFish/placeBoots.
    Game g;
    int fish_count = 0;
    int boot_count = 0;
    int empty_count = 0;

    for (int i = 0; i < static_cast<int>(Game::kFieldSize); ++i) {
        try {
            g.processInput(i);   // empty sector: no throw
            ++empty_count;
        } catch (const FishCaught&) {
            ++fish_count;
        } catch (const BootCaught&) {
            ++boot_count;
        }
    }

    EXPECT_EQ(fish_count,  1);
    EXPECT_EQ(boot_count,  static_cast<int>(Game::kBootCount));
    EXPECT_EQ(empty_count, static_cast<int>(Game::kFieldSize) - 1 - static_cast<int>(Game::kBootCount));
}

TEST(GameField, AttemptsCountEqualsCallsAcrossFullScan) {
    Game g;
    int calls = 0;
    int last_attempts = 0;

    for (int i = 0; i < static_cast<int>(Game::kFieldSize); ++i) {
        ++calls;
        try {
            g.processInput(i);
        } catch (const FishCaught& fc) {
            last_attempts = static_cast<int>(fc.attempts);
        } catch (const BootCaught& bc) {
            last_attempts = static_cast<int>(bc.attempts);
        }
    }
    // Total calls must equal kFieldSize; last catch attempts must match.
    EXPECT_EQ(calls, static_cast<int>(Game::kFieldSize));
    EXPECT_EQ(last_attempts, calls);
}

TEST(GameProcessInput, DoubleVisitSameSectorStillAccepted) {
    // After catching from a sector, revisiting should not crash.
    Game g;
    // Find the first sector that throws (fish or boot)
    for (int i = 0; i < static_cast<int>(Game::kFieldSize); ++i) {
        bool caught = false;
        try { g.processInput(i); }
        catch (const SimulatorError&) { caught = true; }
        if (caught) {
            // Visit same sector again — object already "caught", should be empty now
            EXPECT_NO_THROW({ try { g.processInput(i); } catch (const SimulatorError&) {} });
            break;
        }
    }
}
