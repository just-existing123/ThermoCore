#ifndef DIMENSIONLESS_NUMBERS_H
#define DIMENSIONLESS_NUMBERS_H

/**
 * @file dimensionless_numbers.h
 * @brief Calculations for dimensionless numbers used in heat transfer analysis
 */

namespace DimensionlessNumbers {
    
    /**
     * Calculate Reynolds number
     * @param velocity Fluid velocity (m/s)
     * @param diameter Characteristic diameter (m)
     * @param density Fluid density (kg/m³)
     * @param viscosity Dynamic viscosity (Pa·s)
     * @return Reynolds number (dimensionless)
     */
    double calculateReynolds(double velocity, double diameter, double density, double viscosity);
    
    /**
     * Calculate Prandtl number
     * @param cp Specific heat capacity (J/kg·K)
     * @param viscosity Dynamic viscosity (Pa·s)
     * @param thermal_conductivity Thermal conductivity (W/m·K)
     * @return Prandtl number (dimensionless)
     */
    double calculatePrandtl(double cp, double viscosity, double thermal_conductivity);
    
    /**
     * Calculate Peclet number
     * @param reynolds Reynolds number
     * @param prandtl Prandtl number
     * @return Peclet number (dimensionless)
     */
    double calculatePeclet(double reynolds, double prandtl);
    
    /**
     * Calculate Graetz number for developing flow
     * @param reynolds Reynolds number
     * @param prandtl Prandtl number
     * @param diameter Tube diameter (m)
     * @param length Tube length (m)
     * @return Graetz number (dimensionless)
     */
    double calculateGraetz(double reynolds, double prandtl, double diameter, double length);

} // namespace DimensionlessNumbers

#endif // DIMENSIONLESS_NUMBERS_H
