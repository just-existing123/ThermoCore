#include <iostream>
#include "fluid_properties.h"
#include "heat_exchanger_geometry.h"
#include "dimensionless_numbers.h"
#include "heat_transfer_correlations.h"
#include "thermal_calculations.h"

/**
 * @file demo_modular_usage.cpp
 * @brief Demonstration of how to use the modular heat exchanger library
 */

int main() {
    std::cout << "=== MODULAR HEAT EXCHANGER LIBRARY DEMONSTRATION ===\n\n";
    
    // Example 1: Calculate Reynolds number for water flow in a tube
    std::cout << "Example 1: Reynolds Number Calculation\n";
    double velocity = 2.0;  // m/s
    double diameter = 0.02; // m
    double density = 998;   // kg/m³
    double viscosity = 0.001; // Pa·s
    
    double reynolds = DimensionlessNumbers::calculateReynolds(velocity, diameter, density, viscosity);
    std::cout << "Reynolds number: " << reynolds << std::endl;
    
    // Example 2: Get Nusselt number using Dittus-Boelter correlation
    std::cout << "\nExample 2: Nusselt Number from Dittus-Boelter\n";
    double prandtl = 7.0;
    double nusselt = HeatTransferCorrelations::dittusBoelter(reynolds, prandtl, false);
    std::cout << "Nusselt number: " << nusselt << std::endl;
    
    // Example 3: Calculate heat transfer coefficient
    std::cout << "\nExample 3: Heat Transfer Coefficient\n";
    double thermal_cond = 0.6; // W/m·K
    double htc = ThermalCalculations::convectiveHTC(nusselt, thermal_cond, diameter);
    std::cout << "Heat transfer coefficient: " << htc << " W/m²·K" << std::endl;
    
    // Example 4: Geometric calculations
    std::cout << "\nExample 4: Geometric Calculations\n";
    double tube_area = HeatExchangerGeometry::tubeArea(diameter);
    std::cout << "Single tube area: " << tube_area << " m²" << std::endl;
    
    double total_area = HeatExchangerGeometry::totalTubeArea(diameter, 2.0, 100);
    std::cout << "Total heat transfer area (100 tubes, 2m length): " << total_area << " m²" << std::endl;
    
    // Example 5: LMTD calculation
    std::cout << "\nExample 5: LMTD Calculation\n";
    double lmtd = ThermalCalculations::LMTD_counterCurrent(353, 323, 293, 313);
    std::cout << "LMTD for counter-current flow: " << lmtd << " K" << std::endl;
    
    // Example 6: Using common fluid properties
    std::cout << "\nExample 6: Common Fluid Properties\n";
    FluidProperties water = CommonFluids::getWaterProperties(323); // 50°C
    std::cout << "Water at 50°C:\n";
    std::cout << "  Density: " << water.density << " kg/m³\n";
    std::cout << "  Thermal conductivity: " << water.thermal_cond << " W/m·K\n";
    std::cout << "  Prandtl number: " << water.prandtl << std::endl;
    
    // Example 7: Effectiveness-NTU calculation
    std::cout << "\nExample 7: Effectiveness-NTU Method\n";
    double NTU = 2.0;
    double C_ratio = 0.5;
    double effectiveness = ThermalCalculations::effectiveness_NTU(NTU, C_ratio, 0); // Counter-current
    std::cout << "Effectiveness (NTU=2.0, C*=0.5): " << effectiveness * 100 << "%" << std::endl;
    
    std::cout << "\n=== DEMONSTRATION COMPLETE ===\n";
    std::cout << "This shows how each module can be used independently!\n";
    
    return 0;
}
