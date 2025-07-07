#include "numerical_solver.h"
#include "dimensionless_numbers.h"
#include "heat_transfer_correlations.h"
#include "thermal_calculations.h"
#include "heat_exchanger_geometry.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <algorithm>

NumericalSolver::NumericalSolver(int segments, const GeometryProperties& geom,
                               const FluidProperties& hot, const FluidProperties& cold)
    : num_segments(segments), geometry(geom), hot_fluid(hot), cold_fluid(cold) {
}

NumericalSolver::SolutionResults NumericalSolver::solveTemperatureDistribution() {
    SolutionResults results;
    
    // Initialize arrays
    results.hot_temperatures.resize(num_segments + 1);
    results.cold_temperatures.resize(num_segments + 1);
    results.positions.resize(num_segments + 1);
    
    // Calculate segment length
    double dx = geometry.length / num_segments;
    
    // Initialize boundary conditions
    results.hot_temperatures[0] = hot_fluid.inlet_temp;  // Hot inlet
    results.cold_temperatures[num_segments] = cold_fluid.inlet_temp;  // Cold inlet (counter-current)
    
    // Calculate positions
    for (int i = 0; i <= num_segments; ++i) {
        results.positions[i] = i * dx;
    }
    
    // Calculate flow areas and velocities
    double tube_flow_area = HeatExchangerGeometry::tubeArea(geometry.tube_diameter) * geometry.num_tubes;
    double shell_flow_area = HeatExchangerGeometry::shellFlowArea(
        geometry.shell_diameter, geometry.tube_diameter + 2 * geometry.tube_thickness, geometry.num_tubes);
    
    double cold_velocity = cold_fluid.mass_flow / (cold_fluid.density * tube_flow_area);
    double hot_velocity = hot_fluid.mass_flow / (hot_fluid.density * shell_flow_area);
    
    // Calculate Reynolds numbers
    results.cold_reynolds = DimensionlessNumbers::calculateReynolds(
        cold_velocity, geometry.tube_diameter, cold_fluid.density, cold_fluid.viscosity);
    results.hot_reynolds = DimensionlessNumbers::calculateReynolds(
        hot_velocity, geometry.shell_diameter, hot_fluid.density, hot_fluid.viscosity);
    
    // Calculate Nusselt numbers using appropriate correlations
    results.cold_nusselt = HeatTransferCorrelations::getTubeSideNusselt(
        results.cold_reynolds, cold_fluid.prandtl, true); // Heating
    results.hot_nusselt = HeatTransferCorrelations::getShellSideNusselt(
        results.hot_reynolds, hot_fluid.prandtl);
    
    // Calculate heat transfer coefficients
    results.cold_htc = results.cold_nusselt * cold_fluid.thermal_cond / geometry.tube_diameter;
    results.hot_htc = results.hot_nusselt * hot_fluid.thermal_cond / geometry.shell_diameter;
    
    // Calculate heat transfer surface areas
    double inner_surface_area = HeatExchangerGeometry::totalTubeArea(
        geometry.tube_diameter, geometry.length, geometry.num_tubes);
    // Note: outer_surface_area calculation removed as it's not used
    
    // Calculate overall heat transfer coefficient
    double inner_radius = geometry.tube_diameter / 2.0;
    double outer_radius = inner_radius + geometry.tube_thickness;
    results.overall_htc = ThermalCalculations::overallHTC(
        results.cold_htc, results.hot_htc, inner_radius,
        outer_radius, geometry.wall_thermal_cond);
    
    // Calculate heat capacity rates
    double C_hot = hot_fluid.mass_flow * hot_fluid.specific_heat;
    double C_cold = cold_fluid.mass_flow * cold_fluid.specific_heat;
    
    // Numerical solution using finite difference method
    const int max_iterations = 1000;
    const double tolerance = 1e-6;
    
    // Initial guess for temperature distribution (linear interpolation)
    for (int i = 0; i <= num_segments; ++i) {
        double ratio = static_cast<double>(i) / num_segments;
        results.hot_temperatures[i] = hot_fluid.inlet_temp - 
            ratio * (hot_fluid.inlet_temp - hot_fluid.outlet_temp);
        // For cold fluid in counter-current: inlet at end, outlet at start
        results.cold_temperatures[i] = cold_fluid.outlet_temp + 
            ratio * (cold_fluid.inlet_temp - cold_fluid.outlet_temp);
    }
    
    // Calculate UA and NTU for better convergence
    double segment_area = inner_surface_area / num_segments;
    double UA_segment = results.overall_htc * segment_area;
    double C_min = std::min(C_hot, C_cold);
    double C_max = std::max(C_hot, C_cold);
    double C_ratio = C_min / C_max;
    
    // Iterative solution using proper heat balance
    for (int iter = 0; iter < max_iterations; ++iter) {
        std::vector<double> hot_old = results.hot_temperatures;
        std::vector<double> cold_old = results.cold_temperatures;
        
        // Update hot fluid temperatures (flowing left to right, i=0 to num_segments)
        for (int i = 1; i <= num_segments; ++i) {
            // For counter-current: hot fluid at position i corresponds to cold fluid at position (num_segments-i)
            int cold_index = num_segments - i;
            
            double hot_temp_in = results.hot_temperatures[i-1];
            double cold_temp_local = (results.cold_temperatures[cold_index] + 
                                    results.cold_temperatures[cold_index+1]) / 2.0;
            
            double dT = hot_temp_in - cold_temp_local;
            double heat_transfer = UA_segment * dT;
            
            results.hot_temperatures[i] = hot_temp_in - heat_transfer / C_hot;
        }
        
        // Update cold fluid temperatures (flowing right to left, i=num_segments to 0)
        for (int i = num_segments - 1; i >= 0; --i) {
            // For counter-current: cold fluid at position i corresponds to hot fluid at position (num_segments-i)
            int hot_index = num_segments - i;
            
            double cold_temp_in = results.cold_temperatures[i+1];
            double hot_temp_local = (results.hot_temperatures[hot_index-1] + 
                                   results.hot_temperatures[hot_index]) / 2.0;
            
            double dT = hot_temp_local - cold_temp_in;
            double heat_transfer = UA_segment * dT;
            
            results.cold_temperatures[i] = cold_temp_in + heat_transfer / C_cold;
        }
        
        // Enforce boundary conditions
        results.hot_temperatures[0] = hot_fluid.inlet_temp;  // Hot inlet
        results.cold_temperatures[num_segments] = cold_fluid.inlet_temp;  // Cold inlet
        
        // Apply relaxation to improve stability
        const double relaxation_factor = 0.5;
        for (int i = 0; i <= num_segments; ++i) {
            results.hot_temperatures[i] = relaxation_factor * results.hot_temperatures[i] + 
                                        (1.0 - relaxation_factor) * hot_old[i];
            results.cold_temperatures[i] = relaxation_factor * results.cold_temperatures[i] + 
                                         (1.0 - relaxation_factor) * cold_old[i];
        }
        
        // Re-enforce boundary conditions after relaxation
        results.hot_temperatures[0] = hot_fluid.inlet_temp;
        results.cold_temperatures[num_segments] = cold_fluid.inlet_temp;
        
        // Check for convergence
        double max_change = 0.0;
        for (int i = 0; i <= num_segments; ++i) {
            max_change = std::max(max_change, std::abs(results.hot_temperatures[i] - hot_old[i]));
            max_change = std::max(max_change, std::abs(results.cold_temperatures[i] - cold_old[i]));
        }
        
        if (max_change < tolerance) {
            std::cout << "Converged after " << iter + 1 << " iterations\n";
            break;
        }
        
        if (iter == max_iterations - 1) {
            std::cout << "Warning: Maximum iterations reached. Solution may not be fully converged.\n";
        }
    }
    
    return results;
}

void NumericalSolver::convergenceStudy(int min_segments, int max_segments, int step) {
    std::cout << "Performing convergence study...\n";
    std::cout << std::setw(12) << "Segments" << std::setw(15) << "Hot Outlet (K)" 
              << std::setw(15) << "Cold Outlet (K)" << std::setw(15) << "Overall HTC" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    
    std::ofstream file("convergence_study.csv");
    file << "Segments,Hot_Outlet_K,Cold_Outlet_K,Overall_HTC\n";
    
    for (int segments = min_segments; segments <= max_segments; segments += step) {
        // Create temporary solver with current segment count
        NumericalSolver temp_solver(segments, geometry, hot_fluid, cold_fluid);
        SolutionResults temp_results = temp_solver.solveTemperatureDistribution();
        
        double hot_outlet = temp_results.hot_temperatures[segments];
        double cold_outlet = temp_results.cold_temperatures[0];
        
        std::cout << std::fixed << std::setprecision(2)
                  << std::setw(12) << segments
                  << std::setw(15) << hot_outlet
                  << std::setw(15) << cold_outlet
                  << std::setw(15) << temp_results.overall_htc << std::endl;
        
        file << segments << "," << hot_outlet << "," << cold_outlet << "," 
             << temp_results.overall_htc << "\n";
    }
    
    file.close();
    std::cout << "Convergence study results written to convergence_study.csv\n";
}

void NumericalSolver::writeResultsToFile(const SolutionResults& results, const std::string& filename) {
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing\n";
        return;
    }
    
    // Write header
    file << "Position_m,Hot_Temp_K,Hot_Temp_C,Cold_Temp_K,Cold_Temp_C\n";
    
    // Write data
    for (int i = 0; i <= num_segments; ++i) {
        int cold_index = num_segments - i; // Counter-current flow
        file << std::fixed << std::setprecision(4)
             << results.positions[i] << ","
             << results.hot_temperatures[i] << ","
             << (results.hot_temperatures[i] - 273.15) << ","
             << results.cold_temperatures[cold_index] << ","
             << (results.cold_temperatures[cold_index] - 273.15) << "\n";
    }
    
    file.close();
    std::cout << "Temperature profile written to " << filename << "\n";
    
    // Write summary file
    std::ofstream summary("heat_transfer_summary.txt");
    if (summary.is_open()) {
        summary << "=== HEAT EXCHANGER ANALYSIS SUMMARY ===\n\n";
        summary << "Geometry:\n";
        summary << "  Length: " << geometry.length << " m\n";
        summary << "  Shell diameter: " << geometry.shell_diameter << " m\n";
        summary << "  Tube diameter: " << geometry.tube_diameter << " m\n";
        summary << "  Number of tubes: " << geometry.num_tubes << "\n";
        summary << "  Wall thermal conductivity: " << geometry.wall_thermal_cond << " W/m·K\n\n";
        
        summary << "Calculated Parameters:\n";
        summary << "  Hot fluid Reynolds: " << results.hot_reynolds << "\n";
        summary << "  Cold fluid Reynolds: " << results.cold_reynolds << "\n";
        summary << "  Hot fluid Nusselt: " << results.hot_nusselt << "\n";
        summary << "  Cold fluid Nusselt: " << results.cold_nusselt << "\n";
        summary << "  Hot fluid HTC: " << results.hot_htc << " W/m²·K\n";
        summary << "  Cold fluid HTC: " << results.cold_htc << " W/m²·K\n";
        summary << "  Overall HTC: " << results.overall_htc << " W/m²·K\n\n";
        
        summary << "Temperature Results:\n";
        summary << "  Hot inlet: " << hot_fluid.inlet_temp << " K (" 
                << (hot_fluid.inlet_temp - 273.15) << " °C)\n";
        summary << "  Hot outlet: " << results.hot_temperatures[num_segments] << " K ("
                << (results.hot_temperatures[num_segments] - 273.15) << " °C)\n";
        summary << "  Cold inlet: " << cold_fluid.inlet_temp << " K ("
                << (cold_fluid.inlet_temp - 273.15) << " °C)\n";
        summary << "  Cold outlet: " << results.cold_temperatures[0] << " K ("
                << (results.cold_temperatures[0] - 273.15) << " °C)\n";
        
        summary.close();
        std::cout << "Analysis summary written to heat_transfer_summary.txt\n";
    }
}
