// Copyright 2024. All rights reserved.
//
// Author: icecream
//
// 周期型reminder优先级控制器
// 主要功能：在同一个时刻，可能会同时触发多种周期型reminder，该控制器能按照jidu定义的优先级进行发送
// ReminderID：具体的reminder发送内容，如apa::ReminderMsgID
// std::map<ReminderID, int> int: 优先级index，越小越优先

#include <iostream>
#include <map>

namespace work {

template <class ReminderID>
class CycleReminderPriorityHandler {
 public:
    struct CycleReminderProp {
        int8_t priority = kLowestIndex;
        bool source = false;
    };

    bool init(std::map<ReminderID, CycleReminderProp>&& v) {
        for (const auto& item : v) {
            if (item.second.priority > kLowestIndex || item.second.priority < 0) {
                std::cout << "ReminderId: " << item.first
                    << " priority: " << item.second.priority << " is invalid\n";
                return false;
            }
        }
        _reminders = std::move(v);
        return true;
    }

    // 每帧开始时调用，重置reminder
    void reset() {
        _highest_priority_id = static_cast<ReminderID>(0);
        _highest_priority_index = kLowestIndex;
    }

    // 设置某状态触发了的周期型id
    void set_cycle_reminder(ReminderID id) {
        // 优先级index越小越优先
        auto itr = _reminders.find(id);
        if (itr == _reminders.end() || itr->second.priority > _highest_priority_index) {
            return;
        }

        _highest_priority_index = itr->second.priority;
        _highest_priority_id = id;
    }

    // 当前帧是否要发送reminder
    bool need_send_reminder() const {
        return _highest_priority_id > 0;
    }

    ReminderID get_highest_priority_id() const {
        return _highest_priority_id;
    }

 private:
    static constexpr int8_t kLowestIndex = 100;

    std::map<ReminderID, CycleReminderProp> _reminders;
    ReminderID _highest_priority_id = static_cast<ReminderID>(0);
    int8_t _highest_priority_index = kLowestIndex;
};

}  // namespace work
