#include "detection/EventDetector.h"

#include <math.h>

#include "config/AppConfig.h"

namespace arfc {

EventDetector::EventDetector()
    : last_rel_altitude_m_(0.0f),
      launch_counter_(0),
      descending_counter_(0),
      landing_counter_(0),
      initialized_(false),
      launch_detected_(false),
      apogee_detected_(false),
      descent_detected_(false),
      landing_detected_(false) {}

void EventDetector::reset() {
    last_rel_altitude_m_ = 0.0f;
    launch_counter_ = 0;
    descending_counter_ = 0;
    landing_counter_ = 0;
    initialized_ = false;
    launch_detected_ = false;
    apogee_detected_ = false;
    descent_detected_ = false;
    landing_detected_ = false;
}

EventFlags EventDetector::update(const FlightSample& sample) {
    EventFlags events;

    if (!initialized_) {
        last_rel_altitude_m_ = sample.rel_altitude_m;
        initialized_ = true;
        return events;
    }

    const float altitude_delta_m = sample.rel_altitude_m - last_rel_altitude_m_;

    if (!launch_detected_) {
        if (sample.accel_mag_g >= AppConfig::kLaunchAccelThresholdG) {
            if (launch_counter_ < 255U) {
                ++launch_counter_;
            }
        } else {
            launch_counter_ = 0;
        }

        if (launch_counter_ >= AppConfig::kLaunchDetectSamples) {
            launch_detected_ = true;
            events.launch_flag = true;
            descending_counter_ = 0;
        }
    }

    if (launch_detected_ && !apogee_detected_) {
        if (sample.rel_altitude_m >= AppConfig::kApogeeMinRelAltitudeM &&
            altitude_delta_m <= -AppConfig::kApogeeDeclineDeltaM) {
            if (descending_counter_ < 255U) {
                ++descending_counter_;
            }
        } else {
            descending_counter_ = 0;
        }

        if (descending_counter_ >= AppConfig::kApogeeDeclineSamples) {
            apogee_detected_ = true;
            events.apogee_flag = true;
            descending_counter_ = 0;
        }
    }

    if (apogee_detected_ && !events.apogee_flag && !descent_detected_ &&
        altitude_delta_m <= -AppConfig::kDescentDetectDeltaM) {
        descent_detected_ = true;
        events.descent_flag = true;
    }

    if (descent_detected_ && !landing_detected_) {
        const bool stable_altitude =
            fabsf(altitude_delta_m) <= AppConfig::kLandingAltitudeDeltaM;
        const bool near_one_g =
            fabsf(sample.accel_mag_g - 1.0f) <= AppConfig::kLandingAccelToleranceG;

        if (stable_altitude && near_one_g) {
            if (landing_counter_ < 255U) {
                ++landing_counter_;
            }
        } else {
            landing_counter_ = 0;
        }

        if (landing_counter_ >= AppConfig::kLandingDetectSamples) {
            landing_detected_ = true;
            events.landing_flag = true;
        }
    }

    last_rel_altitude_m_ = sample.rel_altitude_m;
    return events;
}

}  // namespace arfc
