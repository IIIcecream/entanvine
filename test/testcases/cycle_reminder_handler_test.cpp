#include "gtest/gtest.h"

#include "work/cycle_reminder_handler.h"

TEST(cycle_reminder, no_reminder) {
    work::CycleReminderPriorityHandler<int> handler;
    handler.init({{1, {1, true}}, {2, {2, false}}, {3, {3, true}}, {4, {4, true}}});

    handler.reset();
    ASSERT_FALSE(handler.need_send_reminder());
    ASSERT_EQ(handler.get_highest_priority_id(), 0);
}

TEST(cycle_reminder, set_one_within_frame) {
    work::CycleReminderPriorityHandler<int> handler;
    handler.init({{1, {1, true}}, {2, {2, false}}, {3, {3, true}}, {4, {4, true}}});

    handler.reset();
    handler.set_cycle_reminder(1);
    ASSERT_TRUE(handler.need_send_reminder());
    ASSERT_EQ(handler.get_highest_priority_id(), 1);
}

TEST(cycle_reminder, set_0) {
    work::CycleReminderPriorityHandler<int> handler;
    handler.init({{1, {1, true}}, {2, {2, false}}, {3, {3, true}}, {4, {4, true}}});

    std::array<int, 3> arr{0, -1, 5};
    for (const auto& i : arr) {
        handler.reset();
        handler.set_cycle_reminder(i);
        ASSERT_FALSE(handler.need_send_reminder());
        ASSERT_EQ(handler.get_highest_priority_id(), 0);
    }
}

TEST(cycle_reminder, set_high_prio_before_within_frame) {
    work::CycleReminderPriorityHandler<int> handler;
    handler.init({{1, {1, true}}, {2, {2, false}}, {3, {3, true}}, {4, {4, true}}});

    handler.reset();
    handler.set_cycle_reminder(1);
    handler.set_cycle_reminder(2);
    ASSERT_TRUE(handler.need_send_reminder());
    ASSERT_EQ(handler.get_highest_priority_id(), 1);
}

TEST(cycle_reminder, set_high_prio_after_within_frame) {
    work::CycleReminderPriorityHandler<int> handler;
    handler.init({{1, {1, true}}, {2, {2, false}}, {3, {3, true}}, {4, {4, true}}});

    handler.reset();
    handler.set_cycle_reminder(1);
    handler.set_cycle_reminder(2);
    ASSERT_TRUE(handler.need_send_reminder());
    ASSERT_EQ(handler.get_highest_priority_id(), 1);
}

TEST(cycle_reminder, set_more_within_frame1) {
    work::CycleReminderPriorityHandler<int> handler;
    handler.init({{1, {1, true}}, {2, {2, false}}, {3, {3, true}}, {4, {4, true}}});

    handler.reset();
    handler.set_cycle_reminder(4);
    handler.set_cycle_reminder(2);
    handler.set_cycle_reminder(3);
    ASSERT_TRUE(handler.need_send_reminder());
    ASSERT_EQ(handler.get_highest_priority_id(), 2);
}

TEST(cycle_reminder, set_more_within_frame2) {
    work::CycleReminderPriorityHandler<int> handler;
    handler.init({{1, {1, true}}, {2, {2, false}}, {3, {3, true}}, {4, {4, true}}});

    handler.reset();
    handler.set_cycle_reminder(4);
    handler.set_cycle_reminder(2);
    handler.set_cycle_reminder(3);
    handler.set_cycle_reminder(1);
    ASSERT_TRUE(handler.need_send_reminder());
    ASSERT_EQ(handler.get_highest_priority_id(), 1);
}

TEST(cycle_reminder, reset) {
    work::CycleReminderPriorityHandler<int> handler;
    handler.init({{1, {1, true}}, {2, {2, false}}, {3, {3, true}}, {4, {4, true}}});

    handler.reset();
    handler.set_cycle_reminder(4);
    handler.set_cycle_reminder(2);
    handler.set_cycle_reminder(3);
    handler.set_cycle_reminder(1);
    ASSERT_TRUE(handler.need_send_reminder());

    handler.reset();
    ASSERT_FALSE(handler.need_send_reminder());
    ASSERT_EQ(handler.get_highest_priority_id(), 0);
}
