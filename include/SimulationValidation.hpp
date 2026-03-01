#pragma once

#include <string>
#include <vector>
#include <Eigen/Core>

#include "Particle.hpp"

struct EnergyState {
    double kinetic {0.0};
    double potential {0.0};
    double total {0.0};
};

struct ValidationState {
    EnergyState energy;
    Eigen::Vector2d momentum {0.0, 0.0};
};

struct ValidationResult {
    ValidationState current;
    double energy_drift_percent {0.0};
    bool energy_warning {false};
    std::string metrics_line;
    std::string warning_line;
};

ValidationState computeValidationState(const std::vector<Particle>& particles, float gravityConstant);

ValidationResult evaluateValidation(const ValidationState& baseline,
                                    const ValidationState& current,
                                    double energyThresholdPercent);
