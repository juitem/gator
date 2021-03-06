/* Copyright (C) 2019-2020 by Arm Limited. All rights reserved. */

#pragma once

#include "../lib/Optional.h"
#include "CaptureMode.h"
#include "ICounterDirectoryConsumer.h"

#include <memory>
#include <string>
#include <tuple>
#include <vector>

namespace armnn {

    struct EventId {
        std::string category;
        lib::Optional<std::string> device;
        lib::Optional<std::string> counterSet;
        std::string name;
    };

    static inline bool operator==(const EventId & lhs, const EventId & rhs)
    {
        return std::tie(lhs.category, lhs.device, lhs.counterSet, lhs.name) ==
               std::tie(rhs.category, rhs.device, rhs.counterSet, rhs.name);
    }

    static inline bool operator<(const EventId & lhs, const EventId & rhs)
    {
        if (lhs.category < rhs.category) {
            return true;
        }
        if (lhs.category > rhs.category) {
            return false;
        }

        if (lhs.device < rhs.device) {
            return true;
        }
        if (lhs.device > rhs.device) {
            return false;
        }

        if (lhs.counterSet < rhs.counterSet) {
            return true;
        }
        if (lhs.counterSet > rhs.counterSet) {
            return false;
        }

        return lhs.name < rhs.name;
    }

    struct EventProperties {
        std::uint16_t counterSetCount;
        ICounterDirectoryConsumer::Class clazz;
        ICounterDirectoryConsumer::Interpolation interpolation;
        double multiplier;
        std::string description;
        std::string units;
    };

    static inline bool operator==(const EventProperties & lhs, const EventProperties & rhs)
    {
        return std::tie(lhs.counterSetCount,
                        lhs.clazz,
                        lhs.interpolation,
                        lhs.multiplier,
                        lhs.description,
                        lhs.units) ==
               std::tie(rhs.counterSetCount, rhs.clazz, rhs.interpolation, rhs.multiplier, rhs.description, rhs.units);
    }

    using EventKeyMap = std::map<armnn::EventId, int>;

    /**
     * Interface for class that listens for state changes on the session, provides access to global state
     * All methods in this interface should be multithread safe
     */
    class IGlobalState {
    public:
        virtual ~IGlobalState() = default;

        /** @return A map from global event id to APC counter key */
        virtual EventKeyMap getRequestedCounters() const = 0;
        /** @return The requested capture mode */
        virtual CaptureMode getCaptureMode() const = 0;
        /** @return The requested sample period */
        virtual std::uint32_t getSamplePeriod() const = 0;

        /**
         * Notify the global state of a set of events available from an armnn Session
         */
        virtual void addEvents(std::vector<std::tuple<EventId, EventProperties>>) = 0;
    };

}
