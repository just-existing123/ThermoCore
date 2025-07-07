#include <iostream>
#include <iomanip>
#include <algorithm>
#include "fluid_properties.h"
#include "heat_exchanger_geometry.h"
#include "thermal_calculations.h"
#include "heat_transfer_correlations.h"
#include "dimensionless_numbers.h"
#include "numerical_solver.h"

class HeatExchanger {
private:
    GeometryProperties geometry;
    FluidProperties hot_fluid;
    FluidProperties cold_fluid;
    int num_segments;
    
public:
    HeatExchanger(int segments = 100) : num_segments(segments) {}
    
    void inputGeometry();
    void inputHotFluidProperties();
    void inputColdFluidProperties();
    void inputWallProperties();
    void calculateTemperatureProfile();
    void calculateEfficiency();
    void outputResults();
    void performConvergenceStudy();
    
private:
    double calculateLMTD();
};

void HeatExchanger::inputGeometry() {
    std::cout << "\n=== HEAT EXCHANGER GEOMETRY ===\n";
    std::cout << "Enter heat exchanger length (m): ";
    std::cin >> geometry.length;
    std::cout << "Enter shell diameter (m): ";
    std::cin >> geometry.shell_diameter;
    std::cout << "Enter tube inner diameter (m): ";
    std::cin >> geometry.tube_diameter;
    std::cout << "Enter tube wall thickness (m): ";
    std::cin >> geometry.tube_thickness;
    std::cout << "Enter number of tubes: ";
    std::cin >> geometry.num_tubes;
}

void HeatExchanger::inputHotFluidProperties() {
    std::cout << "\n=== HOT FLUID PROPERTIES (Shell Side) ===\n";
    std::cout << "Enter hot fluid inlet temperature (K): ";
    std::cin >> hot_fluid.inlet_temp;
    std::cout << "Enter hot fluid outlet temperature (K): ";
    std::cin >> hot_fluid.outlet_temp;
    std::cout << "Enter hot fluid mass flow rate (kg/s): ";
    std::cin >> hot_fluid.mass_flow;
    std::cout << "Enter hot fluid specific heat (J/kg·K): ";
    std::cin >> hot_fluid.specific_heat;
    std::cout << "Enter hot fluid density (kg/m³): ";
    std::cin >> hot_fluid.density;
    std::cout << "Enter hot fluid thermal conductivity (W/m·K): ";
    std::cin >> hot_fluid.thermal_cond;
    std::cout << "Enter hot fluid viscosity (Pa·s): ";
    std::cin >> hot_fluid.viscosity;
    std::cout << "Enter hot fluid Prandtl number: ";
    std::cin >> hot_fluid.prandtl;
}

void HeatExchanger::inputColdFluidProperties() {
    std::cout << "\n=== COLD FLUID PROPERTIES (Tube Side) ===\n";
    std::cout << "Enter cold fluid inlet temperature (K): ";
    std::cin >> cold_fluid.inlet_temp;
    std::cout << "Enter cold fluid outlet temperature (K): ";
    std::cin >> cold_fluid.outlet_temp;
    std::cout << "Enter cold fluid mass flow rate (kg/s): ";
    std::cin >> cold_fluid.mass_flow;
    std::cout << "Enter cold fluid specific heat (J/kg·K): ";
    std::cin >> cold_fluid.specific_heat;
    std::cout << "Enter cold fluid density (kg/m³): ";
    std::cin >> cold_fluid.density;
    std::cout << "Enter cold fluid thermal conductivity (W/m·K): ";
    std::cin >> cold_fluid.thermal_cond;
    std::cout << "Enter cold fluid viscosity (Pa·s): ";
    std::cin >> cold_fluid.viscosity;
    std::cout << "Enter cold fluid Prandtl number: ";
    std::cin >> cold_fluid.prandtl;
}

void HeatExchanger::inputWallProperties() {
    std::cout << "\n=== WALL PROPERTIES ===\n";
    std::cout << "Enter wall thermal conductivity (W/m·K): ";
    std::cin >> geometry.wall_thermal_cond;
}

void HeatExchanger::calculateTemperatureProfile() {
    std::cout << "\n=== CALCULATING TEMPERATURE PROFILE ===\n";
    
    // Create numerical solver
    NumericalSolver solver(num_segments, geometry, hot_fluid, cold_fluid);
    
    // Solve temperature distribution
    NumericalSolver::SolutionResults results = solver.solveTemperatureDistribution();
    
    // Display calculated parameters
    std::cout << "\nCalculated Parameters:\n";
    std::cout << "Cold fluid Reynolds number: " << results.cold_reynolds << std::endl;
    std::cout << "Hot fluid Reynolds number: " << results.hot_reynolds << std::endl;
    std::cout << "Cold fluid Nusselt number: " << results.cold_nusselt << std::endl;
    std::cout << "Hot fluid Nusselt number: " << results.hot_nusselt << std::endl;
    std::cout << "Cold fluid HTC: " << results.cold_htc << " W/m²·K" << std::endl;
    std::cout << "Hot fluid HTC: " << results.hot_htc << " W/m²·K" << std::endl;
    std::cout << "Overall HTC: " << results.overall_htc << " W/m²·K" << std::endl;
    
    // Display temperature profile
    std::cout << "\nSegment-wise Temperature Profile:\n";
    std::cout << std::setw(12) << "Position(m)" << std::setw(18) << "Hot Fluid Temp(K)" 
              << std::setw(18) << "Hot Fluid Temp(°C)" << std::setw(18) << "Cold Fluid Temp(K)" 
              << std::setw(18) << "Cold Fluid Temp(°C)" << std::endl;
    std::cout << std::string(84, '-') << std::endl;
    
    // Display every 10th segment to avoid cluttering
    for (int i = 0; i <= num_segments; i += std::max(1, num_segments/10)) {
        int cold_index = num_segments - i;
        std::cout << std::fixed << std::setprecision(2)
                  << std::setw(12) << results.positions[i]
                  << std::setw(18) << results.hot_temperatures[i]
                  << std::setw(18) << (results.hot_temperatures[i] - 273.15)
                  << std::setw(18) << results.cold_temperatures[cold_index]
                  << std::setw(18) << (results.cold_temperatures[cold_index] - 273.15) << std::endl;
    }
    
    // Write results to file
    solver.writeResultsToFile(results);
}

void HeatExchanger::calculateEfficiency() {
    // First, get the calculated temperatures from the numerical solver
    NumericalSolver solver(num_segments, geometry, hot_fluid, cold_fluid);
    NumericalSolver::SolutionResults results = solver.solveTemperatureDistribution();
    
    // Use calculated outlet temperatures instead of input guesses
    double hot_outlet_calc = results.hot_temperatures[num_segments];
    double cold_outlet_calc = results.cold_temperatures[0];
    
    // Calculate actual heat transfer using calculated temperatures
    double Q_actual_hot = hot_fluid.mass_flow * hot_fluid.specific_heat * 
                         (hot_fluid.inlet_temp - hot_outlet_calc);
    double Q_actual_cold = cold_fluid.mass_flow * cold_fluid.specific_heat * 
                          (cold_outlet_calc - cold_fluid.inlet_temp);
    
    // Use average of hot and cold side calculations for better accuracy
    double Q_actual = (Q_actual_hot + Q_actual_cold) / 2.0;
    
    // Calculate heat capacity rates
    double C_hot = hot_fluid.mass_flow * hot_fluid.specific_heat;
    double C_cold = cold_fluid.mass_flow * cold_fluid.specific_heat;
    double C_min = std::min(C_hot, C_cold);
    
    // Calculate maximum possible heat transfer
    double Q_max = C_min * (hot_fluid.inlet_temp - cold_fluid.inlet_temp);
    
    // Calculate effectiveness (ensure it's between 0 and 1)
    double effectiveness = (Q_max > 0) ? std::min(1.0, Q_actual / Q_max) : 0.0;
    
    std::cout << "\n=== HEAT EXCHANGER PERFORMANCE ===\n";
    std::cout << "Actual heat transfer rate (hot side): " << Q_actual_hot / 1000.0 << " kW\n";
    std::cout << "Actual heat transfer rate (cold side): " << Q_actual_cold / 1000.0 << " kW\n";
    std::cout << "Average heat transfer rate: " << Q_actual / 1000.0 << " kW\n";
    std::cout << "Maximum possible heat transfer: " << Q_max / 1000.0 << " kW\n";
    std::cout << "Heat exchanger effectiveness: " << effectiveness * 100.0 << "%\n";
    
    // Calculate LMTD using calculated temperatures
    double LMTD = ThermalCalculations::LMTD_counterCurrent(
        hot_fluid.inlet_temp, hot_outlet_calc,
        cold_fluid.inlet_temp, cold_outlet_calc);
    double UA = (LMTD > 0) ? Q_actual / LMTD : 0.0;
    std::cout << "Log Mean Temperature Difference: " << LMTD << " K\n";
    std::cout << "UA value: " << UA / 1000.0 << " kW/K\n";
    double NTU = (C_min > 0) ? UA / C_min : 0.0;
    std::cout << "Number of Transfer Units (NTU): " << NTU << "\n";
    
    // Compare with NTU-effectiveness method (only if valid)
    if (NTU > 0 && C_min > 0 && C_hot > 0 && C_cold > 0) {
        double effectiveness_ntu = ThermalCalculations::effectiveness_NTU(NTU, C_min/std::max(C_hot, C_cold), 0);
        std::cout << "Effectiveness from NTU method: " << effectiveness_ntu * 100.0 << "%\n";
    }
    
    // Print calculated outlet temperatures
    std::cout << "\nCalculated Outlet Temperatures:\n";
    std::cout << "Hot fluid outlet: " << hot_outlet_calc << " K (" 
              << (hot_outlet_calc - 273.15) << " °C)\n";
    std::cout << "Cold fluid outlet: " << cold_outlet_calc << " K (" 
              << (cold_outlet_calc - 273.15) << " °C)\n";
}

double HeatExchanger::calculateLMTD() {
    return ThermalCalculations::LMTD_counterCurrent(
        hot_fluid.inlet_temp, hot_fluid.outlet_temp,
        cold_fluid.inlet_temp, cold_fluid.outlet_temp);
}

void HeatExchanger::performConvergenceStudy() {
    std::cout << "\n=== PERFORMING CONVERGENCE STUDY ===\n";
    NumericalSolver solver(num_segments, geometry, hot_fluid, cold_fluid);
    solver.convergenceStudy(10, 200, 10);
}

void HeatExchanger::outputResults() {
    calculateTemperatureProfile();
    calculateEfficiency();
    
    char choice;
    std::cout << "\nWould you like to perform a convergence study? (y/n): ";
    std::cin >> choice;
    if (choice == 'y' || choice == 'Y') {
        performConvergenceStudy();
    }
}

int main() {
    std::cout << "=== SHELL AND TUBE HEAT EXCHANGER ANALYSIS ===\n";
    std::cout << "This program calculates temperature profiles and efficiency\n";
    std::cout << "using numerical methods for heat transfer analysis.\n\n";
    
    std::cout << "Choose analysis type:\n";
    std::cout << "1. Custom input\n";
    std::cout << "2. Use sample water-to-water data\n";
    std::cout << "3. Use sample oil-to-water data\n";
    std::cout << "Enter choice (1-3): ";
    
    int choice;
    std::cin >> choice;
    
    HeatExchanger hx(50); // 50 segments for analysis
    
    try {
        switch (choice) {
            case 1:
                // Custom input
                hx.inputGeometry();
                hx.inputHotFluidProperties();
                hx.inputColdFluidProperties();
                hx.inputWallProperties();
                break;
                
            case 2: {
                // Sample water-to-water data
                std::cout << "\nUsing sample water-to-water heat exchanger data...\n";
                // This will be loaded automatically in the future
                hx.inputGeometry();
                hx.inputHotFluidProperties();
                hx.inputColdFluidProperties();
                hx.inputWallProperties();
                break;
            }
            
            case 3: {
                // Sample oil-to-water data
                std::cout << "\nUsing sample oil-to-water heat exchanger data...\n";
                hx.inputGeometry();
                hx.inputHotFluidProperties();
                hx.inputColdFluidProperties();
                hx.inputWallProperties();
                break;
            }
            
            default:
                std::cout << "Invalid choice. Using custom input...\n";
                hx.inputGeometry();
                hx.inputHotFluidProperties();
                hx.inputColdFluidProperties();
                hx.inputWallProperties();
                break;
        }
        
        hx.outputResults();
        
        std::cout << "\nAnalysis complete! Check the following files for detailed results:\n";
        std::cout << "- temperature_profile.csv (temperature distribution)\n";
        std::cout << "- convergence_study.csv (if convergence study was performed)\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "\nPress Enter to exit...";
    std::cin.ignore();
    std::cin.get();
    
    return 0;
}
