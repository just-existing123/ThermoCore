#include "heat_transfer_correlations.h"
#include <cmath>

namespace HeatTransferCorrelations {
    
    double dittusBoelter(double reynolds, double prandtl, bool heating) {
        if (reynolds < 2300) {
            return 0.0; // Not applicable for laminar flow
        }
        
        double n = heating ? 0.4 : 0.3; // Exponent depends on heating/cooling
        return 0.023 * std::pow(reynolds, 0.8) * std::pow(prandtl, n);
    }
    
    double siederTate(double reynolds, double prandtl, double viscosity_ratio) {
        if (reynolds < 2300) {
            return 0.0; // Not applicable for laminar flow
        }
        
        return 0.027 * std::pow(reynolds, 0.8) * std::pow(prandtl, 1.0/3.0) * std::pow(viscosity_ratio, 0.14);
    }
    
    double gnielinski(double reynolds, double prandtl) {
        if (reynolds < 2300 || reynolds > 5e6) {
            return 0.0; // Outside valid range
        }
        
        double f = std::pow(0.79 * std::log(reynolds) - 1.64, -2); // Friction factor
        double numerator = (f / 8.0) * (reynolds - 1000.0) * prandtl;
        double denominator = 1.0 + 12.7 * std::sqrt(f / 8.0) * (std::pow(prandtl, 2.0/3.0) - 1.0);
        
        return numerator / denominator;
    }
    
    double laminarTubeConstantWallTemp(double graetz) {
        if (graetz > 100) {
            // Developing flow
            return 1.86 * std::pow(graetz, 1.0/3.0);
        } else {
            // Fully developed flow
            return 3.66;
        }
    }
    
    double laminarTubeConstantHeatFlux() {
        return 4.36; // Fully developed flow
    }
    
    double shellSideTubeBundles(double reynolds, double prandtl, int tube_arrangement) {
        if (reynolds < 2000) {
            // Laminar/transition region
            return 0.664 * std::sqrt(reynolds) * std::pow(prandtl, 1.0/3.0);
        } else {
            // Turbulent flow
            if (tube_arrangement == 0) {
                // Inline arrangement
                return 0.27 * std::pow(reynolds, 0.63) * std::pow(prandtl, 0.36);
            } else {
                // Staggered arrangement (default)
                return 0.36 * std::pow(reynolds, 0.55) * std::pow(prandtl, 0.36);
            }
        }
    }
    
    double naturalConvectionVertical(double rayleigh) {
        if (rayleigh < 1e4) {
            return 0.59 * std::pow(rayleigh, 0.25);
        } else if (rayleigh < 1e9) {
            return 0.13 * std::pow(rayleigh, 1.0/3.0);
        } else {
            return 0.1 * std::pow(rayleigh, 1.0/3.0);
        }
    }
    
    double getTubeSideNusselt(double reynolds, double prandtl, bool heating) {
        if (reynolds > 10000) {
            // Use Gnielinski for high Re
            double nu_gnielinski = gnielinski(reynolds, prandtl);
            if (nu_gnielinski > 0) {
                return nu_gnielinski;
            }
        }
        
        if (reynolds > 2300) {
            // Turbulent flow - use Dittus-Boelter
            return dittusBoelter(reynolds, prandtl, heating);
        } else {
            // Laminar flow
            return laminarTubeConstantWallTemp();
        }
    }
    
    double getShellSideNusselt(double reynolds, double prandtl, int tube_arrangement) {
        return shellSideTubeBundles(reynolds, prandtl, tube_arrangement);
    }

} // namespace HeatTransferCorrelations
