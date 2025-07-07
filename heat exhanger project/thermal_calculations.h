#ifndef THERMAL_CALCULATIONS_H
#define THERMAL_CALCULATIONS_H

/**
 * @file thermal_calculations.h
 * @brief Thermal calculations for heat exchanger analysis
 */

namespace ThermalCalculations {
    
    /**
     * Calculate convective heat transfer coefficient
     * @param nusselt Nusselt number
     * @param thermal_conductivity Fluid thermal conductivity (W/m·K)
     * @param diameter Characteristic diameter (m)
     * @return Heat transfer coefficient (W/m²·K)
     */
    double convectiveHTC(double nusselt, double thermal_conductivity, double diameter);
    
    /**
     * Calculate overall heat transfer coefficient for cylindrical geometry
     * @param h_inner Inner heat transfer coefficient (W/m²·K)
     * @param h_outer Outer heat transfer coefficient (W/m²·K)
     * @param inner_radius Inner radius (m)
     * @param outer_radius Outer radius (m)
     * @param wall_thermal_conductivity Wall thermal conductivity (W/m·K)
     * @return Overall heat transfer coefficient based on inner area (W/m²·K)
     */
    double overallHTC(double h_inner, double h_outer, double inner_radius, 
                     double outer_radius, double wall_thermal_conductivity);
    
    /**
     * Calculate Log Mean Temperature Difference (LMTD) for counter-current flow
     * @param hot_inlet Hot fluid inlet temperature (K)
     * @param hot_outlet Hot fluid outlet temperature (K)
     * @param cold_inlet Cold fluid inlet temperature (K)
     * @param cold_outlet Cold fluid outlet temperature (K)
     * @return LMTD (K)
     */
    double LMTD_counterCurrent(double hot_inlet, double hot_outlet, 
                              double cold_inlet, double cold_outlet);
    
    /**
     * Calculate LMTD for parallel flow
     * @param hot_inlet Hot fluid inlet temperature (K)
     * @param hot_outlet Hot fluid outlet temperature (K)
     * @param cold_inlet Cold fluid inlet temperature (K)
     * @param cold_outlet Cold fluid outlet temperature (K)
     * @return LMTD (K)
     */
    double LMTD_parallelFlow(double hot_inlet, double hot_outlet, 
                            double cold_inlet, double cold_outlet);
    
    /**
     * Calculate heat capacity rate
     * @param mass_flow Mass flow rate (kg/s)
     * @param specific_heat Specific heat capacity (J/kg·K)
     * @return Heat capacity rate (W/K)
     */
    double heatCapacityRate(double mass_flow, double specific_heat);
    
    /**
     * Calculate heat exchanger effectiveness using NTU method
     * @param NTU Number of Transfer Units
     * @param C_ratio Ratio of minimum to maximum heat capacity rates
     * @param flow_arrangement 0 = counter-current, 1 = parallel, 2 = cross-flow
     * @return Effectiveness (dimensionless)
     */
    double effectiveness_NTU(double NTU, double C_ratio, int flow_arrangement = 0);
    
    /**
     * Calculate Number of Transfer Units (NTU)
     * @param UA Overall heat transfer coefficient times area (W/K)
     * @param C_min Minimum heat capacity rate (W/K)
     * @return NTU (dimensionless)
     */
    double calculateNTU(double UA, double C_min);
    
    /**
     * Calculate actual heat transfer rate
     * @param mass_flow Mass flow rate (kg/s)
     * @param specific_heat Specific heat capacity (J/kg·K)
     * @param inlet_temp Inlet temperature (K)
     * @param outlet_temp Outlet temperature (K)
     * @return Heat transfer rate (W)
     */
    double actualHeatTransfer(double mass_flow, double specific_heat, 
                             double inlet_temp, double outlet_temp);
    
    /**
     * Calculate maximum possible heat transfer
     * @param C_min Minimum heat capacity rate (W/K)
     * @param hot_inlet Hot fluid inlet temperature (K)
     * @param cold_inlet Cold fluid inlet temperature (K)
     * @return Maximum heat transfer rate (W)
     */
    double maximumHeatTransfer(double C_min, double hot_inlet, double cold_inlet);
    
    /**
     * Calculate effectiveness from actual and maximum heat transfer
     * @param Q_actual Actual heat transfer rate (W)
     * @param Q_max Maximum possible heat transfer rate (W)
     * @return Effectiveness (dimensionless)
     */
    double calculateEffectiveness(double Q_actual, double Q_max);
    
    /**
     * Calculate fouling factor from clean and dirty overall HTCs
     * @param U_clean Clean overall HTC (W/m²·K)
     * @param U_dirty Dirty overall HTC (W/m²·K)
     * @return Fouling factor (m²·K/W)
     */
    double foulingFactor(double U_clean, double U_dirty);

} // namespace ThermalCalculations

#endif // THERMAL_CALCULATIONS_H
