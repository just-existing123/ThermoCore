#ifndef HEAT_EXCHANGER_GEOMETRY_H
#define HEAT_EXCHANGER_GEOMETRY_H

#include "fluid_properties.h"

/**
 * @file heat_exchanger_geometry.h
 * @brief Geometric calculations for shell and tube heat exchangers
 */

namespace HeatExchangerGeometry {
    
    /**
     * Calculate tube cross-sectional area
     * @param diameter Tube inner diameter (m)
     * @return Cross-sectional area (m²)
     */
    double tubeArea(double diameter);
    
    /**
     * Calculate shell cross-sectional flow area
     * @param shell_diameter Shell inner diameter (m)
     * @param tube_outer_diameter Tube outer diameter (m)
     * @param num_tubes Number of tubes
     * @return Shell flow area (m²)
     */
    double shellFlowArea(double shell_diameter, double tube_outer_diameter, int num_tubes);
    
    /**
     * Calculate total tube heat transfer area
     * @param tube_diameter Tube diameter (m)
     * @param length Heat exchanger length (m)
     * @param num_tubes Number of tubes
     * @return Total heat transfer area (m²)
     */
    double totalTubeArea(double tube_diameter, double length, int num_tubes);
    
    /**
     * Calculate hydraulic diameter for shell side
     * @param shell_diameter Shell diameter (m)
     * @param tube_outer_diameter Tube outer diameter (m)
     * @param num_tubes Number of tubes
     * @return Hydraulic diameter (m)
     */
    double shellHydraulicDiameter(double shell_diameter, double tube_outer_diameter, int num_tubes);
    
    /**
     * Calculate tube velocity
     * @param mass_flow Mass flow rate (kg/s)
     * @param density Fluid density (kg/m³)
     * @param tube_diameter Tube inner diameter (m)
     * @param num_tubes Number of tubes
     * @return Velocity (m/s)
     */
    double tubeVelocity(double mass_flow, double density, double tube_diameter, int num_tubes);
    
    /**
     * Calculate shell velocity
     * @param mass_flow Mass flow rate (kg/s)
     * @param density Fluid density (kg/m³)
     * @param shell_diameter Shell diameter (m)
     * @param tube_outer_diameter Tube outer diameter (m)
     * @param num_tubes Number of tubes
     * @return Velocity (m/s)
     */
    double shellVelocity(double mass_flow, double density, double shell_diameter, 
                        double tube_outer_diameter, int num_tubes);
    
    /**
     * Calculate baffle spacing (simplified - assumes 25% cut segmental baffles)
     * @param shell_diameter Shell diameter (m)
     * @return Recommended baffle spacing (m)
     */
    double recommendedBaffleSpacing(double shell_diameter);
    
    /**
     * Calculate tube pitch for triangular arrangement
     * @param tube_outer_diameter Tube outer diameter (m)
     * @param pitch_ratio Pitch to diameter ratio (typically 1.25)
     * @return Tube pitch (m)
     */
    double tubePitch(double tube_outer_diameter, double pitch_ratio = 1.25);
    
    /**
     * Estimate maximum number of tubes for given shell diameter
     * @param shell_diameter Shell diameter (m)
     * @param tube_outer_diameter Tube outer diameter (m)
     * @param pitch_ratio Pitch to diameter ratio
     * @return Estimated maximum number of tubes
     */
    int estimateMaxTubes(double shell_diameter, double tube_outer_diameter, double pitch_ratio = 1.25);

} // namespace HeatExchangerGeometry

#endif // HEAT_EXCHANGER_GEOMETRY_H
