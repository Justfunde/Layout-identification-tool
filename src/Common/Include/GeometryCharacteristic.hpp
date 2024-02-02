#ifndef __GEOMETRY_CHARACTERISTIC_H__
#define __GEOMETRY_CHARACTERISTIC_H__

#include "Include/LayoutData.hpp"

#include <array>
#include <string>
#include <memory>
#include <unordered_map>
#include <algorithm>
#include <exception>
#include <numeric>
#include <cmath>




template<typename CoordArrT = std::vector<lds::Coord>>
class GeometryCharacteristic
{
   public:
   GeometryCharacteristic(lds::Geometry* Geometry)
   {
      const auto& coordArr = Geometry->coords;
      mean = CalcMean(coordArr);
      variance = CalcVariance(coordArr, mean);
      stdDev = CalcStdDev(variance);
      covariance = CalcCovariance(coordArr, mean);
      correlation = CalcCorrelation(covariance, stdDev.first, stdDev.second);
   }

   GeometryCharacteristic(const GeometryCharacteristic& Characteristic)
   {
      mean        = Characteristic.mean;
      variance    = Characteristic.variance;
      stdDev      = Characteristic.stdDev;
      covariance  = Characteristic.covariance;
      correlation = Characteristic.correlation;
   }

   GeometryCharacteristic(GeometryCharacteristic&& Characteristic) noexcept
   {
      mean        = std::move(Characteristic.mean);
      variance    = std::move(Characteristic.variance);
      stdDev      = std::move(Characteristic.stdDev);
      covariance  = Characteristic.covariance;
      correlation = Characteristic.correlation;
   }

   GeometryCharacteristic& operator=(GeometryCharacteristic Characteristic)
   {
      std::swap(mean, Characteristic.mean);
      std::swap(variance, Characteristic.variance);
      std::swap(stdDev, Characteristic.stdDev);
      covariance  = Characteristic.covariance;
      correlation = Characteristic.correlation;
   }

   GeometryCharacteristic& operator=(GeometryCharacteristic&& Characteristic) noexcept
   {
      mean        = std::move(Characteristic.mean);
      variance    = std::move(Characteristic.variance);
      stdDev      = std::move(Characteristic.stdDev);
      covariance  = Characteristic.covariance;
      correlation = Characteristic.correlation;
   }

   static
   GeometryCharacteristic
      Create(lds::Geometry* Geometry);

   private:

   std::shared_ptr<lds::Element>
      NormGeometry(lds::Geometry* Geometry);

   /** Расчет среднего значение*/
   std::pair<double, double>
      CalcMean(const CoordArrT& Coords);

   /** Расчет дисперсии*/
   std::pair<double, double>
      CalcVariance(const CoordArrT& Coords, const std::pair<double, double>& Mean);

   std::pair<double, double>
      CalcStdDev(const std::pair<double, double>& Variance);

   double
      CalcCovariance(const CoordArrT& Coords, const std::pair<double, double>& Mean);

   double
      CalcCorrelation(double covariance, double stdDevX, double stdDevY);


   private:
   std::pair<double, double> mean;      // среднее значение x и y
   std::pair<double, double> variance;  // дисперсий по х и у
   std::pair<double, double> stdDev;    // стандартные отклонения х и у
   double covariance; //ковариация х и у
   double correlation; //корреляция между х и у
};

template<typename CoordArrT>
GeometryCharacteristic<CoordArrT> GeometryCharacteristic<CoordArrT>::Create(lds::Geometry* Geometry) {
    if (!Geometry) { throw std::invalid_argument("Geometry is null"); }

    switch (Geometry->type) {
        case lds::GeometryType::polygon:
        case lds::GeometryType::rectangle: break;
        case lds::GeometryType::text:
        case lds::GeometryType::path:
        case lds::GeometryType::reference:
        default: throw std::invalid_argument("Invalid geometry type");
    }

   GeometryCharacteristic characteristic(Geometry);
   return characteristic;
}

template<typename CoordArrT>
std::pair<double, double> GeometryCharacteristic<CoordArrT>::CalcMean(const CoordArrT& Coords) {
    auto accCoord = std::accumulate(
        Coords.begin(), Coords.end(), typename CoordArrT::value_type(0, 0),
        [](const auto& acc, const auto& c) {
            return typename CoordArrT::value_type(acc.x + c.x, acc.y + c.y);
        });

    size_t count = std::distance(Coords.begin(), Coords.end());
    return {accCoord.x / count, accCoord.y / count};
}

template<typename CoordArrT>
std::pair<double, double> GeometryCharacteristic<CoordArrT>::CalcVariance(const CoordArrT& Coords, const std::pair<double, double>& Mean) {
    auto meanDevAcc = std::accumulate(
        Coords.begin(), Coords.end(), typename CoordArrT::value_type(0, 0),
        [&Mean](const auto& acc, const auto& c) {
            const auto cX = c.x - Mean.first;
            const auto cY = c.y - Mean.second;
            return typename CoordArrT::value_type(acc.x + cX * cX, acc.y + cY * cY);
        });

    size_t count = std::distance(Coords.begin(), Coords.end());
    return {meanDevAcc.x / (count - 1), meanDevAcc.y / (count - 1)};
}

template<typename CoordArrT>
std::pair<double, double> GeometryCharacteristic<CoordArrT>::CalcStdDev(const std::pair<double, double>& Variance) {
    return {std::sqrt(Variance.first), std::sqrt(Variance.second)};
}

template<typename CoordArrT>
double GeometryCharacteristic<CoordArrT>::CalcCovariance(const CoordArrT& Coords, const std::pair<double, double>& Mean) {
    double covariance = std::accumulate(
        Coords.begin(), Coords.end(), 0.0,
        [&Mean](double sum, const typename CoordArrT::value_type& c) {
            return sum + (c.x - Mean.first) * (c.y - Mean.second);
        });

    size_t count = std::distance(Coords.begin(), Coords.end());
    return covariance / (count - 1);
}

template<typename CoordArrT>
double GeometryCharacteristic<CoordArrT>::CalcCorrelation(double covariance, double stdDevX, double stdDevY) {
    return covariance / (stdDevX * stdDevY);
}



#endif //! __GEOMETRY_CHARACTERISTIC_H__