#include "dimensionless_numbers.h"

namespace DimensionlessNumbers {
    
    double calculateReynolds(double velocity, double diameter, double density, double viscosity) {
        return (density * velocity * diameter) / viscosity;
    }
    
    double calculatePrandtl(double cp, double viscosity, double thermal_conductivity) {
        return (cp * viscosity) / thermal_conductivity;
    }
    
    double calculatePeclet(double reynolds, double prandtl) {
        return reynolds * prandtl;
    }
    
    double calculateGraetz(double reynolds, double prandtl, double diameter, double length) {
        return reynolds * prandtl * diameter / length;
    }

} // namespace DimensionlessNumbers
