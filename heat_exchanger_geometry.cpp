#include "heat_exchanger_geometry.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace HeatExchangerGeometry {
    
    double tubeArea(double diameter) {
        return M_PI * std::pow(diameter / 2.0, 2);
    }
    
    double shellFlowArea(double shell_diameter, double tube_outer_diameter, int num_tubes) {
        double shell_area = M_PI * std::pow(shell_diameter / 2.0, 2);
        double tubes_area = num_tubes * M_PI * std::pow(tube_outer_diameter / 2.0, 2);
        return shell_area - tubes_area;
    }
    
    double totalTubeArea(double tube_diameter, double length, int num_tubes) {
        return M_PI * tube_diameter * length * num_tubes;
    }
    
    double shellHydraulicDiameter(double shell_diameter, double tube_outer_diameter, int num_tubes) {
        double shell_area = M_PI * std::pow(shell_diameter / 2.0, 2);
        double tubes_area = num_tubes * M_PI * std::pow(tube_outer_diameter / 2.0, 2);
        double wetted_perimeter = M_PI * shell_diameter + num_tubes * M_PI * tube_outer_diameter;
        
        return 4.0 * (shell_area - tubes_area) / wetted_perimeter;
    }
    
    double tubeVelocity(double mass_flow, double density, double tube_diameter, int num_tubes) {
        double total_area = num_tubes * tubeArea(tube_diameter);
        return mass_flow / (density * total_area);
    }
    
    double shellVelocity(double mass_flow, double density, double shell_diameter, 
                        double tube_outer_diameter, int num_tubes) {
        double flow_area = shellFlowArea(shell_diameter, tube_outer_diameter, num_tubes);
        return mass_flow / (density * flow_area);
    }
    
    double recommendedBaffleSpacing(double shell_diameter) {
        // Rule of thumb: baffle spacing = 0.2 to 1.0 times shell diameter
        return 0.5 * shell_diameter; // Conservative middle value
    }
    
    double tubePitch(double tube_outer_diameter, double pitch_ratio) {
        return pitch_ratio * tube_outer_diameter;
    }
    
    int estimateMaxTubes(double shell_diameter, double tube_outer_diameter, double pitch_ratio) {
        double pitch = tubePitch(tube_outer_diameter, pitch_ratio);
        double bundle_diameter = shell_diameter - 2 * tube_outer_diameter; // Leave clearance
        
        // Simplified calculation for triangular arrangement
        double tubes_per_row_approx = bundle_diameter / pitch;
        int rows_approx = static_cast<int>(bundle_diameter / (pitch * 0.866)); // 0.866 for triangular
        
        return static_cast<int>(tubes_per_row_approx * rows_approx * 0.8); // 80% packing efficiency
    }

} // namespace HeatExchangerGeometry
