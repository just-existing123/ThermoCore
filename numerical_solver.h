#ifndef NUMERICAL_SOLVER_H
#define NUMERICAL_SOLVER_H

#include <vector>
#include <string>
#include "fluid_properties.h"

/**
 * @file numerical_solver.h
 * @brief Numerical methods for solving heat exchanger temperature distribution
 */

class NumericalSolver {
private:
    int num_segments;
    GeometryProperties geometry;
    FluidProperties hot_fluid;
    FluidProperties cold_fluid;
    
public:
    struct SolutionResults {
        std::vector<double> hot_temperatures;
        std::vector<double> cold_temperatures;
        std::vector<double> positions;
        double overall_htc;
        double hot_reynolds;
        double cold_reynolds;
        double hot_nusselt;
        double cold_nusselt;
        double hot_htc;
        double cold_htc;
    };
    
    NumericalSolver(int segments, const GeometryProperties& geom,
                   const FluidProperties& hot, const FluidProperties& cold);
    
    SolutionResults solveTemperatureDistribution();
    void convergenceStudy(int min_segments, int max_segments, int step);
    void writeResultsToFile(const SolutionResults& results, const std::string& filename = "temperature_profile.csv");
};

#endif // NUMERICAL_SOLVER_H
