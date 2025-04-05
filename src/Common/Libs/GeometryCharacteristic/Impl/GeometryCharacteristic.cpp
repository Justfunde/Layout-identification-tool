
#include "Include/GeometryCharacteristic.hpp"

template class GeometryCharacteristic<std::vector<lds::Coord>>;

GeometryCharacteristic
GeometryCharacteristic::Create(lds::Geometry* Geometry)
{
   if (!Geometry)
   {
      throw std::invalid_argument("");
   }

   switch (Geometry->type)
   {
   case lds::GeometryType::polygon:
   case lds::GeometryType::rectangle:
      break;
   case lds::GeometryType::text:
   case lds::GeometryType::path:
   case lds::GeometryType::reference:
   default:
      throw std::invalid_argument("");
   }
   const auto& coordArr = Geometry->coords;
}

std::pair<double, double>
GeometryCharacteristic::CalcMean(const CoordArrT& Coords)
{
   if (!Geometry)
   {
      throw std::invalid_argument("");
   }
   const auto accCoord =
      std::accumulate(
         Geometry->coords.begin(),
         Geometry->coords.end(),
         lds::Coord(0, 0),
         [](const auto& acc, const auto& c)
   {
      return lds::Coord(acc.x + c.x, acc.y + c.y);
   });
   return std::make_pair(accCoord.x, accCoord.y);
}

std::pair<double, double>
GeometryCharacteristic::CalcVariance(lds::Geometry* Geometry, const std::pair<double, double>& Mean)
{
   if (!Geometry)
   {
      throw std::invalid_argument("");
   }

   const auto meanDevAcc =
      std::accumulate(
         Geometry->coords.begin(),
         Geometry->coords.end(),
         lds::Coord(0, 0),
         [&Mean](const auto& acc, const auto& c)
   {
      const auto cX = c.x - Mean.first;
      const auto cY = c.y - Mean.second;
      return lds::Coord(
         acc.x + cX * cX,
         acc.y + cY * cY);
   });
   auto calcSigma = [](double meanDevAcc, std::size_t n)
   {
      return std::sqrt(static_cast<double>(1) / (n - 1) * meanDevAcc);
   };
   return std::make_pair(calcSigma(meanDevAcc.x, Geometry->coords.size()), calcSigma(meanDevAcc.y, Geometry->coords.size()));
}

std::pair<double, double>
GeometryCharacteristic::CalcStdDev(const std::pair<double, double>& Variance)
{
   return std::make_pair(Variance.first * Variance.first, Variance.second * Variance.second);
}

double
GeometryCharacteristic::CalcCovariance(lds::Geometry* Geometry, const std::pair<double, double>& Mean)
{
   double covariance = std::accumulate(
      Geometry->coords.begin(),
      Geometry->coords.end(),
      0.0,
      [&Mean](double sum, const lds::Coord& c)
   {
      return sum + (c.x - Mean.first) * (c.y - Mean.second);
   });
   return covariance * (Geometry->coords.size() - 1)
}

double
GeometryCharacteristic::CalcCorrelation(double covariance, double stdDevX, double stdDevY)
{
   return covariance / (stdDevX * stdDevY);
}