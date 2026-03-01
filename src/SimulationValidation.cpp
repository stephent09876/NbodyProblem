#include "SimulationValidation.hpp"

#include <cmath>
#include <iomanip>
#include <limits>
#include <sstream>

ValidationState computeValidationState(const std::vector<Particle>& particles, float gravityConstant) {
    ValidationState state;

    for (const Particle& p : particles) {
        state.energy.kinetic += 0.5 * static_cast<double>(p.mass) *
                                static_cast<double>(p.velocity.squaredNorm());
        state.momentum += static_cast<double>(p.mass) * p.velocity.cast<double>();
    }

    for (std::size_t i = 0; i < particles.size(); ++i) {
        for (std::size_t j = i + 1; j < particles.size(); ++j) {
            const Eigen::Vector2f q_ji = particles[j].position - particles[i].position;
            const float distance = q_ji.norm();
            const float minDistance = particles[i].radius + particles[j].radius;

            if (distance < minDistance) {
                continue;
            }

            // U = -sum_{i<j} G * m_i * m_j / ||q_j - q_i||
            state.energy.potential -= static_cast<double>(gravityConstant) *
                                      static_cast<double>(particles[i].mass) *
                                      static_cast<double>(particles[j].mass) /
                                      static_cast<double>(distance);
        }
    }

    state.energy.total = state.energy.kinetic + state.energy.potential;
    return state;
}

ValidationResult evaluateValidation(const ValidationState& baseline,
                                    const ValidationState& current,
                                    double energyThresholdPercent) {
    ValidationResult result;
    result.current = current;

    if (std::abs(baseline.energy.total) > std::numeric_limits<double>::epsilon()) {
        result.energy_drift_percent = std::abs(
            (current.energy.total - baseline.energy.total) / baseline.energy.total
        ) * 100.0;
    } else if (std::abs(current.energy.total) > std::numeric_limits<double>::epsilon()) {
        result.energy_drift_percent = std::numeric_limits<double>::infinity();
    }

    result.energy_warning = result.energy_drift_percent > energyThresholdPercent;

    std::ostringstream metrics;
    metrics << std::fixed << std::setprecision(2)
            << "K: " << current.energy.kinetic
            << " U: " << current.energy.potential
            << " E: " << current.energy.total
            << " dE%: " << result.energy_drift_percent;
    result.metrics_line = metrics.str();

    if (result.energy_warning) {
        std::ostringstream warning;
        warning << std::fixed << std::setprecision(1)
                << "WARNING energy drift > threshold (E:"
                << energyThresholdPercent
                << "%)";
        result.warning_line = warning.str();
    }

    return result;
}
