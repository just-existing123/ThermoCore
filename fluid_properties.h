#ifndef FLUID_PROPERTIES_H
#define FLUID_PROPERTIES_H

#include <string>

/**
 * @file fluid_properties.h
 * @brief Structure to hold fluid properties and common fluid data
 */

struct FluidProperties {
    double inlet_temp;        // Inlet temperature (K)
    double outlet_temp;       // Outlet temperature (K)
    double mass_flow;         // Mass flow rate (kg/s)
    double specific_heat;     // Specific heat capacity (J/kg·K)
    double density;           // Density (kg/m³)
    double thermal_cond;      // Thermal conductivity (W/m·K)
    double viscosity;         // Dynamic viscosity (Pa·s)
    double prandtl;          // Prandtl number
    
    // Constructors
    FluidProperties();
    FluidProperties(double inlet, double outlet, double flow, double cp,
                   double rho, double k, double mu, double pr);
};

struct GeometryProperties {
    double length;            // Heat exchanger length (m)
    double shell_diameter;    // Shell diameter (m)
    double tube_diameter;     // Tube inner diameter (m)
    double tube_thickness;    // Tube wall thickness (m)
    int num_tubes;           // Number of tubes
    double wall_thermal_cond; // Wall thermal conductivity (W/m·K)
    
    // Constructors
    GeometryProperties();
    GeometryProperties(double L, double D_shell, double d_tube, double t_wall,
                      int N_tubes, double k_wall);
};

namespace CommonFluids {
    FluidProperties getWaterProperties(double temperature);
    FluidProperties getAirProperties(double temperature);
    FluidProperties getOilProperties(double temperature);
    void printFluidInfo(const FluidProperties& fluid, const std::string& name);
}

#endif // FLUID_PROPERTIES_H
