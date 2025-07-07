#include "fluid_properties.h"
#include <iostream>
#include <cmath>

// FluidProperties constructors
FluidProperties::FluidProperties() : inlet_temp(0), outlet_temp(0), mass_flow(0), 
                   specific_heat(0), density(0), thermal_cond(0), 
                   viscosity(0), prandtl(0) {}

FluidProperties::FluidProperties(double inlet, double outlet, double flow, double cp,
               double rho, double k, double mu, double pr) 
    : inlet_temp(inlet), outlet_temp(outlet), mass_flow(flow),
      specific_heat(cp), density(rho), thermal_cond(k),
      viscosity(mu), prandtl(pr) {}

// GeometryProperties constructors
GeometryProperties::GeometryProperties() : length(0), shell_diameter(0), tube_diameter(0),
                      tube_thickness(0), num_tubes(0), wall_thermal_cond(0) {}

GeometryProperties::GeometryProperties(double L, double D_shell, double d_tube, double t_wall,
                  int N_tubes, double k_wall)
    : length(L), shell_diameter(D_shell), tube_diameter(d_tube),
      tube_thickness(t_wall), num_tubes(N_tubes), wall_thermal_cond(k_wall) {}

namespace CommonFluids {
    FluidProperties getWaterProperties(double temperature) {
        FluidProperties water;
        
        // Approximate properties for water (temperature in K)
        double T_C = temperature - 273.15; // Convert to Celsius
        
        water.specific_heat = 4180.0; // J/kg·K (approximately constant)
        water.density = 1000.0 - 0.2 * T_C; // kg/m³ (rough approximation)
        water.thermal_cond = 0.6 + 0.002 * T_C; // W/m·K
        water.viscosity = 0.001 * (1.0 - 0.02 * T_C/20.0); // Pa·s (rough)
        water.prandtl = (water.specific_heat * water.viscosity) / water.thermal_cond;
        
        return water;
    }
    
    FluidProperties getAirProperties(double temperature) {
        FluidProperties air;
        
        double T_C = temperature - 273.15;
        
        air.specific_heat = 1005.0; // J/kg·K
        air.density = 1.225 * (273.15 / temperature); // kg/m³ (ideal gas)
        air.thermal_cond = 0.024 + 0.00007 * T_C; // W/m·K
        air.viscosity = 1.81e-5 * (1.0 + 0.0035 * T_C/20.0); // Pa·s
        air.prandtl = (air.specific_heat * air.viscosity) / air.thermal_cond;
        
        return air;
    }
    
    FluidProperties getOilProperties(double temperature) {
        FluidProperties oil;
        
        double T_C = temperature - 273.15;
        
        oil.specific_heat = 2100.0; // J/kg·K
        oil.density = 850.0; // kg/m³
        oil.thermal_cond = 0.14; // W/m·K
        oil.viscosity = 0.01 * std::exp(-0.05 * T_C); // Pa·s (temperature dependent)
        oil.prandtl = (oil.specific_heat * oil.viscosity) / oil.thermal_cond;
        
        return oil;
    }
    
    void printFluidInfo(const FluidProperties& fluid, const std::string& name) {
        std::cout << "\n" << name << " Properties:\n";
        std::cout << "Density: " << fluid.density << " kg/m³\n";
        std::cout << "Specific heat: " << fluid.specific_heat << " J/kg·K\n";
        std::cout << "Thermal conductivity: " << fluid.thermal_cond << " W/m·K\n";
        std::cout << "Viscosity: " << fluid.viscosity << " Pa·s\n";
        std::cout << "Prandtl number: " << fluid.prandtl << "\n";
    }
}
