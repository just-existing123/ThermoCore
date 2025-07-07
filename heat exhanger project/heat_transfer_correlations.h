#ifndef HEAT_TRANSFER_CORRELATIONS_H
#define HEAT_TRANSFER_CORRELATIONS_H

/**
 * @file heat_transfer_correlations.h
 * @brief Heat transfer correlations for Nusselt number calculations
 */

namespace HeatTransferCorrelations {
    
    /**
     * Dittus-Boelter equation for turbulent flow in smooth tubes
     * @param reynolds Reynolds number
     * @param prandtl Prandtl number
     * @param heating true for heating, false for cooling
     * @return Nusselt number
     */
    double dittusBoelter(double reynolds, double prandtl, bool heating = false);
    
    /**
     * Sieder-Tate correlation for turbulent flow in tubes with viscosity correction
     * @param reynolds Reynolds number
     * @param prandtl Prandtl number
     * @param viscosity_ratio Ratio of bulk to wall viscosity
     * @return Nusselt number
     */
    double siederTate(double reynolds, double prandtl, double viscosity_ratio = 1.0);
    
    /**
     * Gnielinski correlation for turbulent flow in smooth tubes
     * Valid for 2300 < Re < 5×10⁶ and 0.5 < Pr < 2000
     * @param reynolds Reynolds number
     * @param prandtl Prandtl number
     * @return Nusselt number
     */
    double gnielinski(double reynolds, double prandtl);
    
    /**
     * Laminar flow in tubes - constant wall temperature
     * @param graetz Graetz number (for developing flow)
     * @return Nusselt number
     */
    double laminarTubeConstantWallTemp(double graetz = 0.0);
    
    /**
     * Laminar flow in tubes - constant heat flux
     * @return Nusselt number
     */
    double laminarTubeConstantHeatFlux();
    
    /**
     * Shell side correlation for cross flow over tube bundles
     * @param reynolds Reynolds number based on shell-side conditions
     * @param prandtl Prandtl number
     * @param tube_arrangement 0 = inline, 1 = staggered
     * @return Nusselt number
     */
    double shellSideTubeBundles(double reynolds, double prandtl, int tube_arrangement = 1);
    
    /**
     * Natural convection correlation for vertical plates/cylinders
     * @param rayleigh Rayleigh number
     * @return Nusselt number
     */
    double naturalConvectionVertical(double rayleigh);
    
    /**
     * Get appropriate Nusselt correlation for tube side
     * @param reynolds Reynolds number
     * @param prandtl Prandtl number
     * @param heating true for heating, false for cooling
     * @return Nusselt number
     */
    double getTubeSideNusselt(double reynolds, double prandtl, bool heating = false);
    
    /**
     * Get appropriate Nusselt correlation for shell side
     * @param reynolds Reynolds number
     * @param prandtl Prandtl number
     * @param tube_arrangement 0 = inline, 1 = staggered
     * @return Nusselt number
     */
    double getShellSideNusselt(double reynolds, double prandtl, int tube_arrangement = 1);

} // namespace HeatTransferCorrelations

#endif // HEAT_TRANSFER_CORRELATIONS_H
