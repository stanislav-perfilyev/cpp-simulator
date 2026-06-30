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
