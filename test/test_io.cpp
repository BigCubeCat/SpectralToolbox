#include <spdlog/spdlog.h>

#include <gtest/gtest.h>

#include "segyreader.hpp"

TEST(IO, TestSegyReader) {
    // пример как писать тесты
    auto reader = segy_reader("test/data/stack_0_18.sgy");
    auto count  = reader.count_traces();
    EXPECT_EQ(count, 40401);
    EXPECT_EQ(reader.binheader() != nullptr, true);
    for (int i = 0; i < count; ++i) {
        auto tr     = reader.trace(i);
        auto header = reader.traceheader(i);
        float r     = 0;
        for (const float &value : tr) {
            r += abs(value);
        }
        EXPECT_EQ(r != 0, true);
    }
}
