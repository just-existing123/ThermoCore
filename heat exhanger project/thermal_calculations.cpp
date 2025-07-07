#include "thermal_calculations.h"
#include <cmath>
#include <algorithm>
#include <iostream>

namespace ThermalCalculations {
    
    double convectiveHTC(double nusselt, double thermal_conductivity, double diameter) {
        return (nusselt * thermal_conductivity) / diameter;
    }
    
    double overallHTC(double h_inner, double h_outer, double inner_radius, 
                     double outer_radius, double wall_thermal_conductivity) {
        double U_inv = (1.0 / h_inner) + 
                       (inner_radius * std::log(outer_radius / inner_radius) / wall_thermal_conductivity) +
                       (inner_radius / (h_outer * outer_radius));
        
        return 1.0 / U_inv;
    }
    
    double LMTD_counterCurrent(double hot_inlet, double hot_outlet, 
                              double cold_inlet, double cold_outlet) {
        double dT1 = hot_inlet - cold_outlet;   // Temperature difference at one end
        double dT2 = hot_outlet - cold_inlet;   // Temperature difference at other end
        
        // Check for invalid temperature differences
        if (dT1 <= 0 || dT2 <= 0) {
            std::cerr << "Warning: Invalid temperature differences in LMTD calculation\n";
            std::cerr << "dT1 = " << dT1 << ", dT2 = " << dT2 << std::endl;
            return std::abs((dT1 + dT2) / 2.0); // Return arithmetic mean as fallback
        }
        
        // Check for nearly equal temperature differences
        if (std::abs(dT1 - dT2) < 1e-6) {
            return dT1; // Avoid division by zero
        }
        
        return (dT1 - dT2) / std::log(dT1 / dT2);
    }
    
    double LMTD_parallelFlow(double hot_inlet, double hot_outlet, 
                            double cold_inlet, double cold_outlet) {
        double dT1 = hot_inlet - cold_inlet;    // Temperature difference at inlet
        double dT2 = hot_outlet - cold_outlet;  // Temperature difference at outlet
        
        if (std::abs(dT1 - dT2) < 1e-6) {
            return dT1; // Avoid division by zero
        }
        
        return (dT1 - dT2) / std::log(dT1 / dT2);
    }
    
    double heatCapacityRate(double mass_flow, double specific_heat) {
        return mass_flow * specific_heat;
    }
    
    double effectiveness_NTU(double NTU, double C_ratio, int flow_arrangement) {
        if (C_ratio < 1e-6) {
            // One fluid has infinite heat capacity (phase change)
            return 1.0 - std::exp(-NTU);
        }
        
        switch (flow_arrangement) {
            case 0: // Counter-current
                if (std::abs(C_ratio - 1.0) < 1e-6) {
                    return NTU / (1.0 + NTU);
                } else {
                    double exp_term = std::exp(-NTU * (1.0 - C_ratio));
                    return (1.0 - exp_term) / (1.0 - C_ratio * exp_term);
                }
                break;
                
            case 1: // Parallel flow
                return (1.0 - std::exp(-NTU * (1.0 + C_ratio))) / (1.0 + C_ratio);
                break;
                
            case 2: // Cross-flow (both fluids unmixed - approximation)
                return 1.0 - std::exp((1.0/C_ratio) * std::pow(NTU, 0.22) * 
                                     (std::exp(-C_ratio * std::pow(NTU, 0.78)) - 1.0));
                break;
                
            default:
                return effectiveness_NTU(NTU, C_ratio, 0); // Default to counter-current
        }
    }
    
    double calculateNTU(double UA, double C_min) {
        return UA / C_min;
    }
    
    double actualHeatTransfer(double mass_flow, double specific_heat, 
                             double inlet_temp, double outlet_temp) {
        return mass_flow * specific_heat * std::abs(inlet_temp - outlet_temp);
    }
    
    double maximumHeatTransfer(double C_min, double hot_inlet, double cold_inlet) {
        return C_min * (hot_inlet - cold_inlet);
    }
    
    double calculateEffectiveness(double Q_actual, double Q_max) {
        if (Q_max < 1e-6) {
            return 0.0; // Avoid division by zero
        }
        return Q_actual / Q_max;
    }
    
    double foulingFactor(double U_clean, double U_dirty) {
        return (1.0 / U_dirty) - (1.0 / U_clean);
    }

} // namespace ThermalCalculations
