#ifndef __GEOMETRY_CHARACTERISTIC_H__
#define __GEOMETRY_CHARACTERISTIC_H__

#include "Include/LayoutData.hpp"
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <stdexcept>
#include <type_traits>



//template<typename T>
class GeometryCharacteristic
{
   //static_assert(std::is_arithmetic<T>::value, "Требуется арифметический тип");

public:
   static GeometryCharacteristic Create(const lds::Geometry* Geometry)
   {
      if (!Geometry) throw std::invalid_argument("Geometry не должен быть nullptr");
      return GeometryCharacteristic(Geometry);
   }

   static
   std::map<lds::Geometry*, GeometryCharacteristic>
   Create(lds::Element* Elem)
   {
      if (!Elem) throw std::invalid_argument("Geometry не должен быть nullptr");

      std::map<lds::Geometry*, GeometryCharacteristic> chars;
      for(const auto it : Elem->geometries)
      {
         auto normGeometry = std::make_unique<lds::Geometry>(*it);
         for(auto& it : normGeometry->coords)
         {
            it.x -= Elem->min.x;
            it.y -= Elem->min.y;
         }
         chars.emplace(it, GeometryCharacteristic::Create(normGeometry.get()));
      }

      return chars;
   }

   const std::pair<double, double>& GetMean()        const { return Mean; };
   const std::pair<double, double>& GetVariance()    const { return Variance; };
   const std::pair<double, double>& GetStdDev()      const { return StdDev; };
   double                    GetCovariance()  const { return Covariance; };
   double                    GetCorrelation() const { return Correlation; };

private:
   explicit GeometryCharacteristic(const lds::Geometry* Geometry)
   {
      std::vector<double> XCoordArr, YCoordArr;

      std::transform(Geometry->coords.begin(), Geometry->coords.end(), std::back_inserter(XCoordArr),
                     [](const auto& Coord) { return static_cast<double>(Coord.x); });
      std::transform(Geometry->coords.begin(), Geometry->coords.end(), std::back_inserter(YCoordArr),
                     [](const auto& Coord) { return static_cast<double>(Coord.y); });

      if (XCoordArr.empty() || YCoordArr.empty()) throw std::invalid_argument("Массивы координат не должны быть пустыми");

      Mean = {CalcMean(XCoordArr), CalcMean(YCoordArr)};
      Variance = {CalcVariance(XCoordArr, Mean.first), CalcVariance(YCoordArr, Mean.second)};
      StdDev = {CalcStdDev(Variance.first), CalcStdDev(Variance.second)};
      Covariance = CalcCovariance(XCoordArr, YCoordArr, Mean);
      Correlation = CalcCorrelation(Covariance, StdDev.first, StdDev.second);
   }

   /*
      Подсчет среднего значения в массиве.
      В контексте сравнения полигонов - позволяет проанализировать положение полигона в пространстве.(Охарактеризовать положение одним числом)
   */
   double CalcMean(const std::vector<double>& Arr)
   {
      return std::accumulate(Arr.begin(), Arr.end(), 0.0) / Arr.size();
   }

   /*
      Сравнивая стандартные отклонения и дисперсии, можно определить степень разброса вершин вокруг центра полигонов.
      Полигоны с меньшим стандартным отклонением и дисперсией будут иметь более компактные формы, в то время как более высокие значения указывают на более растянутые и разбросанные формы.
   */
   double CalcVariance(const std::vector<double>& Coords, double Mean)
   {
      auto VarianceFunc = [Mean](double Acc, double C) { double Diff = C - Mean; return Acc + Diff * Diff; };
      return std::accumulate(Coords.begin(), Coords.end(), 0.0, VarianceFunc) / (Coords.size() - 1);
   }

   /**
    * Сравнение ковариации двух полигонов может показать, имеют ли они схожую ориентацию.
    * Например, если оба полигона имеют положительную ковариацию, это может указывать на то, что оба имеют наклон в одном и том же направлении. Отрицательная ковариация может указывать на противоположный наклон.
   */
   double CalcCovariance(const std::vector<double>& CoordsX, const std::vector<double>& CoordsY, const std::pair<double, double>& Mean)
   {
      if (CoordsX.size() != CoordsY.size()) throw std::invalid_argument("Массивы X и Y должны быть одинаковой длины");
      double Sum = 0.0;
      for (size_t i = 0; i < CoordsX.size(); ++i)
      {
         Sum += (CoordsX[i] - Mean.first) * (CoordsY[i] - Mean.second);
      }
      return Sum / (CoordsX.size() - 1);
   }

   double CalcStdDev(double Variance)
   {
      return std::sqrt(Variance);
   }

   /**
    * Коэффициент корреляции дает понимание о направлении и степени линейной зависимости между координатами точек полигонов.
    * Сильная положительная корреляция может свидетельствовать о схожести формы и ориентации полигонов. Если один полигон имеет высокую положительную корреляцию, а другой — низкую или отрицательную, это может указывать на значительные различия в их формах.
   */
   double CalcCorrelation(double Covariance, double StdDevX, double StdDevY)
   {
      if (StdDevX == 0 || StdDevY == 0) throw std::invalid_argument("Стандартное отклонение не должно быть 0");
      return Covariance / (StdDevX * StdDevY);
   }

   std::pair<double, double> Mean;      // Средние значения по X и Y
   std::pair<double, double> Variance;  // Дисперсии по X и Y
   std::pair<double, double> StdDev;    // Стандартные отклонения по X и Y
   double Covariance;                   // Ковариация между X и Y
   double Correlation;                  // Корреляция между X и Y
};

#endif // !__GEOMETRY_CHARACTERISTIC_H__
